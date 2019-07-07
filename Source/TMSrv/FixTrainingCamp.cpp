#include "DLLMain.h"

bool __stdcall HKD_FixTrainningCamp(INT32 clientid)
{
	bool sucess = true;

	st_Mob *player = GetMobFromIndex(clientid);

	if (player->Equip[0].EFV2 > MORTAL)
		sucess = false;

	else if (player->Status.Level > 9 && player->Status.Level < 999)
		sucess = false;

	if (nBuffer[clientid].AcessLevel > NORMAL_LEVEL)
		sucess = true;

	if (!sucess)
	{
		DoTeleport(clientid, 2100 + (rand() % 10), 2100 + (rand() % 10));

		SendClientMessage(clientid, "Área permitida somente para mortais e níveis abaixo de 10.");
	}

	return sucess;
}