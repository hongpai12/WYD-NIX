#ifndef __BASE_STRUCTURES_H__
#define __BASE_STRUCTURES_H__

#include "../TMSRV/st_packet.h"

static char tmp[1024];
 
#define MAX_PLAYER 1000
#define MAX_AFFECTS 16

#define EF1  Effect[0].Index
#define EFV1 Effect[0].Value
#define EF2  Effect[1].Index
#define EFV2 Effect[1].Value
#define EF3  Effect[2].Index
#define EFV3 Effect[2].Value

#define GetPartyOwner(i) (*(int*)((i * 0x410) + 0x015C6CD0))
static int Mes = *(unsigned short*)(0x00A5EA420); //0 Janeiro, 11 Dezembro

static int DiaDaSemana = *(unsigned short*)(0x00A5EA428);//0 Domingo, 6 Sabado
static int Dia = *(unsigned short*)(0x00A5EA41C);
static int Hora = *(unsigned short*)(0x00A5EA418);
static int Min = *(unsigned short*)(0x00A5EA414);
static int Seg = *(unsigned short*)(0x00A5EA410);

struct AmountItens
{
	int totalAmount;
	int slots[60];
};

typedef struct
{
	WORD Size;

	BYTE Key;
	BYTE CheckSum;

	WORD PacketId;
	WORD ClientId;

	DWORD TimeStamp;
} PacketHeader;

typedef struct
{
	PacketHeader Header;

	struct
	{
		BYTE Index;
		BYTE Master;
		WORD Value;
		DWORD Time;
	} Affect[16];
} p3B9;

// Green Time
typedef struct
{
	PacketHeader Header;
	DWORD Time;
} p3A1h;

typedef struct
{
	WORD X;
	WORD Y;
} st_Position;

typedef struct
{
	WORD Index;
	struct
	{
		BYTE Index;
		BYTE Value;
	} Effect[3];

} st_Item;

typedef struct
{
    char MobName[12];
	short size;
	st_Item Drop;
	short Quant;
	bool Status;
	bool Double;
}Surprise;

typedef struct
{
	INT16 Vida;
	INT16 Type;
	st_Position Born;
	char Name[16];
} treesInfo;

typedef struct
{
    int SkillPoint;
    int TargetType;
    int ManaSpent;
    int Delay;
    int Range;
    int InstanceType;
    int InstanceValue;
    int TickType;
    int TickValue;
    int AffectType;
    int AffectValue;
    int AffectTime;
    char Act_1[8];
    //char Act_2[8]; - só p/ 7xx
    int InstanceAttribute;
    int TickAttribute;
    int Aggressive;
    int Maxtarget;
    int PartyCheck;
    int AffectResist;
    int Passive_Check;
} st_AffectData;

struct stAffects
{
	UINT16 Index;
	UINT16 Master;
	UINT16 Value;
	UINT16 Time;
};

struct stBuffer{
	stAffects Affects[16];
	UINT8 Unknown[912];
};

typedef struct
{
	DWORD AtaqFisico;
	DWORD AtaqMagico;
	DWORD Unknow;
	DWORD Resistencia;
	DWORD Evasao;
} st_MountData;

typedef struct
{
	//# Index,Name,Mesh-Texture,REQ_SIDC,Unique,EF_PRICE,EF_POS,Extreme,Grade,N0,V0,N1,V1,N2,V2,N3,V3,N4,V4,N5,V5,N6,V6,N7,V7,N8,V8,N9,V9,N10,V10,N11,V11//
	//"%d,%64s,%d.%d,%d.%d.%d.%d.%d,%d,%d,%d,%d,%d,
	//"%d,%64s,%d.%d,%d.%d.%d.%d.%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d"
	char Name[64];

	short mesh1;
	short mesh2;

	short unknow;

	short Level;
	short STR;
	short INT;
	short DEX;
	short CON;

	struct {
		short index;
		short value;
	} Effect[12];

	int Price;
	short Unique;
	WORD Pos;

	short Extreme;
	short Grade;
} st_ItemList;

typedef struct
{
	char Name[27];

	short Mesh1;
	short Mesh2;

	short Unknow;

	struct
	{
		short Level;
		short Str;
		short Int;
		short Dex;
		short Con;
	} REQ_SIDC;

	struct {
		short Index;
		short Value;
	} Effect[12];

	int Price;
	short Unique;
	WORD Pos;

	short Extreme;
	short Grade;
} st_ItemListCSV;

