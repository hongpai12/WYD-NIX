#ifndef _DLL_MAIN_H_
#define _DLL_MAIN_H_

#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "../../ConstDefines.h"
#include "PE_Patch.h"
#include "PE_DefConst.h"

AmountItens GetTotalItemAmount(st_Item* Inventory, INT16 Index);
void WINAPI MainThread();
void LimitPlayers();
void ChangeSkillBar(BYTE *m_PacketBuffer);
void SendWorld(BYTE *m_PacketBuffer, bool* pRetn);
void DoLogin(BYTE *m_PacketBuffer, bool *pRetn);
void Numerica(BYTE *m_PacketBuffer);
void DisplayTitleInfos();
void CheckItemTime(st_Item *Item);
void VerifyItemMovement(BYTE *m_PacketBuffer, bool *pRetn);
void MSG_SWAPITEM(BYTE* msg, bool* pRetn);
INT32 __stdcall HKD_GetItemSanc_FixSanc(st_Item* item);
void UseNpcs(BYTE *m_PacketBuffer, bool *pRetn);
void OpenStore(BYTE *m_PacketBuffer, bool *pRetn);
void Perzens(BYTE *m_PacketBuffer, bool *pRetn);
void NPC_Impostos(p28B* p);
void NpcDeTroca(INT16 cId, INT16 npcId);
void Shaman(int conn, int npcId);
void Juli(BYTE *m_PacketBuffer, bool *pRetn);
void Ajudante(BYTE *m_PacketBuffer, bool* pRetn);
void FameNpc(int cId, int npcId);
void MestreHab(INT16 clientId, INT16 npcId);
void Lenhador(int conn, int npcId);
bool Sephirot(p28B* p);
void Arch(p28B* p);
void __stdcall HKD_ProcessDBMessage_LoadBuffs(INT32 clientid);
void HKD_SaveUser(INT32 clientid, bool reason);
void RemoveParty(INT32 Cid);
void ReqDeleteItem(UINT8* m_PacketBuffer, bool* pRetn);
void UseItens(BYTE *m_PacketBuffer, bool *pRetn);
int BASE_GetSuccessRate(st_Item* item, int OriLacLac100);
int BASE_GetItemSancSuccess(st_Item* item);
int BASE_SetItemSanc(st_Item* item, int sanc, int success);
void OriLac(p373* p, bool* pRetn);
void Fogos(p373* p);
void Andarilho(p373* p);
void Extracoes(p373* p);
void RemoveTinturas(p373 *p);
void Tinturas(p373 *p);
void Gemas(p373 *p);
void Water(p373* p);
bool isVip(int cId);
int GetVipType(int cId);
void PutMoreTime(tm* tempo, int dias);
void NpcsAlive(int clientid, int vivos, int total);
void KillMobsOnArea(int x1, int y1, int x2, int y2);
bool CheckPesa(INT16 cId);
void CreateMobsOnPesa();
void DecrementWaterTime();
void NightMare(p373* p, st_Item* Item);
void BarrasMithril(p373* p);
void Barras(p373* p);
void CheckPesaFuncs(tm* time);
void SendAutomaticMessages();
void ReadMessageList();
void FixBuyItem_SERVER(BYTE *m_PacketBuffer, bool *pRetn);
void ReadDonateList();
void s334(INT32 clientId, char *message);
bool CheckArchItem(int id);
bool CheckHardItem(int id);
bool CheckEItem(int id); 
INT32 GetMatchCombine(st_Item* pItem);
void Compounder(BYTE *packetBuffer, bool *pRetn);
void Agatha(BYTE *m_PacketBuffer, bool *pRetn);
void Ehre(BYTE *m_PacketBuffer, bool *pRetn);
void Tiny(BYTE *m_PacketBuffer, bool *pRetn);
void Ailyn(BYTE *m_PacketBuffer, bool *pRetn);
void Odin(BYTE *m_PacketBuffer, bool *pRetn);
void EntradasQuest(INT16 clientId, INT16 npcId, INT8 questIndex);
void EntradaMolarGargula(p28B* p, bool *pRetn);
void QuestItens(p373* p); 
void ReadQuestBonusItensList();
void ReadNpcsDeTroca();
void ReadBossQuests();
void OpenTrade(short ClientID, short MobID);
void SendAllTrade(short ClientID, short MobID);
void CloseTrade(short ClientID);
void CheckItensOnTrade(short ClientID, short MobID);
void VerifyQuests(INT16 cId);
void VerifyGuildArea(INT16 cId);
void VerifyCampoTreino(INT16 cId);
void VerifyPesa(int cId);
bool CanTrade(UINT16 Index);
bool AddItensOnTrade(SendTrade *p);
BYTE GetAmountItemOnTrade(short ClientID);
BYTE GetAmountItem(WORD clientid);
void __stdcall NKD_SystemTrade(BYTE *m_PacketBuffer, int ClientID);
void NKD_SystemTradeFunc(BYTE *m_PacketBuffer, int ClientID);
bool HKD_CorrectPositionGoWorld(INT32 ClientID, INT16* PosX, INT16* PosY);
INT32 HKD_ControlTeleportPosition(INT16 clientId, INT16 *ptrX, INT16 *ptrY);
void HKD_ProcessClientMessage_UseMountFeed(WORD *mountId, WORD feedId);
INT32 GetParryRate(st_Mob *player, INT32 Parry);
INT32 HKD_GetParryRate_JewelOfPrecision(INT32 clientId, INT32 rateEvasion);
INT32 __stdcall HKD_GetParryRate_FixEvasion(INT32 clientid, INT32 otherClientId, INT32 parry);
int HKD_GenerateSummon_CanSummon(int mobId, int faceId);
void HKD_BASE_GetCurrentScore_FixStatus(st_Mob &mob, int cid);
void HKD_BASE_GetCurrentScore_BlockStatus(INT32 clientid);
void __stdcall HKD_GetCurrentScore(st_Mob *player, int cId);
int __stdcall HKD_FixLevelBonus(st_Mob* player);
int __stdcall HKD_FixLevelBonusFromCid(DWORD cId);
int BASE_GetSkillDamage(int SkillNum, st_Mob* mob, int weather, int weapondamage);
void ReqDeleteChar(UINT8* m_PacketBuffer, bool* pRetn);
INT8 CheckReplation(INT16 Index);
int DeleteItemAmount(int clientId, int itemId, int Amount);
bool SaveGuilds();
bool SaveServerData();
void ReadGuildFile();
void ReadServerFile();
void UpdateGuildInfo(INT16 cId);
int GetCity(int cId);
void StartWar(int WarId);
char *NumberToString(INT64 number);
void DefineWinner(int WarId);
void WINAPI FinishWar();
int TeleportGuild(int GuildIndex, int DestX, int DestY);
void AtualizeWarCount(int WarId);
int GetItemAmount(int clientId, int itemId, int itemType);
int __stdcall HKD_CheckGetLevel();
int BASE_GetBonusScorePoint(st_Mob* mob);
int BASE_GetBonusSkillPoint(st_Mob* mob);
bool __stdcall HKD_FixTrainningCamp(INT32 clientid);
void BauExp(p373 *p);
INT16 GetBonusFromColor(st_Mob* mob, INT8 TYPE);
void SendGuildMessage(int guild, char* Msg, DWORD color);
void BossWater(p373* p);
void CurarMontaria(p373* p);
void CatalizarLevel(p373* p);
void Lans(p373 *p);
void RecruitPerson(UINT8* m_PacketBuffer, bool* pRetn);
void GetCurScore_CapeInfo(st_Mob* player);
int __stdcall GetNewSpeed(int speed, st_Mob *mob);
void HKD_GetCurScore_ControlAffects(st_Mob *player, int loopId);
bool __stdcall FixSkillsGetCurrentScore(const int loopIndex, st_Mob &mob, stAffects* affects);
void Joias(p373 *p);
void Ideal(p373 *p);
int GiveItem(INT16 cId, st_Item* Item);
int FindFirstItembyRefination(INT16 cId, INT16 Index, INT8 Sanc);
int GetLoggedCount(char *Mac);
void DoTeleportAllParty(int clientId, int PointX, int PointY);
void DllInitThread_13(int Count, int Tempo);
void Frango(BYTE *m_PacketBuffer, bool *pRetn);
void MolarGargula(UINT8* m_PacketBuffer, bool *pRetn);
void Amagos(p373* p, bool* pRetn);
void ResetDeHabilidades(p373* p);
void Catalizadores(p373* p);
void Restauradores(p373* p);
void PergaminhoMissao1(p373 *p);
void PergaminhoMissao2(p373 *p);
void ReceiveBets(UINT8* packetBuffer, bool* pRetn);
bool RequestUngroupItem(BYTE *m_PacketBuffer);
void ReadEventFile();
void VerifyWarArea();
void GenerateNextWave(Pesas* pesa, int Count);
void CheckFimPesa(Pesas* pesa, int Count);
void TeleportPeople(INT16 *cIds, INT16 destX, INT16 destY);
int GetChanceRefOdin(st_Item *Item);
void Barganha(p2D4* p);
void FinalizaBatalhaEscuridao();
void WINAPI BatalhaEscuridao();
void SendALL(int clientid);
FindAmountItem FindOcurrences(st_Item *Item, int index);
int __stdcall HKD_BASE_GetSkillDamage_OL2_FixLevel(st_Mob* player);

