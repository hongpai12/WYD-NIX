#include <Windows.h>
#include <time.h>

typedef char				int8;
typedef short				int16;
typedef int					int32;
typedef long long           int64;

typedef signed char			sint8;
typedef signed short		sint16;
typedef signed int			sint32;
typedef signed long long    sint64;

typedef unsigned char		uint8;
typedef unsigned short		uint16;
typedef unsigned int		uint32;
typedef unsigned long long  uint64;

typedef unsigned char       uchar;
typedef unsigned short      ushort;
typedef unsigned int        uint;
typedef unsigned long       ulong;
typedef unsigned long long  ulonglong;

typedef const char          cchar;
typedef const unsigned char cuchar;

#define FALSE	0
#define TRUE	1


#define ISALNUM(c) (isalnum((unsigned char)(c)))
#define ISALPHA(c) (isalpha((unsigned char)(c)))
#define ISCNTRL(c) (iscntrl((unsigned char)(c)))
#define ISDIGIT(c) (isdigit((unsigned char)(c)))
#define ISGRAPH(c) (isgraph((unsigned char)(c)))
#define ISLOWER(c) (islower((unsigned char)(c)))
#define ISPRINT(c) (isprint((unsigned char)(c)))
#define ISPUNCT(c) (ispunct((unsigned char)(c)))
#define ISSPACE(c) (isspace((unsigned char)(c)))
#define ISUPPER(c) (isupper((unsigned char)(c)))
#define ISXDIGIT(c) (isxdigit((unsigned char)(c)))
#define TOASCII(c) (toascii((unsigned char)(c)))
#define TOLOWER(c) (tolower((unsigned char)(c)))
#define TOUPPER(c) (toupper((unsigned char)(c)))

//LIMITE WYD 754
#define MAX_AFFECT              32
#define MAX_DATABASE            6
#define MAX_TELEPORT            256
#define MAX_GAMESERVER          6
#define MAX_GUILD_ZONE          5
#define MAX_MOB_BABY            38
#define MAX_PARTY               13
#define MAX_EFFECT              12
#define MAX_SIZENAME            64
#define MAX_EFFECT_INDEX        127
#define MAX_EFFECT_NAME         16
#define MAX_MESH_BUFFER         16
#define MAX_SCORE_BUFFER        32
#define MAX_PARTY_LOAD          300
#define EMU_VERSION             4.0
#define MAX_GUILD               204

#define MAX_USER                750

//LIMITE LEITURA DA DATA
#define __MAX_NPCGENER          10000
#define __MAX_SPAWN_LIST        30000
#define MAX_NPCGENER            __MAX_NPCGENER
#define MAX_SPAWN_LIST          __MAX_SPAWN_LIST
#define MAX_INITITEM_LIST       4096
#define MAX_SKILLDATA           150
#define MAX_GROUP               128

//CLIVER
#define CLIENT_VERSION 755

//INDEX SESSIONS
#define NOT_SESSION             0
#define LOGING_SESSION          1
#define CHARLIST_SESSION        2
#define CREATING_SESSION        4
#define DELETING_SESSION        8
#define ENTERING_SESSION        16
#define NUMERICA_SESSION        32
#define NUMERICA_SESSION2       33
#define GAME_SESSION            64

//WAITING MESSAGES TYPES
#define WAITING_HELLO           0
#define WAITING_LOGIN           1 // Não usado no char_server.
#define WAITING_CLIENT          2
#define WAITING_DATA            3
#define WAITING_SITE            4

//Pesa,Carta
#define PesaN                   0
#define PesaM                   1
#define PesaA                   2
#define CartaN                  0
#define CartaM                  1
#define CartaA                  2

//Color
#define Snow                    0xFFFFFAFA
#define GhostWhite              0xFFF8F8FF
#define WhiteSmoke              0xFFF5F5F5
#define Gainsboro               0xFFDCDCDC
#define FloralWhite             0xFFFFFAF0
#define OldLace                 0xFFFDF5E6
#define Linen                   0xFFFAF0E6
#define AntiqueWhite            0xFFFAEBD7
#define PapayaWhip              0xFFFFEFD5
#define BlanchedAlmond    0xFFFFEBCD
#define Bisque    0xFFFFE4C4
#define PeachPuff    0xFFFFDAB9
#define NavajoWhite    0xFFFFDEAD
#define Moccasin    0xFFFFE4B5
#define Cornsilk    0xFFFFF8DC
#define Ivory    0xFFFFFFF0
#define LemonChiffon    0xFFFFFACD
#define Seashell    0xFFFFF5EE
#define Honeydew    0xFFF0FFF0
#define MintCream    0xFFF5FFFA
#define Azure    0xFFF0FFFF
#define AliceBlue    0xFFF0F8FF
#define lavender    0xFFE6E6FA
#define LavenderBlush    0xFFFFF0F5
#define MistyRose    0xFFFFE4E1
#define White    0xFFFFFFFF
#define Black    0xFF000000
#define DarkSlateGray    0xFF2F4F4F
#define DimGrey    0xFF696969
#define SlateGrey    0xFF708090
#define LightSlateGray    0xFF778899
#define Grey    0xFFBEBEBE
#define LightGray    0xFFD3D3D3
#define MidnightBlue    0xFF191970
#define NavyBlue    0xFF000080
#define CornflowerBlue    0xFF6495ED
#define DarkSlateBlue    0xFF483D8B
#define SlateBlue    0xFF6A5ACD
#define MediumSlateBlue    0xFF7B68EE
#define LightSlateBlue    0xFF8470FF
#define MediumBlue    0xFF0000CD
#define RoyalBlue    0xFF4169E1
#define Blue    0xFF0000FF
#define DodgerBlue    0xFF1E90FF
#define DeepSkyBlue    0xFF00BFFF
#define SkyBlue    0xFF87CEEB
#define LightSkyBlue    0xFF87CEFA
#define SeaGreen    0xFF2E8B57
#define MediumSeaGreen    0xFF3CB371
#define LightSeaGreen    0xFF20B2AA
#define PaleGreen    0xFF98FB98
#define SpringGreen    0xFF00FF7F
#define LawnGreen    0xFF7CFC00
#define Green    0xFF00FF00
#define Yellow 0xFFFFFF00

