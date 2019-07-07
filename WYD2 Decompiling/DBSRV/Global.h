#ifndef GLOBAL_H
#define GLOBAL_H

extern FILE*		g_pLogFile;
extern int32_t		ServerGroup;
extern int32_t		Sapphire;
extern int32_t		MaxDeleteLevel;
extern int32_t		CreateAccountAtLogin;
extern int32_t		ChargedGuildList[MAX_GUILDZONE][MAX_SERVERNUMBER];
extern int32_t		TransGuildConn[20480];
extern int32_t		TransGuildIndex[20480];
extern int32_t		CurrentTransInfo;
extern HMENU		hMainMenu;
extern HWND			hWndMain;
extern int32_t		LastSendTime;
extern uint32_t		CurrentTime;
extern uint8_t		LocalIP[4];
extern uint64_t		SecCounter;
extern uint64_t		MinCounter;
extern uint64_t		HourCounter;
extern int32_t		TransperCharacter;
extern int32_t		UserConnection[MAX_SERVERNUMBER];

extern char			adminclientid[256];
extern char			adminclientpass[256];

extern CPSock		AdminSocket;
extern CPSock		AdminClient;
extern CPSock		ListenSocket;
extern CUser		pAdmin[MAX_ADMIN];
extern uint32_t		pAdminIP[MAX_ADMIN];
extern CUser		TempUser;
extern CUser		pUser[MAX_SERVERNUMBER];
extern char			g_pServerlist[MAX_SERVERGROUP][MAX_SERVERNUMBER][64];

extern uint16_t		g_pGuildWar[65535];
extern uint16_t		g_pGuildAlly[65535];

extern CFileDB		g_pFileDB;
extern STRUCT_MOB	g_pBaseSet[MAX_CLASS];
extern STRUCT_ITEMLIST g_pItemList[MAX_ITEMLIST];
extern STRUCT_MOUNTBONUS g_pMountBonus[MAX_MOUNT];
extern uint8_t		g_pItemGrid[MAX_ITEMGRID][MAX_ITEMGRID];
extern char			g_pEffectNameTable[MAX_ITEMEFFECT][EFFECTNAME_LENGTH];

extern HINSTANCE	g_pInstance;

extern uint8_t		g_pKeyword[512];



#endif // !GLOBAL_H
