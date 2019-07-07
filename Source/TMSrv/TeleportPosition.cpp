#include "DLLMain.h"

INT32 HKD_ControlTeleportPosition(INT16 clientId, INT16 *ptrX, INT16 *ptrY)
{
	INT16 posX = (*ptrX & 0xFFC),
		    posY = (*ptrY & 0xFFC);

	INT16 LoggedMob = nBuffer[clientId].Ingame.LoggedMob;

	st_Mob *player = (st_Mob*)GetMobFromIndex(clientId);

	// Noatum para Quione
	if (posX == 1056 && posY == 1724)
	{
		if (Servidor.QuioneStatus == QUIONE_ALIVE)
		{
			*ptrX = 3928;
			*ptrY = 2880;

			SendClientMessage(clientId, "Boa sorte guerreiro!");

			return 0;
		}
       	
		SendClientMessage(clientId, "Nosso mundo está livre de Quione!");

		return 0;
   	}
	// Deserto para Kefra
	else if(posX == 1312 && posY == 1900)
	{
		*ptrX = 2366;
		*ptrY = 4064;
		return 1000;
	}
	// Kefra para Deserto
	else if(posX == 2364 && posY == 4072)
	{
		*ptrX = 1314;
		*ptrY = 1897;
		return 1000;
	}
	// Noatum para Gelo
 	else if(posX == 1052 && posY == 1708)
   	{
		*ptrX = 3649;
       	*ptrY = 3109;
		player->Info.CityID = 3;
       	return -1;
   	}
	// Gelo para Noatum
   	else if(posX == 3648 && posY == 3108)
   	{
       	*ptrX = 1053;
       	*ptrY = 1710;
       	return -1;
   	}	

	else if (posX == 1004 && posY == 3995)
	{
		*ptrX = 745;
		*ptrY = 3816;
		return -1;
	}

	else if (posX == 745 && posY == 3816)
	{
		*ptrX = 1004;
		*ptrY = 3995;
		return -1;
	}
	// Kefra -> Hall do Kefra
	else if (posX == 2365 && posY == 3892)
	{
		if (nBuffer[clientId].Chars[LoggedMob].Entradas > 0)
		{
			nBuffer[clientId].Chars[LoggedMob].Entradas--;
			*ptrX = 2366;
			*ptrY = 3914;
		}
		else
			SendClientMessage(clientId, "Não possui entradas disponíveis.");

		return -1;
	}

	else if (posX == 2548 && posY == 1740)
	{
		*ptrX = 2285;
		*ptrY = 3689;

		return 1000;
	}
		
	return -1;
}