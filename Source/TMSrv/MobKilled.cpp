#include "MobKilled.h"

#include "Functions.h"
#include "Send.h"

void HKD_MobKilled_DropGold(int index, int gold)
{
	st_Mob *mob = GetMobFromIndex(index);

	int VipType = GetVipType(index);

	if (VipType > NORMAL_PLAYER)
	{
		if (VipType == FOGO_VIP)
			gold = gold + (gold / 2);
		else
			gold *= 2;
	}

	unsigned int xgold = mob->Gold + gold;
	if(xgold > 2000000000)
	{
		int zgold = 2000000000 - mob->Gold;
		if(zgold > 0)
		{
			if(gold > zgold)
				gold = zgold;
		}
		else
		{
			// Atingiu o limite máximo de gold
			return ;
		}
	}

	
	if(mob->Gold + gold >= 2000000000)
	{
		SendClientMessage(index, "Não tem mais espaço para guardar gold.");
		mob->Gold = 2000000000;
	}
	else
		mob->Gold += gold;

	SendClientSignalParm(index, index, 0x3AF, mob->Gold);
}

static const int DontDrop[] = {
	5155, 5156, 5334,
	5335, 5336, 5337, 5338
};

static const int FixDrop[] = {
	410, 412, 413, 419, 420, 697,
	699, 777, 1731, 1739, 2390, 2391,
	2392, 2393, 2394, 2395, 2396, 2397,
	2398, 2399, 2400, 2401, 2402, 2403,
	2404, 2405, 2406, 2407, 2408, 2409,
	2410, 2411, 2412, 2413, 2414, 2415,
	2416, 2417, 2418, 2419, 3171, 3172,
	3173, 3182, 3314, 3324, 3325, 3326,
	3330, 3447, 3448, 4011, 4019, 4020,
	4140, 4141, 4144, 4145, 4146, 4905,
	4117, 4118, 4119, 4120, 4121
};

void __stdcall HKD_MobKilled_ControlDropItem(INT32 clientid, st_Item* item)
{
	// Caso o ID do item seja inexistente
	if (item->Index < 0 || item->Index > 6500)
		return;
	else if (clientid < 0 || clientid > MAX_PLAYER)
		return;
	else
	{
		for (int i = 0; i < sizeof(DontDrop) / sizeof(int); i++)
			if (item->Index == DontDrop[i])
				return;
	}

	nInfo* nB = &nBuffer[clientid];

	int Vip = GetVipType(clientid);

	if (Vip > NORMAL_PLAYER && nB->Ingame.AutoDrop.isOn)
	{
		int maxFilter = 0;

		if (Vip == FOGO_VIP)
			maxFilter = 3;
		else
			maxFilter = 7;

		bool checkFilter = false;

		for (int i = 0; i < maxFilter; i++)
		{
			if (item->Index == nB->Ingame.AutoDrop.Index[i])
			{
				checkFilter = true;

				break;
			}
		}

		if (!checkFilter)
		{
			SendClientMsg(clientid, "![%s] foi deletado pelo autodrop.", itemlistBuffer[item->Index].Name);
			
			st_Mob* player = GetMobFromIndex(clientid);
			sendLog(INFO, clientid, "%s %s Item %s deletado pelo autodrop.", nB->Login, player->Name, item->Index);
			
			return;
		}
	}

	for (int i = 0; i < sizeof(FixDrop) / sizeof(int); i++)
		if (item->Index == FixDrop[i])
			memset(item->Effect, 0, sizeof(BYTE) * 6);

	if (GiveItem(clientid, item) == -1)
		SendClientMsg(clientid, "![%s] foi deletado por não haver espaço no inventário.", itemlistBuffer[item->Index].Name);
}

