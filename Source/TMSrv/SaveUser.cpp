#include "DLLMain.h"

void SaveUserInformations(INT32 clientid)
{
	st_Mob *player = GetMobFromIndex(clientid);		 

	if(Users[clientid].Status > 0)
		memcpy(&nBuffer[clientid].Chars[nBuffer[clientid].Ingame.LoggedMob].Buffs[0], &mBuffer[clientid].Affects[0], sizeof(stAffects) * 16);						
}

void HKD_SaveUser(INT32 clientid, bool reason)
{
	SaveUserInformations(clientid);
	SaveUser(clientid, reason);
	SaveAccount(clientid);		
}