typedef struct
{
	short Level;
	short Defense;
	short Attack;

	unsigned char Merchant;
	unsigned char ChaosRate;

	unsigned short maxHP, maxMP;
	unsigned short curHP, curMP;

	unsigned short Points[4];

	unsigned char Mastery[4];
} st_Status;

typedef struct
{
	WORD Index;
	WORD Master;
	WORD Value;
	WORD Time;
} st_Affect;

typedef struct
{
	char Name[16];
	char CapeInfo;

	struct
	{
		UINT8 Merchant : 6;
		UINT8 CityID : 2;
	} Info;

	UINT16 GuildIndex;
	UINT8 ClassInfo;

	union
	{
		struct
		{
			UINT8 SlowMov : 1;
			UINT8 DrainHP : 1;
			UINT8 VisionDrop : 1;
			UINT8 Evasion : 1;
			UINT8 Snoop : 1;
			UINT8 SpeedMov : 1;
			UINT8 SkillDelay : 1;
			UINT8 Resist : 1;
		};

		UINT8 Value;
	} AffectInfo;

	UINT16 QuestInfo;
	INT32 Gold;
	UINT32 Exp;

	st_Position LastPosition;

	st_Status bStatus;
	st_Status Status;

	st_Item Equip[16];
	st_Item Inventory[64];

	UINT32 Learn;

	UINT16 StatusPoint,
		MasterPoint,
		SkillPoint;

	UINT8 Critical;
	UINT8 SaveMana;

	INT8 SkillBar[4];

	INT8 GuildMemberType;

	UINT8 MagicIncrement;
	UINT8 RegenHP,
		RegenMP;

	INT8 Resists[4];

	stAffects Affects[4];

	UINT32 Mode;                                               // 788 a 791
	UINT32 Leader;                                             // 792 a 795
	UINT32 Formation;                                  // 796 a 799
	UINT32 RouteType;                                  // 800 a 803

	UINT32 LastX;                                              // 804 a 807
	UINT32 LastY;                                              // 808 a 811
	UINT32 LastTime;                                   // 812 a 815
	UINT32 LastSpeed;                                  // 816 a 819

	UINT32 TargetX;                                    // 820 a 823
	UINT32 TargetY;                                    // 824 a 827

	UINT32 NextX;                                              // 828 a 831
	UINT32 NextY;                                              // 832 a 835
	UINT32 NextAction;                                 // 836 a 839

	UINT8 Route[24];                                 // 840 a 863

	UINT32 WaitSec;                                    // 864 a 867

	UINT32 PosX;                                               // 868 a 871
	UINT32 PosY;                                        // 872 a 875

	UINT32 Segment;                                    // 876 a 879
	UINT32 DeltaExp;									//880 a 883 
	UINT32 SegmentX;                                   // 884 a 887
	UINT32 SegmentY;                                   // 888 a 891
	UINT32 SegmentListX[5];                    // 892 a 911
	UINT32 SegmentListY[5];                    // 912 a 931
	UINT32 SegmentWait[5];                             // 932 a 951
	UINT32 SegmentDirection;                   // 952 a 955
	UINT32 SegmentProgress;                    // 956 a 959

	UINT32 GenerateIndex;                              // 960 a 963
	UINT16 CurrentTarget;                    // 964 a 965
	UINT16 EnemyList[4];                             // 966 a 973
	INT16 PartyList[12];                    // 974 a 997

	UINT16 Unk1;                                             // 998 a 999

	UINT32 WeaponDamage;                               // 1000 a 1003
	UINT32 Summoner;                                   // 1004 a 1007
	UINT32 PotionCount;                                // 1008 a 1011
	INT32 Parry;                                       // 1012 a 1015
	UINT32 GuildDisable;                               // 1016 a 1019
	UINT32 DropBonus;                                  // 1020 a 1023 //Diamante
	UINT32 ExpBonus;                                   // 1024 a 1027 //Coral
	UINT32 ForceDamage;                                // 1028 a 1031 //Esmeralda
	UINT32 ReflectDamage;                              // 1032 a 1035 //Garnet

	UINT32 Unk2;
} st_Mob;

struct BossQuest
{
	bool isAlive;
	char Name[16];
	st_Position CreationPos;
	st_Item Gifts[10];
	INT8 Chances[10]; 
	INT16 CountToBorn;
};

