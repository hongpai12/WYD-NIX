#ifndef __MOB_KILLED_H__
#define __MOB_KILLED_H__

#include <Windows.h>
#include "NativeFunctions.h"
#include "../Common/st_Base.h"

void HKD_MobKilled_DropGold(int index, int gold);
void __stdcall HKD_MobKilled_ControlDropItem(INT32 clientid, st_Item* item);
bool __stdcall HKD_MobKilled_EventDead(INT32 target, INT32 attacker);

#endif // __MOB_KILLED_H__