#ifndef __PACKET_CONTROL_H__
#define __PACKET_CONTROL_H__

#include <Windows.h>

#include "GetCreateMob.h"
#include "SendScore.h"
#include "SendEtc.h"
#include "GetCreateMobTrade.h"
#include "SendAffect.h"
#include "AddExp.h"
#include "SendTrade.h"
#include "ExitParty.h"
#include "PE_Patch.h"
#include "Functions.h"
#include "Send.h"
#include "Funcoes.h"

#define SERVER_SIDE 0
#define CLIENT_SIDE 1

void* HKD_PacketControl(UINT32* pSocket, void* pBuffer, UINT32 packetSize, UINT32 typeControl);

#define BASE_NAME		0x07B318C8
#define BASE_CHARNAME	0x07B31FF4
#define BASE_CARGO		0x07B31A9C
#define BASE_STATUS		0x07B318E0
#define BASE_WHISPER	0x07B32314
#define BASE_SOCKET		0x07B318E8
#define BASE_BUFFER		0x07B318F0
#define BASE_DBSOCKET	0x00A16990

#define NORMAL_LEVEL 0
#define MOD_LEVEL    1
#define ADMIN_LEVEL  2

#define NORMAL_PLAYER 0
#define FOGO_VIP 1
#define GELO_VIP 2

#define MSG_COLOR_GRITO  0xFF00CD00
#define MSG_COLOR_GRITOV 0xFFFF2A2A
#define MSG_COLOR_GRITOA 0xFFD9D919
#define MSG_COLOR_NOTICE 0xFFFF4500

//New OPCodes
void FixAddPoint_CLIENT(BYTE *m_PacketBuffer, bool *pRetn);
void Control37A(BYTE *m_PacketBuffer, bool *pRetn);
void Control398(BYTE *m_PacketBuffer, bool *pRetn);
void Commands(BYTE* m_PacketBuffer, bool *pRetn);
INT32 HKD_ProcessClientMessage_ApplyBonus(BYTE *buffer, INT32 clientid);
int __stdcall GetNewLevelInfo(DWORD cId);

//New PacketControl
bool SendPacketControl(BYTE* newPacketBuffer, PacketHeader* oldPacketBuffer, int clientId);
bool RecvPacketControl(PacketHeader* packetBuffer, int clientId);
int __stdcall HKD_AddMessage(int instModule, PacketHeader *oldPacketBuffer, int packetSize);
BYTE* __stdcall HKD_ReadMessage(int instModule, int *unkParam1, int *unkParam2);

#endif // __PACKET_CONTROL_H__