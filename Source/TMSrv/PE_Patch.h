#ifndef __PE_PATCH_H__
#define __PE_PATCH_H__

#include <Windows.h>

#include "AddMessage.h"
#include "ReadMessage.h"
#include "PacketControl.h"

#include "../Common/PE_Hook.h"
#include "../Common/st_Base.h"

void DoIndirections();
void ChangeItemlistStructurePointer();

// CPSock
void NKD_ProcessRecv();
void NKD_ProcessSend();
void NKD_AddMessage_EncKey_1();
void NKD_ReadMessage_DecKey_1();
void NKD_ReadMessage_DecKey_2();

// Server
void NKD_CheckPacket_GetCreateMobSize();
void NKD_CheckPacket_SendTradeSize();
void NKD_MobKilled_DropGold();

//MobKilledEventDead
void NKD_MobKilled_EventMobDead();
BOOL HKD_MobKilled_EventMobDead(unsigned short npcId, unsigned short clientId, unsigned short mobId);

// Limite de buffs
void __stdcall ZeroAffect(UINT16 ClientID);
void CheckAffects();
void FixAffects();
void RemoveAffects();
void GetBuffer();
void AfterAffects();
void NKD_StandingByProcessor_VerifyAbility_1();
void NKD_GetCurrentScore_VerifyAbility_1();
void NKD_GetDoubleCritical_SetNewCriticalRate();
void NKD_GetGetCurrentScore_FixCriticalRate();
void minharola();
void Fix_IntegerDivisionByZero();
void NKD_GetMobAbility_Spectral();
void NKD_CorrectRefs();
void NKD_ProcessDBMessage_LoadBuffs();
void NKD_CorrectSleip();
void NKD_MobKilled_ControlDropItem();
void NKD_MobKilled_EventDead();
void NKD_NewTrade();
void NKD_CorrectPositionGoWorld();
void NKD_ControlTeleportPosition();
void NKD_ProcessClientMessage_UseMountFeed();
void NKD_GenerateSummon_CanSummon();
void NKD_BASE_GetCurrentScore_BlockStatus();

void NKD_AddMessage();
void NKD_ReadMessage();

//Fix Eden
void NKD_GetCurrentScore_SancTransform();
void NKD_Base_GetCurrentScore_FixEdenStatus();
void NKD_Base_GetCurrentScore_FixFaceArch();
void NKD_Base_GetCurrentScore_FixTrans();
void __stdcall HKD_Base_GetCurrentScore_FixFaceArch(st_Mob &player);
void __stdcall HKD_Base_GetCurrentScore_FixTrans(st_Mob &player, INT32 transId);

void NKD_BASE_GetSkillDamage_ChangeMagicIncrement();

void NKD_FixTrainningCamp();
void NKD_MobKilled_ControlExp();
void NKD_ExpInPesaAllArea();

void NKD_CheckGetLevel();

void NKD_BASEGetDamage_02();
void NKD_BASEGetDamage_01();

void NKD_BASEGetSkillDamage();

void NKD_GetCurScore();
void NKD_GetAttack_CriticalHit();
void NKD_ControllBossDamage_01();
void NKD_ControllBossDamage_02();
void NKD_ControllBossDamage_03();
void NKD_ControllBossDamage_04();

void NKD_GetParryRate_JewelOfPrecision();
void NKD_GetParryRate_JewelOfPrecision_2();

void NKD_BASE_GetSkillDamage_OL2_FixLevel();
void NKD_BASE_GetCurScore_FixLevel();
void NKD_ApplyHp_FixLevel();
void NKD_ApplyMp_FixLevel();

void NKD_BASE_GetCur_FixNewMontsSpeed();
void NKD_ProcessClientMessage_ApplyBonus();

void NKD_BASE_CanEquip_REQ_SIDC();
void NKD_BASE_CanEquip_VerifClassInfo();

void  HKD_CloseUser();

void NKD_GetCurScore_ControlAffects();
void NKD_GetParryRate_FixEvasion();
void NKD_GetParryRate_FixEvasion_2();
void NKD_FixApplyBonusLevel();
void NKD_StoreInNewCity_Fix();

void NKD_GetExpApply_01();
void NKD_GetExpApply_02();
void NKD_GetExpApply_03();

void __stdcall DoRemovePossuido(int cId);
void NKD_FixSpectralVisual();
void NKD_FixSkillsGetCurrentScore();
void FixPossuido();
void NKD_Correct_Damage_HT();

void NKD_ProcessClientMessage_FixDamage();
void NKD_GetAttack_FixMobDamage();
#endif // __PE_PATCH_H__