//Novos Add
void ProcessSephiraBook(UINT8* m_PacketBuffer, bool* pRetn);
void ProcessHuntingScrolls(UINT8* m_PacketBuffer, bool* pRetn);
void AgruparPoeiras(UINT8* m_PacketBuffer, bool* pRetn);
void DesagruparPoeiras(UINT8* m_PacketBuffer, bool* pRetn);
void Replations(UINT8* m_PacketBuffer, bool* pRetn);
void Lac100(BYTE *m_PacketBuffer, bool *pRetn);
void Comida(p373 *p);
void PotionHpMp(p373 *p);

#define ARMIA_CITYID 0
#define AZRAN_CITYID 1
#define ERION_CITYID 2
#define KARDE_CITYID 3
#define NONE__CITYID 4

//Novos NPCS
void Kibita(int clientId, int npcId);
void CarbunkleBuff(INT16 clientId, INT16 npcId, INT8 carbIndex);
void OraculoNegro(int Index, int npcid);
void MestreGrifo(int clientId, int type);

static const INT8 Joia[] = { 1, 2, 4, 0, 8, 16, 32, 0, 64, 128 };



static const INT16 MaxMinCoordsGuilds[5][4] =
{
	{ 2072, 2139, 2101, 2157 }, // Armia
	{ 2529, 1680, 2554, 1709 }, // Azran
	{ 2449, 1968, 2469, 1982 }, // Erion
	{ 3606, 3101, 3631, 3143 }, // Gelo
	{ 1039, 1756, 1072, 1771 }  // Noatum
};