//SOCKETS INDEX
#define SERVER_ID               0
#define DATABASE_ID             1

//SIZE AND TYPES SERVER
#define BASE_CHARACTER          0
#define BASE_MOB                1000
#define SIZEOF_CHARACTER        MAX_PLAYER
#define SIZEOF_MOB              512
#define NO_MORE_ACTION          0xFFFFFFFF

//SPAWN TYPES
#define SPAWN_NORMAL            0 // Somente aparece
#define SPAWN_TELEPORT          2 // Efeito usado quando o personagem nasce ou eh teleportado
#define SPAWN_BABYGEN           10 // Efeito de quando uma cria nasce (75x only)

//DELETE MOB TYPES
#define DELETE_NORMAL           0 // Somente desaparece
#define DELETE_DEAD             1 // Animacao da morte do spawn
#define DELETE_DISCONNECT       2 // Efeito de quando o personagem sai do jogo
#define DELETE_UNSPAWN          3 // Efeito quando os monstros ancts somem

//MOVE TYPES
#define ROUTE_NORMAL            0
#define MOVE_NORMAL             0
#define MOVE_TELEPORT           1

//TYPE SLOTS
#define EQUIP_TYPE              0
#define INV_TYPE                1
#define STORAGE_TYPE            2

//EQUIP SLOTS
#define FACE_SLOT               0
#define HELM_SLOT               1
#define ARMA1_SLOT              6
#define ARMA2_SLOT              7
#define MOUNT_SLOT              14
#define CAPE_SLOT               15

//CITY ID
#define ARMIA                   1
#define ERION                   2
#define AZRAN                   3
#define NOATUN                  4
#define GELO                    5

//STATUS MASTERYS
#define WYD_fMaster             5
#define WYD_sMaster             10
#define WYD_tMaster             15

//DELAY DE MOVIMENTO
#define AI_DELAY_MOVIMENTO      2700

//Quests
#define QUEST_C_ARMIA_1         1
#define QUEST_C_ARMIA_2         2
#define QUEST_C_ARMIA_3         3
#define QUEST_COMECO            4

//INDEX DAS EVOLUCOES DE CLASSES
#define CLASS_MORTAL            1
#define CLASS_ARCH              2
#define CLASS_CELESTIAL         3
#define CLASS_SUB_CELESTIAL     4
#define CLASS_HARDCORE          5

//MUTAÇÂO MASTER
#define LOBISOMEM               1
#define URSO                    2
#define ASTAROTH                3
#define TITA                    4
#define EDEN                    5

//MUTAÇÂO FACES
#define LOBISOMEM_FACE          22
#define URSO_FACE               23
#define ASTAROTH_FACE           24
#define TITA_FACE               25
#define EDEN_FACE               32

//BUFS INDEX
#define LENTIDAO                1
#define FM_VELOCIDADE           2
#define RESISTENCIA_N           3
#define EVASAO_N                5
#define POCAO_ATK               6
#define VELOCIDADE_N            7
#define cADD                    8
#define FM_BUFFATK              9
#define ATKMENOS                10
#define FM_ESCUDO_MAGICO        11
#define DEFESA_N                12
#define TK_ASSALTO              13
#define TK_POSSUIDO             14
#define FM_SKILLS               15
#define BM_MUTACAO              16
#define TK_AURAVIDA             17
#define FM_CONTROLE_MANA        18
#define HT_IMUNIDADE            19
#define VENENO                  20
#define HT_MEDITACAO            21
#define FM_TROVAO               22
#define BM_AURA_BESTIAL         23
#define TK_SAMARITANO           24
#define BM_PROTELEMENT          25
#define HT_EVASAO_APRIMORADA    26
#define HT_GELO                 27
#define HT_INIVISIBILIDADE      28
#define LIMITE_DA_ALMA          29
#define PvM                     30
#define HT_ESCUDO_DOURADO       31
#define CANCELAMENTO            32
#define MUTACAO2                33
#define COMIDA                  34
#define BONUS_HP_MP             35
#define HT_VENENO               36
#define HT_LIGACAO_ESPCTRAL     37
#define HT_TROCAESP             38
#define BAU_EXP                 39


