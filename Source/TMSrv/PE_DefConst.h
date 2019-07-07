#ifndef __PE_DEF_CONST_H__
#define __PE_DEF_CONST_H__

#include <Windows.h>
#include "../Common/DefaultEncDecInfoV754.h"
#include "../Common/st_Base.h"
#include "GlobalData.h"
#include "DLLMain.h"

void SetDefaultConstants();
void SetVersion();
void SetEncDec();
void SetSkillDataString();
void SetSkillIndex();
void SetMobIndex();
void SetSockAddrPorts();
void SetPacketSize();
void SetMaxLevel_TrainningCamp();
void ChangeItemlistStructPointer();
void TranslateBaseNPCFiles();
void ChangeLimitAffects();
void RedirectCUserBuffer();
void CorrectSomeSizes();
void ChangeMountStatus();
void LimiteHpMp();
void ChangeTransBonusStructurePointer();
void ChangeSkillDataStructurePointer();
void NPCGenerLimit();

#endif // __PE_DEF_CONST_H__