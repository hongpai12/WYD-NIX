#include "DLLMain.h"

void SendWorld(BYTE *m_PacketBuffer, bool* pRetn)
{
	memset(&m_PacketBuffer[788], 0x0, 1244 - 788);

	p114 *p = (p114*)m_PacketBuffer;

	short clientId = *(short*)&m_PacketBuffer[774];

	nInfo *player = &nBuffer[clientId];

	if(clientId < 1 || clientId > MAX_PLAYER)
		return;

	if (Users[clientId].Status != 22)
	{
		*pRetn = true;

		return;
	}

	if (!player->Ingame.canLog)
	{
		SendClientMessage(clientId, "Desculpe.");

		*pRetn = true;

		return;
	}

	player->Ingame.LoggedMob = p->SlotIndex;

	memcpy(p->SkillBar2, player->Chars[player->Ingame.LoggedMob].SkillBar2, 16);

	p->Hold = 0x0;
	nBuffer[clientId].Ingame.Tab[0] = 0x0;

	if (!strncmp(Servidor.TopKiller, p->Name, 16))
		strncpy(player->Ingame.Tab, "Serial Killer", 26);
	else if (!strncmp(Servidor.SecondKiller, p->Name, 16))
		strncpy(player->Ingame.Tab, "Matador Profissional", 26);
	else if (!strncmp(Servidor.ThirdKiller, p->Name, 16))
		strncpy(player->Ingame.Tab, "Matador Aprendiz", 26);

	sendLog(INFO, clientId, "%s %s entrou no mundo.", player->Login, p->Name);
}