//EVOCAÇOES & MONTARIAS
#define CONDOR                  0
#define JAVALI                  1
#define LOBO                    2
#define E_URSO                  3
#define TIGRE                   4
#define GORILA                  5
#define DRAGAO_NEGRO            6
#define SUCCUBUS                7
#define PORCO                   8
#define JAVALI_                 9
#define LOBO_                   10
#define DRAGAO_MENOR            11
#define URSO_                   12
#define DENTE_DE_SABRE          13
#define SEM_SELA                14
#define FANTASMA                15
#define LEVE                    16
#define EQUIPADO                17
#define ANDALUZ                 18
#define SEM_SELA_               19
#define FANTASMA_               20
#define LEVE_                   21
#define EQUIPADO_               22
#define ANDALUZ_                23
#define FENRIR                  24
#define DRAGAO                  25
#define GRANDE_FENRIR           26
#define TIGRE_DE_FOGO           27
#define DRAGAO_VERMELHO         28
#define UNICORNIO               29
#define PEGASUS                 30
#define UNISUS                  31
#define GRIFO                   32
#define HIPPO_GRIFO             33
#define GRIFO_SANGRENTO         34
#define SVADILFARI              35
#define SLEIPNIR                36

//EMOTION 50 22
#define p39x_MISS               -3
#define p39x_NORMAL             0
#define p39x_D_NORMAL           1 // Divide o atk em 2
#define p39x_CRITICO            2
#define p39x_D_CRITICO          3 // Divide o atk em 2
#define p39x_DANO_INVISIVEL     4
#define p39x_DANO_ADMIN         5 // Confirmar, mas o dano almenta absurdamente
#define p39x_DANO_ADMIN_CRIT    7
#define p39x_SUPER              8 //Atck com aquele shok de ARCH+
#define p39x_D_SUPER            9
#define p39x_S_CRITICO          10
#define p39x_D_S_CRITICO        11
#define p39x_S_DANO_INVISIVEL   12


/*********************
** ESTRUTURAS BASES **
**********************/
typedef struct
{
    short ItemID;

    struct
    {
        uchar Index;
        uchar Value;
    } Effect[3];
} sItem;

#define item_data       sItem
#define EF1             Effect[0].Index
#define EFV1            Effect[0].Value
#define EF2             Effect[1].Index
#define EFV2            Effect[1].Value
#define EF3             Effect[2].Index
#define EFV3            Effect[2].Value

#define _100kk           100000000
#define _50kk            50000000

typedef struct
{
    short X;
    short Y;
} sPoint;

typedef struct
{
	uchar Index;
    uchar Master;

    short Value;
    int Time;
} sAffect;

typedef struct
{
    sItem TradeItem[15];
    BYTE TradeItemSlot[15];
    WORD OtherClient;
    int Gold;
    bool Confirma;
    bool Waiting;
} sBufferTrade;



typedef struct
{
    int group[3][2];
    int Time[3];
    sPoint Teleport[3];
    sPoint AreaMax[3];
    sPoint AreaMin[3];
    int Mobs[3][9];
    int Npcs[3][8];
    int NpcsVivos[3];
    int Status[3];
} sPesadelo;

typedef struct
{
    int Sala[25];
    int CurrentQuestion[25];
    int Mobs[25];
    int Time[25];
    int Status[25];
    bool Respostas[25];
    char Perguntas[25][128];
    sPoint O[25];
    sPoint X[25];
} sCuboMaldade;

typedef struct
{
    char GuildName[16]; //Guild Dona (se estiver em branco, coloca que nao tem dona
    int GuildOwn; //Index da guild Dona
    int Gold; //Gold da cidade
    int Apostas; //Maior aposta (Nao pode mostrar no site
    int Taxas; // Tax Colector
    int Desafiante; //Guild Desafiante na Guerra
    char GuildDf[16]; //Nome da Guild Desafiante
    bool InWar; //True =  Está em Guerre, False = Nao esta em guerra.
} sCity;

typedef struct
{
    sCity Armia;
    sCity Erion;
    sCity Azran;
    sCity Noatun;
    sCity Gelo;
} sCityManager;

typedef struct
{
    short Level;
	short Defense;
	short Attack;

    uchar Merchant : 4;
    uchar Direction : 4;
    char Speed : 4;
    char ChaosRate : 4;

	short maxHP, maxMP;
	short curHP, curMP;

	short STR, INT;
	short DEX, CON;

	uchar wMaster;//1
	uchar fMaster;//2
	uchar sMaster;//3
	uchar tMaster;
} sStatus;

typedef struct
{
    bool DuelStart;
    int Time;
    int p1;
    int p2;
} sDuelo;

typedef struct
{
    bool EventStart;
    int Status;
    int Time;
    int P_Count;
    sPoint AreaMin;
    sPoint AreaMax;
    sPoint TeleportPlayers[4];
    int Registers[200];
} sDM;

typedef struct
{
	char Name[16];
    char CapeInfo;

    struct
    {
        uchar Merchant : 6;
        uchar CityID : 2;
    } Info;

    ushort GuildIndex;
    uchar ClassInfo;

	struct
    {
        BYTE SlowMov : 1;
        BYTE DrainHP : 1;
        BYTE VisionDrop : 1;
        BYTE Evasion : 1;
        BYTE Snoop : 1;
        BYTE SpeedMov : 1;
        BYTE SkillDelay : 1;
        BYTE Resist : 1;
    } AffectInfo;

    ushort QuestInfo;

    int Gold;
    int Exp;

    sPoint Last;

    sStatus bStatus;
    sStatus Status;

    sItem Equip[16];
    sItem Inventory[64];

    uint Learn;
    short pStatus;
    short pSkill;
    short pMaster;
    char Critical;
    char SaveMana;
    char SkillBar1[4];
    char GuildMemberType;
    BYTE Unknow222;
    char RegenHP;
    char RegenMP;
    char Resist[4];
    short SlotIndex;
    short ClientIndex;
    short Perfuracao;
    char SkillBar2[16];
    short Evasion;
    int Hold;
    char Tab[26];
    long HPDRAIN;
    DWORD perfu;
    DWORD Absorb;
    int TimeStamp;
    WORD AtkSpeed;
    WORD AccLevel;
    sPoint Current;
    sPoint Dest;
    BYTE SUB;
    char GuildName[16];
    sAffect Affect[16];
    int ClassMaster;
    sPoint Gema;
    uint Quests;
    WORD MagicIncrement;
} sCharacter;

