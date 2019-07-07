#include "DLLMain.h"

void ReceiveBets(UINT8* packetBuffer, bool* pRetn)
{
	*pRetn = true;

	p28F *p = (p28F*)packetBuffer;

	INT16 cId = p->Header.ClientId;

	if (cId < 0 || cId > MAX_PLAYER)
		return;

	else if (p->npcid < MAX_PLAYER || p->npcid > 30000)
		return;
	
	int CityId = GetCity(cId);

	char szMsg[120];
	sprintf(szMsg, "Imposto recolhido: %s", NumberToString(Servidor.ImpostoAcumulado[CityId]));

	SendSay(p->npcid, szMsg);
}