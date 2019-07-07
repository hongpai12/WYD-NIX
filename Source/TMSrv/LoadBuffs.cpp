#include "DLLMain.h"

void __stdcall HKD_ProcessDBMessage_LoadBuffs(INT32 clientid)
{
	memcpy(&mBuffer[clientid].Affects[0], &nBuffer[clientid].Chars[nBuffer[clientid].Ingame.LoggedMob].Buffs[0], sizeof(stAffects) * MAX_AFFECTS);		

	GetCurrentScore(clientid);
	SendScore(clientid);			
	SendEtc(clientid);
}