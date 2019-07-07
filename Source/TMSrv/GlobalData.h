#ifndef __GLOBAL_DATA_H__
#define __GLOBAL_DATA_H__

#include <Windows.h>
#include <stdio.h>
#include "../Common/st_Base.h"

#include "DLLMain.h"

extern GuildInfos		 Guilds[0xFFFF];
extern stBuffer			 mBuffer[30000];
extern CUser			 Users[1000];
extern nInfo			 nBuffer[1000];
extern sInfos			 Servidor;
extern st_ItemListCSV	 itemlistBuffer[6500];
extern Aguas			 Agua[3];
extern Pesas			 Pesa[3];
extern Donate            DonateList[300];
extern QuestItensOnEnter QuestItensEntered[5];
extern st_Trade			 Trade[1000];
extern Resets			 ResetList[50];
extern Tabs				 TabList[50];
extern Quests            QuestList[5];
extern STRUCT_SPELL      g_pSpell[MAX_SKILLLIST];
extern DWORD             MagicIncrementBonus[30000];
extern stMOBGener		 Genner[10000];
extern WarInfos          War[4];
extern Events            Eventos;

//typedef struct
//{
//	MYSQL cConn;
//	bool status;
//
//} MySqlServerInfos;
//
//extern MySqlServerInfos  SqlInfos;

st_ItemListCSV *GetItemlistBuffer();

CUser* GetUserFromIndex(INT16 index);

Surprise *Get_supv();

st_Mob* _GetMobFromIndex(INT16 index);

sTeleport *Getteleport();

void LoadTeleport();

#endif // __GLOBAL_DATA_H__