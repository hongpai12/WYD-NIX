#include "DLLMain.h"

void RemoveParty(INT32 Cid)
{
	st_Mob *player = GetMobFromIndex(Cid);

	int LOCAL_1 = player->Leader;

	if(LOCAL_1 < 0 || LOCAL_1 >= 30000)
	    return;

	if(LOCAL_1 && LOCAL_1 < 750)
	{
		if(Users[LOCAL_1].Status != 22)
		{
			player->Leader = 0;
			return;
		}
	}

	if(Cid > 0 && Cid < 750)
	    SendRemoveParty(Cid, 0);

	if(LOCAL_1)
	{
		SendRemoveParty(LOCAL_1, Cid);

		st_Mob *Leader = GetMobFromIndex(LOCAL_1);

		if(Cid < 1000)
			sendLog(INFO, Cid, "%s %s saiu da party de %s %s.", Users[Cid].Username, player->Name, Users[LOCAL_1].Username, Leader->Name);

		player->Leader = 0;

		for(int LOCAL_2 = 0; LOCAL_2 < 12; LOCAL_2++)
		{
			int LOCAL_3 = Leader->PartyList[LOCAL_2];

			if(!LOCAL_3)
				continue;

			// Remove o clientid que saiu do grupo da array do lider
			if(LOCAL_3 == Cid)
				Leader->PartyList[LOCAL_2] = 0;

			// Remove os nomes da janela do client que saiu do grupo
			if(LOCAL_3 > 0 && LOCAL_3 < 750)
				if(Users[LOCAL_3].Status == 22)
					SendRemoveParty(LOCAL_3, Cid);
		}

	}
	else // Lider saindo do grupo
	{
		INT8 groupCount = 0;
		int LOCAL_4 = 0;

		// Encontra o número total de players no grupo
		for(INT8 i = 0; i < 12; i++)
			if(player->PartyList[i] < 750 && Users[player->PartyList[i]].Status == 22)
				groupCount++;

		if(player->PartyList[0] < 750 && groupCount >= 2)
		{
			st_Mob *newLider = GetMobFromIndex(player->PartyList[0]);

			// Salva a info no primeiro membro do grupo
			memcpy(newLider->PartyList, &player->PartyList[1], sizeof INT16 * 11);

			// Informa o novo lider de sua condição
			newLider->Leader = 0;
			SendRemoveParty(player->PartyList[0], Cid);
			SendAddParty(player->PartyList[0], player->PartyList[0], 0);

			LOCAL_4 = 1;

			sendLog(INFO, player->PartyList[0], "%s Herdou a party de %s.", newLider->Name, player->Name);
			sendLog(INFO, Cid, "Lider %s %s deixou a party.", Users[Cid].Username, player->Name);
		}	
		else
		{
			for (int LOCAL_4 = 0; LOCAL_4 < 12; LOCAL_4++)
			{
				int LOCAL_5 = player->PartyList[LOCAL_4];

				if (LOCAL_5 <= 0 || LOCAL_5 > 30000)
					continue;

				player->PartyList[LOCAL_4] = 0;

				st_Mob *Member = GetMobFromIndex(LOCAL_5);
				Member->Leader = 0;

				if (LOCAL_5 <= 0 || LOCAL_5 >= 750)
					continue;

				if (Users[LOCAL_5].Status == 22)
					SendRemoveParty(LOCAL_5, 0);
			}

			return;
		}

		for(; LOCAL_4 < 12; LOCAL_4++)
		{
			int LOCAL_5 = player->PartyList[LOCAL_4];

			if(LOCAL_5 <= 0 || LOCAL_5 > 30000)
				continue;

			st_Mob *mob = GetMobFromIndex(LOCAL_5);

			mob->Leader = player->PartyList[0];

			SendRemoveParty(LOCAL_5, Cid);

			SendAddParty(LOCAL_5, player->PartyList[0], 0);
		}

		memset(player->PartyList, 0x0, sizeof INT16 * 12);
	}
}
