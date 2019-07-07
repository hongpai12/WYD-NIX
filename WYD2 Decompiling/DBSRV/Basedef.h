#ifndef BASEDEF_H
#define BASEDEF_H

/* DEFINES */
#define GAME_PORT			8281
#define DB_PORT				7514
#define ADMIN_PORT			8895

#define TIMER_SEC			0
#define TIMER_MIN			1

#define MOBNAME_LENGTH		16
#define MAX_EQUIPMENT		16
#define MAX_INVENTORY		64
#define MAX_CARGO			128
#define MAX_SKILLBAR		4

#define MAX_SERVERNUMBER	10
#define MAX_SERVERGROUP		11
#define MAX_CLASS			8

#define MAX_USER			1000
#define MAX_ADMIN			12

#define ACCOUNTNAME_LENGTH	16
#define ACCOUNTPASS_LENGTH	12
#define NUMERICTOKEN_LENGTH 6

#define MOB_PER_ACCOUNT		4

#define ITEMNAME_LENGTH		64
#define MAX_STATICEFFECT	12
#define MAX_ITEMLIST		8000
#define MAX_MOUNT			50
#define MAX_ITEMGRID		8

#define EFFECTNAME_LENGTH	32
#define MAX_ITEMEFFECT		127

#define MAX_GUILDZONE		5 // 0 Armia, 1 Erion, 2 Arzan, 3 Noatun, 4 gelo ? 

#define MESSAGE_LENGTH		256

#define MAX_STRING			96 // SendNotice, SendClientMsg, etc

#ifdef _DBSRV
#define MAX_DBACCOUNT		(MAX_SERVERNUMBER  * MAX_USER)
#elif _TMSRV
#define MAX_SKILLDATA		104
#define MAX_INITITEM		64

#define MAX_TRADE			15
#define MAX_AUTOTRADE		
#define MAX_SUMMONLIST		50
#define MAX_NPCGENERATOR	10000
#define MAX_SEGMENT			5
#define MAX_ROUTE			24
#endif
/* END DEFINES */

/* STRUCTS */
struct STRUCT_POSITION
{
	uint16_t	X;
	uint16_t	Y;
};
struct STRUCT_AFFECT
{
	uint16_t	Index;
	uint16_t	Master;
	uint16_t	Value;
	uint16_t	Time;
};
struct STRUCT_SCORE
{
	uint16_t	Level;
	uint16_t	Defense;
	uint16_t	Damage;
	struct
	{
		uint8_t Merchant : 4;
		uint8_t Direction : 4;
	} Reserved;

	struct
	{
		uint8_t Speed : 4;
		uint8_t AttSpeed : 4;
	} AttackRun;

	uint16_t	MaxHP, MaxMP;
	uint16_t	CurHP, CurMP;
	int16_t		STR, INT;
	int16_t		DEX, CON;
	uint8_t		Special[4];
};
struct STRUCT_ITEM
{
	uint16_t	Index;
	
	struct _Effects
	{
		uint8_t Index;
		uint8_t Value;
	} Effects[3];

#define EF1		Effects[0].Index
#define EF2		Effects[1].Index
#define EF3		Effects[2].Index
#define EFV1	Effects[0].Value
#define EFV2	Effects[1].Value
#define EFV3	Effects[2].Value
};
struct STRUCT_MOB
{
	char			Name[MOBNAME_LENGTH];
	uint8_t			CapeInfo;
	uint8_t			Merchant;
	int16_t			GuildIndex;
	uint8_t			ClassInfo;

	struct
	{
		uint8_t		SlowMov : 1;
		uint8_t		DrainHP : 1;
		uint8_t		VisionDrop : 1;
		uint8_t		Evasion : 1;
		uint8_t		Snoop : 1;
		uint8_t		SpeedMov : 1;
		uint8_t		SkillDelay : 1;
		uint8_t		Resist : 1;
	} AffectInfo;

