#include "DLLMain.h"

void UseNpcs(BYTE *m_PacketBuffer, bool *pRetn)
{
	p28B *p = (p28B*)m_PacketBuffer;

	INT16 cId = p->Header.ClientId,
		npcId = p->npcid;

	if (cId < 0 || cId > MAX_PLAYER)
		return;

	if (npcId > 30000 || npcId < 1000)
		return;

	st_Mob *player = GetMobFromIndex(cId),
		*npc = GetMobFromIndex(npcId);

	int npcLevel = npc->bStatus.Level;

	if (BASE_GetDistance(player->TargetX, player->TargetY, npc->TargetX, npc->TargetY) > 12)
	{
		*pRetn = true;

		SendClientMessage(cId, "Você está muito distante do npc. Aproxime-se mais.");

		return;
	}

	sendLog(INFO, cId, "%s %s usou o npc %s.", Users[cId].Username, player->Name, npc->Name);

	if (npc->Info.Merchant == 19 && player->Equip[10].Index == 1742)
	{
		*pRetn = Sephirot(p);

		return;
	}
	
	else if (npc->Info.Merchant == 31 && npc->bStatus.Level == 3)
	{
		MestreHab(cId, npcId);

		*pRetn = true;

		return;
	}

	else if (npc->Info.Merchant == 8 && (npcLevel > 0 && npcLevel < 4))
	{
		Perzens(m_PacketBuffer, pRetn);
		*pRetn = true;

		return;
	}

	else if (!strncmp(npc->Name, "Sobrevivente", 16))
	{
		if (nBuffer[cId].Chars[nBuffer[cId].Ingame.LoggedMob].Entradas > 1000)
			SendSay(npcId, "Limite de 1.000 entradas por pessoa.");
		else
		{
			int slot = GetFirstSlot(cId, 4127, INVENTORY);
			if (slot == -1)
				SendSay(npcId, "Onde está o Pergaminho Selado?");
			else
			{
				nBuffer[cId].Chars[nBuffer[cId].Ingame.LoggedMob].Entradas += 100;
				SendSay(npcId, "Recebeu 100 entradas.");
			}
		}

		*pRetn = true;

		return;
	}

	else if (npc->Info.Merchant == 42)
	{
		switch (npcLevel)
		{
			case 3:
			{
				*pRetn = true;

				Arch(p);

				break;
			}

			case 4: case 5: case 6: case 7: case 8:
			{
				*pRetn = true;

				EntradasQuest(cId, npcId, npcLevel - 4);

				break;
			}

			case 9:
			{
				*pRetn = true;

				Kibita(cId, npcId);

				break;
			}

			case 10:
				EntradaMolarGargula(p, pRetn);
				break;

			case 36:
			{
				NpcDeTroca(cId, npcId);

				*pRetn = true;
				
				break;
			}

			case 37:
			{
				Lenhador(cId, npcId);

				*pRetn = true;
			}
				break;

			case 38:
			{
				Shaman(cId, npcId);
				*pRetn = true;
			}
			default:
			{
				*pRetn = true;

				break;
			}

		}
	}

	else if (npc->Info.Merchant == 43)
	{
		switch (npcLevel)
		{
			case 4: case 5: case 6: case 7: case 8:
			{
				*pRetn = true;

				CarbunkleBuff(cId, npcId, npcLevel - 4);

				break;
			}
			case 9:
			{
				*pRetn = true;

				OraculoNegro(cId, npcId);

				break;
			}
			case 10:
			{
				*pRetn = true;

				NPC_Impostos(p);

				break;
			}
			case 11:
			{
				Ajudante((BYTE*)p, pRetn);
				
				break;
			}
			default:
			{
				*pRetn = true;

				break;
			}
		}
	}

	else if (npc->Info.Merchant == 30)
	{
		switch (npc->bStatus.Level)
		{
			case 1: case 2:
				Juli(m_PacketBuffer, pRetn);
				*pRetn = true;
				break;
		}
	}
}

