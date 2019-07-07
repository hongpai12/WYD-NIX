#include "Main.h"
#include "DumpAHack.h"

HANDLE hEvent;

void SystemProcessesScan()
{
	HANDLE processSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (processSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(PROCESSENTRY32);

		if (Process32First(processSnap, &pe32))
		{
			do
			{
				HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);

				if (process != NULL)
				{
					if (ScanProcessMemory(process))
					{
						for (int i = 0; i < MAX_PROCESS_DUMP; i++)
						{
							char tmpBuffer[MAX_DUMP_SIZE];
							SIZE_T bytesRead = 0;
							ReadProcessMemory(process, (LPCVOID)ProcessesDump[i].Offset, (LPVOID)tmpBuffer, sizeof(tmpBuffer), &bytesRead);
							if (!memcmp(tmpBuffer, ProcessesDump[i].MemDump, MAX_DUMP_SIZE))
							{
								TerminateProcess(process, 0);
								ExitProcess(0);
							}
						}
					}
				}
			} while (Process32Next(processSnap, &pe32));
		}
	}
	CloseHandle(processSnap);
}

bool ScanProcessMemory(HANDLE process)
{
	for (int i = 0; i < MAX_PROCESS_DUMP; i++)
	{
		char tmpBuffer[MAX_DUMP_SIZE];

		SIZE_T bytesRead = 0;
		ReadProcessMemory(process, (LPCVOID)ProcessesDump[i].Offset, (LPVOID)tmpBuffer, sizeof(tmpBuffer), &bytesRead);
		if (!memcmp(tmpBuffer, ProcessesDump[i].MemDump, MAX_DUMP_SIZE))
		{
			return true;
			break;
		}
	}
	return false;
}

void MainThread()
{
again:
	WaitForSingleObject(hEvent, 500);
	SystemProcessesScan();
	goto again;
}

extern "C" __declspec(dllexport) void Protection()
{
	SystemProcessesScan();
	hEvent = CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(MainThread), NULL, 0, 0);
}