struct Quests
{
	UINT8 CidID[40];
	INT16 KillCount;
	BossQuest Boss;
};

typedef struct {// shepher 
	PacketHeader Header;
	
	int HOLD;
	unsigned short KilledMob;
	unsigned short Killer;
	unsigned int Exp;
} p338;

struct p277
{
	PacketHeader Header;
	short Mode;
	short Info;
	int unk;
};

typedef struct
{
	PacketHeader Header;
	short npcId,
		type;
	int sellSlot;
} p37A;

typedef struct
{
	PacketHeader Header;
	int Slot,
		mobId,
		Price,
		Tax;
	st_Item Item;
} p398;

struct p334{
	PacketHeader Header;
	char Cmd[16];
	char Arg[96];
};

struct st_CharList
{
	st_Position Position[4]; // 0
	char Name[4][16]; // 16
	st_Status Status[4]; // 80
	st_Item Equip[4][16]; // 192

	INT16 GuildId[4]; // 704
	INT32 Gold[4]; // 712
	INT32 Experience[4]; //728
}; // 744

struct st_Socket
{
	INT32 FD; // 0-32
	INT8* SendBuffer; // 4-36
	INT8* RecvBuffer; // 8-40

	INT32 SizeOfSend; // 12-44
	INT32 SizeOfRecv; // 16-48

	INT32 IndexOfRecv; // 20-52
	INT32 IndexOfSend; // 24-56

	INT32 Hello; // 28-60
}; // 32 - 64
struct CUser
{
	char Username[16]; // 0
	INT32 CharPosition; //16
	IN_ADDR In_Addr; // 20
	//inet_n2a(user->In_Addr);
	INT32 Status; // 24
	INT32 IsAutoTrading; // 28

	st_Socket Service; // 32

	INT8 Unknown_64[404]; //64

	struct
	{
		st_Item Item[128]; // 468
		INT32 Gold; // 1492
	} Storage;

	INT16 AttackCount; // 1496
	INT16 Unknown_1498; // 1498

	struct
	{
		PacketHeader Header; // 1500

		st_Item Item[8]; // 1512
		INT8 Slots[8]; // 1576

		INT32 Gold; // 1584

		bool Accept; // 1588;

		INT8 Unknown_1589; // 1589

		INT16 OtherClientId; // 1590
	} Trade;

	struct
	{
		PacketHeader Header; // 1592

		char Name[24]; // 1604

		st_Item Item[12]; // 1628
		INT8 Slots[12]; // 1724

		INT32 Price[12]; // 1736

		INT16 Unknown_1784; // 1784

		INT16 Index; // 1786
	} AutoTrade;

	struct
	{
		INT32 LastType; // 1788

		INT32 TimeStamp; // 1792
	} Attack;

	INT32 Unknown_1796; // 1796

	struct
	{
		INT32 PacketId; // 1800
		INT32 TimeStamp; // 1804
	} Movement;

	INT32 IllusionTime; //1808

	UINT32 CrackErrorCount; // 1812

	INT32 Billing; // 1816

	st_CharList CharList; //1820

	char LastWhisper[16]; // 2564

	INT32 IsBillConnect; // 2580
	char UserStatus[4]; // 2584

	struct
	{
		PacketHeader Header;

		INT32 nCommand;
		INT32 nDummy[4];
	} Session;

	INT8 SkillBar[8]; //2620

	INT32 nTargetX; // 2628
	INT32 nTargetY; // 2632

	struct
	{
		INT32 Whisper; // 2636
		INT32 Guild; // 2640
		INT32 Party; // 2644
		INT32 Chat; // 2648

		INT8 AutoTradeName[24]; // 2652

		INT32 PK; // 2676
	} AllStatus;

	struct
	{
		UINT32 CountHp; // 2680
		UINT32 CountMp; // 2684

		INT32 bQuaff; // 2688;
	} Potion;

	INT32 AdapterKeys[4]; // 2692

	struct
	{
		INT32 Mode; // 2708
		INT32 Index; // 2712

		INT32 ClassId; // 2716 - confirmar.
	} Challenger;

	INT32 LastReceiveTime; // 2720
	INT32 Admin; // 2724

	INT32 Child; // 2728
	INT32 CheckBillingTime; // 2732
	INT32 CharLoginTime; // 2736

	INT32 Range; // 2740

	INT8 Unknown_2744[404]; // 2744
}; // 3148