void OpenStore(BYTE *m_PacketBuffer, bool *pRetn)
{
	p28B *p = (p28B*)m_PacketBuffer;

	*pRetn = true;

	int cId = p->Header.ClientId,
		npcId = p->npcid;

	if (cId < 0 || cId > MAX_PLAYER || npcId < 1000 || npcId > 30000)
		return;

	st_Mob* player = GetMobFromIndex(cId),
		*npc = GetMobFromIndex(npcId);

	if (!strncmp(npc->Name, "Ajudante", 16))
	{
		*pRetn = true;

	}
	else if (!strncmp(npc->Name, "Armeiro", 16))
	{
		*pRetn = true;

	}
}

void Ajudante(BYTE *m_PacketBuffer, bool* pRetn)
{
	*pRetn = true;

	p28B *p = (p28B*)m_PacketBuffer;

	INT16 cId = p->Header.ClientId;

	st_Mob *player = (st_Mob*)GetMobFromIndex(cId),
		*npc = (st_Mob*)GetMobFromIndex(p->npcid);

	char szTmp[120];

	int LoggedMob = nBuffer[cId].Ingame.LoggedMob;
	if (LoggedMob < 0 || LoggedMob > 3)
	{
		SendSay(p->npcid, "Desculpe.");

		return;
	}
	else if (nBuffer[cId].Chars[LoggedMob].Quests.Armeiro3)
	{
		sprintf(szTmp, "[%s] você já completou esta quest!", player->Name);
		SendSay(p->npcid, szTmp);

		return;
	}
	else if (!player->Equip[6].Index)
	{
		sprintf(szTmp, "[%s] equipe uma arma para receber o adicional.", player->Name);
		SendSay(p->npcid, szTmp);

		return;
	}

	int slot = GetFirstSlot(cId, 5151, INVENTORY);
	if (slot == -1)
	{
		sprintf(szTmp, "[%s] onde está o Pacote de Ferro?", player->Name);
		SendSay(p->npcid, szTmp);

		return;
	}

	AmountMinus(&player->Inventory[slot]);
	SendItem(cId, INVENTORY, slot, &player->Inventory[slot]);

	player->Equip[6].EF1 = 43;
	player->Equip[6].EFV1 = 4;

	SendClientMessage(cId, "! Parabéns! Você concluiu a missão Armado e Preparado, aproveite suas melhorias."); 
	SendClientMessage(cId, "! Fale com o NPC Ferreiro na Vila Congelada para receber uma nova missão.");
	// TODO: definir adds aleatórios e entregar 
}

void Juli(BYTE *m_PacketBuffer, bool *pRetn)
{
	*pRetn = true;

	p28B *p = (p28B*)m_PacketBuffer;

	INT16 cId = p->Header.ClientId;

	st_Mob *player = (st_Mob*)GetMobFromIndex(cId),
		   *npc = (st_Mob*)GetMobFromIndex(p->npcid);

	st_Position Dest[2];

	Dest[0].X = 3649;
	Dest[0].Y = 3133;
	Dest[1].X = 2480;
	Dest[1].Y = 1651;

	if (player->Gold < 1000)
		SendSay(p->npcid, "Para ser teleportado traga-me 1.000 de gold.");
	else
		DoTeleport(cId, Dest[npc->bStatus.Level - 1].X, Dest[npc->bStatus.Level - 1].Y);
	
	return;
}

void FameNpc(int cId, int npcId)
{
	st_Mob *mob = (st_Mob*)0x0015C69B8;

	if (nBuffer[cId].Chars[nBuffer[cId].Ingame.LoggedMob].KillCount < 1)
	{
		char szTmp[120];
		sprintf(szTmp, "[%s] você não possui pontos suficientes para a conversão.", mob[cId].Name);

		SendSay(npcId, szTmp);

		return;
	}

	nBuffer[cId].Chars[nBuffer[cId].Ingame.LoggedMob].Fame += nBuffer[cId].Chars[nBuffer[cId].Ingame.LoggedMob].KillCount;
	
	SendClientMsg(cId, "%d pontos foram adicionados a sua Fame, totalizando %d.", nBuffer[cId].Chars[nBuffer[cId].Ingame.LoggedMob].KillCount, nBuffer[cId].Chars[nBuffer[cId].Ingame.LoggedMob].Fame);

	nBuffer[cId].Chars[nBuffer[cId].Ingame.LoggedMob].KillCount = 0;

	return;
}

