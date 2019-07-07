#include "Functions.h"
//7.57 Implemention (Not Complete)

/*
INT8* PacketSecurity::m_KeyTable = (INT8*)0x004CB920;
TimeStamp* PacketSecurity::m_Time = (TimeStamp*)0x08BF17D8;

void PacketSecurity::Encrypt(INT8* pBuffer, INT8* data, INT32 packetSize)
{
	PacketHeader* pHeader = (PacketHeader*)pBuffer;

	INT32 checkSum[2] = {0, 0};
	INT8 hashKey = Random() & 0x800000FF;

	pHeader->Size = (INT16)packetSize;
	pHeader->Key = hashKey;
	pHeader->TimeStamp = m_Time->Current;

	m_Time->Old = m_Time->Current;

	INT32 keyIncrement = m_KeyTable[(hashKey & 255) * 2];

	for(INT32 i = 4; i < packetSize; i++, keyIncrement++)
	{
		INT32 keyResult = m_KeyTable[(((keyIncrement & 255) & 0x800000FF) * 2) + 1];

		switch(i & 3)
		{
			case 00:
			{
				pBuffer[i] = data[i] + ((keyResult  & 255) << 1);
				break;
			}
			case 01:
			{
				pBuffer[i] = data[i] - ((keyResult  & 255) >> 3);
				break;
			}
			case 02:
			{
				pBuffer[i] = data[i] + ((keyResult  & 255) << 2);
				break;
			}
			case 03:
			{
				pBuffer[i] = data[i] - ((keyResult  & 255) >> 5);
				break;
			}
		}

		checkSum[0] += data[i];
		checkSum[1] += pBuffer[i];
	}

	pHeader->CheckSum = ((checkSum[1] & 255) - (checkSum[0] & 255)) & 255;
}*/