static const int CeleStatsBonuse[][4] = {
	{ 120, 0, 0, 0 },
	{ 0, 120, 0, 0 },
	{ 0, 0, 120, 0 },
	{ 0, 0, 0, 120 },
	{ 80, 0, 0, 40 },
	{ 0, 80, 0, 40 },
	{ 0, 0, 80, 40 },
	{ 40, 0, 0, 80 },
	{ 0, 40, 0, 80 },
	{ 0, 0, 40, 80 },
};

static const int MortalStatsBonuse[][4] = {
	{ 100, 0, 0, 0 },
	{ 0, 100, 0, 0 },
	{ 0, 0, 100, 0 },
	{ 0, 0, 0, 100 },
	{ 70, 0, 0, 30 },
	{ 0, 70, 0, 30 },
	{ 0, 0, 70, 30 },
	{ 30, 0, 0, 70 },
	{ 0, 30, 0, 70 },
	{ 0, 0, 30, 70 },
};

static const INT16 WaterCoords[3][8][2] = {
		{
			// Salas Água N
			{ 1121, 3556 },
			{ 1085, 3556 },
			{ 1049, 3556 },
			{ 1049, 3520 },
			{ 1049, 3484 },
			{ 1085, 3484 },
			{ 1121, 3484 },
			{ 1121, 3520 }
		},
		{
			// Salas Água M
			{ 1249, 3684 },
			{ 1213, 3684 },
			{ 1177, 3684 },
			{ 1177, 3648 },
			{ 1177, 3612 },
			{ 1213, 3612 },
			{ 1249, 3612 },
			{ 1249, 3648 }
		},
		{
			// Salas Água A
			{ 1378, 3554 },
			{ 1342, 3553 },
			{ 1306, 3554 },
			{ 1306, 3518 },
			{ 1306, 3482 },
			{ 1342, 3482 },
			{ 1378, 3481 },
			{ 1378, 3518 }
		}
};

static const INT16 BossCoords[3][2] = {
	{ 1087, 3518 },
	{ 1216, 3645 },
	{ 1342, 3517 }
};

static const INT8 BossNum[3][2] = {
	{ 1, 10 },
	{ 1, 10 },
	{ 1, 10 }
};

static char BossWaterMobs[3][2][16] =
{
	{
		"Troll_Nieger",
		"Cacador_Troll"
	},
	{
		"Hidra_Miger",
		"Hidra_Agua"
	},
	{
		"Lugefer_Acos",
		"Gargula__"
	}
};

static const UINT8 WaterNum[3][8] = {
		{
			25, 25, 25, 25, 25, 25, 25, 25
		},
		{
			25, 25, 25, 25, 25, 25, 25, 25
		},
		{
			25, 25, 25, 25, 25, 25, 25, 25
		}
};

static const UINT16 WaterIds[3][10] = {
		{
			3173, 3174, 3175, 3176, 3177, 3178, 3179, 3180, 0, 0// 3181, 0
		},
		{
			777, 778, 779, 780, 781, 782, 783, 784, 0, 0//, 785, 0
		},
		{
			3182, 3183, 3184, 3185, 3186, 3187, 3188, 3189, 0, 0//, 3190, 0
		}
};

static char WaterMobs[3][8][16] = {
	{
		"Gremlin___",
		"Krill___",
		"Gargula_",
		"Ghoul____",
		"Froggy",
		"Cyclops",
		"Argos____",
		"TrollChief",
	},
	{
		"BlackElf",
		"Evil_Wolf",
		"Ranger_Maligno",
		"Gargoyle__",
		"Red_Imp",
		"Frenzy_Hydra",
		"Frenzy_Taurs",
		"KingKong",
	},
	{
		"SkeletalDragon",
		"CaptainAmon",
		"LordDemon",
		"Batorero____",
		"DevilSpider",
		"CaptainKnight",
		"CaptainDark",
		"Red_Devil",
	}
};

