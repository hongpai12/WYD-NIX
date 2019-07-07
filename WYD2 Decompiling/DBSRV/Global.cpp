#include "stdafx.h"

int32_t		ServerGroup;
int32_t		Sapphire;
int32_t		MaxDeleteLevel;
int32_t		CreateAccountAtLogin;
int32_t		ChargedGuildList[MAX_GUILDZONE][MAX_SERVERNUMBER];
int32_t		TransGuildConn[20480];
int32_t		TransGuildIndex[20480];
int32_t		CurrentTransInfo;
HMENU		hMainMenu;
HWND		hWndMain;
int32_t		LastSendTime;
uint32_t	CurrentTime;
uint8_t		LocalIP[4];
uint64_t	SecCounter;
uint64_t	MinCounter;
uint64_t	HourCounter;
int32_t		TransperCharacter;
int32_t		UserConnection[MAX_SERVERNUMBER];

char		adminclientid[256];
char		adminclientpass[256];

CPSock		AdminSocket;
CPSock		AdminClient;
CPSock		ListenSocket;
CUser		pAdmin[MAX_ADMIN];
uint32_t	pAdminIP[MAX_ADMIN];
CUser		TempUser;
CUser		pUser[MAX_SERVERNUMBER];
char		g_pServerlist[MAX_SERVERGROUP][MAX_SERVERNUMBER][64];

uint16_t		g_pGuildWar[65535];
uint16_t		g_pGuildAlly[65535];

CFileDB		g_pFileDB;
STRUCT_MOB	g_pBaseSet[MAX_CLASS];
FILE*		g_pLogFile = nullptr;
STRUCT_ITEMLIST g_pItemList[MAX_ITEMLIST];
STRUCT_MOUNTBONUS g_pMountBonus[MAX_MOUNT];

uint8_t		g_pItemGrid[MAX_ITEMGRID][MAX_ITEMGRID] =
{
	{ 1, 0, 0, 0, 0, 0, 0, 0 },
	{ 1, 0, 1, 0, 0, 0, 0, 0 },
	{ 1, 0, 1, 0, 1, 0, 0, 0 },
	{ 1, 0, 1, 0, 1, 0, 1, 0 },
	{ 1, 1, 0, 0, 0, 0, 0, 0 },
	{ 1, 1, 1, 1, 0, 0, 0, 0 },
	{ 1, 1, 1, 1, 1, 1, 0, 0 },
	{ 1, 1, 1, 1, 1, 1, 1, 1 }
};

char		g_pEffectNameTable[MAX_ITEMEFFECT][EFFECTNAME_LENGTH];

HINSTANCE	g_pInstance;

uint8_t		g_pKeyword[512];