void Shaman(int conn, int npcId)
{
	st_Mob *mob = (st_Mob*)0x15C69B8;

	short requestItens[6] = {448, 449, 450, 693, 694, 695};
	int item = 0;
	int itemPos = -1;
	
	for (auto i = 0; i < 6; i++)
	{
		item = GetFirstSlot(conn, requestItens[i], INVENTORY);
		if (item != -1)
		{
			itemPos = i;
			break;
		}		
	}

	if (itemPos == -1)
	{
		SendClientMessage(conn, "Traga-me o item [Círculo Divino Puro] para continuar.");

		return;
	}
	
	if (itemPos > -1 && itemPos <= 2)
	{
		if (nBuffer[conn].Chars[nBuffer[conn].Ingame.LoggedMob].Quests.ResetDivinePure != 0)
		{
			SendClientMessage(conn, "Você já concluiu essa quest.");

			return;
		}

		nBuffer[conn].Chars[nBuffer[conn].Ingame.LoggedMob].Quests.ResetDivinePure = 1;

		int pos = requestItens[itemPos] - 447;
		if (mob[conn].bStatus.Mastery[pos] <= 100)
			mob[conn].bStatus.Mastery[pos] = 0;
		else
			mob[conn].bStatus.Mastery[pos] -= 50;


		AmountMinus(&mob[conn].Inventory[item]);
		SendItem(conn, INVENTORY, item, &mob[conn].Inventory[item]);

		BASE_GetBonusSkillPoint(mob);
		BASE_GetBonusScorePoint(mob);
		BASE_GetHpMp(mob);

		GetCurrentScore(conn);

		SendEtc(conn);
		SendScore(conn);
		SendStats(conn);

		SendSay(npcId, "Resetado com sucesso.");

		return;
	}

	else if (itemPos > 2)
	{
		int pos = requestItens[itemPos] - 692;

		if (mob[conn].bStatus.Mastery[pos] <= 100)
			mob[conn].bStatus.Mastery[pos] = 0;
		else
			mob[conn].bStatus.Mastery[pos] -= 100;

		AmountMinus(&mob[conn].Inventory[item]);
		SendItem(conn, INVENTORY, item, &mob[conn].Inventory[item]);

		BASE_GetBonusSkillPoint(mob);
		BASE_GetBonusScorePoint(mob);
		BASE_GetHpMp(mob);

		GetCurrentScore(conn);

		SendEtc(conn);
		SendScore(conn);
		SendStats(conn);

		SendSay(npcId, "Resetado com sucesso.");

		return;
	}

}

void MestreHab(INT16 clientId, INT16 npcId)
{
	char szMsg[256];

	st_Mob *player = (st_Mob*)GetMobFromIndex(clientId);

	int AmountCount = 0;

	for (int i = 0; i < 60; i++)
	{
		if (player->Inventory[i].Index == 697)
			AmountCount += GetItemAmount(&player->Inventory[i]);
	}

	if (AmountCount < 30)
	{
		SendSay(npcId, "Traga-me 30 safiras para a composição.");

		return;
	}

	for (INT8 i = 1; i < 8; i++)
	{
		if (player->Equip[i].Index != 0)
		{
			SendSay(npcId, "Desequipe seus equipamentos.");
			return;
		}
	}

	for (int i = 0; i < 30; i++)
	{
		int slot = GetFirstSlot(clientId, 697, INVENTORY);

		AmountMinus(&player->Inventory[slot]);
		SendItem(clientId, INVENTORY, slot, &player->Inventory[slot]);
	}

	if ((player->bStatus.Points[FORCA] - 100) < BaseSIDCHM[player->ClassInfo][0])
		player->bStatus.Points[FORCA] = BaseSIDCHM[player->ClassInfo][0];
	else
		player->bStatus.Points[FORCA] -= 100;

	if ((player->bStatus.Points[INTEL] - 100) < BaseSIDCHM[player->ClassInfo][1])
		player->bStatus.Points[INTEL] = BaseSIDCHM[player->ClassInfo][1];
	else
		player->bStatus.Points[INTEL] -= 100;

	if ((player->bStatus.Points[DEXTE] - 100) < BaseSIDCHM[player->ClassInfo][2])
		player->bStatus.Points[DEXTE] = BaseSIDCHM[player->ClassInfo][2];
	else
		player->bStatus.Points[DEXTE] -= 100;

	if ((player->bStatus.Points[CONST] - 100) < BaseSIDCHM[player->ClassInfo][3])
		player->bStatus.Points[CONST] = BaseSIDCHM[player->ClassInfo][3];
	else
		player->bStatus.Points[CONST] -= 100;

	BASE_GetBonusSkillPoint(player);
	BASE_GetBonusScorePoint(player);
	BASE_GetHpMp(player);

	GetCurrentScore(clientId);

	SendEtc(clientId);
	SendScore(clientId);
	SendStats(clientId);

	SendSay(npcId, "Resetado com sucesso.");

	BYTE packet[108];

	BASE_GetCreateMob(clientId, (BYTE*)&packet);
	GridMulticast(clientId, player->TargetX, player->TargetY, (void*)&packet);

	SendCreateMob(clientId, clientId, 1);
}