	uint16_t		QuestInfo;
	int32_t			Gold;
	uint32_t		Exp;
	STRUCT_POSITION LastPosition;
	STRUCT_SCORE	BaseScore;
	STRUCT_SCORE	CurrentScore;
	STRUCT_ITEM		Equip[MAX_EQUIPMENT];
	STRUCT_ITEM		Inventory[MAX_INVENTORY];
	uint32_t		Learn;
	int16_t			StatusPoint;
	int16_t			MasterPoint;
	int16_t			SkillPoint;
	uint8_t			Critical;
	uint8_t			SaveMana;
	uint8_t			SkillBar[MAX_SKILLBAR];
	int8_t			GuildMemberType;
	uint8_t			MagicIncrement;
	uint8_t			RegenHP;
	uint8_t			RegenMP;
	uint8_t			Resists[4];
};

struct STRUCT_SELCHAR
{
	STRUCT_POSITION		LastPosition[MOB_PER_ACCOUNT];
	char				Name[MOB_PER_ACCOUNT][MOBNAME_LENGTH];
	STRUCT_SCORE		Score[MOB_PER_ACCOUNT];
	STRUCT_ITEM			Equipment[MOB_PER_ACCOUNT][MAX_EQUIPMENT];
	int16_t				GuildIndex[MOB_PER_ACCOUNT];
	int32_t				Gold[MOB_PER_ACCOUNT];
	uint32_t			Exp[MOB_PER_ACCOUNT];
};

struct STRUCT_ITEMLIST
{
	char Name[ITEMNAME_LENGTH];

	int16_t Mesh;
	int16_t SubMesh;

	int16_t Unknown;

	int16_t Level;
	int16_t STR;
	int16_t INT;
	int16_t DEX;
	int16_t CON;

	struct
	{
		int16_t Index;
		int16_t Value;
	} Effect[MAX_STATICEFFECT];

	int32_t Price;
	int16_t Unique;
	int16_t Pos;
	int16_t Extra;
	int16_t Grade;
};

struct STRUCT_MOUNTBONUS
{
	uint32_t	PhysicalDamage;
	uint32_t	MagicDamage;
	uint32_t	Parry;
	uint32_t	Resist;
	uint32_t	Speed;

};
#ifdef _DBSRV

struct STRUCT_ACCOUNTFILE
{
	char				AccountName[ACCOUNTNAME_LENGTH];
	char				AccountPass[ACCOUNTPASS_LENGTH];
	char				NotUsed[180];
	STRUCT_MOB			Character[MOB_PER_ACCOUNT];
	STRUCT_ITEM			Cargo[MAX_CARGO];
	int32_t				Gold;
	uint8_t				SkillBar[MOB_PER_ACCOUNT][8];
};

#elif _TMSRV

struct STRUCT_SPELL
{
	int32_t SkillPoint;
	int32_t TargetType;
	int32_t ManaSpent;
	int32_t Delay;
	int32_t Range;
	int32_t InstanceType;
	int32_t InstanceValue;
	int32_t TickType;
	int32_t TickValue;
	int32_t AffectType;
	int32_t AffectValue;
	int32_t AffectTime;
	uint8_t Act_1[8];
	uint8_t Act_2[8];
	int32_t InstanceAttribute;
	int32_t TickAttribute;
	int32_t Aggressive;
	int32_t MaxTarget;
	int32_t PartyCheck;
	int32_t AffectResist;
	int32_t Passice;
	int32_t Unknown;
};

struct STRUCT_INITITEM
{
	int16_t PosX;
	int16_t PosY;
	int16_t nIndex;
	int16_t Rotate;
};
#endif 
/* END STRUCTS */

/* PACKETS */
#define FLAG_GAME2CLIENT	0x100
#define FLAG_CLIENT2GAME	0x200
#define FLAG_GAME2DB		0x800
#define FLAG_DB2GAME		0x400
#define FLAG_DB2NP			0x1000
#define FLAG_NP2DB			0x2000

/* END PACKETS */

/***************************************************************
	STANDARD - Pacotes padrão
****************************************************************/
struct MSG_STANDARD
{
	uint16_t	Size;
	uint8_t		Key;
	uint8_t		CheckSum;
	uint16_t	PacketId;
	uint16_t	ClientId;
	uint32_t	TimeStamp;
};
struct MSG_STANDARDPARM
{
	MSG_STANDARD	Header;
	int32_t			Parm;
};
struct MSG_STANDARDPARM2
{
	MSG_STANDARD	Header;
	int32_t			Param[2];
};
struct MSG_STANDARDPARM3
{
	MSG_STANDARD	Header;
	int32_t			Param[3];
};

