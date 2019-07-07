#include "ReadMessage.h"
#include "Naked.h"
#include "Hook.h"
#include "Functions.h"
#include "Send.h"
#include <time.h>

bool Recv(UINT32 recvFrom, UINT8* m_PacketBuffer, UINT8* pBuffer, UINT32 packetSize)
{
	bool retn = false;

	PacketHeader* pHeader = (PacketHeader*)m_PacketBuffer;

	pHeader->ClientId = recvFrom;

	if (pHeader->PacketId == 0x2D4)
	{
		Barganha((p2D4*)m_PacketBuffer);
		
		return true;
	}

	if (!nBuffer[recvFrom].Ingame.TimeStamp)
		nBuffer[recvFrom].Ingame.TimeStamp = pHeader->TimeStamp;
	else
	{
		if (nBuffer[recvFrom].Ingame.TimeStamp == pHeader->TimeStamp)
			return true;

		nBuffer[recvFrom].Ingame.TimeStamp = pHeader->TimeStamp;
	}

	switch (pHeader->PacketId)
	{
		case 0x20D:
			DoLogin((BYTE*)m_PacketBuffer, &retn);
			break;

		case 0x20F:
		{
			if (Users[recvFrom].Status == 22)
			{
				sendLog(WARN, recvFrom, "O jogador tentou burlar o createchar. Status : %d", Users[recvFrom].Status);
				SendBan(Users[recvFrom].Username);

				CloseUser(recvFrom);

				retn = true;
			}
		}
		break;

		case 0x272:
		{
			if (nBuffer[pHeader->ClientId].Ingame.isAdmin)
				retn = false;

			retn = true;
		}
		break;

		case 0x277:
			FixAddPoint_CLIENT((BYTE*)m_PacketBuffer, &retn);
			break;

		case 0x28B:
			UseNpcs((BYTE*)m_PacketBuffer, &retn);
			break;

		case 0x28E:
		case 0x28F:
			ReceiveBets(m_PacketBuffer, &retn);
			break;

		case 0x2D2:
			Odin((BYTE*)m_PacketBuffer, &retn);
			break;

		case 0x2D3:
			Ehre((BYTE*)m_PacketBuffer, &retn);
			break;

		case 0x2E1:
			SkillAlchemy(m_PacketBuffer, &retn);
			break;

		case 0x2E4:
			ReqDeleteItem(m_PacketBuffer, &retn);
			break;

		case 0x2E5:
			retn = RequestUngroupItem((BYTE*)m_PacketBuffer);
			break;

		case 0x334:
			Commands((BYTE*)m_PacketBuffer, &retn);
			break;

		case 0x373:
			UseItens((BYTE*)m_PacketBuffer, &retn);
			break;

		case 0x376:
			//MSG_SWAPITEM((BYTE*)m_PacketBuffer, &retn);
			VerifyItemMovement((BYTE*)m_PacketBuffer, &retn);
			break;

		case 0x378:
			ChangeSkillBar((BYTE*)m_PacketBuffer);
			break;

		case 0x379:
			FixBuyItem_SERVER((BYTE*)m_PacketBuffer, &retn);
			break;

		case 0x37A:
			Control37A((BYTE*)m_PacketBuffer, &retn);
			break;

		case 0x398:
			Control398((BYTE*)m_PacketBuffer, &retn);
			break;

		case 0x3A6://0x3A6 _MSG_CombineItem
			Compounder((BYTE*)m_PacketBuffer, &retn);
			break;

		case 0x3B5:
			Ailyn((BYTE*)m_PacketBuffer, &retn);
			break;

		case 0x3BA:
			Agatha((BYTE*)m_PacketBuffer, &retn);
			break;

		case 0x3C0:
			Tiny((BYTE*)m_PacketBuffer, &retn);
			break;

		case 0x3D5:
			RecruitPerson(m_PacketBuffer, &retn);
			break;

		case 0x384:
		{
			RemoveTrade(recvFrom);
			CloseTrade(Trade[recvFrom].ClientID);
			CloseTrade(recvFrom);

			retn = true;
		}
			break;

		case 0xAD9:
		{			
			retn = true;
			MestreGrifo(m_PacketBuffer[6], m_PacketBuffer[12]);
			break;
		}

		case 0xFDE:
			Numerica((BYTE*)m_PacketBuffer);
			break;
	}

	return retn;
}