void Lenhador(int conn, int npcId)
{
	st_Mob *mob = (st_Mob*)0x15C69B8;

	if (mob[conn].Equip[6].Index != 0)
	{
		SendSay(npcId, "Desequipe sua arma para ir lenhar.");

		return;
	}

	if (mob[conn].Equip[14].Index != 0)
	{
		SendSay(npcId, "Você não pode ir de montaria...");

		return;
	}

	DoTeleport(conn, 1060, 1475);
	SendClientMessage(conn, "Boa sorte!");

	mob[conn].Equip[6].Index = 916;

	SendCarry(conn);
}

void NpcDeTroca(INT16 cId, INT16 npcId)
{
	st_Mob *player = GetMobFromIndex(cId),
		*npc = GetMobFromIndex(npcId);

	INT8 TradeCount = npc->Status.Points[DEXTE] - 1500;

	if (TradeCount < 0 || TradeCount > 5)
	{
		SendSay(npcId, "Desculpe.");

		return;
	}

	INT8 slots[5] = { -1, -1, -1, -1, -1 };

	for (int i = 0; i < 5; i++)
	{
		INT16 id = Eventos.Trade[TradeCount].RequestItem[i].Index;

		if (!id)
			continue;

		INT8 slot = GetFirstSlot(cId, id, INVENTORY);

		if (slot == -1)
		{
			SendClientMsg(cId, "Onde está o item %s?", itemlistBuffer[id].Name);

			return;
		}

		slots[i] = slot;
	}

	for (int i = 0; i < 5; i++)
	{
		if (slots[i] == -1)
			continue;

		memset(&player->Inventory[slots[i]], 0, sizeof st_Item);

		SendItem(cId, INVENTORY, slots[i], &player->Inventory[slots[i]]);
	}

	INT8 pos = -1;

	srand(time(NULL) / 5 * (rand() % 500) * 5);

	do
	{
		// Garante que uma posição com prêmio válido será gerada
		pos = rand() % 10;
	} while (!Eventos.Trade[TradeCount].PrizeItem[pos].Index);

	st_Item premio;

	memcpy(&premio, &Eventos.Trade[TradeCount].PrizeItem[pos], sizeof st_Item);

	GiveItem(cId, &premio);
}

