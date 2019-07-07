#ifndef __MAIN_H_
#define __MAIN_H_
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <mshtmhst.h>
#include <d3d9.h>
#include <tlhelp32.h>

#include "PE_Hook.h"
#include "../ConstDefines.h"


void SystemProcessesScan();
bool ScanProcessMemory(HANDLE process);
void MainThread();
extern "C" __declspec(dllexport) void Protection();

void NKD_CorrectBone();
void NKD_LoadFile();
void HKD_AddIndex();
void CorrectBone(INT32 id, INT32* bone);
void LoadFile(INT32 ebp, INT32 id);
void LoadCostume(INT32 ebp, char* texture, char* msh1, char* msh2, char* msh3, char* msh4, char* msh5, char* msh6);
void AddIndex(int id, int *value);
void NKD_ViewLevelMount();
void NKD_ItemDescription();
void AddLine();
void NKD_AddItemSanc();
void NKD_ChangeD1DColor();
void NKD_ChangeCliver_02();
void NKD_ChangeCliver_01();

void NKD_FormatGoldString_01();
void NKD_FormatGoldString_02();
void NKD_FormatGoldString_03();

void NKD_ChangeMagicIncrement_01();
void NKD_ChangeMagicIncrement_02();
void NKD_ChangeMagicIncrement_03();
void NKD_ChangeMagicIncrement_04();
void NKD_ChangeMagicIncrement_05();

static int(*AddMessage)(BYTE *, int) = (int(__cdecl *)(BYTE *, int)) 0x0549CBF;

; // Lib: CPSock.obj Class : CPSock

extern DWORD newMagicIncrement;

HMODULE LoadPage();

extern DWORD ColorD1D;

typedef struct
{
	INT16 Size; // 0 - 1

	INT8 Key; // 2
	INT8 CheckSum;//3

	INT16 PacketId; //4 - 5
	INT16 ClientId;

	UINT32 TimeStamp;
} PacketHeader;

struct p334{
	PacketHeader Header;
	char Cmd[16];
	char Arg[96];
};


struct GuildInfo
{
	INT8 Cidadania; // Canal de cidadania da guild

	UINT16 GuildID, // ID da guild
		Members; // Número total de membros

	char LiderName[16]; // Nick do lider
	char GuildName[20]; // Nome da guild

	char SubLiderName[3][16]; // Nome dos Subs

	UINT8 Kingdom; // Reino da guild, para comparação na hora de recrutar e para igualar
	// No GetCreateMob

	INT64 Points; // Número total de pontos da guild
	INT16 MobDeadCount; // Conta de mobs mortos no pilar
	INT8 CidadeApostada, // Cidade na qual fez aposta, lembrando que só é possível apostar em 1 cidade por vez
		CidadePossuida;
	INT64 ValorApostado; // Valor de pontos apostado pela guild
};

struct pAAA
{
	PacketHeader Header;

	GuildInfo Guild;
};

extern GuildInfo Guild;

void Initialize();
void Change_D1D_color(int color);
void PacketControl(char *Data, int *Lenght, int Type);
bool HKD_ReadPacket(PacketHeader* packet);
bool HKD_SendPacket(PacketHeader* packet);
void Send();
void Recv();
void NKD_ShowRefine();

void NKD_FixAddPoint_1();
void NKD_FixAddPoint_2();
void NKD_FixAddPoint_3();
void NKD_FixAddPoint_4();

void NKD_FixAddPoint_CorrectVis();
int HKD_ChangeTabColor(char* msg);
void NKD_ChangeTabColor();

void NKD_CorrectPet();
void NKD_CorrectPet_2();
void NKD_CorrectPet_3();
void NKD_CorrectPet_4();

DWORD WINAPI Thread_Process();
void SetNewExp_Addr();
void SetGodExp_Addr();

#define send_jmp 0x0428EB6// 7.55 
#define recv_jmp 0x04289EF// 7.55

#define send_call 0x0428EAC// 7.55 4267EC 7.53 0x0425297 7.60
#define recv_call 0x04288F9// 7.55

#define SEND_TYPE 0
#define RECV_TYPE 1

struct p20D
{
	PacketHeader Header; // 0 - 11

	char Login[16]; // 12 - 27
	char Password[12]; // 28 - 39

	INT32 Version; // 40 - 43
	INT32 Unknows; // 44- 47

	char Keys[16]; // 48 - 63
	char IPAddress[16]; // 64 - 79
};

struct st_Item
{
	INT16 Index;

	struct
	{
		UINT8 Index;
		UINT8 Value;
	} Effect[3];
};

#define EF1 Effect[0].Index
#define EFV1 Effect[0].Value
#define EF2 Effect[1].Index
#define EFV2 Effect[1].Value
#define EF3 Effect[2].Index
#define EFV3 Effect[2].Value

/*
Ahack
*/

#define MAX_DUMP_OFFSETS 20
#define MAX_DUMP_SIZE    32
#define MAX_PROCESS_DUMP 20

typedef struct Shield_ProcessDump
{
	unsigned int  Offset;
	unsigned char MemDump[MAX_DUMP_SIZE];
	char          CheatName[100];
} *PShield_ProcessDUMP;
extern Shield_ProcessDump ProcessesDump[MAX_PROCESS_DUMP];

#endif