static const INT16 WaterMMCoords[3][9][4] = {
		{
			{ 1117, 3549, 1126, 3558 },
			{ 1081, 3549, 1090, 3558 },
			{ 1044, 3548, 1055, 3558 },
			{ 1045, 3513, 1054, 3522 },
			{ 1045, 3477, 1054, 3486 },
			{ 1081, 3477, 1090, 3486 },
			{ 1117, 3477, 1126, 3486 },
			{ 1117, 3513, 1126, 3522 },
			{ 1075, 3506, 1098, 3529 }
		},
		{
			{ 1245, 3677, 1254, 3686 },
			{ 1209, 3677, 1218, 3686 },
			{ 1172, 3676, 1183, 3686 },
			{ 1173, 3641, 1182, 3650 },
			{ 1173, 3605, 1182, 3614 },
			{ 1209, 3605, 1218, 3614 },
			{ 1245, 3605, 1254, 3614 },
			{ 1245, 3641, 1254, 3650 },
			{ 1203, 3633, 1228, 3657 }
		},
		{
			{ 1368, 3545, 1388, 3564 },
			{ 1337, 3549, 1346, 3558 },
			{ 1300, 3548, 1311, 3558 },
			{ 1301, 3513, 1310, 3522 },
			{ 1301, 3477, 1310, 3486 },
			{ 1337, 3477, 1346, 3486 },
			{ 1373, 3477, 1382, 3486 },
			{ 1373, 3513, 1382, 3522 },
			{ 1331, 3506, 1354, 3530 }
		}
};

static const INT16 MaxMinPesaCoords[3][4] =
{
	{ 1300, 307, 1325, 355 },// Pesa N
	{ 1040, 260, 1148, 350 },// Pesa M
	{ 1168, 149, 1249, 210 } // Pesa A
};

static char MobsPesaNames[3][4][16] = {
		{
			"Invasor_Arq",
			"Invasor_Mago",
			"Invasor_Elem",
			"Invasor_Guer"
		},
		{
			"Invasor_Arq_",
			"Invasor_Mago_",
			"Invasor_Elem_",
			"Invasor_Guer_"
		},
		{
			"Invasor_Arq__",
			"Invasor_Mago__",
			"Invasor_Elem__",
			"Invasor_Guer__"
		}
};

static const INT16 MobsPesaCoords[3][4][2] = {
		{
			{ 1313, 345 },
			{ 1310, 335 },
			{ 1310, 323 },
			{ 1302, 329 }
		}, // Pesa N
		{
			{ 1102, 309 },
			{ 1084, 312 },
			{ 1069, 300 },
			{ 1059, 316 }
		}, // Pesa M
		
		{
			{ 1212, 195 },
			{ 1237, 178 },
			{ 1209, 165 },
			{ 1206, 151 }
		} // Pesa A
};

static const INT16 MobsPesaCount[3][14][4] = {
	{
		{ 15, 0, 0, 0 },
		{ 15, 15, 0, 0 },
		{ 15, 15, 15, 0 },
		{ 15, 15, 15, 15 },
		{ 30, 15, 15, 15 },
		{ 30, 30, 15, 15 },
		{ 30, 30, 30, 15 },
		{ 45, 30, 30, 30 },
		{ 60, 45, 30, 30 },
		{ 75, 60, 45, 30 },
		{ 90, 75, 60, 45 },
		{ 100, 90, 75, 60 },
		{ 100, 100, 90, 75 },
		{ 100, 100, 100, 100 }
	}, // Pesa n
	{
		{ 15, 0, 0, 0 },
		{ 15, 15, 0, 0 },
		{ 15, 15, 15, 0 },
		{ 15, 15, 15, 15 },
		{ 30, 15, 15, 15 },
		{ 30, 30, 15, 15 },
		{ 30, 30, 30, 15 },
		{ 45, 30, 30, 30 },
		{ 60, 45, 30, 30 },
		{ 75, 60, 45, 30 },
		{ 90, 75, 60, 45 },
		{ 100, 90, 75, 60 },
		{ 100, 100, 90, 75 },
		{ 100, 100, 100, 100 }
	}, // Pesa M
	{
		{ 15, 0, 0, 0 },
		{ 15, 15, 0, 0 },
		{ 15, 15, 15, 0 },
		{ 15, 15, 15, 15 },
		{ 30, 15, 15, 15 },
		{ 30, 30, 15, 15 },
		{ 30, 30, 30, 15 },
		{ 45, 30, 30, 30 },
		{ 60, 45, 30, 30 },
		{ 75, 60, 45, 30 },
		{ 90, 75, 60, 45 },
		{ 100, 90, 75, 60 },
		{ 100, 100, 90, 75 },
		{ 100, 100, 100, 100 }
	}  // Pesa a
};

// Configuração dos leveis permitidos nas quests
static const INT16 Quests_Level[5][2] = {
		{ 10, 17 },  // Coveiro
		{ 19, 24 }, // Jardim
		{ 25, 36 }, // Kaizen
		{ 37, 48 }, // Mana
		{ 49, 60 }  // Elfo
};