void NPC_Impostos(p28B* p)
{
	INT16 cId = p->Header.ClientId;

	int CityId = GetCity(cId);
	if (CityId < 0 || CityId > 3)
	{
		SendSay(p->npcid, "Desculpe.");

		return;
	}

	time_t rawnow = time(NULL);
	struct tm *now = localtime(&rawnow);

	if (now->tm_wday || now->tm_hour < 19)
	{
		char szMsg[120];
		sprintf(szMsg, "Imposto recolhido: %s", NumberToString(Servidor.ImpostoAcumulado[CityId]));

		SendSay(p->npcid, szMsg);

		return;
	}

	st_Mob* player = GetMobFromIndex(cId);

	if (Guilds[player->GuildIndex].CidadePossuida != CityId)
	{
		SendSay(p->npcid, "Desculpe.");

		return;
	}

	st_Item BarraBi;
	memset(&BarraBi, 0, sizeof st_Item);

	// Entrega das barras de Bilhão
	if (Servidor.ImpostoAcumulado[CityId] > 1000000000)
	{
		int amount = Servidor.ImpostoAcumulado[CityId] / 1000000000;
		if (amount > 120)
			amount = 120;

		BarraBi.Index = 4011;
		BarraBi.EF1 = EF_AMOUNT;
		BarraBi.EFV1 = amount;
		
		if (GiveItem(cId, &BarraBi) == -1)
		{
			SendSay(p->npcid, "Espaço insuficiente.");

			return;
		}

		Servidor.ImpostoAcumulado[CityId] %= 1000000000;
	}

	// Entrega das barras de 100 milhões
	if (Servidor.ImpostoAcumulado[CityId] > 100000000)
	{
		int amount = Servidor.ImpostoAcumulado[CityId] / 100000000;
		if (amount > 120)
			amount = 120;

		BarraBi.Index = 4010;
		BarraBi.EF1 = EF_AMOUNT;
		BarraBi.EFV1 = amount;

		if (GiveItem(cId, &BarraBi) == -1)
		{
			SendSay(p->npcid, "Espaço insuficiente.");

			return;
		}

		Servidor.ImpostoAcumulado[CityId] %= 100000000;
	}

	// Entrega das barras de 50 milhões
	if (Servidor.ImpostoAcumulado[CityId] > 50000000)
	{
		int amount = Servidor.ImpostoAcumulado[CityId] / 50000000;
		if (amount > 120)
			amount = 120;

		BarraBi.Index = 4029;
		BarraBi.EF1 = EF_AMOUNT;
		BarraBi.EFV1 = amount;

		if (GiveItem(cId, &BarraBi) == -1)
		{
			SendSay(p->npcid, "Espaço insuficiente.");

			return;
		}

		Servidor.ImpostoAcumulado[CityId] %= 50000000;
	}

	// Entrega das barras de 10 milhões
	if (Servidor.ImpostoAcumulado[CityId] > 10000000)
	{
		int amount = Servidor.ImpostoAcumulado[CityId] / 10000000;
		if (amount > 120)
			amount = 120;

		BarraBi.Index = 4028;
		BarraBi.EF1 = EF_AMOUNT;
		BarraBi.EFV1 = amount;

		if (GiveItem(cId, &BarraBi) == -1)
		{
			SendSay(p->npcid, "Espaço insuficiente.");

			return;
		}

		Servidor.ImpostoAcumulado[CityId] %= 10000000;
	}

	// Entrega das barras de 5 milhões
	if (Servidor.ImpostoAcumulado[CityId] > 5000000)
	{
		int amount = Servidor.ImpostoAcumulado[CityId] / 5000000;
		if (amount > 120)
			amount = 120;

		BarraBi.Index = 4027;
		BarraBi.EF1 = EF_AMOUNT;
		BarraBi.EFV1 = amount;

		if (GiveItem(cId, &BarraBi) == -1)
		{
			SendSay(p->npcid, "Espaço insuficiente.");

			return;
		}

		Servidor.ImpostoAcumulado[CityId] %= 5000000;
	}

	if (Servidor.ImpostoAcumulado[CityId] > 1000000)
	{
		int amount = Servidor.ImpostoAcumulado[CityId] / 1000000;
		if (amount > 120)
			amount = 120;

		BarraBi.Index = 4026;
		BarraBi.EF1 = EF_AMOUNT;
		BarraBi.EFV1 = amount;

		if (GiveItem(cId, &BarraBi) == -1)
		{
			SendSay(p->npcid, "Espaço insuficiente.");

			return;
		}

		Servidor.ImpostoAcumulado[CityId] %= 1000000;
	}

	SendSay(p->npcid, "Imposto recolhido com sucesso.");
}