/*************************************************************
	GAME2DB - Pacotes que vem do gameserver
**************************************************************/
#define _MSG_DBNewCharacter			(2 | FLAG_GAME2DB)
#define _MSG_DBAccountLogin         (3 | FLAG_GAME2DB)
#define _MSG_DBCharacterLogin		(4 | FLAG_GAME2DB)
#define _MSG_DBNoNeedSave			(5 | FLAG_GAME2DB)
#define _MSG_DBSavenQuit			(6 | FLAG_GAME2DB)
struct MSG_DBSavenQuit
{
	MSG_STANDARD	Header;
	int32_t			SlotId;
	STRUCT_MOB		Mob;
	STRUCT_ITEM		Cargo[MAX_CARGO];
	int32_t			Gold;
	int8_t			SkillBar[8];
	char			AccountName[ACCOUNTNAME_LENGTH];
};
#define _MSG_DBSaveMob				(7 | FLAG_GAME2DB)
struct MSG_DBSaveMob
{
	MSG_STANDARD	Header;
	int32_t			SlotId;
	STRUCT_MOB		Mob;
	STRUCT_ITEM		Cargo[MAX_CARGO];
	int32_t			Gold;
	int8_t			SkillBar[8];
	char			AccountName[ACCOUNTNAME_LENGTH];
	int32_t			Export;
	
};
#define _MSG_DBDeleteCharacter		(9 | FLAG_GAME2DB)
struct MSG_DBDeleteCharacter
{
	MSG_STANDARD	Header;
	int32_t			SlotId;
	char			CharacterName[MOBNAME_LENGTH];
	char			Password[ACCOUNTPASS_LENGTH];
};

#define _MSG_UpdateSapphire			(10 | FLAG_GAME2DB)
struct MSG_UpdateSapphire
{
	MSG_STANDARD	Header;
	int32_t			Sapphire;
};
#define _MSG_MessageDBRecord		(12 | FLAG_GAME2DB)

#define _MSG_GuildZoneReport		(13 | FLAG_GAME2DB)
struct MSG_GuildZoneReport
{
	MSG_STANDARD	Header;
	int32_t			Guild[MAX_GUILDZONE];
};

#define	_MSG_MessageDBImple			(11 | FLAG_GAME2DB | FLAG_DB2GAME)
struct	MSG_MessageDBImple
{
	MSG_STANDARD	Header;
	char			String[MESSAGE_LENGTH];
};
#define _MSG_MessageDBRecord		(12 | FLAG_GAME2DB) 
#define _MSG_ReqGuildWar			(14 | FLAG_CLIENT2GAME | FLAG_DB2GAME | FLAG_GAME2DB) // MSG_STANDARDPARM2
#define _MSG_ReqGuildAlly			(18 | FLAG_CLIENT2GAME | FLAG_DB2GAME | FLAG_GAME2DB) // MSG_STANDARDPARM2
#define _MSG_Unknown				(16 | FLAG_DB2NP | FLAG_GAME2DB | FLAG_DB2GAME)

/**************************************************************
	DB2GAME - Pacotes do dataserver para o gameserver
**************************************************************/
#define _MSG_DBSavingQuit			(10 | FLAG_DB2GAME)
struct MSG_DBSavingQuit
{
	MSG_STANDARD	Header;
	char			AccountName[ACCOUNTNAME_LENGTH];
	int				Mode;
};
#define _MSG_DBCNFSavenQuit			(11 | FLAG_DB2GAME)
#define _MSG_DBCNFAccountLogin		(22 | FLAG_DB2GAME)
#define _MSG_DBCNFCharacterLogin	(23 | FLAG_DB2GAME)
#define _MSG_DBCNFNewCharacter		(24 | FLAG_DB2GAME)
#define _MSG_DBCNFDeleteCharacter	(25 | FLAG_DB2GAME)
#define _MSG_DBNewCharacterFail		(29 | FLAG_DB2GAME)
#define _MSG_DBDeleteCharacterFail	(30 | FLAG_DB2GAME)
#define _MSG_DBAlreadyPlaying		(31 | FLAG_DB2GAME)
#define _MSG_DBStillPlaying			(32 | FLAG_DB2GAME)    
#define _MSG_DBAccountLoginFail_Account		(33 | FLAG_DB2GAME)
#define _MSG_DBAccountLoginFail_Pass		( 34 | FLAG_DB2GAME)
#define _MSG_DBSendSapphireCount	(35 | FLAG_DB2GAME)
#define _MSG_DBAccountLoginFail_Block		(36 | FLAG_DB2GAME)
#define _MSG_DBAccountLoginFail_Disable		(37 | FLAG_DB2GAME)
#define _MSG_DBChargedGuildList		(39 | FLAG_DB2GAME)
struct MSG_DBChargedGuildList
{
	MSG_STANDARD	Header;
	int32_t			ChargedGuildList[MAX_GUILDZONE][MAX_SERVERNUMBER];
};