struct p36b // Request Refresh Itens 754
{
	PacketHeader Header;
	WORD ItemEff[16];
	BYTE pAnctCode[16];
};

typedef struct {
	char Name[16];	 // 0 a 15 
	char CapeInfo;	 // 16 
	char Merchant;	 // 17 
	unsigned short GuildIndex;	 // 18 a 19 
	unsigned char ClassInfo;	 // 20 
	unsigned char AffectInfo;	 // 21 
	unsigned short QuestInfo;	 // 22 a 23 
	int Gold;	 // 24 a 27 
	unsigned int Exp;	 // 28 a 31 
	st_Position LastPosition;	 // 32 a 35 
	st_Status bStatus;	 // 36 a 63 
	st_Status Status;	 // 64 a 91 
	st_Item Equip[16];	 // 92 a 219 
	st_Item Inventory[64];	// 220 a 731 
	unsigned int Learn;	 // 732 a 735 
	unsigned short StatusPoint;	 // 736 a 737 
	unsigned short MasterPoint;	 // 738 a 739 
	unsigned short SkillPoint;	 // 740 a 741 
	unsigned char Critical;	 // 742 
	unsigned char SaveMana;	 // 743 
	char SkillBar[4];	 // 744 a 747 
	char GuildMemberType;	 // 748 
	unsigned char MagicIncrement;	// 749 
	unsigned char RegenHP;	 // 750 
	unsigned char RegenMP;	 // 751 
	char Resist1;	 // 752 
	char Resist2;	 // 753 
	char Resist3;	 // 754 
	char Resist4;	 // 755 
} stMOB754;

struct stMOBGener
{
	UINT32 Mode; // 0 - 3
	UINT32 MinuteGenerate; // 4 - 7

	UINT32 MaxNumMob; // 8 - 11

	UINT32 MobCount; // 12 - 15

	UINT32 MinGroup; // 16 - 19
	UINT32 MaxGroup; // 20 - 23

	UINT32 Segment_X[5]; //24 - 43
	UINT32 Segment_Y[5]; // 44 - 63
	UINT32 StartRange; // 64 - 67

	UINT32 Segment1Range; // 68 - 71
	UINT32 Segment2Range; // 72 - 75
	UINT32 Segment3Range; // 76 - 79

	UINT32 DestRange; // 80 - 83

	UINT32 StartWait; // 84 - 87

	UINT32 Segment1Wait; // 88 - 91 ou Segment3Wait
	UINT32 Segment2Wait; // 92 - 95

	UINT32 Segment3Wait; // 96 - 99 // Não usado na TMsrv -> acho que é um erro, no ParseStrings ele move para o Segment1Wait o que está em Segmen3Wait e.e'

	UINT32 DestWait; // 100 - 103

	char SegmentAction[5][80]; // 104 - 503
	char FightAction[4][80]; // 504 - 823
	char FleeAction[4][80]; // 824 - 1143
	char DieAction[4][80]; // 1144 - 1463

	UINT32 Formation; // 1464 - 1467
	UINT32 RouteType; // 1468 - 1471 

	UINT32 Unknow; // 1472 - 1475 -> Ele pega o valor do MinuteGenerate (?)

	stMOB754 Leader; // 1476 - 2231	
	stMOB754 Follower; // 2232 - 2987
};

struct p36b_613 // Request Refresh Itens 613
{
	PacketHeader Header;
	WORD ItemEff[16];
};

typedef struct
{
	PacketHeader Header;

	st_Position WorldPos;

	char Name[16];
	char CapeInfo;

	struct
	{
		BYTE Merchant : 6;
		BYTE CityID : 2;
	} Info;

	WORD GuildIndex;
	BYTE ClassInfo;

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

	WORD QuestInfo;

	int Gold;
	int Exp;

	st_Position Last;
	st_Status bStatus;
	st_Status Status;

	st_Item Equip[16];
	st_Item Inventory[64];

	DWORD Learn;
	WORD pStatus;
	WORD pSkill;
	WORD pMaster;
	BYTE Critical;
	BYTE SaveMana;

	char SkillBar1[4];
	char GuildMemberType;

	BYTE MagicIncrement;
	BYTE RegenHP;
	BYTE RegenMP;

	BYTE Resist1, Resist2;
	BYTE Resist3, Resist4;

	short SlotIndex;
	short ClientIndex;
	short unk;

	char SkillBar2[16];
	short Evasion;
	int Hold;

	char Tab[28];
} p114;