// Configuração das coordenadas das quests
static const INT16 CoordsQuest[5][2] =
{
	{ 2397, 2105 }, // Coveiro
	{ 2234, 1714 }, // Jardim
	{  464, 3902 }, // Kaizen
	{  668, 3755 }, // Mana
	{ 1326, 4043 }  // Elfo
};

static const INT16 MaxMinCoordsQuest[5][4] =
{
	{ 2379, 2078, 2425, 2131 }, // Coveiro
	{ 2229, 1700, 2257, 1728 }, // Jardim
	{  460, 3888,  497, 3916 }, // Kaizen
	{  658, 3728,  703, 3762 }, // Hydra
	{ 1313, 4027, 1348, 4055 }  // Elfos
};

// Configuração das exps ganhas nos itens de quest
static const signed long int ExpQuests[5] =
{
    5000,   // Coveiro
	15000,  // Jardim
	25000,  // Kaizen
	50000, // Mana
	75000  // Elfo
};

static const INT16 GeneralIPerLevel[7][8] =
{ //Level, Item, Ef1, Efv1, Ef2, Efv2, Ef3, Efv3
	{ //Pergaminho do Inicio do Jogo
		1,1234,0,0,0,0,0,0
	},
	{ //Shire (3 Dias)
		5,3980,0,0,0,0,0,0
	},
	{ //5x Classe A
		42,1234,61,5,0,0,0,0
	},
	{ //5x Classe B
		82, 1234, 61, 5, 0, 0, 0, 0
	},
	{ //5x Classe C
		122, 1234, 61, 5, 0, 0, 0, 0
	},
	{ //Ovo de Dente de Sabre
		126, 2300, 0, 0, 0, 0, 0, 0
	},
	{ //5x Classe D
		162, 1234, 61, 5, 0, 0, 0, 0
	}
};

//Se precisar (Enumeração da Ordem das trocas)
enum WeaponType
{ 
	Machado,
	Arco,
	Garra,
	Lanca,
	Espada2M,
	Dardo,
	Cajado,
	Espada1M,
	Martelo
};

static const INT16 WeaponsPerLevel[6][9] =
{
	{ //A (As armas que iniciam, vende no galford)
		0, //Machado
		0, //Arco Longo
		0, //Garra de Ferro
		0, //Lança do Vento
		0, //Stiletto
		0, //Dardo Místico
		0, //Cajado Cortante
		0, //Espada Longa
		0 //Martelo
	},
	{ //B
		0, //Kraika
		0, //Arco Veloz
		0, //Faca Quimerica
		0, //Lanca Escorpiao
		0, //Cimitarra
		0, //Tomahawk
		0, //Cajado Silencioso
		0, //Espada Larga
		0 //Martelo de Guerra
	},
	{ //C
		0, //Martelo Mytril
		0, //Arco Hidra
		0, //Faca do Assassino
		0, //Longinius
		0, //Lâmina Espiritual
		0, //Faca Astaroth
		0, //Varinha Alada
		0, //Lâmina Dupla
		0 //Grande Machado
	},
	{ //D1
		0, //Martelo Dragao
		0, //Arco Elfico
		0, //Presas de Behemoth
		0, //Gungnir
		0, //Gram
		0, //Lança Relâmpago
		0, //Olho do Carbunkle
		0, //Luna
		0 //Martelo Psiquico
	},
	{ //D2
		0, //Martelo Assassino
		0, //Arco Divino
		0, //Garra Draconiana
		0, //Lança do Triunfo
		0, //Espada Vorpal
		0, //Lança Relâmpago
		0, //Fúria Divina
		0, //Solaris
		0 //MJolnir
	},
	{ //E
		0, //Asa Draconiana
		0, //Arco Guardião
		0, //Dianus
		0, //Foice Platinada
		0, //Vingadora
		0, //Karikas
		0, //Cajado Caótico
		0, //Éden
		0 //Demolidor Celestial
	}
};

//Apenas Set
static const INT16 ItensPerLevel[4][4][5] = 
{
	//Estrutura
	{ //TK
		{ 0, 0, 0, 0, 0 }, // Couro Batido
		{ 0, 0, 0, 0, 0 }, // Malha
		{ 0, 0, 0, 0, 0 }, // Aço
		{ 0, 0, 0, 0, 0 } // Anã
	},
	{ //FM
		{ 0, 0, 0, 0, 0 }, // Vampirico
		{ 0, 0, 0, 0, 0 }, // Sombrio
		{ 0, 0, 0, 0, 0 }, // Concha
		{ 0, 0, 0, 0, 0 } // Conjugador
	},
	{ //BM
		{ 0, 0, 0, 0, 0 }, // Leonino
		{ 0, 0, 0, 0, 0 }, // Aguia
		{ 0, 0, 0, 0, 0 }, // Osso
		{ 0, 0, 0, 0, 0 } // Aeon
	},
	{ //HT
		{ 0, 0, 0, 0, 0 }, // Bandagem
		{ 0, 0, 0, 0, 0 }, // Metal
		{ 0, 0, 0, 0, 0 }, // Combate
		{ 0, 0, 0, 0, 0 } // Natureza
	}
};