#define mob_data sCharacter

typedef struct
{
	char Name[16];
    char CapeInfo;

    struct
    {
        uchar Merchant : 6;
        uchar CityID : 2;
    } Info;

    ushort GuildIndex;
    uchar ClassInfo;

	struct
    {
        BYTE SlowMov : 1;
        BYTE DrainHP : 1;
        BYTE VisionDrop : 1;
        BYTE Evasion : 1;
        BYTE Snoop : 1;
        BYTE SpeedMov : 1;
        BYTE SkillDelay : 1;
        BYTE Resist : 1;
    } AffectInfo;

    ushort QuestInfo;

    int Gold;
    int Exp;

    sPoint Last;

    sStatus bStatus;
    sStatus Status;

    sItem Equip[16];
    sItem Inventory[64];

    uint Learn;
    ushort pStatus;
    ushort pSkill;
    ushort pMaster;
    uchar Critical;
    uchar SaveMana;
    char SkillBar1[4];
    char GuildMemberType;
    BYTE Unknow222;
    char RegenHP;
    char RegenMP;
    uchar Resist[4];
} st_MobFile;

typedef struct
{
    char name[12];

    uchar merchant;
    uchar city_index;
    ushort quest_info;

    int gold;
    uint exp;

    ushort last_x;
    ushort last_y;

    ushort level;
    ushort cur_hp;
    ushort cur_mp;

    ushort str;
    ushort _int;
    ushort dex;
    ushort con;

    uchar weapon;
    uchar fmaster;
    uchar smaster;
    uchar tmaster;

    uint learn;
    int hold;
} char_mob_data;

typedef struct
{
    short PosX[4];
    short PosY[4];
    char Name[4][16];
    sStatus Status[4];
    sItem Equip[4][16];
    ushort GuildIndex[4];
    int Gold[4];
    int Exp[4];
}charlist_data;

typedef struct
{
    char Name[24];
    sItem Item[12];

    uchar Slot[12];

    int Gold[12];
    short Unknown;
    short Index;
} sTrade;

typedef struct
{
	charlist_data charList;

	char Name[16];
	char AdapterInfo[16];

    sItem Storage[128];
	int Gold;

	int sessionStatus;
	time_t lastSave;
	time_t lastSave2;

    short pBabyMob[MAX_PARTY];

    bool Trading;
    sTrade Trade;
    int Cash;
} sClient;

typedef struct
{
    short Index;
    short LiderID;
    short Players[MAX_PARTY];
    short Count;
} sParty;

typedef struct
{
	sPoint Src;
	sPoint Dst;
	int Price;
	int Func;
} sTeleport;

typedef struct
{
    short X;
    short Y;
    short Range;
    short Wait;
	char Action[96];
} sPosition;

typedef struct
{
    int MaxNumMob;
    int CountGroup;
    short SpawnGroup[MAX_GROUP];

    int RouteType;
    int MinuteGenerate;

    int act_cont;

    sPosition Start;
    sPosition Dest;
    sCharacter Mob;
} sNPCGener;

typedef struct
{
	int Index;
	int tSpawn;
    int groupID;
    int cur;

    bool GameGuard;

    //Nao alterar os bool a baixo, são para calcular o ataque do monstro
    bool InBattle;
    bool DEAD;
    bool InPVP;
    bool InArena;
    bool InDuelo;
    bool InParty;
    bool InTvT;
    bool Lf;
    bool Rf;

    sBufferTrade TradeBuffer;
    bool InTrade;
    clock_t TradeTime;

    clock_t delayPotionHP;
    clock_t delayPotionMP;

    bool x1;
    bool x2;
    bool x3;

    //AFECTS
    bool Lentidao;
    bool Veneno;
    bool Invisivel;
    bool Cancelamento;
    bool Enfraquecer;
    bool Evocacao;
    bool TurnGuild;

    int EnemyList[20];
    int DamageMax;
    int EnemyIndex;
    int EvocLider;
    int AtkCont;
    int Time[17];

	sCharacter Character;

	sPoint Current;
    sPoint Destiny;
	clock_t nextAction;
	time_t RespawnDelay;
} sSpawn;

typedef struct
{
    sStatus bStatus;
    BYTE face;
    BYTE classinfo;
    BYTE ClassMaster;
    int exp;
    int pStatus;
    int pMaster;
    int pSkill;
    uint learn;
    int hold;
    bool SUB;
    char SkillBar1[4];
    char SkillBar2[16];
    sAffect Affect[16];
} sSubCele;

typedef struct
{
    int Index;
    int ownerID;

    sItem itemData;
    clock_t dropTime;

    short Rotation;
    short Status;
    short posX;
    short posY;
} sInit;

typedef struct
{
    short TargetID;
    short Damage;
} sTarget;

