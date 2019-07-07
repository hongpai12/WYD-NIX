#include "PacketControl.h"

INT32 GetIndexStruct(UINT32 pBase, UINT32 pStruct, size_t sizeStruct)
{
	if (!pBase || !pStruct || !sizeStruct)
		return -1;

	return ((pStruct - pBase) / sizeStruct);
}

void* HKD_PacketControl(UINT32* pSocket, void* pBuffer, UINT32 packetSize, UINT32 typeControl)
{
	PacketHeader* pHeader = (PacketHeader*)pBuffer;

	if (typeControl < 0 || typeControl > 1)
	{
		printf("\n\nerror: typeControl undefined = %d\n\n", typeControl);
		return pBuffer;
	}

	if ((UINT32)pSocket == BASE_DBSOCKET)
	{
		/*printf("%s DBSrv: PacketSize = %d PacketId = 0x%X\n",
			typeControl == 1 ? "Send to" : "Recv from",
			packetSize, pHeader->PacketId);*/
	}
	else
	{
		static UINT8 m_PacketBuffer[101][2][65536];

		bool clear = false;
		UINT8* m_Packet = NULL;

		const INT32 clientId = GetIndexStruct((UINT32)&Users[0], (UINT32)pSocket, 0xC4C);

		if (!clientId)
		{
			printf("\n\nerror: get clientId = %d in HKD_PacketControl Type: %s\n\n", clientId, typeControl == 1 ? "Send" : "Recv");
			return pBuffer;
		}

		m_Packet = m_PacketBuffer[clientId][typeControl];

		__try
		{
			memcpy_s(m_Packet, 65536, pBuffer, packetSize);
		}
		__except (1)
		{
			printf("\n\nerror: memcpy_s in HKD_PacketControl Type: %s\n\n", typeControl == 1 ? "Send" : "Recv");
		}

		switch (typeControl)
		{
		case 00:
			clear = Recv(clientId, m_Packet, (UINT8*)pBuffer, packetSize);
			break;
		case 01:
			clear = pSend(clientId, m_Packet, (UINT8*)pBuffer, packetSize);
			break;
		}

		/*printf("%s ClientId = %d: PacketSize = %d PacketId = 0x%X PacketCId = %d\n",
			typeControl == 1 ? "Send to" : "Recv from",
			clientId, packetSize, pHeader->PacketId, pHeader->ClientId);
			*/
		if (clear)
			memset(m_Packet, 0, packetSize + 1);

		return m_Packet;
	}

	return pBuffer;
}