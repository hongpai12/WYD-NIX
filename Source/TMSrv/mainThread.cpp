#include "DLLMain.h"

void WINAPI MainThread()
{
	while (1)
	{
		clock_t InitialTime = clock();

		time_t rawnow = time(NULL);
		struct tm *now = localtime(&rawnow);

		Servidor.OnlineCount = 0;

		st_Mob *mob = (st_Mob*)(0x0015C69B8);

		for (int i = 0; i < MAX_PLAYER; i++)
		{
			// Salva os dados do usuário
			if (Users[i].Status)
			{
				Servidor.OnlineCount++;

				if (Users[i].Status == 22)
				{
					VerifyQuests(i);
					VerifyPesa(i);
					VerifyGuildArea(i);
					VerifyCampoTreino(i);

					st_Mob* player = &mob[i];

					if (player->Equip[0].EFV2 < SEMIDEUS)
					{
						for (int z = 1; z < 16; z++)
						{
							if (CheckArchItem(player->Equip[z].Index))
							{
								memset(&player->Equip[z], 0, sizeof st_Item);
								SendItem(i, EQUIP, z, &player->Equip[z]);
							}
						}
					}

					if (player->Equip[7].Index > 0)
					{
						for (int z = 0; z < 3; z++)
							if (player->Equip[7].Effect[z].Index >= 116 && player->Equip[7].Effect[z].Index <= 125)
							{
								player->Equip[7].Effect[z].Index = 43;
								SendItem(i, EQUIP, 7, &player->Equip[7]);
								break;
							}
					}
					
					if (player->Equip[6].Index > 0)
					{
						for (int z = 0; z < 3; z++)
							if (player->Equip[6].Effect[z].Index >= 116 && player->Equip[6].Effect[z].Index <= 125)
							{
								player->Equip[6].Effect[z].Index = 43;
								SendItem(i, EQUIP, 6, &player->Equip[6]);
								break;
							}
					}

					if (player->Inventory[60].Index != 3467)
					{
						player->Inventory[60].Index = 3467;
						SendItem(i, INVENTORY, 60, &player->Inventory[60]);
					}

					if (player->Inventory[61].Index != 3467)
					{
						player->Inventory[61].Index = 3467;
						SendItem(i, INVENTORY, 61, &player->Inventory[61]);
					}

					if (!(now->tm_sec % 5) && player->GuildIndex)
						UpdateGuildInfo(i);
				}

				if (nBuffer[i].Ingame.ArrobaTime > 0)
					nBuffer[i].Ingame.ArrobaTime--;
				if (nBuffer[i].Ingame.hasFailed > 0)
					nBuffer[i].Ingame.hasFailed--;

				if (!(now->tm_sec % 5))
					SaveAccount(i);
			}
		}

		if (!now->tm_sec)
		{
			// Todo início de minuto
			// Chamar neste if as funções executadas em x minutos
			// Verificar o resto da divisão do minuto atual pela quantidade de minutos desejada

			if (!now->tm_min)
			{
				if (Servidor.QuioneStatus == QUIONE_DEAD)
				{
					CreateMob("Quione", 3928, 2880, "boss");
					SendNotice("Quione despertou!!");

					Servidor.QuioneStatus = QUIONE_ALIVE;
				}

				if (!now->tm_hour)
				{
					if (now->tm_wday == 6)
						SendNotice("Double exp ativado!! Bom up a todos.");
					else if (now->tm_wday == 1)
						SendNotice("Double exp desativado. Boa sorte a todos.");
					else if (now->tm_wday == 5) // Sexta feira
					{
						for (INT32 i = 0; i < 64000; i++)
							Guilds[i].CidadeApostada = NONE__CITYID;
					}
				}

				if (now->tm_wday == 6)
				{
					if (now->tm_hour == 17)
					{
						// War Noatun
						for (INT32 i = 0; i < 64000; i++)
							if (Guilds[i].CidadePossuida)
								Guilds[i].CidadePossuida = 0;
					}
					else if (now->tm_hour >= 18 && now->tm_hour <= 21 && now->tm_wday == 6)
					{
						// Guerras de cidade
						if (now->tm_hour == 18)
						{
							StartWar(KARDE_CITYID);
						}
						else if (now->tm_hour == 19)
						{
							DefineWinner(KARDE_CITYID);
							StartWar(ERION_CITYID);
						}
						else if (now->tm_hour == 20)
						{
							DefineWinner(ERION_CITYID);
							StartWar(AZRAN_CITYID);
						}
						else if (now->tm_hour == 21)
						{
							DefineWinner(AZRAN_CITYID);
							StartWar(ARMIA_CITYID);
						}
						else if (now->tm_hour == 22)
							DefineWinner(ARMIA_CITYID);
					}
				}
			}

			if (!now->tm_min || now->tm_min == 30)
				StartThread(BatalhaEscuridao);
			else if (now->tm_min == 10 || now->tm_min == 40)
				FinalizaBatalhaEscuridao();

			else if (now->tm_hour >= 17 && now->tm_hour <= 20 && now->tm_wday == 6)
			{
				if (now->tm_min == 55 || now->tm_min == 56 || now->tm_min == 57 || now->tm_min == 58 || now->tm_min == 59)
				{
					char szMsg[120];
					sprintf(szMsg, "Guerra começa em %d minutos, preparem-se participantes.", 60 - now->tm_min);
					SendNotice(szMsg);
				}	
			}

			if (!now->tm_min || now->tm_min == 15 || now->tm_min == 30 || now->tm_min == 45)
			{
				for (int i = 0; i < 3; i++)
				{
					// Remove os players da área e depois mata os mobs
					ClearArea(MaxMinPesaCoords[i][0], MaxMinPesaCoords[i][1], MaxMinPesaCoords[i][2], MaxMinPesaCoords[i][3]);
					KillMobsOnArea(MaxMinPesaCoords[i][0], MaxMinPesaCoords[i][1], MaxMinPesaCoords[i][2], MaxMinPesaCoords[i][3]);
				}

				for (int i = 0; i < sizeof(QuestList) / sizeof(Quests); i++)
				{
					ClearArea(MaxMinCoordsQuest[i][0], MaxMinCoordsQuest[i][1], MaxMinCoordsQuest[i][2], MaxMinCoordsQuest[i][3]);
					KillMobsOnArea(MaxMinCoordsQuest[i][0], MaxMinCoordsQuest[i][1], MaxMinCoordsQuest[i][2], MaxMinCoordsQuest[i][3]);

					QuestList[i].KillCount = 0;

					for (int z = 0; z < 40; z++)
						QuestList[i].CidID[z] = 0;
				}

				ClearArea(3600, 3600, 3691, 3691);
				ClearArea(3730, 3474, 3820, 3564);
				ClearArea(3860, 3600, 3948, 3700);

				memset(&Pesa, 0, sizeof(Pesas) * 3);
			}
			else if (now->tm_min == 1 || now->tm_min == 16 || now->tm_min == 31 || now->tm_min == 46)
				CreateMobsOnPesa();

			if (!(now->tm_min % 5)) // 5 em 5 minutos
				SendAutomaticMessages();

			int i = 0;
			while (ResetList[i].Time)
			{
				if (!(now->tm_min % ResetList[i].Time))
				{
					if (ResetList[i].MinLevel > 0)
						ClearAreaLevel(ResetList[i].MinCoords.X, ResetList[i].MinCoords.Y, ResetList[i].MaxCoords.X,
						ResetList[i].MaxCoords.Y, ResetList[i].MinLevel, ResetList[i].MaxLevel);
					else
						ClearArea(ResetList[i].MinCoords.X, ResetList[i].MinCoords.Y, ResetList[i].MaxCoords.X, ResetList[i].MaxCoords.Y);
				}

				i++;
			}

			SaveGuilds();
			SaveServerData();
		}

		Servidor.Seconds++;

		if (Servidor.Seconds >= 60)
		{
			Servidor.Minuts++;
			Servidor.Seconds -= 60;
		}
		if (Servidor.Minuts >= 60)
		{
			Servidor.Minuts -= 60;
			Servidor.Hours++;

			sendLog(INFO, 1000, "Servidor atingiu %d hora(s) online.", Servidor.Hours);
		}
		if (Servidor.Hours >= 24)
		{
			Servidor.Days++;
			Servidor.Hours -= 24;

			sendLog(INFO, 1000, "Servidor atingiu %d dia(s) online.", Servidor.Days);
		}

		for (int i = 0; i < 3; i++)
			CheckFimPesa(&Pesa[i], i);

		DecrementWaterTime();
		DisplayTitleInfos();

		clock_t FinalTime = clock();

		int sleepTime = 1000 - (FinalTime - InitialTime);
		if (sleepTime < 0)
			sleepTime = 0;
		if (sleepTime > 1000)
			sleepTime = 1000;
		// Não cometer a negragem de mexer no counter do sleep
		// Muito menos a negragem de dar break na porra do loop
		// Se der return nessa porra dessa função vou comer teu cu
		// Pensando bem, mexe em nada nessa porra
		Sleep(sleepTime);
	}
}