typedef struct
{
    short Damage;
    char DoubleCritical;
} sDamage;

typedef struct
{
    char Name[64];
    short Mesh1;
    int Mesh2;
    short Level;
    short Str;
    short Int;
    short Dex;
    short Con;
    struct
    {
        short Index;
        short Value;
    } Effect[12];

    int Price;
    short Unique;
    ushort Pos;
    short Extreme;
    short Grade;
} sItemData;

typedef struct
{
	char Portuguese[64];
} sItemName;

typedef struct
{
    int Points;
    int Target;
    int Mana;
    int Delay;
    int Range;
    int InstanceType;
    int InstanceValue;
    int TickType;
    int TickValue;
    int AffectType;
    int AffectValue;
    int Time;
    char Act[16];
    int InstanceAttribute;
    int TickAttribute;
    int Aggressive;
    int Maxtarget;
    int PartyCheck;
    int AffectResist;
    int Passive_Check;
} sSpellData;

typedef struct
{
    int owner_index;
    int chall_index;
    int area_guild_x, area_guild_y;
    int city_x, city_y;
    int city_min_x, city_min_y, city_max_x, city_max_y;
    int area_guild_min_x, area_guild_min_y, area_guild_max_x, area_guild_max_y;
    int war_min_x, war_min_y, war_max_x, war_max_y;
    int guilda_war_x, guilda_war_y;
    int guildb_war_x, guildb_war_y;
    int impost;
} sGuildZone;


#define LOGIN_OPCODE 0x20D

typedef struct
{
    short Size;
    char Key;
    char Hash;
    short Code;
    short Index;
    uint Time;
} MSG_HEADER;

typedef struct
{
    short Size;
    char Key;
    char ChkSum;
    short Code;
    short Index;
    unsigned int Time;
} sHeader;

typedef struct
{
    short size;
    short code;
} SiteHeader;

#define header_data sHeader

#define pak &tmp[0]
#define paklen xlen(pak)
#define xlen(buffer_) *(unsigned short*)buffer_

#define GetByte(offset,buffer_) *(unsigned char*)&((char*)(buffer_))[offset]
#define GetShort(offset,buffer_) *(unsigned short*)&((char*)(buffer_))[offset]
#define GetInt(offset,buffer_) *(unsigned int*)&((char*)(buffer_))[offset]
#define GetCharPtr(offset,buffer_) &((char*)(buffer_))[offset]

#define SetByte(val,offset) *(unsigned char*)&tmp[offset] = (unsigned char)val
#define SetShort(val,offset) *(unsigned short*)&tmp[offset] = (unsigned short)val
#define SetInt(val,offset) *(unsigned int*)&tmp[offset] = (unsigned int)val
#define SetString(var,offset) strcpy((char*)&tmp[offset], (char*)var)
#define SetBytes(val,offset,size) memcpy(&tmp[offset], &val, size)

#define GetInt8(offset) (Packet[offset])
#define GetUInt8(offset) ((uint8)Packet[offset])
#define GetInt16(offset) (*(int16*)&Packet[offset])
#define GetUInt16(offset) (*(uint16*)&Packet[offset])
#define GetInt32(offset) (*(int32*)&Packet[offset])
#define GetUInt32(offset) (*(uint32*)&Packet[offset])
#define GetPtrChar(offset) ((char*)&Packet[offset])
#define GetString(var,offset,size) (memcpy(var, &Packet[offset], size))
// Sets
#define SetInt8(val,offset) (*(int8*)&tmp[offset] = val)
#define SetUInt8(val,offset) (*(uint8*)&tmp[offset] = val)
#define SetInt16(val,offset) (*(int16*)&tmp[offset] = val)
#define SetUInt16(val,offset) (*(uint16*)&tmp[offset] = val)
#define SetInt32(val,offset) (*(int32*)&tmp[offset] = val)
#define SetUInt32(val,offset) (*(uint32*)&tmp[offset] = val)

#define SetSByte(val,offset,_buf) *(unsigned char*)&((char*)(_buf))[offset] = (unsigned char)val
#define SetSShort(val,offset,_buf) *(unsigned short*)&((char*)(_buf))[offset] = (unsigned short)val
#define SetSInt(val,offset,_buf) *(unsigned int*)&((char*)(_buf))[offset] = (unsigned int)val
#define SetSLong(val,offset,_buf) *(long*)&((char*)(_buf))[offset] = (long)val
#define SetSString(var,offset,_buf) strcpy((char*)&((char*)(_buf))[offset], (char*)var)
#define SetSBytes(val,offset,size,_buf) memcpy(&((char*)(_buf))[offset], &val, size)

#define Box(str, str2) MessageBox(NULL, str, str2, MB_OK)

#define CheckPacket(pak,t) \
    if(pak->Header.Size != sizeof(t)) \
    { \
        return false; \
    }
#define CheckPacketSignal(pak,opcode) \
    if(pak->Size != sizeof(sHeader)) \
    { \
        return false; \
    }
#define CheckPacketSignalParam(pak,opcode) \
    if(pak->Header.Size != (sizeof(sHeader) + 4)) \
    { \
        return false; \
    }

//
// DataServer (DB -> GS)
//
typedef struct
{
    sHeader Header;
    short GIndex;
    char GName[16];
    char Nick[16];
    char msg[255];
    short Canal;
    short Color;
    bool error;
} REQ_GUILD;