typedef struct
{
	PacketHeader Header;
	DWORD SrcType,
		  SrcSlot,
		  DstType,
		  DstSlot;
	WORD  PosX,
		  PosY;
	DWORD warp;
} p373;

typedef struct
{
	INT16 Index[7];

	bool isOn;
} aDrop;

typedef struct
{
	INT8 LoggedMob, // Personagem em que o usuário logou
		IncorrectNumeric, // Número de vezes que errou a senha numérica
		ArrobaTime;

	UINT8 hasFailed;

	INT16 userWhisper, // Usuário que o enviou mensagem anteriormente
		  LastClicked;

	char Tab[26],
		 SND[60];

	bool canLog, // Já digitou numérica?
		 isMod,
		 isAdmin;

	struct 
	{
		INT8 Minuto,
			 gritos;
	} grito;

	aDrop AutoDrop;

	clock_t LastAttackTime;
	UINT32 TooFastAttackCounter;
	DWORD TimeStamp;
} ingame;

typedef struct
{
	INT8 Type;

	tm Validade;
} vip;

struct NewInvent
{
	st_Item sInv1[64],
			sInv2[64];

	bool hasBuyed[2];
};

typedef struct
{
	// Este é um exemplo de quests
	// Como o valor é UINT64, são 64 bits, ou seja, 64 quests
	UINT64 MolarGargula : 1; // 1
	UINT64 ResetDivinePure : 1; // 2
	UINT64 Armeiro1 : 1; // 3
	UINT64 Armeiro2 : 1; // 4
	UINT64 Armeiro3 : 1; // 5

	UINT64 Unknow : 59;
} pQuestInfo;

typedef struct
{
	char SkillBar2[16]; // Segunda skillbar

	INT8 SoulInfo,      // Configuração da Soul
		Cidadania;      // Canal no qual o personagem é cidadão

	INT32 Fame;         // Fama do personagem

	INT16 SemiDeusLevel, // Nível em que foi feito o semideus
		  DeusLevel;     // Nível em que foi feito o Deus

	stAffects Buffs[16];

	INT8 DeadCount;

	NewInvent Inventarios;

	INT16 KillCount,
		Entradas; // Entradas para o Hall do Kefra 

	pQuestInfo Quests;
} chars;

typedef struct
{
	int levelAxe;
	int woodGath;
} questPorto;

typedef struct 
{
	// Senha numérica
	char Numerica[6],
		 Login[16];

	INT32 Donate,
		  AcessLevel;

	vip Vip;

	chars Chars[4];

	char MacAdress[20];

	questPorto Port;

	ingame Ingame;
} nInfo;

struct p378
{
	PacketHeader Header;
	char SkillBar1[4];
	char SkillBar2[16];
};

struct p20D
{
	PacketHeader Header;

	char Login[16]; // 12 - 27
	char Password[12]; // 28 - 39

	INT32 Version; // 40 - 43
	INT32 Unknows; // 44- 47

	char Keys[16]; // 48 - 63
	char IPAddress[16]; // 64 - 79
};

struct pFDE{
	PacketHeader Header;
	char num[6];
	char unk[10];
	int RequestChange;
};

#define ESTADO_LIBERADO_ACESSO 0
#define ESTADO_MANUTENC_ACESSO 1
#define ADMIN_ACESSO 2
#define QUIONE_DEAD 1
#define QUIONE_ALIVE 2
#define FIGHTER_ALIVE 1
#define FIGHTER_DEAD  2
#define BATALHA_INICIADA   1
#define BATALHA_FINALIZADA 2

typedef struct
{
	int ShadowFightTeams[2][10];
	INT8 FighterStatus[2][10];
	INT8 Status;
} ShadowFight;

typedef struct
{
	// Tempo do servidor online
	INT8 Seconds,
		 Minuts,
		 Hours,
		 Days,
		 Channel,
		 Estado,
		 DropBonus,
		 ExpBonus;

	INT16 OnlineCount;

	INT16 DeadMobCount;
	
	INT8 TaxaImposto[5];
	INT64 ImpostoAcumulado[5];
	UINT16 PossuidorIndex[5];

	INT32 ExpCount;

	char TopKiller[16],
		 SecondKiller[16],
		 ThirdKiller[16];
	INT16 TopKillerCount,
		  SecondKillerCount,
		  ThirdKillerCount;

	INT8 QuioneStatus;

	ShadowFight Batalha;
} sInfos;