#define _MSG_DBImportItem			(15 | FLAG_GAME2DB | FLAG_DB2GAME)
struct MSG_DBImportItem
{
	MSG_STANDARD	Header;
	int32_t			ServerId;
	char			Account[ACCOUNTNAME_LENGTH];
	STRUCT_ITEM		Item;
};


/*************************************************************
	DB2NP & NP2DB - NPKO - DBSRV
*************************************************************/
#define _MSG_NPReqIDPASS            (1 | FLAG_DB2NP)
#define _MSG_NPIDPASS				(2 | FLAG_DB2NP)
struct MSG_NPIDPASS
{
	MSG_STANDARD	Header;
	char			Account[ACCOUNTNAME_LENGTH];
	int32_t			Encode1;
	char			Pass[ACCOUNTPASS_LENGTH];
	int32_t			Encode2;
};
#define _MSG_NPReqAccount           (3 | FLAG_NP2DB)
struct MSG_NPReqAccount
{
	MSG_STANDARD	Header;
	char			Account[ACCOUNTNAME_LENGTH];
	char			CharacterName[MOBNAME_LENGTH];
};
#define _MSG_NPNotFound            (4 | FLAG_DB2NP)
#define _MSG_NPAccountInfo         (5 | FLAG_DB2NP) 
struct MSG_NPAccountInfo
{
	MSG_STANDARD		Header;
	STRUCT_ACCOUNTFILE	Account;
	int16_t				Session;
	int16_t				State; // 0:Normal  1:Blocked(@)  2:Deleted(_)  3:Disabled(#)
};
#define _MSG_NPReqSaveAccount      (6 | FLAG_NP2DB) //   MSG_NPAccountInfo
#define _MSG_NPDisable             (7 | FLAG_NP2DB | FLAG_DB2NP)
#define _MSG_NPEnable              (8 | FLAG_NP2DB | FLAG_DB2NP) //   NPEnableParm 
struct MSG_NPEnable
{
	MSG_STANDARD	Header;
	char			AccountName[ACCOUNTNAME_LENGTH];
};
#define _MSG_NPNotice              (9 | FLAG_NP2DB | FLAG_DB2NP | FLAG_DB2GAME) //   Parm 
struct MSG_NPNotice
{
	MSG_STANDARD	Header;
	int				Parm1;
	int				Parm2;
	char			AccountName[ACCOUNTNAME_LENGTH];
	char			String[MAX_STRING];
};
#define _MSG_NPState               (10 | FLAG_NP2DB | FLAG_DB2NP)
#define _MSG_NPCreateCharacter     (11 | FLAG_NP2DB | FLAG_DB2NP) 
struct MSG_NPCreateCharacter
{
	MSG_STANDARD	Header;
	int32_t			Slot;
	char			AccountName[ACCOUNTNAME_LENGTH];
	STRUCT_MOB		Mob;
};
#define _MSG_NPDeleteCharacter		(12 | FLAG_NP2DB | FLAG_DB2NP) // Result 0: Success Delete Parm2
struct MSG_NPDeleteCharacter									  // 1: Threre's Same Name Already
{																  // 2: No space
	MSG_STANDARD	Header;
	int32_t			SlotId;
	char			AccountName[ACCOUNTNAME_LENGTH];
	int32_t			Result;
	char			CharacterName[MOBNAME_LENGTH];
};