typedef struct
{
    sHeader Header;
    charlist_data SelList;
    sItem Storage[128];

    int Gold;
    char Name[16];
    char Keys[16];

    int Cash;
    int unk2;
} SELCHARLIST;

typedef struct
{
    sHeader Header;
    charlist_data SelList;
} CREATECHAR;

typedef struct
{
    sHeader Header;
    char Name[16];
    int classe;
    int face;
    int Cythera;
} CREATESUB;

typedef struct
{
    sHeader Header;
    charlist_data SelList;
} DELETECHAR;

typedef struct
{
    sHeader Header;
    sPoint Point;
    sCharacter Character;
    char Zero[1244 - (sizeof(sHeader) + sizeof(sPoint) + sizeof(sCharacter))];
} charworld_data;

//
// GameServer (GS -> DB)
//

typedef struct
{
    sHeader Header;
    char Name[16];
    char Pwd[12];

    int Version;
    int unk1;

    char Keys[16];
    char Address[16];
} REQLOGIN;

typedef struct
{
    sHeader Header;
    char Pwd[12];
    char Usr[12];
    char Unk[80];
} pCL_Login;

typedef struct
{
    sHeader Header;
    int Session;
} REQLOGOUT;

typedef struct
{
    SiteHeader Header;
    char Login[16];
    char Senha[12];
} sAcc;

typedef struct
{
    SiteHeader Header;
    char Login[16];
    int cash;
} sCash;

typedef struct
{
    int Msg;
} Msg_s;

typedef struct
{
    sHeader Header;
    int SlotIndex;
    char Name[16];
    int ClassIndex;
} REQCREATECHAR;

typedef struct {
	sHeader Header;
	char Numeric[6];
	char Unknown[10];
	int ChangeNumeric;
} REQLOGIN2;

typedef struct {
    sHeader Header;
    int cityID;
} REQCITY;

/// NEW STRUCT DE GUILDS

typedef struct {
    sHeader Header;
    sCityManager City;
} CITY;

typedef struct
{
    sHeader Header;
    int CharIndex;
    char Zero[18];
} getchar_data;

typedef struct
{
    sHeader Header;
    int SUB;
    int char_slot;
} CHANGE_SUB;

typedef struct
{
    sHeader Header;
    int SUB;
    int char_slot;
    sSubCele sub;
} pCHANGE_SUB;

typedef struct
{
    sHeader Header;
    int SlotIndex;
    sCharacter Character;
    int flag;
} GS_SaveChar;

typedef struct
{
    sHeader Header;
    sItem Storage[128];
    int Gold;
    int Cash;
} GS_SaveCargo;

typedef struct
{
    sHeader Header;
    int dat;
} REQCITYMANAGER;

typedef struct
{
    sHeader Header;
    int GameGuard;
} GAMEGUARD;

//
// Client (GS -> CL)
//

typedef struct
{
    sHeader Header;
    int dat1;
} pCL_P1h;

typedef struct
{
    sHeader Header;
} MSG_SIGNAL;

typedef struct
{
    sHeader Header;
    int data;
} MSG_SIGNALPARM;

typedef struct
{
    sHeader Header;
    int data[2];
} MSG_SIGNALPARM2;
typedef struct
{
    sHeader Header;
    int data[3];
} MSG_SIGNALPARM3;

// Request Client Message
typedef struct
{
    sHeader Header;
    char strMessage[96];
} pCL_101h;

//Send Weather
typedef struct
{
    sHeader Header;
    int Weather;
} pCL_18Bh;

// Request Remove Item
typedef struct
{
    sHeader Header;
    short initID;
    short NotUsed1;
} pCL_16Fh;

typedef struct
{
    sHeader Header;
    int invType;
    int invSlot;
    int unk;
    sPoint dest;
} pCL_175h;

typedef struct
{
    sHeader Header;
    int Merchant;
    sItem venda[27];
    int unk;
} pCL_17Ch;

// Request Create Item
typedef struct
{
    sHeader Header;
    short invType;
    short invSlot;
    sItem itemData;
} pCL_182h;

// Request Refresh Inventory
typedef struct
{
    sHeader Header;
    sItem inv[64];
    int gold;
} pCL_185h;

//GetCreateItemOnGround
typedef struct
{
    sHeader Header;
    sPoint loc;
    unsigned short Index;
    sItem ItemP;
    char Rotation;
    char Status;
} pCL_26Eh;

// Request Pick Item
typedef struct
{
    sHeader Header;
    int invType;
    int InvSlot;
    short initID;
    short posX;
    short posY;
    short Unknown1;
} pCL_270h;

// Request Drop Item
typedef struct
{
    sHeader Header;
    int invType;
    int InvSlot;
    int Unknown1;
    short posX;
    short posY;
    int Unknown2;
} pCL_272h;

// Request Add Points
typedef struct
{
    sHeader Header;
    short Mode;
    short Info;
    int unk;
} pCL_277h;

// Teleport no chao
typedef struct
{// over
    sHeader header;
    BYTE unk[12];
    WORD X;
    WORD Y;
} pCL_290h;

// Request Sell Items
typedef struct
{
    sHeader Header;
    short npcID;
    short Warp;
} pCL_27Bh;

//NPC CLICK
typedef struct
{
	sHeader Header;
	ushort npcid;
	uchar Unknow1;
	uchar Unknow2;
	short ClickOk;
} pCL_28Bh;