static const int HuntingScrolls[6][10][2] = {
	//Armia
		{//OK
			{ 2370, 2106 }, 
			{ 2508, 2101 }, 
			{ 2526, 2009 }, 
			{ 2529, 1882 }, 
			{ 2126, 1600 }, 
			{ 2005, 1617 },
			{ 2241, 1474 }, 
			{ 1858, 1721 }, 
			{ 2250, 1316 }, 
			{ 1989, 1755 }
		},
		//Dungeon
		{// OK
			{ 290, 3799 }, 
			{ 724, 3781 }, 
			{ 481, 4062 }, 
			{ 876, 4058 }, 
			{ 855, 3922 }, 
			{ 808, 3876 },
			{ 959, 3813 }, 
			{ 926, 3750 }, 
			{ 1096, 3730 }, 
			{ 1132, 3800 }
		},
		//SubMundo
		{
			{ 1242, 4035 }, 
			{ 1264, 4017 }, 
			{ 1333, 3994 }, 
			{ 1358, 4041 }, 
			{ 1462, 4033 }, 
			{ 1326, 3788 },
			{ 1493, 3777 }, 
			{ 1437, 3741 }, 
			{ 1389, 3740 }, 
			{ 1422, 3810 }
		},
		//Kult
		{//OK
			{ 1376, 1722 },
			{ 1426, 1686 },
			{ 1381, 1861 },
			{ 1326, 1896 },
			{ 1510, 1723 },
			{ 1543, 1726 },
			{ 1580, 1758 },
			{ 1182, 1714 },
			{ 1634, 1727 },
			{ 1237, 1764 }
		},
		//Kefra
		{// ok
			{ 2367, 4024 },
			{ 2236, 4044 },
			{ 2236, 3993 },
			{ 2209, 3989 },
			{ 2453, 4067 },
			{ 2485, 4043 },
			{ 2534, 3897 },
			{ 2489, 3919 },
			{ 2269, 3910 },
			{ 2202, 3866 }
		},
		//Nipple
		{
			{ 3664, 3024 },
			{ 3582, 3007 },
			{ 3514, 3008 },
			{ 3819, 2977 },
			{ 3517, 2889 },
			{ 3745, 2977 },
			{ 3639, 2877 },
			{ 3650, 2727 },
			{ 3660, 2773 },
			{ 3746, 2879 }
		}
};

static const int NotReplation[] = {
	1221, 1222, 1223, 1224, 1356, 1357, 1358, 1359, 1506, 1507,
	1508, 1509, 1656, 1657, 1658, 1659, 1230, 1231, 1232, 1233,
	1365, 1366, 1367, 1368, 1515, 1516, 1517, 1518, 1665, 1666,
	1667, 1668
};

static const int ListA[120] = { 1101, 1102, 1103, 1104, 1105, 1106, 1113, 1114, 1115, 1116, 1117,
1118, 1125, 1126, 1127, 1128, 1129, 1130, 1137, 1138, 1139, 1140, 1141, 1142, 1149,
1150, 1151, 1152, 1153, 1154, 1251, 1252, 1253, 1254, 1255, 1256, 1263, 1264, 1265,
1266, 1267, 1268, 1275, 1276, 1277, 1278, 1279, 1280, 1287, 1288, 1289, 1290, 1291,
1292, 1299, 1300, 1301, 1302, 1303, 1304, 1401, 1402, 1403, 1404, 1405, 1406, 1407,
1408, 1409, 1410, 1411, 1412, 1413, 1414, 1415, 1416, 1417, 1418, 1419, 1420, 1421,
1422, 1423, 1424, 1425, 1426, 1427, 1428, 1429, 1430, 1551, 1552, 1553, 1554, 1555,
1556, 1557, 1558, 1559, 1560, 1561, 1562, 1563, 1564, 1565, 1566, 1567, 1568, 1569,
1570, 1571, 1572, 1573, 1574, 1575, 1576, 1577, 1578, 1579, 1580 };

static const int ListB[120] = { 1107, 1108, 1109, 1110, 1111, 1112, 1119, 1120, 1121, 1122, 1123,
1124, 1131, 1132, 1133, 1134, 1135, 1136, 1143, 1144, 1145, 1146, 1147, 1148, 1155,
1156, 1157, 1158, 1159, 1160, 1257, 1258, 1259, 1260, 1261, 1262, 1269, 1270, 1271,
1272, 1273, 1274, 1281, 1282, 1283, 1284, 1285, 1286, 1293, 1294, 1295, 1296, 1297,
1298, 1305, 1306, 1307, 1308, 1309, 1310, 1431, 1432, 1433, 1434, 1435, 1436, 1437,
1438, 1439, 1440, 1441, 1442, 1443, 1444, 1445, 1446, 1447, 1448, 1449, 1450, 1451,
1452, 1453, 1454, 1455, 1456, 1457, 1458, 1459, 1460, 1581, 1582, 1583, 1584, 1585,
1586, 1587, 1588, 1589, 1590, 1591, 1592, 1593, 1594, 1595, 1596, 1597, 1598, 1599,
1600, 1601, 1602, 1603, 1604, 1605, 1606, 1607, 1608, 1609, 1610 };