#define _MSG_ReqTransper			(170 | FLAG_GAME2CLIENT | FLAG_CLIENT2GAME | FLAG_DB2GAME | FLAG_GAME2DB)
struct MSG_ReqTransper
{
	MSG_STANDARD	Header;
	int32_t			Result;	// 0:Success 1:Same Name Exist 2:No Space at Transper Server 3:Unknown Error	
	int32_t			Slot;
	char			OldName[MOBNAME_LENGTH];
	char			NewName[MOBNAME_LENGTH];

};
/*************************************************************
	GAME2CLIENT - Pacotes do gameserver para o client
**************************************************************/
#define _MSG_CNFAccountLogin		(14 | FLAG_GAME2CLIENT)
struct MSG_CNFAccountLogin
{
	MSG_STANDARD	Header;
	STRUCT_SELCHAR	SelChar;
	STRUCT_ITEM		Cargo[MAX_CARGO];
	int32_t			Gold;
	char			AccountName[ACCOUNTNAME_LENGTH];
};
#define _MSG_CNFNewCharacter		(16 | FLAG_GAME2CLIENT)
struct MSG_CNFNewCharacter
{
	MSG_STANDARD	Header;
	STRUCT_SELCHAR  SelChar;
};
#define _MSG_CNFDeleteCharacter		(18 | FLAG_GAME2CLIENT)
struct MSG_CNFDeleteCharacter
{
	MSG_STANDARD	Header;
	STRUCT_SELCHAR	SelChar;
};
#define _MSG_CNFCharacterLogin		(22 | FLAG_GAME2CLIENT)
struct MSG_CNFCharacterLogin
{
	MSG_STANDARD	Header;
	STRUCT_POSITION LastPosition;
	STRUCT_MOB		Mob;
	int32_t			SlotIndex;
	int32_t			ClientIndex;
	uint8_t			SkillBar[8];
};

/*************************************************************
	CLIENT2GAME - Pacotes do client para o gameserver
**************************************************************/
#define _MSG_AccountLogin			(13 | FLAG_CLIENT2GAME)
struct MSG_AccountLogin
{
	MSG_STANDARD	Header;
	char			AccountName[ACCOUNTNAME_LENGTH];
	char			Password[ACCOUNTPASS_LENGTH];

	int32_t			Version;
	int32_t			Unknown;

	char			IPAddr[16];
	char			Keys[16];
	char			UNK[36];
};
#define _MSG_NewCharacter			(15 | FLAG_CLIENT2GAME)
struct MSG_NewCharacter
{
	MSG_STANDARD	Header;
	int32_t			Slot;
	char			CharacterName[MOBNAME_LENGTH];
	int32_t			ClassId;
};

/*********************************************************************************************
	BASE FUNCTIONS
*********************************************************************************************/
bool		BASE_CheckPacket(MSG_STANDARD* msg);
bool		BASE_CanCargo(STRUCT_ITEM* item, STRUCT_ITEM* cargo, int32_t destX, int32_t destY);
void		BASE_ClearItem(STRUCT_ITEM* item);
void		BASE_ClearMob(STRUCT_MOB* mob);
void		BASE_GetFirstKey(char* source, char* dest);
int32_t		BASE_GetGuild(STRUCT_ITEM* item);
int32_t		BASE_GetItemAbility(STRUCT_ITEM* item, uint8_t ef);
int32_t		BASE_GetItemSanc(STRUCT_ITEM* item);
bool		BASE_InitializeBasedef();
bool		BASE_InitializeServerlist();
void		BASE_InitializeModuleDir();
bool		BASE_InitializeEffectName();
bool		BASE_InitializeItemList();
bool		BASE_InitializeEncryption();
bool		BASE_ReadItemListFile(char* filename, bool build);
bool		BASE_WriteItemList(bool write);
int32_t		BASE_GetSum2(uint8_t* p, int32_t pSize);

/*********************************************************************************************
	NEW FUNCTIONS
*********************************************************************************************/
bool		BASE_InitializeBaseSet();


/*********************************************************************************************
	GAMESERVER ONLY FUNCTIONS
*********************************************************************************************/
#ifdef _TMSRV

#endif

#endif // !BASEDEF_H