//Guild Expulsar
typedef struct
{
    sHeader Header;
    WORD Index;
    WORD Other;
} pCL_28Ch;

//NPC Click
typedef struct
{
	sHeader Header;
	WORD npcid;
	WORD unk;
} pCL_28Eh;

typedef struct
{
	sHeader Header;
	WORD npcid;
	int aposta;
} pCL_28Fh;

// Request Change City ID
typedef struct
{
    sHeader Header;
    int cityID;
} pCL_291h;

//Group and Ungroup Itens
typedef struct
{
    sHeader Header;
    int slot;
	int item;
	int num;
} pCL_2E5h;

typedef struct
{
    sHeader Header;
    int slot;
	short item;
	short type;
} pCL_2E4h;

// Request Spawn Say
typedef struct
{
    sHeader Header;
    char spawnMessage[96];
} pCL_333h;

// Request Command
typedef struct
{
    sHeader Header;
    char eCommand[16];
    char eValue[100];
} pCL_334h;

typedef struct
{
    sHeader Header;
    char Nick[16];
    char Msg[100];
    DWORD cor;
} pCL_334sh;

// Request Refresh Score
typedef struct
{
    sHeader Header;
    sStatus Status;

	BYTE Critical;
	BYTE SaveMana;

	struct
	{
		BYTE Time;
		BYTE Index;
	} Affect[16];

	BYTE GuildMemberType,
		 GuildIndex;

	BYTE RegenHP,
		 RegenMP;

	BYTE Resist1;
	BYTE Resist2;
	BYTE Resist3;
	BYTE Resist4;

	WORD CurrHP;
	WORD CurrMP;

	WORD MagicIncrement;

	BYTE Special[4];

} pCL_336h;

// Request Refresh Etc
typedef struct
{
    sHeader Header;
    int hold;
    int exp;
    int learn;
    short status;
    short master;
    short skills;
    short magic;
    int gold;
} pCL_337h;

// MOB DEAD
typedef struct
{
	sHeader Header;
	int Hold;
    short killed;
	short killer;
	int Exp;
} pCL_338h;

typedef struct
{
	sHeader Header;

	struct
    {
        short X, Y;
    } Current;

    WORD ClientId;
	char Name[12];

	BYTE ChaosPoints;
    BYTE CurrentKill;
    WORD TotalKill;
	WORD Item_Refine[16];

	struct // Buffs
	{
    	BYTE Time;
    	BYTE Index;
	} Affect[16];

	WORD GuildIndex;
	sStatus Status;

	struct
	{
    	unsigned short Type : 8;
    	unsigned short MemberType : 8;
	} Spawn;

	BYTE AnctCode[16];
	char Tab[26];

	//BYTE ___Unknow[45];

	char StoreName[27];
} pCL_363h;

// Spawn Info
typedef struct
{
    sHeader Header;

    // Spawn Info
    struct
    {
        short X, Y;
    } Current;

    WORD ClientId;
	char Name[12];

	BYTE ChaosPoints;
    BYTE CurrentKill;
    WORD TotalKill;

	WORD Item_Refine[16];

	struct // Buffs
	{
    	BYTE Time;
    	BYTE Index;
	} Affect[16];

	WORD GuildIndex; // 100

	sStatus Status;

	struct
	{
        char Type;
    	char MemberType;
	} Spawn;

	BYTE AnctCode[16];
	char Tab[26];

	BYTE Unknow[4];
} pCL_364h;

// Request Action
typedef struct
{
    sHeader Header;
    short xSrc, ySrc;
    int mSpeed;
    int mType;
    short xDst, yDst;
    char mCommand[24];
} pCL_366h;

// Request Emotion
typedef struct
{
    sHeader Header;
    short effType;
    short effValue;
    int Unknown1;
} pCL_36Ah;

// Request Refresh Itens
typedef struct
{
    sHeader Header;

    struct
    {
        unsigned short ItemID : 12;
        unsigned short Sanc : 4;
    } ItemEff[16];

    char pAnctCode[16];
} pCL_36Bh;

// Request Use Item
typedef struct
{
    sHeader Header;
    int SrcType;
    int SrcSlot;
    int DstType;
    int DstSlot;

    short PosX;
    short PosY;
    int unk;
} pCL_373h;

//Door system
typedef struct {
    sHeader Header;
    int InitID;
    int Status;
} pCL_374h;

// Request Move Item
typedef struct
{
    sHeader Header;
    unsigned char destType;
    unsigned char destSlot;
    unsigned char srcType;
    unsigned char srcSlot;
    int Unknown;
} pCL_376h;

// Request Chance SkillBar
typedef struct
{
    sHeader Header;
    char SkillBar1[4];
    char SkillBar2[16];
} pCL_378h;

// Request Buy Items
typedef struct
{
    sHeader Header;
    short mobID;
    short sellSlot;
    short invSlot;
    short Unknown1;
    int Unknown2;
} pCL_379h;

// Request Sell Items
typedef struct
{
    sHeader Header;
    short mobID;
    short invType;
    int invSlot;
} pCL_37Ah;

typedef struct
{
    sHeader Header;
    WORD LiderID;
    WORD RemoveID;
} pCL_37Eh;

//Confirmação de entrada no grupo // confirma entrada
 typedef struct
{
    sHeader Header;
    WORD LiderID;
    WORD Level;
    WORD MaxHp;
    WORD CurHp;
    WORD IdParty;
    char Nick[16];
    WORD ID;
} pCL_37Dh;