bool __stdcall HKD_MobKilled_EventDead(INT32 target, INT32 attacker)
{
	bool morre = true;

	if (target < 0 || target > 30000)
		return morre;
	else if (attacker < 0 || attacker > 30000)
		return morre;

	time_t rawnow = time(NULL);
	struct tm *now = localtime(&rawnow);

	if (attacker > MAX_PLAYER)
	{
		st_Mob *evock = GetMobFromIndex(attacker);
		if (evock->Summoner > 0 && evock->Summoner < MAX_PLAYER)
			attacker = evock->Summoner;
	}

	st_Mob *mobs = (st_Mob*)(0x0015C69B8),
		   *player = &mobs[attacker],
		   *mob = &mobs[target];

	MapAttribute map = GetAttribute(player->TargetX, player->TargetY);

	if (target < MAX_PLAYER)
	{
		
#pragma region PERGARESSU
		INT16 PergaRessu = GetFirstSlot(target, 3463, INVENTORY);
		if (PergaRessu != -1 && !map.Village)
		{
			AmountMinus(&mob->Inventory[PergaRessu]);
			SendItem(target, INVENTORY, PergaRessu, &mob->Inventory[PergaRessu]);

			sendLog(INFO, target, "%s %s ressucitado pelo pergaminho da ressureição.", nBuffer[target].Login, mob->Name);

			return false;
		}
#pragma endregion

		if (attacker < MAX_PLAYER && Servidor.Batalha.Status == BATALHA_INICIADA)
		{
			if (player->TargetX >= 280 && player->TargetX <= 362 &&
				player->TargetY >= 270 && player->TargetY <= 350)
			{
				int AliveCount[2] = { 0, 0 };

				for (int i = 0; i < 2; i++)
				{
					for (int e = 0; e < 10; e++)
					{
						if (Servidor.Batalha.ShadowFightTeams[i][e] == target)
							Servidor.Batalha.FighterStatus[i][e] = FIGHTER_DEAD;

						if (Servidor.Batalha.FighterStatus[i][e] == FIGHTER_ALIVE)
							AliveCount[i]++;
					}
				}
				
				if (AliveCount[0] <= 0 || AliveCount[1] < 0)
				{
					INT16 ids[6]     = { 3338, 3026, 3026, 4011, 3501, 1726 };
					INT8  adds[6][6] = { 
						{ EF_SANC,     7, EF_NONE, EF_NONE, EF_NONE, EF_NONE },
						{ EF_DAMAGE, 120, EF_NONE, EF_NONE, EF_NONE, EF_NONE },
						{ EF_MAGIC,   50, EF_NONE, EF_NONE, EF_NONE, EF_NONE },
						{ EF_NONE, EF_NONE, EF_NONE, EF_NONE, EF_NONE, EF_NONE },
						{ EF_NONE, EF_NONE, EF_NONE, EF_NONE, EF_NONE, EF_NONE },
						{ EF_NONE, EF_NONE, EF_NONE, EF_NONE, EF_NONE, EF_NONE }
					};

					int WinnerId = -1;

					if (AliveCount[0] <= 0)
						WinnerId = 1;
					else
						WinnerId = 0;

					srand(time(NULL) / 5 * (rand() % 500) * 5);

					for (int i = 0; i < 10; i++)
					{
						int wId = Servidor.Batalha.ShadowFightTeams[WinnerId][i];

						if (wId < 0 || wId > MAX_PLAYER)
							continue;
						else if (Users[wId].Status != 22)
							continue;

						int aleatorio = rand() % 6;

						st_Item item;
						item.Index = ids[aleatorio];
						memcpy(&item.Effect, adds[aleatorio], 6);

						if (GiveItem(wId, &item) == -1)
							sD1D(0x7530, wId, "Item perdido por falta de espaço.", MSG_COLOR_NOTICE);
						
						SendClientMessage(wId, "Parabéns pela vitória.");
					}

					for (int i = 0; i < 2; i++)
					{
						for (int e = 0; e < 10; e++)
						{
							strncpy(nBuffer[Servidor.Batalha.ShadowFightTeams[i][e]].Ingame.Tab, "", 26);
							Servidor.Batalha.FighterStatus[i][e] = FIGHTER_DEAD;
							Servidor.Batalha.ShadowFightTeams[i][e] = 0;
						}
					}

					Servidor.Batalha.Status = BATALHA_FINALIZADA;

					ClearArea(280, 270, 362, 350);
				}
			}
		}

		int ActualWar = -1;
		if (now->tm_hour == 18)
			ActualWar = KARDE_CITYID;
		else if (now->tm_hour == 19)
			ActualWar = ERION_CITYID;
		else if (now->tm_hour == 20)
			ActualWar = AZRAN_CITYID;
		else if (now->tm_hour == 21)
			ActualWar = ARMIA_CITYID;

		if (ActualWar != -1)
		{
#pragma region WAR
			if (War[ActualWar].Status == GUERRA_INICIADA &&
				player->TargetX >= 1050 && player->TargetY >= 1945 &&
				player->TargetX <= 1250 && player->TargetY <= 2150)
			{
				if (((mob->CapeInfo == 7 && player->CapeInfo == 8) || (mob->CapeInfo == 8 && player->CapeInfo == 7)) && player->GuildIndex)
					Guilds[player->GuildIndex].Points += 5;

				if (now->tm_wday == 6 && now->tm_hour >= 18 && now->tm_hour <= 21)
				{
					int ActualWar = 0;
					if (now->tm_hour == 18)
						ActualWar = KARDE_CITYID;
					else if (now->tm_hour == 19)
						ActualWar = ERION_CITYID;
					else if (now->tm_hour == 20)
						ActualWar = AZRAN_CITYID;
					else
						ActualWar = ARMIA_CITYID;

					if (player->GuildIndex != mob->GuildIndex &&
					  ((player->GuildIndex == War[ActualWar].Desafiado  && mob->GuildIndex == War[ActualWar].Desafiante) ||
					   (player->GuildIndex == War[ActualWar].Desafiante && mob->GuildIndex == War[ActualWar].Desafiado)))
					{
						Guilds[player->GuildIndex].Points++;

						if (player->GuildIndex == War[ActualWar].Desafiado)
							War[ActualWar].TamanhoExercitoDesafiante--;
						else
							War[ActualWar].TamanhoExercitoDesafiado--;

						char szMsg[120];
						sprintf(szMsg, "%s matou %s!!", player->Name, mob->Name);

						SendGuildMessage(player->GuildIndex, szMsg, 0xFFCDAD00);
						SendGuildMessage(mob->GuildIndex,    szMsg, 0xFFCDAD00);

						AtualizeWarCount(ActualWar);

						if (War[ActualWar].TamanhoExercitoDesafiado <= 0 || War[ActualWar].TamanhoExercitoDesafiante <= 0)
						{
							SendNoticeArea(1050, 1945, 1250, 2150, "Guerra finalizada, aguarde até 5 minutos para término.");
							StartThread(FinishWar);
						}
					}
				}
			}
#pragma endregion
		}
#pragma region HUNTED
		if (attacker < MAX_PLAYER && map.PvP)
		{
			// Mapa com PvP
			char szMsg[120];

			nInfo* nT = &nBuffer[target];
			nInfo* nA = &nBuffer[attacker];
				
			// Zera a pontuação do morto
			if (nT->Chars[nT->Ingame.LoggedMob].KillCount > 0)
				nT->Chars[nT->Ingame.LoggedMob].KillCount = 0;

			nA->Chars[nA->Ingame.LoggedMob].KillCount++;

			// Caso o morto seja o atual top killer, ele se fode
			if (!strcmp(Servidor.TopKiller, mob->Name) || !strcmp(Servidor.SecondKiller, mob->Name) || !strcmp(Servidor.ThirdKiller, mob->Name))
			{
				sprintf(szMsg, "%s matou o Killer %s!", player->Name, mob->Name);

				strncpy(nBuffer[target].Ingame.Tab, "", 26);

				BYTE packet[108];

				BASE_GetCreateMob(target, (BYTE*)&packet);
				GridMulticast(target, mob->TargetX, mob->TargetY, (void*)&packet);

				SendCreateMob(target, target, 1);

				SendNotice(szMsg);
			}
			// Caso o assassino seja o atual top killer, avisa que seu kill streak aumentou
			else if (!strcmp(Servidor.TopKiller, player->Name))
			{
				sprintf(szMsg, "%s fez de %s sua %d vítima!", player->Name, mob->Name, nA->Chars[nA->Ingame.LoggedMob].KillCount);

				SendNotice(szMsg);
			}

			// Não é nenhum dos 3 principais, verificar se passou algum deles 
			if (strncmp(player->Name, Servidor.TopKiller, 16) && strncmp(Servidor.SecondKiller, player->Name, 16) && strncmp(Servidor.ThirdKiller, player->Name, 16))
			{
				if (nA->Chars[nA->Ingame.LoggedMob].KillCount > Servidor.ThirdKillerCount)
				{
					strncpy(Servidor.ThirdKiller, player->Name, 16);
					Servidor.ThirdKillerCount = nA->Chars[nA->Ingame.LoggedMob].KillCount;

					sprintf(szMsg, "%s se tornou o mais novo procurado.", player->Name);

					SendNotice(szMsg);
				}
			}

			else if (!strncmp(player->Name, Servidor.ThirdKiller, 16))
			{
				Servidor.ThirdKillerCount = nA->Chars[nA->Ingame.LoggedMob].KillCount;
				
				// Tomou o lugar do segundo colocado
				if (Servidor.ThirdKillerCount > Servidor.SecondKillerCount)
				{
					char tmpName[20];

					// Salva o nick do ex segundo top killer numa variável temporária
					strncpy(tmp, Servidor.SecondKiller, 16);
					// Coloca o ex terceiro top killer como o segundo
					strncpy(Servidor.SecondKiller, player->Name, 16);
					// Coloca o ex segundo colocado como terceiro
					strncpy(Servidor.ThirdKiller, tmp, 16);

					// Mesma idéia porém com o kill streak
					int tmpKillCount = Servidor.SecondKillerCount;
					Servidor.SecondKillerCount = Servidor.ThirdKillerCount;
					Servidor.ThirdKillerCount = tmpKillCount;

					// Seta os novos tabs
					strncpy(nA->Ingame.Tab, "Matador Profissional", 26);

					BYTE packet[108];

					BASE_GetCreateMob(attacker, (BYTE*)&packet);
					GridMulticast(attacker, player->TargetX, player->TargetY, (void*)&packet);

					SendCreateMob(attacker, attacker, 1);
					
					int exCid = GetUserByName(Servidor.ThirdKiller);
					if (exCid > 0 && exCid < MAX_PLAYER && Users[exCid].Status == 22)
					{
						strncpy(nBuffer[exCid].Ingame.Tab, "Matador Aprendiz", 26);

						BYTE packet[108];

						BASE_GetCreateMob(exCid, (BYTE*)&packet);
						GridMulticast(exCid, player->TargetX, player->TargetY, (void*)&packet);

						SendCreateMob(exCid, exCid, 1);
					}
				}
			}

			else if (!strncmp(player->Name, Servidor.SecondKiller, 16))
			{
				Servidor.SecondKillerCount = nA->Chars[nA->Ingame.LoggedMob].KillCount;

				// Tomou o lugar do primeiro colocado
				if (Servidor.SecondKillerCount > Servidor.TopKillerCount)
				{
					char tmpName[20];

					// Salva o nick do ex top killer numa variável temporária
					strncpy(tmp, Servidor.TopKiller, 16);
					// Coloca o ex segundo top killer como o top
					strncpy(Servidor.TopKiller, player->Name, 16);
					// Coloca o ex top como segundo
					strncpy(Servidor.SecondKiller, tmp, 16);

					// Mesma idéia porém com o kill streak
					int tmpKillCount = Servidor.TopKillerCount;
					Servidor.TopKillerCount = Servidor.SecondKillerCount;
					Servidor.SecondKillerCount = tmpKillCount;

					// Seta os novos tabs
					strncpy(nA->Ingame.Tab, "Serial Killer", 26);

					BYTE packet[108];

					BASE_GetCreateMob(attacker, (BYTE*)&packet);
					GridMulticast(attacker, player->TargetX, player->TargetY, (void*)&packet);

					SendCreateMob(attacker, attacker, 1);

					int exCid = GetUserByName(Servidor.SecondKiller);
					if (exCid > 0 && exCid < MAX_PLAYER && Users[exCid].Status == 22)
					{
						strncpy(nBuffer[exCid].Ingame.Tab, "Matador Profissional", 26);

						BYTE packet[108];

						BASE_GetCreateMob(exCid, (BYTE*)&packet);
						GridMulticast(exCid, player->TargetX, player->TargetY, (void*)&packet);

						SendCreateMob(exCid, exCid, 1);
					}
				}
			}
		}
#pragma endregion
	}

	else 
	{
#pragma region AGUA
		INT8 Type = -1;
		INT8 Sala = -1;

		for (int i = 0; i < 3; i++)
		{
			for (int e = 0; e < 9; e++)
			{
				if (mob->TargetX >= WaterMMCoords[i][e][0] && mob->TargetX <= WaterMMCoords[i][e][2] &&
					mob->TargetY >= WaterMMCoords[i][e][1] && mob->TargetY <= WaterMMCoords[i][e][3])
				{
					Type = i;
					Sala = e;

					Agua[Type].Rooms[Sala].MobsLeft--;

					if (!Agua[Type].Rooms[Sala].MobsLeft)
					{
						// Quest acabou
						Agua[Type].Rooms[Sala].TimeLeft = 5;

						st_Item Item;
						memset(&Item, 0x0, sizeof(st_Item));

						Item.Index = WaterIds[Type][Sala + 1];

						GiveItem(Agua[Type].Rooms[Sala].Leaders, &Item);

						for (int z = 0; z < 13; z++)
							SendClientSignalParm(Agua[Type].Rooms[Sala].Cids[z], 0x7530, 0x3A1, 5);
					}
					else
					{
						INT8 cid = -1;
						while (cid < 12)
						{
							cid++;
							NpcsAlive(Agua[Type].Rooms[Sala].Cids[cid], Agua[Type].Rooms[Sala].MobsLeft, WaterNum[Type][Sala]);
						}
					}

					break;
				}
			}
		}
#pragma endregion

#pragma region PESA
		for (int i = 0; i < 3; i++)
		{
			Pesas* pesa = &Pesa[i];

			if (!pesa->Status)
				continue;

			if (mob->TargetX >= MaxMinPesaCoords[i][0] && mob->TargetX <= MaxMinPesaCoords[i][2] &&
				mob->TargetY >= MaxMinPesaCoords[i][1] && mob->TargetY <= MaxMinPesaCoords[i][3])
			{
				pesa->MobsLeft--;

				if (pesa->MobsLeft <= 0)
				{
					pesa->Wave++;

					GenerateNextWave(pesa, i);
				}

				break;
			}
		}
#pragma endregion

#pragma region BOSS_QUESTS
		for (int i = 0; i < 5; i++)
		{
			if (mob->TargetX >= MaxMinCoordsQuest[i][0] && mob->TargetY >= MaxMinCoordsQuest[i][1] &&
				mob->TargetX <= MaxMinCoordsQuest[i][2] && mob->TargetY <= MaxMinCoordsQuest[i][3])
			{
				if (!strncmp(mob->Name, QuestList[i].Boss.Name, 16) && QuestList[i].Boss.isAlive)
				{
					// Matou o boss
					QuestList[i].Boss.isAlive = false;
					char szMsg[120];
					sprintf(szMsg, "%s derrotou o %s e trouxe um pouco de paz ao continente!", player->Name, mob->Name);
					SendNotice(szMsg);

					srand(time(NULL) / 5 * (rand() % 500) * 5);

					int aleatorio = rand() % 100;

					for (int e = 0; e < 10; e++)
					{
						if (aleatorio <= QuestList[i].Boss.Chances[e])
						{
							st_Item Item;
							memcpy(&Item, &QuestList[i].Boss.Gifts[e], sizeof st_Item);

							INT16 cIds[12];

							if (player->Leader > 0 && player->Leader < MAX_PLAYER && Users[player->Leader].Status == 22)
							{
								memcpy(&cIds, mobs[player->Leader].PartyList, (sizeof(INT16) * 12));
								GiveItem(player->Leader, &Item);
							}
							else
							{
								memcpy(&cIds, mobs[attacker].PartyList, (sizeof(INT16) * 12));
								GiveItem(attacker, &Item);
							}

							for (int z = 0; z < 12; z++)
								if (Users[cIds[z]].Status == 22 &&
									mobs[cIds[z]].TargetX >= MaxMinCoordsQuest[i][0] && mobs[cIds[z]].TargetY >= MaxMinCoordsQuest[i][1] &&
									mobs[cIds[z]].TargetX <= MaxMinCoordsQuest[i][2] && mobs[cIds[z]].TargetY <= MaxMinCoordsQuest[i][3])
									GiveItem(cIds[z], &Item);

							break;
						}
					}
				}
				else if (!QuestList[i].Boss.isAlive)
				{
					QuestList[i].KillCount++;

					if (QuestList[i].KillCount >= QuestList[i].Boss.CountToBorn)
					{
						// Cria o Boss e avisa a todos presentes na quest de seu nascimento
						QuestList[i].KillCount = 0;
						CreateMob(QuestList[i].Boss.Name, QuestList[i].Boss.CreationPos.X, QuestList[i].Boss.CreationPos.Y, "quests_boss");

						char szMsg[120];
						sprintf(szMsg, "Cuidado, %s acaba de nascer!", QuestList[i].Boss.Name);
						SendNotice(szMsg);

						QuestList[i].Boss.isAlive = true;
					}
					else
					{
						// Envia o counter atualizado para os participantes da quest
						for (int e = 0; e < MAX_PLAYER; e++)
							if (mobs[e].TargetX >= MaxMinCoordsQuest[i][0] && mobs[e].TargetY >= MaxMinCoordsQuest[i][1] &&
								mobs[e].TargetX <= MaxMinCoordsQuest[i][2] && mobs[e].TargetY <= MaxMinCoordsQuest[i][3])
								NpcsAlive(e, QuestList[i].KillCount, QuestList[i].Boss.CountToBorn);
					}
				}
			}
		}
#pragma endregion

		if (attacker < MAX_PLAYER)
		{
			Servidor.DeadMobCount++;

			if (Servidor.DeadMobCount >= 15000)
			{
				Servidor.DeadMobCount -= 15000;

				SendNotice("15.000 Monstros foram mortos, bônus de 1.500 exp a todos online.");

				for (int i = 0; i < MAX_PLAYER; i++)
					if (Users[i].Status != 0)
						mobs[i].Exp += 1500;
			}

#pragma region GAIN_POINTS_TO_GUILD_ON_PILAR
			if (mob->TargetX > 1169 && mob->TargetX < 1205 &&
				mob->TargetY > 1700 && mob->TargetY < 1737) // Pilar deserto
			{
				if (player->GuildIndex)
				{
					Guilds[player->GuildIndex].MobDeadCount++;

					if (Guilds[player->GuildIndex].MobDeadCount >= 100)
					{
						Guilds[player->GuildIndex].MobDeadCount -= 100;
						Guilds[player->GuildIndex].Points += 10;

						SendGuildMessage(player->GuildIndex, "Foram ganhos 10 pontos de guild pelo up no pilar.", 0xFF00BFFF);
					}
				}
			}
#pragma endregion

#pragma region EVENTO_DROP
			if (Eventos.Drop.isOn)
			{
				srand(time(NULL) / 5 * (rand() % 500) * 5);

				if ((rand() % 100) <= Eventos.Drop.Rate && Eventos.Drop.Quantity > 0 && Eventos.Drop.Id)
				{
					st_Item Item;
					memset(&Item, 0x0, sizeof(st_Item));

					Item.Index = Eventos.Drop.Id;
					HKD_MobKilled_ControlDropItem(attacker, &Item);

					Eventos.Drop.Quantity--;
					Eventos.Drop.Dropped++;

					char szTmp[300];

					sprintf_s(szTmp, "Jogador %s dropou %s, foram dropados [%d] restam [%d].", player->Name, itemlistBuffer[Item.Index].Name, Eventos.Drop.Dropped, Eventos.Drop.Quantity);

					SendNotice(szTmp);
					sendLog(INFO, 1000, szTmp);
				}
			}
#pragma endregion
		}

#pragma region BOSSES
		else if (!strcmp(mob->Name, "Kefra"))
		{
			char szMsg[120];

			if (player->GuildIndex)
			{
				sprintf(szMsg, "Jogador [%s] da guild [%s] acaba de matar o Kefra!", player->Name, Guilds[player->GuildIndex].GuildName);

				Guilds[player->GuildIndex].Points += 5000;

				char newMsg[120];
				sprintf(newMsg, "[%s] matou o Kefra e conquistou 1000 pontos para a guild!", player->Name);

				SendGuildMessage(player->GuildIndex, newMsg, 0xFF00BFFF);
			}
			else
				sprintf(szMsg, "Jogador [%s] matou o Kefra.", player->Name);

			SendNotice(szMsg);
			sendLog(INFO, 1000, szMsg);
		}

		else if (!strcmp(mob->Name, "Quione"))
		{
			char szMsg[120];

			if (player->GuildIndex)
			{
				sprintf(szMsg, "Jogador [%s] da guild [%s] acaba de matar a Quione!", player->Name, Guilds[player->GuildIndex].GuildName);

				Guilds[player->GuildIndex].Points += 5000;

				char newMsg[120];
				sprintf(newMsg, "[%s] matou a Quione e conquistou 1000 pontos para a guild!", player->Name);

				SendGuildMessage(player->GuildIndex, newMsg, 0xFF00BFFF);
			}
			else
				sprintf(szMsg, "Jogador [%s] matou a Quione.", player->Name);

			Servidor.QuioneStatus = QUIONE_DEAD;

			srand(time(NULL) / 5 * (rand() % 500) * 5);

			st_Item premio;
			memset(&premio, 0x0, sizeof st_Item);
			premio.Index = 667 + rand() % 5;

			HKD_MobKilled_ControlDropItem(attacker, &premio);

			SendNotice(szMsg);
			sendLog(INFO, 1000, szMsg);
		}

		else if (!strcmp(mob->Name, "Minerador_Esq"))
		{
			if (nBuffer[attacker].Chars[nBuffer[attacker].Ingame.LoggedMob].Quests.Armeiro1 && !nBuffer[attacker].Chars[nBuffer[attacker].Ingame.LoggedMob].Quests.Armeiro2)
			{
				srand(time(NULL) / 5 * (rand() % 500) * 5);

				int chance = rand() % 100;
				if (chance < 8)
				{
					st_Item premio;
					memset(&premio, 0, sizeof st_Item);
					premio.Index = 5150;

					GiveItem(attacker, &premio);
				}
			}
		}
#pragma endregion

#pragma region GAIN_EXP_BONUS_FROM_SET
		INT16 exp = GetBonusFromColor(player, 123);

		if (exp > 0)
		{
			nInfo* nP = &nBuffer[attacker];

			nP->Chars[nP->Ingame.LoggedMob].DeadCount++;

			if (nP->Chars[nP->Ingame.LoggedMob].DeadCount >= 20)
			{
				player->Exp += exp;
				SendScore(attacker);
				nP->Chars[nP->Ingame.LoggedMob].DeadCount -= 20;

				p338 packet;
				memset(&packet, 0, sizeof p338);

				packet.Header.PacketId = 0x338;
				packet.Header.Size = sizeof p338;
				packet.Header.ClientId = 30000;

				packet.HOLD = 0;
				packet.KilledMob = target;
				packet.Killer = attacker;

				packet.Exp = exp;

				Sendpack((BYTE*)&packet, attacker, sizeof p338);
			}
		}
#pragma endregion
		
	}

	return morre;
}