struct p376
{
	PacketHeader Header;
	BYTE DstType;
	BYTE DstSlot;
	BYTE SrcType;
	BYTE SrcSlot;
	int Unknown;
};

struct p379
{
	PacketHeader Header;
	short mobID;
	short sellSlot;
	short invSlot;
	short Unknown1;
	int Unknown2;
};

struct Donate
{
	st_Item Item;
	UINT32 Price;
	UINT16 Store;
};

struct pD1D
{
	PacketHeader Header;
	char message[92];
	DWORD color;
};

struct p28B//NPC CLICK
{
	PacketHeader Header;
	WORD npcid;
	BYTE Unknow1;
	BYTE Unknow2;
	short ClickOk;
};

struct p2E4
{
	PacketHeader Header;

	INT32 Slot;

	INT16 Index,
	      Type;
};

struct p2E5 {
	PacketHeader Header;
	short SlotID;
	short Unk;
	short ItemID;
	short Unk2;
	short Amount;
	int Unk3;
};

struct Room
{
	bool isActive;

	UINT8 MobsLeft;
	INT16 Cids[13];
	INT16 Leaders;

	UINT8 TimeLeft;
};

struct Aguas
{
	Room Rooms[9];
};

struct Pesas
{
	// Se o pesa está ativo ou não
	bool  Status;
	// Em qual wave estão
	INT8  Wave,
    // Quantos grupos estão na quest
		Groups;
	// Jogadores fazendo a quest
	INT16 cId[3][13],
		  MobsLeft,// Mobs a serem mortos
		  TimeLeft;
};

struct pWindow
{
	PacketHeader Header;

	st_Item ItemId[7];

	UINT8 Slot[7];
};

typedef struct
{
	PacketHeader Header;

	st_Item ItemId[8];

	char Slot[8];

} st_Window;

typedef struct
{
	st_Item Itens[10];
	INT8 Chances[10];
} QuestItensOnEnter;

struct GuildInfos
{
	INT8 Cidadania; // Canal de cidadania da guild

	UINT16 GuildID, // ID da guild
		   Members; // Número total de membros

	char LiderName[16]; // Nick do lider
	char GuildName[20]; // Nome da guild

	char SubLiderName[3][16]; // Nome dos Subs

	UINT8 Kingdom; // Reino da guild, para comparação na hora de recrutar e para igualar
				   // No GetCreateMob

	INT64 Points;  // Número total de pontos da guild
	INT16 MobDeadCount;  // Conta de mobs mortos no pilar
	INT8 CidadeApostada, // Cidade na qual fez aposta, lembrando que só é possível apostar em 1 cidade por vez
		 CidadePossuida;
	INT64 ValorApostado; // Valor de pontos apostado pela guild
};

#define GUERRA_NAOINICIADA 0
#define GUERRA_INICIADA    1
#define GUERRA_TERMINADA   2

struct WarInfos
{
	INT32 Desafiante,
		  Desafiado;

	INT16 TamanhoExercitoDesafiante, // Número de membros da guild atacante
		  TamanhoExercitoDesafiado; // Número de membros da guild defensora

	INT64 TotalBet;

	INT8 Status;
};

struct AAA
{
	PacketHeader Header;

	GuildInfos Guild;
};

typedef struct
{
	PacketHeader Header;

	st_Item TradeItem[15];
	char TradeItemSlot[15];

	BYTE Unknow;

	DWORD Gold;
	bool Confirma;
	WORD OtherClientid;
} SendTrade;

typedef struct
{
	bool isTrading;
	bool WaitingData;
	bool Confirm;

	DWORD Gold;
	clock_t Timer;

	WORD ClientID;

	st_Item Itens[15];
	char TradeItemSlot[15];
} st_Trade;

struct Resets
{
	UINT8 Time;
	st_Position MinCoords,
		        MaxCoords;
	UINT16 MinLevel,
		   MaxLevel;
};

typedef struct Tabs
{
	INT16 Dex;
	char Tab[26];
};

