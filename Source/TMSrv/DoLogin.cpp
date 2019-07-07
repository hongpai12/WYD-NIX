#include "DLLMain.h"

void DoLogin(BYTE *m_PacketBuffer, bool *pRetn)
{
	FILE *pFile = NULL;

	p20D *p = (p20D*)m_PacketBuffer;

	INT16 cId = p->Header.ClientId;

	// Evita que se faça login em conta banida
	for (int i = 0; i < 16; i++)
	{
		if (p->Login[i] == ' ')
			p->Login[i] = 0x0;
	}

	if(!p->Login[0])
		*pRetn = true;	

	else if(cId > MAX_PLAYER || cId < 0)
	{
		SendClientMessage(cId, "Servidor cheio no momento. Tente novamente mais tarde");

		*pRetn = true;

		sendLog(INFO, cId, "%s Não pode acessar por conta de limite de conexões.", p->Login);

		return;
	}

	else if(p->Version != CLIVER)
	{
		*pRetn = true;

		SendClientMessage(cId, "Execute o Launcher ou visite o site para baixar as novas atualizações.");

		sendLog(ERROR, cId, "%s Tentou fazer login com o cliver %d.", p->Login, p->Version);

		return;
	}
		
	else if(CheckBan(p->Login))
	{
		*pRetn = true;

		SendClientMessage(cId, "Conta desativada, entre em contato com o suporte.");

		sendLog(WARN, cId, "%s Tentou fazer login enquanto banido.", p->Login);

		return;
	}

	else if (p->Unknows != 0xFAB)
	{
		*pRetn = true;

		SendClientMessage(cId, "Atualize o client através do Launcher ou via site.");

		sendLog(WARN, cId, "%s tentou fazer login com o unknow %d", p->Login, p->Unknows);

		return;
	}

	else if (!strcmp(p->IPAddress, "") || strlen(p->IPAddress) <= 10)
	{
		*pRetn = true;

		return;
	}
	
	memset(&nBuffer[cId], 0, sizeof nInfo);

	char szTMP[1024];	

	sprintf_s(szTMP, "C://StoredData/Data/%s.bin", p->Login);

	fopen_s(&pFile, szTMP, "rb");

	if(pFile)
	{
		int mobSize = sizeof nInfo - (sizeof nBuffer[0].Ingame);
		
		fread(&nBuffer[cId], 1, mobSize, pFile);

		fclose(pFile);
	}

	if (cId < 0 || cId > MAX_PLAYER)
	{
		*pRetn = true;
		return;
	}

	if (nBuffer[cId].AcessLevel < MOD_LEVEL)
	{
		if (Servidor.Estado >= ESTADO_MANUTENC_ACESSO)
		{
			*pRetn = true;

			SendClientMessage(cId, "Servidor em manutenção, tente novamente mais tarde.");

			return;
		}

		else if (GetLoggedCount(p->IPAddress) >= 2)
		{
			*pRetn = true;

			SendClientMessage(cId, "Limite de duas conexões por máquina.");

			return;
		}
	}

	if (!strcmp(nBuffer[cId].Login, ""))
		strncpy(nBuffer[cId].Login, p->Login, strlen(p->Login));

	if (!strcmp(nBuffer[cId].MacAdress, ""))
		strncpy(nBuffer[cId].MacAdress, p->IPAddress, strlen(p->IPAddress));
	/*else if (strcmp(nBuffer[cId].MacAdress, p->IPAddress))
	{
		*pRetn = true;

		SendClientMessage(cId, "Máquina não reconhecida, login negado.");
		sendLog(WARN, cId, "[%s] tentou fazer login em [%s]", p->IPAddress, p->Login);

		return;
	}*/
	
	nBuffer[cId].Ingame.canLog  = false;

	sendLog(INFO, cId, "%s Fez login no servidor.", p->Login);
}