//Pedido de grupo enviado pelo jogador para o servidor
typedef struct
{
    sHeader Header;
    WORD IdParty;
    WORD Level;
    WORD MaxHp;
    WORD CurHp;
    WORD LiderID;
    char Nick[16];
    BYTE Unknow;
    WORD AlvoID;
} pCL_37Fh;

//packet confirmação de grupo enviado pelo servidor // manda o pedido pro jogador
typedef struct
{
    sHeader Header;
    WORD LiderID;
    char Nick[16];
} pCL_3ABh;

// Request Create Trade
typedef struct
{
    sHeader Header;
    sTrade Trade;
} pCL_397h;

// Request Buy Item Trade
typedef struct
{
    sHeader Header;
    int Slot;
    int Index;
    int Gold;
    int Unknown;
    sItem Item;
} pCL_398h;

// Lig/Dslg PVP
typedef struct
{
    sHeader Header;
    int PvP;
} pCL_399h;

// Request Open Trade
typedef struct
{
    sHeader Header;
    int Index;
} pCL_39Ah;

//Attack Sigle Mob
typedef struct
{
	sHeader Header;
	short AttackerID; // Id de quem Realiza o ataque
	short AttackCount;
	sPoint AttackerPos; // Posicao X e Y de quem Ataca
	sPoint TargetPos; // Posicao X e Y de quem Sofre o Ataque
	short SkillIndex; // Id da skill usada
	short CurrentMp; // Mp atual de quem Ataca
	char Motion;
	char SkillParm;
	char FlagLocal;
	char DoubleCritical; // 0 para critico Simples, 1 para critico Duplo
	int Hold;
	int CurrentExp;
	short ReqMp; // Mp necessario para usar a Skill
	short Rsv;
	short TargetID;
	short Damage;
} pCL_39Dh;

//atack reto
typedef struct
{
	sHeader Header;
	short AttackerID; // Id de quem Realiza o ataque
	short AttackCount;
	sPoint AttackerPos; // Posicao X e Y de quem Ataca
	sPoint TargetPos; // Posicao X e Y de quem Sofre o Ataque
	short SkillIndex; // Id da skill usada
	short CurrentMp; // Mp atual de quem Ataca
	char Motion;
	char SkillParm;
	char FlagLocal;
	char DoubleCritical; // 0 para critico Simples, 1 para critico Duplo
	int Hold;
	int CurrentExp;
	short ReqMp; // Mp necessario para usar a Skill
	short Rsv;
	sTarget Target[2];
} pCL_39Eh;

//duelo
typedef struct
{
    sHeader Header;
    DWORD SenderClientID;
    DWORD Type;
} pCL_39Fh;

//atack area;
typedef struct
{
	sHeader Header;
	short AttackerID; // Id de quem Realiza o ataque
	short AttackCount;
	sPoint AttackerPos; // Posicao X e Y de quem Ataca
	sPoint TargetPos; // Posicao X e Y de quem Sofre o Ataque
	short SkillIndex; // Id da skill usada
	short CurrentMp; // Mp atual de quem Ataca
	char Motion;
	char SkillParm;
	char FlagLocal;
	char DoubleCritical; // 0 para critico Simples, 1 para critico Duplo
	int Hold;
	int CurrentExp;
	short ReqMp; // Mp necessario para usar a Skill
	short Rsv;
	sTarget Target[13];
} pCL_36Ch;

//Trade
typedef struct
{
     sHeader Header;
     sItem TradeItem[15];
     BYTE TradeItemSlot[15];
     BYTE Unknow;
     DWORD Gold;
     bool Confirma;
     WORD OtherClientid;
} pCL_383h;

typedef struct
{
    sHeader Header;
} pCL_384h;

// Green Time
typedef struct
{
	sHeader Header;
	DWORD Time;
} pCL_3A1h;

// Request Item Anct
typedef struct
{
    sHeader Header;
    sItem items[8];
    char slot[8];
} pCL_3A6h;

//Mob Count
typedef struct
{
	sHeader Header;
	WORD cur_mob;
	WORD max_mob;
} pCL_3BBh;

//Aylin
typedef struct//opcode 2D2
{
    sHeader Header;
    sItem Items[8];
    BYTE slot[8];
} pCL_3B5h;

typedef struct
{
    BYTE slot[8];
} sSlot;

//Shany
typedef struct//opcode 2C4
{
    sHeader Header;
    sItem Items[8];
    BYTE slot[8];
} pCL_2C4h;

//Lindy
typedef struct//opcode 2C3
{
    sHeader Header;
    sItem Items[8];
    BYTE slot[8];
} pCL_2C3h;

//Agatha
typedef struct
{
    sHeader Header;
    sItem Items[6];
    BYTE unknown[24];
} pCL_3BAh;

//SendAffect
typedef struct
{
	sHeader Header;
	sAffect Affect[16];
} pCL_3B9h;

//Tiny
typedef struct//opcode 3C0
{
    sHeader Header;
    sItem Items[8];
    BYTE slot[8];
} pCL_3C0h;

typedef struct
{
    sHeader Header;
    int dat1;
    int dat2;
} pCL_3D5h;

typedef struct
{
	sHeader Header;
	int WarpID;
	int Type;
} pCL_AD9h;

typedef struct
{
	sHeader Header;
	char msg[96];
} pCL_D1Dh;