typedef struct
{
	int	SkillPoint; // 0 - 3
	int	TargetType; // 4 - 7
	int	ManaSpent; // 8 - 11
	int	Delay; // 12 - 15
	int	Range; // 16 - 19
	int	InstanceType; // 20 - 23
	int	InstanceValue; // 24 - 27
	int	TickType; // 28 - 31
	int	TickValue; // 32 - 35
	int	AffectType; // 36 - 39
	int	AffectValue; // 40 - 43
	int	AffectTime; // 44 - 47
	unsigned char Act[8]; // 48 - 55
	int	InstanceAttribute; // 56 - 59
	int	TickAttribute; // 60 - 63
	int	Aggressive; // 64 - 67
	int	MaxTarget; // 68 - 71
	int	bParty; // 72 - 75
	int	AffectResist; // 76 - 79
	int	Passive; // 80 - 83
} STRUCT_SPELL;
//  Index:          Numero da Skill - 0-23(TK) 24-47(FM) 48-71(BM) 72-96(HT)
//  SkillPoint:     Pontos para aprender as skills
//  TargetType:     0:Si proprio  1:Outros  2:Si proprio ou Outros 3:Area 3x3  4:Area 5x5
//  ManaSpent:      Custo de Mana
//  Delay:          Tempo necessario para usar novamente a skill
//  Range:          Alcance
//  InstanceType:   Efeito aplicado a skill - 0:Sem Efeito 1:Incremento de HP 2:% de redução de HP 3:flash
//  InstanceValue:  Almentar ou diminuir o HP ou flash.
//  TickType:       Tick(4 segundos)Mana processado efeito - 0:Sem Efeito 1:Incremento de HP 2:% de redução de HP 3:Redução de HP em area 3x3 4:Incremento de HP em area 3x3
//  TickValue:      Aumento ou Redução de HP
//  AffectType:     Altera os Pontos do Personagem - 0:Sem Efeito 1:Incrementa algo 2:% de alteração na Resistencia 3:% de alteração na Dex 4:Velocidade de Ataque +
//  AffectValue:

typedef struct
{
	INT32 MinDamage;
	INT32 MaxDamage;
	INT32 MinDefense;
	INT32 MaxDefense;
	INT32 MinHP;
	INT32 MaxHP;
	INT32 MinAtkSpeed;
	INT32 MaxAtkSpeed;
	INT8 Unk2[36];
} st_TransBonus;

#define MAX_TRANS_BONUS 5
static st_TransBonus TransBonus[MAX_TRANS_BONUS];

typedef struct
{
	PacketHeader Header;
	INT32 SlotIndex;
	char Name[16];
	char Pwd[12];
} REQDELETECHAR;

struct p3D5
{
	PacketHeader Header;
	int RecruitCid;
	int unk;
};

struct p28F
{
	PacketHeader Header;
	WORD npcid;
	int BetValue;
};

struct p26E
{
	st_Position pos;
	short Init;
	st_Item Item;
	BYTE Rotation;
	BYTE Type;
	short Status;
	unsigned short unk;
};

struct pCL_181h // SHEPHER
{
	PacketHeader Header;

	unsigned short curHP;
	unsigned short curMP;
	unsigned short maxHP;
	unsigned short maxMP;
};

struct DropEvent
{
	bool isOn;

	UINT8 Rate;

	INT16 Id;

	UINT32 Quantity;
	UINT32 Dropped;
};

struct TradeEvent
{
	st_Item RequestItem[5],
		    PrizeItem[10];

	INT8 Rates[5];
};

struct Events
{
	DropEvent Drop;
	TradeEvent Trade[6];
};

union MapAttribute
{
	struct
	{
		UINT8 Village : 1;
		UINT8 CantGo : 1;
		UINT8 CantSummon : 1;
		UINT8 House : 1;
		UINT8 Teleport : 1;
		UINT8 Guild : 1;
		UINT8 PvP : 1;
		UINT8 Newbie : 1;
	};
	UINT8 Value;
};

typedef struct
{
	int Amount;
	int Slots[128];
	st_Item *Itens[128];
} FindAmountItem;




//0x8B9E78C
struct stInitItem
{
	st_Item itemId; // 0 - 8
	int mode; // 8 - 12
	int posX; // 12 - 16
	int posY; // 16 - 20
	int maskidx; // 20 - 24
	int indx; // 24 - 28
	int rotate; // 28 - 32
	int state; // 32 - 36
	int unk; // 36 - 40
	int nextState; // 40 - 44
	BYTE notUsed[24];
};

struct p2D4
{
	PacketHeader Header;
	WORD unknow; // 12 - 13
	WORD confirm; // 14 - 15
	WORD slotId; // 16 - 17
	WORD unkw_2; // 18 - 19
};

#endif // __BASE_STRUCTURES_H__