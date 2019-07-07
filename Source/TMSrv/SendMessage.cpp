#include "AddMessage.h"

bool pSend(UINT32 sendTo, UINT8* m_PacketBuffer, UINT8* pBuffer, UINT32 packetSize)
{
	bool retn = false;

	PacketHeader* pHeader = (PacketHeader*)m_PacketBuffer;

	switch (pHeader->PacketId)
	{
		case 0x114:
			SendWorld((BYTE*)m_PacketBuffer, &retn);
			break;

		case 0x809:
			ReqDeleteChar(m_PacketBuffer, &retn);
			break;
	}

	return retn;
}