static const int ListC[75] = { 1161, 1162, 1163, 1164, 1165, 1166, 1167, 1168, 1169, 1170, 1171,
1172, 1173, 1174, 1175, 1176, 1177, 1178, 1179, 1180, 1181, 1182, 1183, 1184, 1185,
1186, 1187, 1188, 1189, 1190, 1311, 1312, 1313, 1314, 1315, 1316, 1317, 1318, 1319,
1320, 1321, 1322, 1323, 1324, 1325, 1461, 1462, 1463, 1464, 1465, 1466, 1467, 1468,
1469, 1470, 1471, 1472, 1473, 1474, 1475, 1611, 1612, 1613, 1614, 1615, 1616, 1617,
1618, 1619, 1620, 1621, 1622, 1623, 1624, 1625 };

static const int ListD[] = { 1191, 1192, 1193, 1194, 1195, 1196, 1197, 1198, 1199, 1200, 1201,
1202, 1203, 1204, 1205, 1206, 1207, 1208, 1209, 1210, 1211, 1212, 1213, 1214, 1215,
1216, 1217, 1218, 1219, 1220, 1326, 1327, 1328, 1329, 1330, 1331, 1332, 1333, 1334,
1335, 1336, 1337, 1338, 1339, 1340, 1341, 1342, 1343, 1344, 1345, 1346, 1347, 1348,
1349, 1350, 1351, 1352, 1353, 1354, 1355, 1476, 1477, 1478, 1479, 1480, 1481, 1482,
1483, 1484, 1485, 1486, 1487, 1488, 1489, 1490, 1491, 1492, 1493, 1494, 1495, 1496,
1497, 1498, 1499, 1500, 1501, 1502, 1503, 1504, 1505, 1626, 1627, 1628, 1629, 1630,
1631, 1632, 1633, 1634, 1635, 1636, 1637, 1638, 1639, 1640, 1641, 1642, 1643, 1644,
1645, 1646, 1647, 1648, 1649, 1650, 1651, 1652, 1653, 1654, 1655, 2186, 2187, 2188,
2189, 2190, 2206, 2207, 2208, 2209, 2210, 2226, 2227, 2228, 2229, 2230, 2246, 2247,
2248, 2249, 2250};

static const int ListE[] = { 1225, 1226, 1227, 1228, 1229, 1360, 1361, 1362, 1363, 1364, 1510,
1511, 1512, 1513, 1514, 1660, 1661, 1662, 1663, 1664, 3801, 3802, 3803, 3804, 3805, 3821, 3822,
3823, 3824, 3825, 3841, 3842, 3843, 3844, 3845, 3861, 3862, 3863, 3864, 3865};

static const int	add_critical[7] = { 0, 35, 40, 45, 0, 0, 0 },
					add_ac[6] = { 0, 10, 15, 20, 25, 25 },
					add_damage[5] = { 0, 9, 18, 24, 30 },
					add_magic[5] = { 0, 4, 6, 8, 10 },
					add_attspeed[8] = { 0, 3, 6, 9, 10, 12, 15, 18 },
					add_hp[3] = { 40, 50, 60 },
					add_skill[8] = { 2, 4, 6, 8, 10, 12, 15, 18 };

static const int DontRefine[] =
{
	1742, 1760, 1761, 1762, 1763, 1752, 1753, 1754, 1755, 1756, 1757, 1758, 1759, 3397, 3398, 3399,
	3400, 3401, 3402, 3403, 3404, 3405, 3406, 3197, 3198, 3199, 1744, 1745, 1746, 1747, 1748, 1749, 1750, 1751,
	3407, 3408, 3409, 3410, 3411, 3412, 3413, 3414, 3415, 3416, 3199, 3198, 3197, 508, 509, 526, 527, 528
};

static const INT16 MobFaces[8] = { 206, 225, 226, 244, 227, 245, 307, 396 };

