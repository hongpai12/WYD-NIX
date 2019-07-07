#include "ExitParty.h"
#include "DLLMain.h"

void FixExitParty_SERVER(BYTE *m_PacketBuffer, BYTE *packetBuffer)
{
	PacketHeader *packetHeader = (PacketHeader*)packetBuffer;

	*(WORD*)&m_PacketBuffer[14] = packetHeader->ClientId;
	*(WORD*)&m_PacketBuffer[14] = 0x0;
}

struct Teste_37E{
	PacketHeader Header;
	unsigned short LiderID;
	unsigned short RemoveID;
};

void FixExitParty_CLIENT(BYTE *packetBuffer, bool *pRetn)
{
	*pRetn = true;

	Teste_37E *p = (Teste_37E*)packetBuffer;

	int cId = p->Header.ClientId,
		mobId = p->LiderID;

	if (mobId < 0 || mobId >= MAX_PLAYER)
		return;

	if (mobId != 0)
	{
		st_Mob *player = GetMobFromIndex(cId);
		if (!player->Leader)
			RemoveParty(mobId);
		else
			SendClientMsg(cId, "Apenas o líder do grupo");
	}
	else
		RemoveParty(cId);
}