// 0x004011A4
void HKD_NewMobKilled(int cId1, int cId2, int arg3, int arg4)
{
	if (cId1 < 0 || cId2 < 0 || cId1 > 0x7530 || cId2 > 0x7530)
		//sendLog(ERROR, 0, "MobKilled fora do range.");
		return;

	st_Mob *player = GetMobFromIndex(cId2),
		   *mob    = GetMobFromIndex(cId1);

	if (!mob->Mode)
		//sendLog(ERROR, cId1, ".Mode invalid on MobKilled");
		return;

	st_Item* FairyMob = &mob->Equip[13];

	if (FairyMob->Index == 769)
	{
		int FairySanc = BASE_GetItemSanc(FairyMob);

		if (FairySanc <= 0)
			BASE_ClearItem(FairyMob);
		else
		{
			// Retira 1 refinação da Nyerds
			FairySanc--;

			for (int i = 0; i < 3; i++)
				if (FairyMob->Effect[i].Index == EF_SANC)
					FairyMob->Effect[i].Value = FairySanc;
		}

		if (cId1 < MAX_PLAYER)
		{
			SendItem(cId1, EQUIP, 13, FairyMob);
			SendEmotion(cId1, 0xE, 2);
		}

		SendEquip(cId1);

		mob->Status.curHP = mob->Status.maxHP;

		if (cId1 < MAX_PLAYER)
			SetReqHp(cId1);

		SendScore(cId1);

		return;
	}

	if (*(DWORD*)0x008BF1860 > 1 /*CastleState*/ && mob->TargetX == 1046 && mob->TargetY == 1690 && cId1 < MAX_PLAYER) 
	{
		DoTeleport(cId1, 1057, 1742);

		return;
	}

	int PlayerFace = player->Equip[0].Index;

	if (cId2 >= MAX_PLAYER && player->CapeInfo == 4 && PlayerFace >= 315 && PlayerFace <= 345 && mob->CapeInfo != 4)
	{
		int SummonerId = mob->Summoner;

		if (SummonerId > 0 && SummonerId < MAX_PLAYER)
		{
			st_Mob* Summoner = GetMobFromIndex(SummonerId);

			if (Summoner->Mode && Users[SummonerId].Status)
			{
				st_Item *SummonerMount = &Summoner->Equip[14];

				if (SummonerMount->Index >= 2330 && SummonerMount->Index < 2360)
				{
					char LOCAL_6 = SummonerMount->Effect[1].Index; // EBP - 18h
					char LOCAL_7 = SummonerMount->Effect[2].Value; // EBP - 1Ch
					char LOCAL_8 = LOCAL_6 + 100; // EBP - 20h

					if (SummonerMount->Index == 2330)
						LOCAL_8 = LOCAL_6 + 25;
					else if (SummonerMount->Index == 2331)
						LOCAL_8 = LOCAL_6 + 50;

					if (LOCAL_6 < mob->Status.Level && LOCAL_6 < 100)
					{
						LOCAL_7 = LOCAL_7 + 1;

						if (LOCAL_7 >= LOCAL_8)
						{
							LOCAL_7 = 1;
							LOCAL_6 = LOCAL_6 + 1;

							SummonerMount->Effect[2].Value = LOCAL_7;
							SummonerMount->Effect[1].Index = LOCAL_6;

							SendClientMsg(SummonerId, "Sua montaria subiu de level");

							MountProcess(SummonerId, 0);
						}
						else
							SummonerMount->Effect[2].Value = LOCAL_7;
							
						SendItem(SummonerId, 0, 14, SummonerMount);
					}
				}
			}
		}
	}

	p338 packet;
	packet.Header.PacketId = 0x338;
	packet.Header.Size = sizeof p338; // 0x14
	packet.Header.ClientId = 30000;

	packet.KilledMob   = cId1;
	packet.Killer = cId2;

	mob->Status.curHP = 0;

	INT32 PlayerLeader = player->Leader;
	if (!PlayerLeader)
		PlayerLeader = cId2;

	if (cId2 > MAX_PLAYER && player->CapeInfo == 4)
	{
		int PlayerSummoner = player->Summoner;

		if (PlayerSummoner <= 0 || PlayerSummoner > MAX_PLAYER || Users[PlayerSummoner].Status != 22)
		{
			GridMulticast_2(mob->TargetX, mob->TargetY, &packet, 0);

			if (cId1 > 100)
				DeleteMob(cId1, 1);

			return;
		}
		else
			cId2 = PlayerSummoner;
	}

	if (cId1 < MAX_PLAYER && mob->bStatus.Level <= 359)
	{
		int MobLevel = mob->bStatus.Level;

		if (MobLevel < 0 || MobLevel >= 355)
			return;

		INT32 ActualLevelExp = 0,//g_pNextLevel[MobLevel];
			NextLevelExp = 0,//g_pNextLevel[MobLevel + 1];
			MidleExp = NextLevelExp - ActualLevelExp,
			LOCAL_20 = MidleExp / 20;

		if (MobLevel >= 30)
			LOCAL_20 = LOCAL_20 / 22;
		if (MobLevel >= 40)
			LOCAL_20 = LOCAL_20 / 25;
		if (MobLevel >= 50)
			LOCAL_20 = LOCAL_20 / 30;
		if (MobLevel >= 60)
			LOCAL_20 = LOCAL_20 / 35;
		if (MobLevel >= 70)
			LOCAL_20 = LOCAL_20 / 40;
		if (MobLevel >= 80)
			LOCAL_20 = LOCAL_20 / 45;
		if (MobLevel >= 90)
			LOCAL_20 = LOCAL_20 / 50;
		if (MobLevel >= 100)
			LOCAL_20 = LOCAL_20 / 55;
		if (MobLevel >= 150)
			LOCAL_20 = LOCAL_20 / 70;
		if (MobLevel >= 200)
			LOCAL_20 = LOCAL_20 / 85;
		if (MobLevel >= 250)
			LOCAL_20 = LOCAL_20 / 100;

		if (LOCAL_20 < 0)
			LOCAL_20 = 0;
		if (LOCAL_20 > 30000)
			LOCAL_20 = 30000;

		int pCurKill = GetCurKill(cId2),
			pTotKill = GetTotKill(cId2),
			pPkPoint = GetPKPoint(cId2);

		int dPKPoint = GetPKPoint(cId1),
			dGuilty  = GetGuilty(cId1),
			dCurKill = GetCurKill(cId1);

		int dCapeInfo = mob->CapeInfo,
			pCapeInfo = player->CapeInfo;

		INT32 LOCAL_29 = 0,
			  LOCAL_30 = 0;

		if (dPKPoint <= 10)
			LOCAL_20 = LOCAL_20 * 5;
		else if (dPKPoint <= 25)
			LOCAL_20 = LOCAL_20 * 3;

		int dTargetX = mob->TargetX >> 7,
			dTargetY = mob->TargetY >> 7;

		if (cId2 < MAX_PLAYER)
		{
			if (*(DWORD*)0x008BF1820 != 0)
				LOCAL_20 = LOCAL_20 / 3;
			else
				LOCAL_20 = LOCAL_20 / 6;

			if ((dCapeInfo == 7 && pCapeInfo == 8) || (dCapeInfo == 8 && pCapeInfo == 7))
				LOCAL_29 = 1;

			int dGuildIndex = mob->GuildIndex,
				pGuildIndex = player->GuildIndex;

			int LOCAL_35 = 65536;
			
			if (dGuildIndex > 0 && dGuildIndex < LOCAL_35 &&
				pGuildIndex > 0 && pGuildIndex < LOCAL_35 &&
				*(WORD*)(dGuildIndex * 2 + 0x007B118C8) == pGuildIndex &&
				*(WORD*)(pGuildIndex * 2 + 0x007B118C8) == dGuildIndex)
				LOCAL_30 = 1;

			if (*(DWORD*)0x008BF1860 && dTargetX == 8 && dTargetY == 13)
				LOCAL_30 = 1;

			GridMulticast_OL2(mob->TargetX, mob->TargetY, (BYTE*)&packet, 0);

			INT32 LOCAL_36 = BASE_GetArena(mob->TargetX, mob->TargetY),
				  LOCAL_37 = BASE_GetVillage(mob->TargetX, mob->TargetY),
				  LOCAL_38 = 0;

			if (dTargetX == 1 && dTargetY == 31)
				LOCAL_38 = 1;

			if (LOCAL_36 == 5 && LOCAL_37 == 5 && LOCAL_38 == 0)
			{
				if (*(DWORD*)0x008BF1820)
					LOCAL_20 = LOCAL_20 >> 2;

				if (LOCAL_29)
					LOCAL_20 = LOCAL_20 + ((LOCAL_20 * dCurKill) >> 2);

				if (LOCAL_20 > 150000)
					LOCAL_20 = 150000;
				if (LOCAL_20 < 0)
					LOCAL_20 = 0;

				if (MobLevel >= 20)
				{
					if (cId2 < MAX_PLAYER || *(DWORD*)0x004CBBAC != 0)
						mob->Exp -= LOCAL_20;
					
					if (mob->Exp < 0)
					{
						mob->Exp = 0;
						LOCAL_20 = 0;
					}

					if (*(DWORD*)0x004CBBAC != 0 && cId2 < MAX_PLAYER)
					{
						SendClientMsg(cId1, "Perdeu [%d] de experiência.", LOCAL_20);

						SendEtc(cId1);
					}
				}
				else
					SendClientMessage(cId1, "Abaixo do nível 20 não há perda de experiência.");
			    // 0x0045B561
			}
            
			// 0x0045BF43
		}
	}

	// 0x0045C18F
}