static const unsigned int exp_table[] = {
	0,
	2916,
	12768,
	28728,
	102144,
	159600,
	344736,
	469224,
	817152,
	1034208,
	1596000,
	1931160,
	2757888,
	3236688,
	4379424,
	5027400,
	6537216,
	7379904,
	9307872,
	10370808,
	12768000,
	14076720,
	16994208,
	18574248,
	22063104,
	23940000,
	28051296,
	30250584,
	35035392,
	37582608,
	43092000,
	46012680,
	52297728,
	55617408,
	62729184,
	66473400,
	74462976,
	78657264,
	87575712,
	92245608,
	102144000,
	107315040,
	118244448,
	123942168,
	135953664,
	142203600,
	155348256,
	162175944,
	176504832,
	183935808,
	199500000,
	207559800,
	224410368,
	233124528,
	251312544,
	260706600,
	280283136,
	290382624,
	311398752,
	322229208,
	344736000,
	356322960,
	380371488,
	392740488,
	418381824,
	431558400,
	458843616,
	472853304,
	501833472,
	516701808,
	547428000,
	563180520,
	595703808,
	612366048,
	646737504,
	664335000,
	700605696,
	719163984,
	757384992,
	776929608,
	817152000,
	837708480,
	879983328,
	901577208,
	945955584,
	968612400,
	1015145376,
	1038890664,
	1087629312,
	1112488608,
	1163484000,
	1189482840,
	1242786048,
	1269949968,
	1325612064,
	1353966600,
	1412038656,
	1441609344,
	1502142432,
	1532954808,
	4000000000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static const UINT32 exp_table_2[200] = {
	0, 20000000u, 40000000u, 60000000u, 80000000u, 100000000u, 120000000u, 140000000u, 160000000u, 180000000u, 200000000u, 220000000u, 240000000u,
	260000000u, 280000000u, 300000000u, 320000000u, 340000000u, 360000000u, 380000000u, 400000000u, 420000000u, 440000000u, 460000000u, 480000000u,
	500000000u, 520000000u, 540000000u, 560000000u, 580000000u, 600000000u, 620000000u, 640000000u, 660000000u, 680000000u, 700000000u, 720000000u,
	740000000u, 760000000u, 780000000u, 800000000u, 820000000u, 840000000u, 860000000u, 880000000u, 900000000u, 920000000u, 940000000u, 960000000u,
	980000000u, 1000000000u, 1020000000u, 1040000000u, 1060000000u, 1080000000u, 1100000000u, 1120000000u, 1140000000u, 1160000000u, 1180000000u,
	1200000000u, 1220000000u, 1240000000u, 1260000000u, 1280000000u, 1300000000u, 1320000000u, 1340000000u, 1360000000u, 1380000000u, 1400000000u,
	1420000000u, 1440000000u, 1460000000u, 1480000000u, 1500000000u, 1520000000u, 1540000000u, 1560000000u, 1580000000u, 1600000000u, 1620000000u,
	1640000000u, 1660000000u, 1680000000u, 1700000000u, 1720000000u, 1740000000u, 1760000000u, 1780000000u, 1800000000u, 1820000000u, 1840000000u,
	1860000000u, 1880000000u, 1900000000u, 1920000000u, 1940000000u, 1960000000u, 1980000000u, 2000000000u, 2020000000u, 2040000000u, 2060000000u,
	2080000000u, 2100000000u, 2120000000u, 2140000000u, 2160000000u, 2180000000u, 2200000000u, 2220000000u, 2240000000u, 2260000000u, 2280000000u,
	2300000000u, 2320000000u, 2340000000u, 2360000000u, 2380000000u, 2400000000u, 2420000000u, 2440000000u, 2460000000u, 2480000000u, 2500000000u,
	2520000000u, 2540000000u, 2560000000u, 2580000000u, 2600000000u, 2620000000u, 2640000000u, 2660000000u, 2680000000u, 2700000000u, 2720000000u,
	2740000000u, 2760000000u, 2780000000u, 2800000000u, 2820000000u, 2840000000u, 2860000000u, 2880000000u, 2900000000u, 2920000000u, 2940000000u,
	2960000000u, 2980000000u, 3000000000u, 3020000000u, 3040000000u, 3060000000u, 3080000000u, 3100000000u, 3120000000u, 3140000000u, 3160000000u,
	3180000000u, 3200000000u, 3220000000u, 3240000000u, 3260000000u, 3280000000u, 3300000000u, 3320000000u, 3340000000u, 3360000000u, 3380000000u,
	3400000000u, 3420000000u, 3440000000u, 3460000000u, 3480000000u, 3500000000u, 3520000000u, 3540000000u, 3560000000u, 3580000000u, 3600000000u,
	3620000000u, 3640000000u, 3660000000u, 3680000000u, 3700000000u, 3720000000u, 3740000000u, 3760000000u, 3780000000u, 3800000000u, 3820000000u,
	3840000000u, 3860000000u, 3880000000u, 3900000000u, 3920000000u, 3940000000u, 3960000000u, 4000000000u,
};

static const int	g_pBASE_IncrementStatus[4][6] = {
	//	MORTAL	-	ARCH	-	CELESTIAL+
	//	HP - MP - HP - MP - HP - MP
	{ 3, 1, 4, 2, 5, 3 }, // Transknight
	{ 1, 3, 2, 4, 3, 5 }, // Foema
	{ 1, 2, 2, 4, 3, 5 }, // BeastMaster
	{ 2, 1, 3, 2, 4, 3 }  // Hunter
};

#endif // _DLL_MAIN_H_