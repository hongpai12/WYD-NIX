#include "DLLMain.h"

void Numerica(BYTE *m_PacketBuffer)
{
	pFDE *p = (pFDE*)m_PacketBuffer;

	nInfo *player = &nBuffer[p->Header.ClientId];

	PacketHeader header;
	memset(&header, 0, sizeof header);

	header.Size = 12;
	header.ClientId = 0x0;
	header.PacketId = 0xFDF;

	if(!player->Ingame.canLog || p->RequestChange == 1)
	{
		if(!strcmp(player->Numerica,""))
		{
			strncpy(player->Numerica, p->num, 6);

			player->Ingame.canLog = true;

			Sendpack(m_PacketBuffer, p->Header.ClientId, sizeof pFDE);

			sendLog(INFO, p->Header.ClientId, "%s definiu a numérica %s.", player->Login, player->Numerica);

			if (!SaveAccount(p->Header.ClientId))
				sendLog(ERROR, p->Header.ClientId, "%s não conseguiu ter seus dados salvos.", player->Login);
		}
		else
		{
			if(p->RequestChange == 1 && player->Ingame.canLog)
			{
				strncpy(player->Numerica, p->num, 6);

				Sendpack(m_PacketBuffer, p->Header.ClientId, sizeof pFDE);

				sendLog(INFO, p->Header.ClientId, "%s redefiniu a numérica %s.", player->Login, player->Numerica);

				if (!SaveAccount(p->Header.ClientId))
					sendLog(ERROR, p->Header.ClientId, "%s não conseguiu ter seus dados salvos.", player->Login);
			}
			else if(!strncmp(player->Numerica, p->num, 6))
			{
				Sendpack(m_PacketBuffer, p->Header.ClientId, sizeof pFDE);
				player->Ingame.canLog = true;

				sendLog(INFO, p->Header.ClientId, "%s acertou a numérica %s.", player->Login, player->Numerica);
			}
			else
			{
				player->Ingame.IncorrectNumeric ++;

				//sendLog(INFO, p->Header.ClientId, "%s errou a numérica.", player->Login);

				if(player->Ingame.IncorrectNumeric > 3)
				{
					CloseUser_OL2(p->Header.ClientId);

					sendLog(INFO, p->Header.ClientId, "%s errou a numérica 3 vezes, desconectado.", player->Login);
					
					return;
				}

				Sendpack((BYTE*)&header, p->Header.ClientId, 12);
			}
		}
	}

	return;
}