#include "DLLMain.h"

bool HKD_CorrectPositionGoWorld(INT32 ClientID, INT16* PosX, INT16* PosY)
{
	st_Mob *player = GetMobFromIndex(ClientID);	
	nInfo* nPlayer = &nBuffer[ClientID];

	if(player->Info.CityID == 3) // Cidade de Karden implementada.
	{
		*PosX = 3645 + (Rand() % 3);
		*PosY = 3122 + (Rand() % 3);
	}		

	if (player->Equip[15].Index == 4006)
	{
		memset(&player->Equip[15], 0, sizeof(st_Item));

		SendItem(ClientID, EQUIP, 15, &player->Inventory[15]);

		player->Equip[15].Index = 0;
	}

	/*if (player->bStatus.Level <= 9)
	{
		*PosX = 2141;
		*PosY = 1947;
	}*/
	
	BASE_GetEmptyMobGrid(ClientID, (int*)&PosX, (int*)&PosY);					

	SendEtc(ClientID);		

	return true;
}