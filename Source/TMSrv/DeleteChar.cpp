#include "DLLMain.h"

void ReqDeleteChar(UINT8* m_PacketBuffer, bool* pRetn)
{
	REQDELETECHAR* p = (REQDELETECHAR*)m_PacketBuffer;

	INT16 cId = p->Header.ClientId;
	INT32 Slot = p->SlotIndex;

	if (cId < 0 || cId > MAX_PLAYER)
		return;
	if (Slot < 0 || Slot > 3)
		return;

	char szTMP[256];
	char szFirst[256];
	BASE_GetFirstKey(p->Name, szFirst);

	sprintf(szTMP, "C://Nix/SV1/DBSRV/run/char/%s/%s", szFirst, p->Name);
	
	remove(szTMP);

	memset(&nBuffer[cId].Chars[Slot], 0, sizeof chars);
	sendLog(INFO, cId, "%s deletou o personagem %d", nBuffer[cId].Login, Slot);
}