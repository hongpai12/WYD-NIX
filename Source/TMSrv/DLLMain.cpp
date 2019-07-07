/*
 * Esta dll é protegida pelas leis internacionais de software
 * e pela lei de Deus.
 *
 * Afinal de contas, só Deus sabe como essa merda funciona...
 * Não apagar nada no projeto.
 *
 * Nix Team - ErickMota
 * Todos os direitos reservados
 */

#include "DLLMain.h"

BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD catchReason, LPVOID lpResrv)
{
    switch (catchReason)
    {
        case DLL_PROCESS_ATTACH:
		{
			setlocale(LC_ALL, "Portuguese");

			DoIndirections();

			StartThread(MainThread);
			Servidor.Estado = ESTADO_MANUTENC_ACESSO;
			
			Servidor.Days		 = 0;
			Servidor.Hours		 = 0;
			Servidor.Minuts		 = 0;
			Servidor.Seconds     = 0;
			Servidor.OnlineCount = 0;
			Servidor.ExpCount    = 0;
			Servidor.ExpBonus    = 0;
			Servidor.QuioneStatus = QUIONE_DEAD;

			break;
		}

        case DLL_PROCESS_DETACH:
		{
			FreeLibrary(hInstDLL);
			break;
		}
    }

    return TRUE;
}