#include "PacketControl.h"

bool SendPacketControl(BYTE* newPacketBuffer, PacketHeader* oldPacketBuffer, int clientId)
{
	// Para zerar um packet basta setar retn como true, 
	// O hook se encarregará do resto

	bool retn = false;
	switch (oldPacketBuffer->PacketId)
	{
		case 0x336:
			FixSendScore_SERVER((BYTE*)newPacketBuffer, (BYTE*)oldPacketBuffer);
			SendAffect(oldPacketBuffer->ClientId);
			break;

		case 0x337:
			FixSendEtc_SERVER((BYTE*)newPacketBuffer, (BYTE*)oldPacketBuffer);
			break;

		case 0x338:
			FixSendExp_SERVER((BYTE*)newPacketBuffer, (BYTE*)oldPacketBuffer);
			break;

		case 0x363:
			FixGetCreateMobTrade_SERVER((BYTE*)newPacketBuffer, (BYTE*)oldPacketBuffer);
			break;

		case 0x364:
			FixGetCreateMob_SERVER((BYTE*)newPacketBuffer, (BYTE*)oldPacketBuffer);
			break;

		case 0x36B:
			FixRefreshItens_SERVER((BYTE*)newPacketBuffer, (BYTE*)oldPacketBuffer);
			break;

		case 0x37E:
			FixExitParty_SERVER((BYTE*)newPacketBuffer, (BYTE*)oldPacketBuffer);
			break;

		case 0x36C:
		case 0x39D:
			FixSendAffect_SERVER((BYTE*)newPacketBuffer, (BYTE*)oldPacketBuffer);
			break;

		case 0x39E:
			FixSendAffect39E_SERVER((BYTE*)newPacketBuffer, (BYTE*)oldPacketBuffer);
			break;
	}

	return retn;
}

bool RecvPacketControl(PacketHeader* packetBuffer, int clientId)
{
	// Para zerar um packet basta setar retn como true, 
	// O hook se encarregará do resto

	bool retn = false;
	switch (packetBuffer->PacketId)
	{
		case 0x213:
			memset((BYTE*)&packetBuffer[20], 0x0, 36 - 20);
			break;

		case 0x374:
			memset(&packetBuffer[14], 0, 2);
			break;

		case 0x37E:
			FixExitParty_CLIENT((BYTE*)packetBuffer, &retn);
			break;

		case 0x39D:
		case 0x36C:
		case 0x39E:
		{
			//clock_t actualTime = clock(),
			//		lastTime = nBuffer[packetBuffer->ClientId].Ingame.LastAttackTime;

			//if (lastTime != 0 && actualTime - lastTime < 5000)
			//{
			//	SendClientMessage(packetBuffer->ClientId, "Algo de errado aconteceu, entre em contato com a administração.");

			//	return true;
			//}

			//nBuffer[packetBuffer->ClientId].Ingame.LastAttackTime = actualTime;

			if (packetBuffer->PacketId == 0x39E)
				FixSendAffect39E_CLIENT((BYTE*)packetBuffer, &retn);
			else
				FixSendAffect_CLIENT((BYTE*)packetBuffer, &retn);

			break;
		}
	}

	return retn;
}

int __stdcall HKD_AddMessage(int instModule, PacketHeader *oldPacketBuffer, int packetSize)
{
	__try
	{
		int retnValue = 0;
		int clientId = GetUserFromSocket(instModule);

		BYTE *m_Buffer = new BYTE[65536];

		memcpy_s(m_Buffer, 65536, oldPacketBuffer, packetSize);

		// Compara o size do pacote no header e passado por parametro
		if (*(WORD*)m_Buffer != (WORD)packetSize)
		{
			*(WORD*)m_Buffer = (WORD)packetSize;
		}

		// Faz as modificações necessárias no pacote
		bool clear = SendPacketControl(m_Buffer, oldPacketBuffer, clientId);
		if (clear)
			memset(m_Buffer, 0x0, packetSize);

		// Chama a função original
		__asm
		{
			PUSH DWORD PTR SS : [packetSize]
				PUSH DWORD PTR SS : [m_Buffer]
				MOV ECX, instModule
				CALL _AddMessage

				MOV retnValue, EAX
		}

		delete[] m_Buffer;

		return retnValue;
	}
	__except (TRUE)
	{
		return 0;
	}
}

BYTE* __stdcall HKD_ReadMessage(int instModule, int *unkParam1, int *unkParam2)
{
	__try
	{
		BYTE* packetBuffer = NULL;
		int clientId = GetUserFromSocket(*(WORD*)instModule);

		// Chama função original
		__asm
		{
			PUSH DWORD PTR SS : [unkParam2]
				PUSH DWORD PTR SS : [unkParam1]
				MOV ECX, instModule

				CALL ReadMessage

				MOV packetBuffer, EAX
		}

		if (!packetBuffer)
			return packetBuffer;

		// Faz as modificações necessárias no pacote
		bool clear = RecvPacketControl((PacketHeader*)packetBuffer, clientId);
		if (clear)
			memset(packetBuffer, 0x0, *(WORD*)packetBuffer);

		return packetBuffer;
	}
	__except (TRUE)
	{
		return 0;
	}
}