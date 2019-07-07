#include "DLLMain.h"

void ChangeSkillBar(BYTE *m_PacketBuffer)
{
	p378 *p = (p378*)m_PacketBuffer;

	nInfo *player = &nBuffer[p->Header.ClientId];

	// Salva os Slots das skils no player.
	memcpy(player->Chars[player->Ingame.LoggedMob].SkillBar2, p->SkillBar2, 16);
}