void Perzens(BYTE *m_PacketBuffer, bool *pRetn)
{
	p28B *p = (p28B*)m_PacketBuffer;

	INT16 cId = p->Header.ClientId;

	st_Mob *player = (st_Mob*)GetMobFromIndex(cId),
		*npc = (st_Mob*)GetMobFromIndex(p->npcid);

	static const INT16 Itens[][2] =
	{
		{ 4128, 3986 }, 
		{ 4129, 3987 },
		{ 4130, 3989 },

	};

	INT8 Index = npc->Status.Level - 1;

	INT32 freeSlot = GetFirstSlot(cId, Itens[Index][0], INVENTORY);

	char szMsg[108];
	if (freeSlot == -1)
	{
		sprintf(szMsg, "Traga-me a %s.", itemlistBuffer[Itens[Index][0]].Name);
		SendSay(p->npcid, szMsg);
		return;
	}

	memset(&player->Inventory[freeSlot], 0x0, sizeof(st_Item));
	player->Inventory[freeSlot].Index = Itens[Index][1];

	sprintf(szMsg, "Recebeu [%s].", itemlistBuffer[Itens[Index][1]].Name);

	SendSay(p->npcid, szMsg);
	SendItem(cId, INVENTORY, freeSlot, &player->Inventory[freeSlot]);

	return;
}

void Arch(p28B* p)
{
	INT16 cId = p->Header.ClientId,
		  npcid = p->npcid;

	st_Mob *player = (st_Mob*)GetMobFromIndex(cId),
		   *npc = (st_Mob*)GetMobFromIndex(npcid);

	if (player->Status.Level < 64)
	{
		SendSay(npcid, "Somente a partir do nível 65.");

		return;
	}

	else if (player->Equip[0].EFV2 != MORTAL)
	{
		SendSay(npcid, "Somente mortais.");

		return;
	}

	else if (player->Equip[10].Index != 1742)
	{
		SendSay(npcid, "Onde está a Imortalidade ?");

		return;
	}

	else if (player->Equip[11].Index < 1760 || player->Equip[11].Index > 1763)
	{
		SendSay(npcid, "Traga-me um sephirot para a composição.");

		return;
	}

	else if (player->Equip[1].Index)
	{
		SendSay(npcid, "Por favor, desequipe seu elmo.");

		return;
	}

	if ((player->Equip[0].Index >= 22 && player->Equip[0].Index <= 25) || player->Equip[0].Index == 32) 
	{	
		SendClientMessage(cId, "Impossível continuar enquanto transformado.");

		return;
	}

	player->Equip[1].Index = 3500;

	INT32 pClass = player->Equip[11].Index - 1760;

	memset(&player->Equip[10], 0, sizeof(st_Item));
	memset(&player->Equip[11], 0, sizeof(st_Item));

	nBuffer[cId].Chars[nBuffer[cId].Ingame.LoggedMob].SemiDeusLevel = player->bStatus.Level;

	INT16 face = (player->ClassInfo == 2 ? (21 + 5) + pClass : (player->Equip[0].Index + 5) + pClass);

	player->ClassInfo = pClass;

	player->Equip[0].Index = face;
	player->Equip[0].EFV2 = SEMIDEUS;
	player->Equip[0].EFV3 = face;

	player->StatusPoint += 600;

	for (int i = 0; i < 4; i++)
		player->bStatus.Points[i] = BaseSIDCHM[pClass][i];

	player->Learn = 0;
	player->Learn |= (1 << 30);

	SendClientMessage(cId, "Parabéns!!! Você se tornou um SemiDeus.");

	SendCharList(cId);

	SendClientSignalParm(cId, cId, 0x3B4, nBuffer[cId].Ingame.LoggedMob);

	return;
}

