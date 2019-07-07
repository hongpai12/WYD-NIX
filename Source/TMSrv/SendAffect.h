#ifndef __FIX_SEND_AFFECT_H__
#define __FIX_SEND_AFFECT_H__

#include "NativeFunctions.h"
#include "../Common/st_Base.h"

#pragma region Packet Struct v613
typedef struct
{
	PacketHeader Header;

	WORD ClientId;
	WORD AtackCount;

	st_Position PositionInitial;
	st_Position PositionFinal;

	WORD SkillId; // SkillId

	WORD Offset_26; // Unknow
	WORD a2;

	BYTE Offset_29[10]; // 0x0...

	WORD AffectedMobId;

	WORD AffectInfo;
} p613_SendAffect;
#pragma endregion

#pragma region Packet Struct v754
typedef struct
{
	PacketHeader Header;

	WORD ClientId;
	WORD AtackCount;

	st_Position PositionInitial;
	st_Position PositionFinal;

	WORD SkillId; // SkillId

	DWORD Offset_26; // Unknow

	BYTE Offset_29[10]; // 0x0...

	DWORD Hold; // Poss�vel hold...

	WORD affectedMobId;

	WORD affectInfo;
} p754_SendAffect;

typedef struct
{
	BYTE Index;
	BYTE Master;
	WORD Value;
	DWORD Time;
} p_Affect;

typedef struct
{
	PacketHeader Header;

	p_Affect Affect[16];
} p754_GetAffect;

struct stTarget
{
	UINT16 Index;
	UINT16 Damage;
};

struct p39D_613 {
	PacketHeader Header;
	UINT16 m_ClientId; // 12
	UINT16 m_AttackCount; // 14 
	st_Position m_AttackerPosition; // 16
	st_Position m_AffectedPosition; // 20
	UINT16 m_SkillId; // 24
	UINT16 m_CurrentMp; // 26

	UINT8 m_Motion; // 28
	UINT8 m_SkillParam; // 29
	UINT8 m_FlagLocal; // 30
	UINT8 m_Critical; // 31
	UINT32 m_Hold; // 32
	UINT32 m_CurrentExp; // 36
	stTarget m_Target; // 44
};

typedef struct
{
	PacketHeader Header;
	UINT16 m_ClientId;
	UINT16 m_AttackCount;

	st_Position m_AttackerPosition;
	st_Position m_AffectedPosition;

	UINT16 m_SkillId;
	UINT16 m_CurrentMp;

	UINT8 m_Motion;
	UINT8 m_SkillParam;
	UINT8 m_FlagLocal;
	UINT8 m_Critical;

	UINT32 m_Hold;
	UINT32 m_CurrentExp;

	UINT16 m_ReqMp;
	UINT16 Unknown_42;

	stTarget m_Target;
} p39D_754;

typedef struct {
	PacketHeader Header;
	UINT16 m_ClientId; // 12
	UINT16 m_AttackCount; // 14
	st_Position m_AttackerPosition; // 16
	st_Position m_AffectedPosition; // 20
	UINT16 m_SkillId; // 24
	UINT16 m_CurrentMp; // 26

	UINT8 m_Motion; // 28
	UINT8 m_SkillParam; // 29
	UINT8 m_FlagLocal; // 30
	UINT8 m_Critical; // 31
	UINT32 m_Hold; // 32
	UINT32 m_CurrentExp; // 36
	UINT16 m_ReqMp; // 40

	UINT16 Unknown_42; // 42
	stTarget m_Target[13]; // 44
}p36C;
#pragma endregion

void FixSendAffect_SERVER(BYTE *m_PacketBuffer, BYTE *packetBuffer);
void FixSendAffect_CLIENT(BYTE *m_PacketBuffer, bool* pRetn);
void FixSendAffect39E_SERVER(BYTE *m_PacketBuffer, BYTE *packetBuffer);
void FixSendAffect39E_CLIENT(BYTE *m_PacketBuffer, bool* pRetn);
void __stdcall SendAffect(UINT32 ClientID);
void SkillAlchemy(UINT8 *m_PacketBuffer, bool *pRetn);
int __stdcall ControllDamage(DWORD Attacker, DWORD Target, DWORD Damage, DWORD TYPE);
INT32 HKD_GetAttack_CriticalHit(INT32 clientId, p39D_613 *p);
bool __stdcall ControllBossDamage(DWORD mobId, DWORD damage);

#endif // __FIX_SEND_AFFECT_H__