#include "MainLaunch.h"

BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD catchReason, LPVOID lpResrv)
{
	switch (catchReason)
	{
	case DLL_PROCESS_ATTACH:
		
		DEF_STR((char*)"%sNix2.exe", (char*)0x0043AA78);
		break;

	case DLL_PROCESS_DETACH:
		FreeLibrary(hInstDLL);
		break;
	}

	return TRUE;
}