bool Sephirot(p28B* p)
{
	INT16 npcid = p->npcid,
		cId = p->Header.ClientId;
	st_Mob *player =  GetMobFromIndex(cId),
		   *npc    =  GetMobFromIndex(npcid);

	if (p->ClickOk != 1)
	{
		SendSay(npcid, "Deseja mesmo compor o sephirot ?");
		sendLog(INFO, cId, "%s %s deseja compor sephirot no npc %s?", Users[cId].Username, player->Name, npc->Name);

		return true;
	}

	else if (player->Gold < 30000000)
	{
		SendSay(npcid, "São necessários 30 milhões de gold para compor Sephirot.");
		sendLog(INFO, cId, "%s %s tentou compor sephirot sem possuir gold.", Users[cId].Username, player->Name);

		return true;
	}
	
	short Stones[8];
	bool hasStones = true;

	for (int i = 0; i < 8; i++)
	{
		Stones[i] = GetFirstSlot(cId, 1744 + i, INVENTORY);

		if (Stones[i] == -1)
		{
			SendSay(npcid, "Traga-me todas as oito pedras.");
			sendLog(INFO, cId, "%s %s tentou compor sephirot sem possuir as oito pedras.", Users[cId].Username, player->Name);

			return true;
		}
	}

	for (int i = 0; i < sizeof(Stones) / sizeof(short); i++)
	{
		memset(&player->Inventory[Stones[i]], 0, sizeof(st_Item));
		SendItem(cId, INVENTORY, Stones[i], &player->Inventory[Stones[i]]);
	}

	st_Item sephirot;
	memset(&sephirot, 0x0, sizeof(st_Item));

	sephirot.Index = 1760 + npc->ClassInfo;

	GiveItem(cId, &sephirot);

	player->Gold -= 30000000;

	SendEtc(cId);
	SendSay(npcid, "Recebeu Sephirot. Equipe-o e procure o rei.");
	sendLog(INFO, cId, "%s %s compôs sephirot no npc %s", Users[cId].Username, player->Name, npc->Name);

	return true;
}

void EntradaMolarGargula(p28B* p, bool *pRetn)
{
	*pRetn = true;

	INT16 cId = p->Header.ClientId;
	st_Mob *player = GetMobFromIndex(cId);
	if (player->bStatus.Level < 40 || player->bStatus.Level > 44)
	{
		SendClientMessage(cId, "Somente entre os níveis 40 e 45");

		return;
	}

	DoTeleport(cId, 793, 4062);
	SendClientMessage(cId, "Boa sorte.");
}

void EntradasQuest(INT16 clientId, INT16 npcId, INT8 questIndex)
{
	char szTMP[256];

	st_Mob *player = GetMobFromIndex(clientId);

	if (player->bStatus.Level < Quests_Level[questIndex][0] || player->bStatus.Level > Quests_Level[questIndex][1])
	{
		sprintf(szTMP, "Level permitido: [%d~%d].", Quests_Level[questIndex][0] + 1, Quests_Level[questIndex][1] + 1);

		SendSay(npcId, szTMP);

		return;
	}

	if (player->Equip[0].EFV2 != MORTAL)
	{
		SendSay(clientId, "Você não é mais um Mortal.");

		return;
	}

	INT16 itemId = 4038 + questIndex,
		slotId = GetFirstSlot(clientId, itemId, INVENTORY);

	if (slotId == -1)
	{
		sprintf(szTMP, "Aonde está %s?", itemlistBuffer[itemId].Name);

		SendSay(npcId, szTMP);

		return;
	}

	for (int i = 0; i < 40; i++)
	{
		if (!QuestList[questIndex].CidID[i])
		{
			QuestList[questIndex].CidID[i] = clientId;
			break;
		}
		else if (i == 39)
		{
			SendSay(npcId, "Quest cheia, tente novamente mais tarde.");

			return;
		}
	}

	srand(time(NULL) / 5 * (rand() % 500) * 5);
	INT16 Chance = rand() % 100;

	for (int i = 0; i < 10; i++)
	{
		if (Chance < QuestItensEntered[questIndex].Chances[i])
		{
			if (QuestItensEntered[questIndex].Itens[i].Index)
			{
				SendClientMsg(clientId, "!Você foi agraciado com um [%s].", itemlistBuffer[QuestItensEntered[questIndex].Itens[i].Index].Name);
				GiveItem(clientId, &QuestItensEntered[questIndex].Itens[i]);
				break;
			}
		}
	}

	if (questIndex == 0)
	{
		INT32 chance = rand() % 1000000000;

		if (chance == 666)
		{
			SendClientMsg(clientId, "Parabéns, você acaba de receber um porcão tunado.");

			st_Item Item;
			memset(&Item, 0, sizeof st_Item);

			Item.Index = 2360;
			Item.EF2 = 120;
			Item.EFV2 = 20;

			GiveItem(clientId, &Item);
		}
	}
	
	NpcsAlive(clientId, QuestList[questIndex].KillCount, QuestList[questIndex].Boss.CountToBorn);

	AmountMinus(&player->Inventory[slotId]);

	DoTeleport(clientId, CoordsQuest[questIndex][0], CoordsQuest[questIndex][1]);

	SendItem(clientId, INVENTORY, slotId, &player->Inventory[slotId]);

	sprintf(szTMP, "Quest iniciada para %s.", player->Name);

	SendSay(npcId, szTMP);
}

