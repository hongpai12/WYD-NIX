#ifndef __FUNCOES_H__
#define __FUNCOES_H__

#include "DLLMain.h"
#include "PacketControl.h"
#include "Functions.h"
#include "Send.h"
#include "../Common/ItemEffect.h"

static const INT16 NotSell[] = {
	508, 509, 526, 527, 528, 3503, 3504, 3505, 3506, 3980, 3981, 3982, 3983, 3984, 3985, 3986,
	3987, 3987, 3988, 3989, 4152, 4153, 4155, 4156
};

static int BaseSIDCHM[4][6] = {
		{ 8, 4, 7, 6, 80, 45 }, // Transknight
		{ 5, 8, 5, 5, 60, 65 }, // Foema
		{ 6, 6, 9, 5, 70, 55 }, // BeastMaster
		{ 8, 9, 13, 6, 75, 60 }  // Hunter
};

#define MORTAL 1
#define ARCH 2
#define SEMIDEUS 2
#define DEUS 3
#define CELESTIAL 3
#define SCELESTIAL 4
#define HARDCORE 5
#define SHARDCORE 6
#define MAX_HPMP 64000
#define MAX_STATUS_POINTS 32000
#define EQUIP 0
#define INVENTORY 1
#define CARGO 2
#define INFO 0
#define ERROR 1
#define WARN 2
#define DEBUG 3
#define FORCA 0
#define INTEL 1
#define DEXTE 2
#define CONST 3

#define BAN_3_HOURS 1
#define BAN_1_DAY   2
#define BAN_3_DAYS  3
#define BAN_1_WEEK  4
#define BAN_1_MONTH 5
#define BAN_ETERNAL 6

//Não declaradas aqui
int BASE_GetHpMp(st_Mob* mob);
void AmountMinus(st_Item *item);
int GetFirstSlot(int clientId, int itemId, int itemType);
void startLog();
void sendLog(INT8 type, INT16 cId, char* Log, ...);
void Sendpack(BYTE *bufr, int clientId, int paksize);
INT64 SystemTimeDifference(const SYSTEMTIME time1, const SYSTEMTIME time2);
bool FileExist(const char* Name);
bool CheckBan(const char *login);
void SendBan(const char *login);
void SendBan(const char *login, SYSTEMTIME banTime);
bool SaveAccount(int clientId);
bool CanAgroup(st_Item* Item);
INT8 GetMaxAmount(INT16 Index);
int GetItemAmount(st_Item* Item);
void PutAmount(st_Item* Item, int Amount);
void TimeLimitCheck(st_Item *Item, INT16 cId, bool Fairy, char* Name);
unsigned long int GetCoinOnItens(st_Item *Inventory);
void SendClientMsg(int cId, char* Msg, ...);
int GetUserByName(char* Name);
void sD1D(int clientid, int destino, char *msg, DWORD cor);
int GetItemSanc(st_Item* Item);
int GetItemSanc_OL2(st_Item* Item);
int BASE_SetItemSanc(st_Item* item, int sanc, int success);
bool SetItemSanc(st_Item* Item, int Sanc);
void SendAll(int clientid);
void RefinarItemMais(st_Item *item, int value);
void ReadResetList();
void ReadTabList();
void ReadChannel();
void Composition(st_Window *pServer, st_Mob *player);

#endif