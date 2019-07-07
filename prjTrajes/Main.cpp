#include "Main.h"

GuildInfo Guild;

BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD catchReason, LPVOID lpResrv)
{
	switch (catchReason)
	{
		case DLL_PROCESS_ATTACH:
			Initialize();
			/*SetNewExp_Addr();
			Protection();*/
			break;

		case DLL_PROCESS_DETACH:
			FreeLibrary(hInstDLL);
			break;
	}

	return TRUE;
}