void CarbunkleBuff(INT16 clientId, INT16 npcId, INT8 carbIndex)
{
	st_Mob *player = GetMobFromIndex(clientId),
		*npc = GetMobFromIndex(npcId);

	if (player->Equip[0].EFV2 != MORTAL){

		SendSay(npcId, "Apenas mortais podem ter minha ajuda!");

		return;
	}

	if (player->bStatus.Level > 20){

		SendSay(npcId, "Vejo que você já está muito experiente...");

		return;
	}

	switch (carbIndex){
		case 1:
		{ //Velocidade
			Buff(clientId, 150, 150, 41);
			break;
		}

		case 2:
		{ //Defesa
			Buff(clientId, 150, 150, 43);
			break;
		}

		case 3:
		{ //Dano
			Buff(clientId, 150, 150, 44);
			break;
		}

		case 4:
		{ //Skill
			Buff(clientId, 150, 150, 45);
			break;
		}

		default:
		{ //Todas
			Buff(clientId, 150, 150, 45);
			Buff(clientId, 150, 150, 43);
			Buff(clientId, 150, 150, 44);
			Buff(clientId, 150, 150, 41);
			break;
		}
	}

	char msg[256];

	sprintf_s(msg, "Agora você se sente mais forte %s ?", player->Name);

	SendSay(npcId, msg);

	return;
}

void Kibita(int clientId, int npcId)
{
	st_Mob *player = GetMobFromIndex(clientId);
	nInfo *mob = &nBuffer[clientId];

	if (mob->Chars[mob->Ingame.LoggedMob].Cidadania)
	{
		SendClientMessage(clientId, "Somente caso não tenha cidadania.");

		return;
	}

	if (player->Gold < 5000000)
	{
		SendClientMessage(clientId, "São necessários 5.000.000 de gold para adquirir cidadania.");

		return;
	}

	mob->Chars[mob->Ingame.LoggedMob].Cidadania = Servidor.Channel;
	player->Gold -= 5000000;
	SendEtc(clientId);

	SendSay(npcId, "Bem vindo.");
}

void OraculoNegro(int Index, int npcid)
{
	st_Mob *spw = GetMobFromIndex(Index);
	if (spw->Gold < 100000000)
	{
		SendSay(npcid, "Traga 100 milhões de gold para continuar!");

		return;
	}
	else
	{
		bool ret = false;
		int slot = GetFirstSlot(Index, 1740, INVENTORY);
		int slot2 = GetFirstSlot(Index, 1741, INVENTORY);

		if (slot == -1)
		{
			SendSay(npcid, "Preciso que você traga as duas almas para compor!");
			return;
		}

		if (slot2 == -1)
		{
			SendSay(npcid, "Preciso que você traga as duas almas para compor!");
			return;
		}

		if (slot2 == (slot - 1))
		{
			SendSay(npcid, "Posicione as duas almas formando a imortalidade!");
			return;
		}

		memset(&spw->Inventory[slot], 0, sizeof(st_Item));
		memset(&spw->Inventory[slot2], 0, sizeof(st_Item));

		spw->Inventory[slot].Index = 1742;
		spw->Gold -= 100000000;

		SendItem(Index, INVENTORY, slot, &spw->Inventory[slot]);
		SendItem(Index, INVENTORY, slot2, &spw->Inventory[slot2]);

		SendEtc(Index);

		SendSay(npcid, "Sua imortalidade foi composta com sucesso!");

		return;
	}
}

static const INT32 CoordsGrifo[5][2] =
{
	{2112, 2051},
	{2371, 2099},
	{2220, 1714},
	{2365, 2279},
	{1826, 1771}
};

void MestreGrifo(int clientId, int type)
{
	if (clientId < 0 || clientId > MAX_PLAYER)
		return;
	else if (type < 0 || type > 4)
		return;

	DoTeleport(clientId, CoordsGrifo[type][0], CoordsGrifo[type][1]);
}
