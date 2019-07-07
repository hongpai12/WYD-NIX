#include "PacketControl.h"

//bool StartSqlConnection()
//{
//	if (SqlInfos.status)
//	{
//		printf("Conexão MySql não pode ser iniciada por já estar ativa.\n");
//
//		return false ;
//	}
//
//	if (!mysql_init(&SqlInfos.cConn))
//	{
//		printf("Houve um problema durante a criação do socket MySql\n");
//		
//		return false;
//	}
//    
//	char  *Host = "185.28.21.241",
//		  *User = "u596569333_nixra",
//		  *Senha = "k3JkjMmvXJxBfqUxIf",
//		  *Database = "u596569333_qoewu";
//	
//	if (mysql_real_connect(&SqlInfos.cConn, Host, User, Senha, Database, 3306, NULL, 0))
//	{
//		printf("Conexão feita com sucesso a database MySql\n");
//
//		return true;
//	}
//	else 
//		printf("Error: %s", mysql_error(&SqlInfos.cConn));
//
//	return false;
//}



void WINAPI BatalhaEscuridao()
{
	// Caso a ultima batalha ainda esteja rolando libera a área
	if (Servidor.Batalha.Status == BATALHA_INICIADA)
	{
		for (int i = 0; i < 2; i++)
		{
			for (int e = 0; e < 10; e++)
			{

				Servidor.Batalha.FighterStatus[i][e] = FIGHTER_DEAD;
				Servidor.Batalha.ShadowFightTeams[i][e] = 0;
			}
		}
		
		ClearArea(280, 270, 362, 350);
		sD1D(0x7530, 0, "[Aviso] Não houve vencedores na Batalha da Escuridão.", MSG_COLOR_NOTICE);
		return;
	}

	int maxClientOn = 0, onlineCount = 0;
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		if (Users[i].Status == 22)
		{
			maxClientOn = i;
			onlineCount++;
		}
	}

	if (maxClientOn <= 0)
		return;
	else if (onlineCount < 20)
	{
		sD1D(0x7530, 0, "[Aviso] Não houve Batalha da Escuridão devido a número de jogadores.", MSG_COLOR_NOTICE);
		return;
	}

	Servidor.Batalha.Status = BATALHA_INICIADA;

	srand(time(NULL) / 5 * (rand() % 500) * 5);

	int dests[2][2] = { {290, 310}, {355, 310} };

	for (int i = 0; i < 2; i++)
	{
		for (int e = 0; e < 10; e++)
		{
			int tcId = -1;
			
			do{
				tcId = rand() % maxClientOn;

				if (Users[tcId].Status != 22)
					tcId = -1;
			} while (tcId == -1);

			Servidor.Batalha.ShadowFightTeams[i][e] = tcId;
			Servidor.Batalha.FighterStatus[i][e]    = FIGHTER_ALIVE;

			if (!i)
				strncpy(nBuffer[tcId].Ingame.Tab, "GRUPO GELO", 26);
			else
				strncpy(nBuffer[tcId].Ingame.Tab, "GRUPO FOGO", 26);

			//RemoveParty(tcId);

			DoTeleport(tcId, dests[i][0], dests[i][1]);
		}
	}

	sD1D(0x7530, 0, "[Aviso] Batalha da Escuridão iniciada.", MSG_COLOR_NOTICE);
}

void FinalizaBatalhaEscuridao()
{
	if (Servidor.Batalha.Status == BATALHA_INICIADA)
	{
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
		sD1D(0x7530, 0, "[Aviso] Não houve vencedores na Batalha da Escuridão.", MSG_COLOR_NOTICE);
		return;
	}
}

void WINAPI GivePrize()
{
	int cId = -1;
	srand(time(NULL) / 5 * (rand() % 500) * 5);

	do{
		cId = rand() % MAX_PLAYER;
		
		if (Users[cId].Status != 22)
			cId == -1;
	} while (cId == -1);

	st_Item premio;
	memset(&premio, 0, sizeof st_Item);

	premio.Index = 413;
	premio.EF1   =  61;
	premio.EFV1  =  10;

	PutItem(cId, &premio);

	st_Mob *player = GetMobFromIndex(cId);

	char szTmp[120];
	sprintf(szTmp, "Jogador [%s] foi premiado.", player->Name);
	SendNotice(szTmp);
}

void Barganha(p2D4* p)
{
	INT16 cId    = p->Header.ClientId,
		  slotId = p->slotId;

	if (cId < 0 || cId > MAX_PLAYER)
		return;
	else if (slotId < 0 || slotId > 59)
		return;

	st_Mob *player = GetMobFromIndex(cId);
	st_Item *Item = &player->Inventory[slotId];

	if (Item->Index < 0 || Item->Index > 6500)
		return;

	INT16 Pos = itemlistBuffer[Item->Index].Pos;
	if (Pos < 2 || Pos > 32)
	{
		SendClientMessage(cId, "Uso inadequado da skill.");

		return;
	}

	int SabioSlot = GetFirstSlot(cId, 1774, INVENTORY);
	if (SabioSlot == -1)
	{
		SendClientMessage(cId, "Onde está a Pedra do Sábio?");

		return;
	}
	
	int ItemUnique = itemlistBuffer[Item->Index].Unique;
	bool check = false;
	
	if (ItemUnique == 5 || ItemUnique == 14 || ItemUnique == 24 || ItemUnique == 34 ||
		ItemUnique == 6 || ItemUnique == 15 || ItemUnique == 25 || ItemUnique == 35 ||
		ItemUnique == 7 || ItemUnique == 16 || ItemUnique == 26 || ItemUnique == 36 ||
		ItemUnique == 8 || ItemUnique == 17 || ItemUnique == 27 || ItemUnique == 37 ||
		ItemUnique == 1 || ItemUnique ==  2 || ItemUnique ==  3 || ItemUnique ==  4)
		check = true;

	if (!check)
	{
		SendClientMessage(cId, "Uso incorreto.");

		return;
	}

	else if (!Item->EF1 && !Item->EF2 && !Item->EF3)
	{
		SendClientMessage(cId, "Não há adicional para extrair.");

		return;
	}

	st_Item Prize;
	memset(&Prize, 0, sizeof st_Item);

	if (Pos == 2)
		Prize.Index = 3021;
	else if (Pos == 4)
		Prize.Index = 3022;
	else if (Pos == 8)
		Prize.Index = 3023;
	else if (Pos == 16)
		Prize.Index = 3024;
	else if (Pos == 32)
		Prize.Index = 3025;
	else return;

	memcpy(&Prize.EF1, &Item->EF1, 6);

	if (GiveItem(cId, &Prize) == -1)
	{
		SendClientMessage(cId, "Espaço insuficiente.");

		return;
	}

	memset(&Item->EF1, 0, 6);
	AmountMinus(&player->Inventory[SabioSlot]);

	SendItem(cId, INVENTORY, SabioSlot, &player->Inventory[SabioSlot]);
	SendItem(cId, INVENTORY,    slotId,                          Item);
}

int GetChanceRefOdin(st_Item *Item)
{
	if ((Item->EFV1 >= 234 && Item->EFV1 < 238) ||
		(Item->EFV2 >= 234 && Item->EFV2 < 238) ||
		(Item->EFV3 >= 234 && Item->EFV3 < 238))
		return 5;
	else if ((Item->EFV1 >= 238 && Item->EFV1 < 242) ||
		(Item->EFV2 >= 238 && Item->EFV2 < 242) ||
		(Item->EFV3 >= 238 && Item->EFV3 < 242))
		return 3;
	else if ((Item->EFV1 >= 242 && Item->EFV1 < 246) ||
		(Item->EFV2 >= 242 && Item->EFV2 < 246) ||
		(Item->EFV3 >= 242 && Item->EFV3 < 246))
		return 2;
	else if ((Item->EFV1 >= 246 && Item->EFV1 < 250) ||
		(Item->EFV2 >= 246 && Item->EFV2 < 250) ||
		(Item->EFV3 >= 246 && Item->EFV3 < 250))
		return 1;
	else
		return 0;
}

void SendALL(int clientid)
{
	sendinv1(clientid);
	sendinv2(clientid);
	sendstats(clientid);
	sendequip(clientid);
	return;
}


void TeleportPeople(INT16 *cIds, INT16 destX, INT16 destY)
{
	for (INT16 i = 0; i < sizeof(cIds) / sizeof(INT16); i++)
	{
		if (cIds[i] < 0 && cIds[i] > 750)
			continue;

		else if (Users[cIds[i]].Status != 22)
			continue;

		DoTeleport(cIds[i], destX, destY);
	}
}

st_Item* GetItemPointer(st_Mob* mob, st_Item* cargo, int type, int pos)
{
	st_Item* ret = NULL;

	if (type == EQUIP)
	{
		if (pos < 0 || pos >= 16)
			return NULL;

		ret = &mob->Equip[pos];
	}
	else if (type == INVENTORY)
	{
		if (pos < 0 || pos >= 64)
			return NULL;

		ret = &mob->Inventory[pos];
	}
	else if (type == CARGO)
	{
		if (pos < 0 || pos >= 128)
			return NULL;

		ret = &cargo[pos];
	}
	else
		return NULL;

	if (ret->Index <= 0 || ret->Index > 6500)
		return NULL;

	return ret;
}

FindAmountItem FindOcurrences(st_Item *Item, int index)
{
	FindAmountItem retn;
	memset(&retn, 0, sizeof(FindAmountItem));

	if (index < 0 || index > 6500)
		return retn;

	for (int i = 0; i < sizeof(Item) / sizeof(st_Item); i++)
	{
		if (Item[i].Index == index)
		{
			// Salva em qual slot está a info
			retn.Slots[retn.Amount] = i;
			// Retorna um ponteiro para o item, melhor para mexer
			retn.Itens[retn.Amount] = &Item[i];
			// Aumenta a quantidade de itens disponíveis
			retn.Amount++;
		}
	}

	return retn;
}

void CheckFimPesa(Pesas* pesa, int Count)
{
	if (!pesa->Status)
		return;
	else if (pesa->Wave <= 0)
		return;

	pesa->TimeLeft--;

	if (pesa->TimeLeft <= 0)
	{
		int MinX = MaxMinPesaCoords[Count][0],
			MinY = MaxMinPesaCoords[Count][1],
			MaxX = MaxMinPesaCoords[Count][2],
			MaxY = MaxMinPesaCoords[Count][3];

		//SendNoticeArea(MinX, MinY, MaxX, MaxY, "Sua quest acabou.");
		ClearArea(     MinX, MinY, MaxX, MaxY);
		KillMobsOnArea(MinX, MinY, MaxX, MaxY);

		pesa->Groups = 0;
		pesa->MobsLeft = 0;
		pesa->TimeLeft = 0;
		pesa->Wave = 0;
		pesa->Status = false;
	}
}

void GenerateNextWave(Pesas* pesa, int Count)
{
	if (pesa->Wave <= 13)
	{
		// Cria o número de mobs certo por spawn configurado na array
		// e = posição no mapa onde o mob será gerado
		// f = número de mobs que serão gerados
		for (int e = 0; e < 4; e++)
		{
			int destX = MobsPesaCoords[Count][e][0],
				destY = MobsPesaCoords[Count][e][1],
				Max   = MobsPesaCount[Count][pesa->Wave][e];

			for (int f = 0; f < Max; f++)
			{
				CreateMob(MobsPesaNames[Count][e], destX, destY, "npc");
				pesa->MobsLeft++;
			}
		}

		pesa->TimeLeft = 60;

		// Envia o tempo verde para os participantes da quest
		// Envia também o número da Wave
		for (int e = 0; e < 3; e++)
		{
			for (int z = 0; z < 13; z++)
			{
				int cId = pesa->cId[e][z];
				if (cId > 0 && cId < MAX_PLAYER && Users[cId].Status == 22)
				{
					SendClientSignalParm(cId, 0x7530, 0x3A1, pesa->TimeLeft);
					NpcsAlive(cId, pesa->Wave, 13);
				}
			}
		}
	}
	else
	{
		// Boss
	}
}

int GetLoggedCount(char *Mac)
{
	int retn = 0;

	if (!strcmp(Mac, ""))
		return 0;

	for (int i = 0; i < MAX_PLAYER; i++)
		if (Users[i].Status == 22)
			if (!strcmp(Mac, nBuffer[i].MacAdress))
				retn++;

	return retn;
}

int FindFirstItembyRefination(INT16 cId, INT16 Index, INT8 Sanc)
{
	if (cId < 0 || cId > MAX_PLAYER)
		return -1;
	else if (Index < 0 || Index > 6500)
		return -1;

	int slot = -1;

	st_Mob* player = GetMobFromIndex(cId);

	for (int i = 0; i < 60; i++)
	{
		if (player->Inventory[i].Index == Index)
			if (GetItemSanc(&player->Inventory[i]) == Sanc)
				return i;
		
	}

	return slot;
}

int GiveItem(INT16 cId, st_Item* Item)
{
	if (cId < 0 || cId > MAX_PLAYER)
		return -1;
	else if (Item == NULL)
		return -1;

	st_Mob* player = GetMobFromIndex(cId);
	nInfo* nP = &nBuffer[cId];

	// Primeiramente tenta colocar o item no inventário ativo
	for (int i = 0; i < 60; i++)
	{
		if (!player->Inventory[i].Index)
		{
			memcpy(&player->Inventory[i], Item, sizeof st_Item);
			SendItem(cId, INVENTORY, i, Item);

			// Retorna o slot onde o item foi entregue, para futuras verificações
			return i;
		}
	}

	if (nP->Chars[nP->Ingame.LoggedMob].Inventarios.hasBuyed[0])
	{
		for (int i = 0; i < 60; i++)
		{
			if (!nP->Chars[nP->Ingame.LoggedMob].Inventarios.sInv1[i].Index)
			{
				memcpy(&nP->Chars[nP->Ingame.LoggedMob].Inventarios.sInv1[i], Item, sizeof st_Item);

				return i + 60;
			}
		}
	}

	if (nP->Chars[nP->Ingame.LoggedMob].Inventarios.hasBuyed[1])
	{
		for (int i = 0; i < 60; i++)
		{
			if (!nP->Chars[nP->Ingame.LoggedMob].Inventarios.sInv2[i].Index)
			{
				memcpy(&nP->Chars[nP->Ingame.LoggedMob].Inventarios.sInv2[i], Item, sizeof st_Item);

				return i + 120;
			}
		}
	}

	return -1;
}

void SendGuildMessage(int guild, char* Msg, DWORD color)
{
	for (int i = 0; i < 750; i++)
	{
		if (Users[i].Status != 22)
			continue;

		st_Mob *player = GetMobFromIndex(i);

		if (player->GuildIndex == guild)
			sD1D(0x7530, i, Msg, color);
	}
}

INT16 GetBonusFromColor(st_Mob* mob, INT8 TYPE)
{
	if (TYPE < 116 || TYPE > 125)
		return 0;

	INT16 bonus = 0;

	for (int i = 1; i < 7; i++)
	{
		for (int e = 0; e < 3; e++)
		{
			if (mob->Equip[i].Effect[e].Index == TYPE)
			{
				if (TYPE == 123)
					bonus += 100;
				else 
					bonus += 2;

				break;
			}
		}
	}

	return bonus;
}

void VerifyCampoTreino(INT16 cId)
{
	if (nBuffer[cId].AcessLevel >= MOD_LEVEL && (nBuffer[cId].Ingame.isMod || nBuffer[cId].Ingame.isAdmin))
		return;

	st_Mob *player = GetMobFromIndex(cId);

	if (player->TargetX >= 2062 && player->TargetX <= 2160 && player->TargetY >= 1934 && player->TargetY <= 2048)
	{
		/*if (player->bStatus.Level > 9 && player->bStatus.Level <= 99 || player->Equip[0].EFV2 > MORTAL)
		{*/
			DoRecall(cId);
			SendClientMessage(cId, "Somente para mortais abaixo do nível 10.");
		//}
	}
}

void VerifyGuildArea(INT16 cId)
{
	if (nBuffer[cId].AcessLevel >= MOD_LEVEL && nBuffer[cId].Ingame.isAdmin)
		return;

	st_Mob *player = GetMobFromIndex(cId);

	for (int i = 0; i < 5; i++)
	{
		if (player->TargetX >= MaxMinCoordsGuilds[i][0] && player->TargetY >= MaxMinCoordsGuilds[i][1] &&
			player->TargetX <= MaxMinCoordsGuilds[i][2] && player->TargetY <= MaxMinCoordsGuilds[i][3])
		{
			if (player->GuildIndex != Servidor.PossuidorIndex[i] && Servidor.PossuidorIndex[i])
			{
				SendClientMessage(cId, "Somente membros de guild dominante.");
				DoRecall(cId);

				return;
			}
		}
	}
}

void VerifyQuests(INT16 cId)
{
	bool canStay = false;

	st_Mob *player = GetMobFromIndex(cId);

	INT8 questAtual = -1;

	for (INT8 a = 0; a < sizeof(QuestList) / sizeof(Quests); a++)
	{
		if (player->TargetX >= MaxMinCoordsQuest[a][0] && player->TargetX <= MaxMinCoordsQuest[a][2] &&
			player->TargetY >= MaxMinCoordsQuest[a][1] && player->TargetY <= MaxMinCoordsQuest[a][3])
		{
			questAtual = a;
			break;
		}
	}

	if (questAtual == -1)
		return;

	for (int e = 0; e < 40; e++)
	{
		// Tentar fazer com while depois
		if (QuestList[questAtual].CidID[e] == cId)
		{
			canStay = true;
			break;
		}
	}

	if (player->Equip[0].EFV2 != MORTAL)
		canStay = false;

	if (player->bStatus.Level < Quests_Level[questAtual][0] || player->bStatus.Level > Quests_Level[questAtual][1])
		canStay = false;

	if (nBuffer[cId].Ingame.isAdmin)
		canStay = true;

	if (!canStay)
	{
		DoTeleport(cId, 2100, 2100);
		SendClientMessage(cId, "Não possui autorização para estar nesta quest.");
	}
}

void VerifyPesa(int cId)
{
	st_Mob *player = GetMobFromIndex(cId);

	for (int i = 0; i < 3; i++)
	{
		if (player->TargetX >= MaxMinPesaCoords[i][0] && player->TargetX <= MaxMinPesaCoords[i][2] &&
			player->TargetY >= MaxMinPesaCoords[i][1] && player->TargetY <= MaxMinPesaCoords[i][3])
		{
			bool canStay = false;

			for (int e = 0; e < 3; e++)
			{
				for (int a = 0; a < 13; a++)
					if (cId == Pesa[i].cId[e][a])
					{
						canStay = true;
						break;
					}
			}

			if (nBuffer[cId].Ingame.isAdmin)
				canStay = true;

			if (nBuffer[cId].Ingame.isMod)
				canStay = true;

			if (!canStay)
			{
				DoTeleport(cId, 2100, 2100);

				SendClientMessage(cId, "Não possui autorização para estar nesta área.");
			}
		}
	}
}

INT8 CheckReplation(INT16 Index)
{
	for (int i = 0; i < sizeof(ListA) / sizeof(int); i++)
		if (Index == ListA[i])
			return REPLATION_A_TYPE;
	
	for (int i = 0; i < sizeof(ListB) / sizeof(int); i++)
		if (Index == ListB[i])
			return REPLATION_B_TYPE;

	for (int i = 0; i < sizeof(ListC) / sizeof(int); i++)
		if (Index == ListC[i])
			return REPLATION_C_TYPE;

	for (int i = 0; i < sizeof(ListD) / sizeof(int); i++)
		if (Index == ListD[i])
			return REPLATION_D_TYPE;

	for (int i = 0; i < sizeof(ListE) / sizeof(int); i++)
		if (Index == ListE[i])
			return REPLATION_E_TYPE;

	return REPLATION_ERROR;
}

void AmountMinus(st_Item *item)
{
	int index = 0;
	int amount = 0;
	for (int i = 0; i < 3; i++)
	{
		if (item->Effect[i].Index == EF_AMOUNT)
		{
			index = i;
			amount = item->Effect[i].Value;
			break;
		}
	}

	if (amount <= 1)
		memset(item, 0, sizeof st_Item);
	else
		item->Effect[index].Value--;
}

int GetFirstSlot(int clientId, int itemId, int itemType)
{
	if (clientId < 0 || clientId > 30000)
		return -1;
	if (itemId < 0 || itemId > 6500)
		return -1;

	st_Mob *player = _GetMobFromIndex(clientId);

	switch (itemType)
	{
		case EQUIP: //Equip
			for (int i = 0; i < 16; i++)
			{
				if (player->Equip[i].Index == itemId)
					return i;
			}
			break;

		case INVENTORY: //Inventory
			for (int i = 0; i < 60; i++)
			{
				if (player->Inventory[i].Index == itemId)
					return i;
			}
			break;
		case CARGO:
			for (int i = 0; i < 128; i++)
			{
				if (Users[clientId].Storage.Item[i].Index == itemId)
					return i;
			}
			break;
	}

	return -1;
}

void sD1D(int clientid, int destino, char *msg, DWORD cor)
{
	pD1D p;

	p.Header.Size = sizeof pD1D;
	p.Header.PacketId = 0xD1D;
	p.Header.ClientId = clientid;

	strncpy(p.message, msg, sizeof p.message);

	p.color = cor;

	if (!destino)
	{
		for (int i = 0; i < MAX_PLAYER; i++)
			Sendpack((BYTE*)&p, i, sizeof pD1D);
	}
	else
		Sendpack((BYTE*)&p, destino, sizeof pD1D);
}

int GetUserByName(char* Name)
{
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		st_Mob* player = GetMobFromIndex(i);

		if (!strcmp(Name, player->Name))
			return i;
	}

	return -1;
}

void SendClientMsg(int cId, char* Msg, ...)
{
	char buff[120];

	va_list va;
	va_start(va, Msg);
	vsprintf_s(buff, 120, Msg, va);
	va_end(va);

	SendClientMessage(cId, buff);
}

void sendLog(INT8 type, INT16 cId, char* Log, ...)
{
	if (cId < 0 || cId > 30000)
		return;
	else if (cId < MAX_PLAYER && !Users[cId].Status)
		return;

	char buff[1000];

	va_list va;
	va_start(va, Log);
	vsprintf_s(buff, 1000, Log, va);
	va_end(va);
	
	time_t rawnow = time(NULL);
	struct tm *now = localtime(&rawnow);

	char Diretory[160];

	if (cId < MAX_PLAYER)
		sprintf(Diretory, "C://StoredData/Logs/%s_%02d-%02d-%04d.txt", nBuffer[cId].Login, now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
	else
		sprintf(Diretory, "C://StoredData/Logs/Server_%02d-%02d-%04d.txt", now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);

	FILE *fs = fopen(Diretory, "a");

	if (fs == NULL)
		return;

	char Data[124];
	sprintf(Data, "[%02d:%02d:%02d]", now->tm_hour, now->tm_min, now->tm_sec);

	char FinalLog[1024];
	if (cId < MAX_PLAYER)
		sprintf(FinalLog, "%s [%s][%s] %s\n", Data, nBuffer[cId].MacAdress, inet_n2a(Users[cId].In_Addr), buff);
	else
		sprintf(FinalLog, "%s %s\n", Data, buff);

	fprintf(fs, FinalLog);
	fclose(fs);
}

void Sendpack(BYTE *bufr, int clientId, int paksize)
{
	static DWORD call_addr = 0x00401221;
	int socketAddress = (UINT32)&Users[clientId].Service.FD;
	__asm
	{
		MOV ECX, socketAddress

			PUSH paksize
			PUSH bufr
			CALL call_addr
	}
}

INT64 SystemTimeDifference(const SYSTEMTIME time1, const SYSTEMTIME time2)
{
	union timeunion {
		FILETIME fileTime;
		LARGE_INTEGER ul;
	};

	timeunion ft1;
	timeunion ft2;

	SystemTimeToFileTime(&time1, &ft1.fileTime);
	SystemTimeToFileTime(&time2, &ft2.fileTime);

	return ft2.ul.QuadPart - ft1.ul.QuadPart;
}

bool FileExist(const char* Name)
{
	if (FILE * file = fopen(Name, "r"))
	{
		fclose(file);
		return true;
	}

	return false;
}

bool CheckBan(const char *login)
{	
	//retorna true se estiver banido
	char fileName[1024];
	sprintf_s(fileName, "C://StoredData/Ban/%s.bin", login);

	if (!FileExist(fileName))
		return false;

	FILE *pFile = NULL;
	fopen_s(&pFile, fileName, "rb");
	INT8 buffer[sizeof SYSTEMTIME];

	if (pFile != NULL)
	{
		fread(buffer, 1, sizeof SYSTEMTIME, pFile);
		fclose(pFile);
	}

	SYSTEMTIME banTime, nowTime;
	memcpy(&banTime, &buffer, sizeof SYSTEMTIME);
	GetLocalTime(&nowTime);

	INT64 timeDiff = SystemTimeDifference(nowTime, banTime);

	if (timeDiff > 0)
		return true;

	remove(fileName);
	return false;
}

void SendBan(const char *login)
{
	//ban permanente
	
	char fileName[1024];
	sprintf_s(fileName, "C://StoredData/Ban/%s.bin", login);

	SYSTEMTIME mTime;
	GetLocalTime(&mTime);
	mTime.wYear = 9999;

	FILE *pFile = NULL;

	if (pFile != NULL)
	{
		fopen_s(&pFile, fileName, "wb");
		fwrite(&mTime, 1, sizeof SYSTEMTIME, pFile);
		fclose(pFile);
	}
}

bool Banir(INT16 cId, INT8 Type)
{
	if (cId < 0 || cId > MAX_PLAYER)
		return false;

	if (!Users[cId].Status)
		return false;

	char login[20];
	strcpy(login, nBuffer[cId].Login);

	SYSTEMTIME mTime;
	GetLocalTime(&mTime);
	
	switch (Type)
	{
		case BAN_3_HOURS:
		{
			mTime.wHour += 3;

			if (mTime.wHour > 23)
			{
				mTime.wHour -= 23;
				mTime.wDay++;
			}
		}
	}
}

void SendBan(const char *login, const SYSTEMTIME banTime)
{
	//ban definido por data
	
	char fileName[1024];
	sprintf_s(fileName, "C://StoredData/Ban/%s.bin", login);

	FILE *pFile = NULL;
	if (pFile != NULL)
	{
		fopen_s(&pFile, fileName, "wb");
		fwrite(&banTime, 1, sizeof SYSTEMTIME, pFile);
		fclose(pFile);
	}
}

bool SaveAccount(int clientId)
{
	if (clientId < 0 || clientId > 740)
		return false;

	char szTMP[1024];
	FILE *pFile = NULL;

	sprintf_s(szTMP, "C://StoredData/Data/%s.bin", Users[clientId].Username);

	fopen_s(&pFile, szTMP, "wb");
	if (pFile)
	{
		int mobSize = sizeof nInfo - (sizeof nBuffer[0].Ingame);

		fwrite((void*)&nBuffer[clientId], 1, mobSize, pFile);

		fclose(pFile);

		return true;
	}

	sendLog(ERROR, clientId, "Erro ao salvar a conta %s.", nBuffer[clientId].Login);

	return false;
}

void DisplayTitleInfos()
{
	return;
	char szTitle[260];

	time_t rawnow = time(NULL);
	struct tm *now = localtime(&rawnow);

	char Onlines[40];

	if (Servidor.OnlineCount > 0)
		sprintf(Onlines, "(%d jogador(es) online.)", Servidor.OnlineCount);

	sprintf(szTitle, "[%02d/%02d/%04d %02d:%02d:%02d][%02d:%02d:%02d:%02d] Nix Project Channel %d %s", now->tm_mday, now->tm_mon, now->tm_year + 1900, now->tm_hour, now->tm_min, now->tm_sec, Servidor.Days, Servidor.Hours, Servidor.Minuts, Servidor.Seconds, Servidor.Channel, Servidor.OnlineCount > 0 ? Onlines : "");

	SetConsoleTitle(szTitle);
}

static const int MesesConfig[] = {
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

void CheckItemTime(st_Item *Item)
{
	if (Item->Index < 0 || Item->Index > 6500)
		return;

	if (Item->EFV1 > MesesConfig[Item->EFV2]) // Se o dia atual + tempo da mount passar do dia limite do mês atual aqui se concerta ele 
	{
		Item->EFV1 -= MesesConfig[Item->EFV2]; // Decrescemos exatamente o tempo necessário para passagem de um mês
		Item->EFV2++; // Incrementa-se a variável do mês
	}

	if (Item->EFV2 > 12)
	{
		Item->EFV2 -= 12;
		Item->EFV3++;
	}
}

INT16 AgroupIndex[] = { 
	 410,  412,  413,  419,  420,  697,
	 699,  777, 1731, 1739, 2390, 2391,
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

INT8  AgroupLimit[] = { 
	 10,  10,  10,  10,  10,  10,
	 10, 120,  10,  10, 120, 120,
	120, 120, 120, 120, 120, 120,
	120, 120, 120, 120, 120, 120,
	120, 120, 120, 120, 120, 120,
	120, 120, 120, 120, 120, 120,
	120, 120, 120, 120,  10,  10,
	120, 120,  10,  10,  10,  10,
	120, 120, 120,   5, 120, 120,
	 10, 120,  10,  10, 120, 120,
	 10,  10,  10,  10,  10
};

bool CanAgroup(st_Item* Item)
{
	for (int i = 0; i < sizeof(AgroupIndex) / sizeof INT16; i++)
		if (Item->Index == AgroupIndex[i])
			return true;

	return false;
}

INT8 GetMaxAmount(INT16 Index)
{
	for (int i = 0; i < sizeof(AgroupLimit) / sizeof(INT8); i++)
		if (Index == AgroupIndex[i])
			return AgroupLimit[i];

	return 0;
}

int GetItemAmount(st_Item* Item)
{
	int TotalAmount = 0;

	for (int i = 0; i < 3; i++)
		if (Item->Effect[i].Index == EF_AMOUNT)
			TotalAmount += Item->Effect[i].Value;

	// Caso não haja nenhum valor de amount no item, adiciona 1
	// Todo item tem 1 de amount
	if (TotalAmount <= 0)
		TotalAmount = 1;

	return TotalAmount;
}

void PutAmount(st_Item* Item, int Amount)
{
	for (int i = 0; i < 3; i++)
	{
		if (Item->Effect[i].Index == EF_AMOUNT)
		{
			Item->Effect[i].Value = Amount;

			return;
		}
	}

	for (int i = 0; i < 3; i++)
	{
		if (!Item->Effect[i].Index)
		{
			Item->Effect[i].Index = EF_AMOUNT;
			Item->Effect[i].Value = Amount;

			return;
		}
	}
}

void TimeLimitCheck(st_Item *Item, INT16 cId, bool Fairy, char* Name)
{
	// Fairy é true quando se passou um minuto
	// Dessa forma o sistema já sabe quando é para descontar um minuto das fadas ou não
	// Quando não for fada, enviar Fairy como false
	// Quando não se tiver passado 1 minuto, enviar Fairy como falsa
	if ((Item->Index >= 3980 && Item->Index <= 3989) || (Item->Index >= 4152 && Item->Index <= 4160) || Item->Index == 3467)
	{
		time_t rawnow = time(NULL);
		struct tm *now = localtime(&rawnow);

		if (now->tm_mday > Item->EFV1 && ++now->tm_mon >= Item->EFV2 && (now->tm_year - 100) >= Item->EFV3)
		{
			SendClientMessage(cId, "Item expirou.");

			AmountMinus(Item);

			SendItem(cId, EQUIP, 14, Item);

			sendLog(INFO, cId, "%s %s item %s expirou.",  nBuffer[cId].Login, Name, itemlistBuffer[Item->Index].Name);
		}
	}
	else if (Item->Index >= 3900 && Item->Index <= 3908 && Fairy)
	{
		if (Item->EFV3 >= 1)
		{
			Item->EFV3--;

			SendItem(cId, EQUIP, 13, Item);
		}
		else
		{
			if (Item->EFV2 >= 1)
			{
				Item->EFV2--;
				Item->EFV3 = 59;

				SendItem(cId, EQUIP, 13, Item);
			}
			else
			{
				if (Item->EFV1 >= 1)
				{
					Item->EFV1--;
					Item->EFV2 = 24;
					Item->EFV3 = 0;

					SendItem(cId, EQUIP, 13, Item);
				}
				else
				{
					AmountMinus(Item);

					SendClientMessage(cId, "Sua fada expirou.");
					SendItem(cId, EQUIP, 13, Item);

					sendLog(INFO, cId, "%s %s item %s expirou.", nBuffer[cId].Login, Name, itemlistBuffer[Item->Index].Name);
				}
			}
		}
	}
}

unsigned long int GetCoinOnItens(st_Item *Inventory)
{
	if (Inventory == NULL)
		return 0;

	unsigned long int retn = 0;

	for (int i = 0; i < sizeof Inventory / sizeof st_Item; i++)
	{
		// Id do slot inválido, segue para o próximo slot
		if (Inventory[i].Index < 0 || Inventory[i].Index > 6500)
			continue;

		// Preço do item invalido, segue para o próximo slot
		if (itemlistBuffer[Inventory[i].Index].Price < 0 || itemlistBuffer[Inventory[i].Index].Price > 2000000000)
			continue;

		unsigned long int PriceOfItem = itemlistBuffer[Inventory[i].Index].Price;

		for (int e = 0; e < 3; e++)
		{
			if (Inventory[i].Effect[e].Index == EF_AMOUNT)
			{
				// Caso esteja em pack, multiplica o preço pago pelo número de itens no pack
				PriceOfItem = PriceOfItem * Inventory[i].Effect[e].Value;

				break;
			}
		}

		if ((retn + PriceOfItem) > 2000000000)
		{
			// Valor total não pode passar de 2Bi
			retn = 2000000000;

			break;
		}

		retn += PriceOfItem;
	}

	return retn;
}

int GetItemSanc(st_Item* Item)
{
	int retn = 0;

	for (int i = 0; i < 3; i++)
		if (Item->Effect[i].Index == 43 || (Item->Effect[i].Index >= 116 && Item->Effect[i].Index <= 125))
		{
			retn = Item->Effect[i].Value;
			break;
		}

	if (retn)
	{
		if (retn < 230)
			retn = retn % 10;
		else
			retn = retn - 220;
		if (retn >= 10 && retn <= 35)
			retn = (retn - 10) / 4 + 10;
	}

	return retn;
}

int GetItemSanc_OL2(st_Item* Item)
{
	int retn = 0;

	for (int i = 0; i < 3; i++)
		if (Item->Effect[i].Index == 43 || (Item->Effect[i].Index >= 116 && Item->Effect[i].Index <= 125))
		{
			retn = Item->Effect[i].Value;
			break;
		}

	return retn;
}

bool SetItemSanc(st_Item* Item, int Sanc)
{
	for (int i = 0; i < 3; i++)
	{
		if (Item->Effect[i].Index == 43 || (Item->Effect[i].Index >= 116 && Item->Effect[i].Index <= 125))
		{
			Item->Effect[i].Value = Sanc;
			return true;
		}
	}
	for (int i = 0; i < 3; i++)
	{
		if (!Item->Effect[i].Index)
		{
			Item->Effect[i].Index = EF_SANC;
			Item->Effect[i].Value = Sanc;
			return true;
		}
	}

	return false;
}

int BASE_SetItemSanc(st_Item* item, int sanc, int success) //00405070 - ok - atualizada(reconhecendo tinturas)
{
	if (sanc < 0 || sanc > 15)
		sanc = 0;

	if (success < 0) // TODO refazer o calculo para reconhecer itens +10 +
		success = 0;
	if (success > 20)
		success = 20;

	int getRef = (success * 10) + sanc;
	if (item->EF1 == 43 || (item->EF1 >= 116 && item->EF1 <= 125))
		item->EFV1 = getRef;
	else if (item->EF2 == 43 || (item->EF1 >= 116 && item->EF1 <= 125))
		item->EFV2 = getRef;
	else if (item->EF3 == 43 || (item->EF1 >= 116 && item->EF1 <= 125))
		item->EFV3 = getRef;
	else
		return FALSE;

	return TRUE;
}

bool isVip(int cId)
{
	if (cId < 0 || cId > 750)
		return false;

	else if (Users[cId].Status != 22)
		return false;

	// Ponteiro para as infos da conta do personagem
	nInfo* player = &nBuffer[cId];

	// Caso seja mod/admin
	if (player->AcessLevel > NORMAL_PLAYER)
		return true;

	// Pega os dados do tempo atual
	time_t rawnow = time(NULL);
	struct tm *now = localtime(&rawnow);

	// O ano em que o vip era válido é menor que o atual
	if (player->Vip.Validade.tm_year < now->tm_year)
		return false;
	// O ano é maior que o atual, logo vip válido
	else if (player->Vip.Validade.tm_year > now->tm_year)
		return true;
	
	// O ano é igual ao atual, verificar o resto
	if (player->Vip.Validade.tm_mon < now->tm_mon) // O mes em que era válido é menor que o atual
		return false;
	else if (player->Vip.Validade.tm_mon > now->tm_mon) // O mes de validade é maior que o atual
		return true;

	// O mes atual é igual ao de validade, verificar dia
	if (player->Vip.Validade.tm_mday < now->tm_mday)
		return false;

	return true;
}

int GetVipType(int cId)
{
	if (cId < 0 || cId > 750)
		return -1;
	else if (!isVip(cId))
		return NORMAL_PLAYER;

	return nBuffer[cId].Vip.Type;
}

void PutMoreTime(tm* tempo, int dias)
{
	int dia = tempo->tm_mday,
		mes = tempo->tm_mon,
		ano = tempo->tm_year;

	dia += dias;

	if (dia > MesesConfig[mes])
	{
		dia -= MesesConfig[mes];
		mes++;
	}

	if (mes > 11)
	{
		mes -= 12;
		ano++;
	}

	tempo->tm_mday = dia;
	tempo->tm_mon  = mes;
	tempo->tm_year = ano;
}

struct p3BB
{
	PacketHeader Header;

	short Vivos,
		  Total;
};

void NpcsAlive(int clientid, int vivos, int total)
{
	BYTE BufferSend[4096];
	*(short*)&BufferSend[0] = 16;
	*(short*)&BufferSend[4] = 0x3BB;
	*(short*)&BufferSend[6] = 30000;
	*(short*)&BufferSend[12] = vivos;
	*(short*)&BufferSend[14] = total;

	Sendpack((BYTE*)BufferSend, clientid, 16);
}

void DecrementWaterTime()
{
	for (int e = 0; e < 9; e++)
	{
		for (int i = 0; i < 3; i++)
		{
			if (Agua[i].Rooms[e].isActive)
			{
				if (Agua[i].Rooms[e].TimeLeft > 0)
					Agua[i].Rooms[e].TimeLeft--; // Decremento do tempo restante
				else
				{
					// Tempo desta sala acabou
					DoTeleportArea(WaterMMCoords[i][e][0], WaterMMCoords[i][e][1], WaterMMCoords[i][e][2], WaterMMCoords[i][e][3], 1966, 1768);
					KillMobsOnArea(WaterMMCoords[i][e][0], WaterMMCoords[i][e][1], WaterMMCoords[i][e][2], WaterMMCoords[i][e][3]);

					Agua[i].Rooms[e].MobsLeft = 0;
					Agua[i].Rooms[e].TimeLeft = 0;
					Agua[i].Rooms[e].isActive = false;
					Agua[i].Rooms[e].Leaders = 0;

					INT8 cid = -1;
					while (cid < 12)
					{
						cid++;
						Agua[i].Rooms[e].Cids[cid] = 0;
					}
				}
			}
		}
	}
}

void CreateMobsOnPesa()
{
	for (int i = 0; i < 3; i++)
	{
		Pesas* pesa = &Pesa[i];

		if (pesa->Groups > 0)
			pesa->Status = true;

		// Caso ninguém tenha entrado na quest
		if (!pesa->Status)
			continue;

		// Primeira wave, 60 segundos para matar
		// Total de 13 waves, caso não tenha finalizado a wave até o fim dos 60 segundos a quest acaba
		pesa->Wave = 1;
		pesa->TimeLeft = 60;

		// Cria o número de mobs certo por spawn configurado na array
		// e = posição no mapa onde o mob será gerado
		// f = número de mobs que serão gerados
		for (int e = 0; e < 4; e++)
		{
			int destX = MobsPesaCoords[i][e][0],
				destY = MobsPesaCoords[i][e][1],
				Max   = MobsPesaCount[i][0][e];

			for (int f = 0; f < Max; f++)
			{
				CreateMob(MobsPesaNames[i][e], destX, destY, "npc");
				pesa->MobsLeft++;
			}
		}

		// Envia o tempo verde para os participantes da quest
		// Envia também o número da Wave
		for (int e = 0; e < 3; e++)
		{
			for (int z = 0; z < 13; z++)
			{
				int cId = pesa->cId[e][z];
				if (cId > 0 && cId < MAX_PLAYER && Users[cId].Status == 22)
				{
					SendClientSignalParm(cId, 0x7530, 0x3A1, 60);
					NpcsAlive(cId, 1, 13);
				}
			}
		}
	}
}

void KillMobsOnArea(int x1, int y1, int x2, int y2)
{
	st_Mob *mobs = (st_Mob*)0x0015C69B8;

	for (int i = 1000; i < 30000; i++)
	{
		if (mobs[i].TargetX >= x1 && mobs[i].TargetY >= y1 &&
			mobs[i].TargetX <= x2 && mobs[i].TargetY <= y2)
			MobKilled(i, 0x7530, 0, 0);
	}
}

void SendAutomaticMessages()
{
	sD1D(0x7530, 0, "[Aviso] Seja bem vindo ao Nix.",						  MSG_COLOR_NOTICE);
	sD1D(0x7530, 0, "[Aviso] Curta nossa fanpage: fb.com/nixserver",		  MSG_COLOR_NOTICE);
	sD1D(0x7530, 0, "[Aviso] Siga-nos no twitter: twitter.com/NixServer",	  MSG_COLOR_NOTICE);
	sD1D(0x7530, 0, "[Aviso] Ajude o Nix a manter-se online, faça doações.",  MSG_COLOR_NOTICE);
	sD1D(0x7530, 0, "[Aviso] Atenção, GM's nunca pedem seus dados privados!", MSG_COLOR_NOTICE);
	sD1D(0x7530, 0, "[Aviso] Lembre-se, este é um Open Beta.",                MSG_COLOR_NOTICE);
	sD1D(0x7530, 0, "[Aviso] Reporte os bugs que encontrar.",                 MSG_COLOR_NOTICE);
	sD1D(0x7530, 0, "[Aviso] Aproveite a estadia e cuidado com a noite!",	  MSG_COLOR_NOTICE);
}

void ReadDonateList()
{
	FILE *fs = NULL;
	fopen_s(&fs, "C://StoredData/Lists/DonateList.txt", "r");
	
	if (!fs)
	{
		MessageBoxA(NULL, "Erro na leitura da lista de donates.", "Error", MB_OK);
		ExitProcess(1);
	}
	
	memset(DonateList, 0x0, sizeof(DonateList) / sizeof(Donate));

	char line[1024];
	int i = 0;

	while (fgets(line, sizeof(line), fs))
	{
		if (i < 300)
		{
			if (*line == '\n' || *line == '#')
				continue;

			sscanf(line, "%d,%d,%d,%d,%d,%d,%d,%d,%d", &DonateList[i].Item.Index, &DonateList[i].Item.Effect[0].Index,
				&DonateList[i].Item.Effect[0].Value, &DonateList[i].Item.Effect[1].Index, &DonateList[i].Item.Effect[1].Value,
				&DonateList[i].Item.Effect[2].Index, &DonateList[i].Item.Effect[2].Value, &DonateList[i].Price, &DonateList[i].Store);
		}
		else
		{
			//SendLog("system", "Erro: Contagem máxima na lista de donates atingida.");				
			break;
		}

		i++;
	}

	fclose(fs);
}

void s334(INT32 clientId, char *message)
{// Feita por Elisson / -Guerra-
	p334 p;
	memset(&p, 0, sizeof p);

	st_Mob *player = GetMobFromIndex(clientId);

	p.Header.ClientId = clientId;
	p.Header.PacketId = 0x334;
	p.Header.Size = sizeof p334;

	strncpy(p.Arg, message, 96);
	strncpy(p.Cmd, (char*)player->Name, 16);

	for (INT16 i = 0; i < MAX_PLAYER; i++)
	{
		if (Users[i].Status != 22)
			continue;

		else if (i != clientId)
		{
			st_Mob* mob = GetMobFromIndex(i);

			if (mob->CapeInfo == player->CapeInfo)
			    Sendpack((BYTE*)&p, i, sizeof p334);
		}
	}
}

bool CheckEItem(int id)
{
	bool retn = false;

	// Graças ao Unique podemos saber se o set é [E]
	// Caso seja setamos o retorno como true 
	int ItemUnique = itemlistBuffer[id].Unique;
	if (ItemUnique == 10 || ItemUnique == 20 || ItemUnique == 30 || ItemUnique == 40)
		retn = true;

	return retn;
}

bool CheckArchItem(int id)
{
	
	///SET'S
	if (id >= 1221 && id <= 1224) return true;
	else if (id >= 1356 && id <= 1359) return true;
	else if (id >= 1506 && id <= 1509) return true;
	else if (id >= 1656 && id <= 1659) return true;
	///ARMAS ARCH
	else if (id == 811 || id == 826 || id == 841) return true;
	else if (id == 856 || id == 871 || id == 886) return true;
	else if (id == 871 || id == 886 || id == 903 || id == 904) return true;
	else if (id == 912 || id == 937 || id == 2379 || id == 2380) return true;
	///ARMAS ARCH ANCT
	else if ((id >= 2491 && id <= 2494) || (id >= 2551 && id <= 2554)) return true;
	else if ((id >= 2611 && id <= 2614) || (id >= 2671 && id <= 2674)) return true;
	else if ((id >= 2791 && id <= 2794) || (id >= 2859 && id <= 2866)) return true;
	else if ((id >= 2895 && id <= 2898) || (id >= 2935 && id <= 2938)) return true;

	return false;
}

bool CheckHardItem(int id)
{
	int retorno = false;
	///SET'S
	if (id >= 1901 && id <= 1910){
		retorno = true;
	}

	///ARMAS ARCH
	else if (id == 1230 || id == 1231 || id == 1232 || id == 1667)
		retorno = true;
	else if (id == 1233 || id == 1365 || id == 1366 || id == 1668)
		retorno = true;
	else if (id == 1367 || id == 1368 || id == 1515 || id == 1516)
		retorno = true;
	else if (id == 1517 || id == 1518 || id == 1665 || id == 1666)
		retorno = true;

	return retorno;
}

INT32 GetMatchCombine(st_Item* pItem)
{
	for (int i = 0; i < 6; i++)
	{
		if (pItem[i].Index <= 0 || pItem[i].Index >= 6500)
			return FALSE;
	}

	if (itemlistBuffer[pItem[0].Index].Unique < 41 || itemlistBuffer[pItem[0].Index].Unique > 49)
		return FALSE;

	if (itemlistBuffer[pItem[0].Index].Pos <= 0)
		return FALSE;

	INT8 rate = 1;
	for (int j = 2; j < 6; j++)
	{
		if (BASE_GetItemAbility(&pItem[j], EF_POS) == 0)
			return FALSE;

		int ItemSanc = GetItemSanc(&pItem[j]),
			ItemSanc_2 = GetItemSanc_OL2(&pItem[j]);

		if (ItemSanc == 7)// +7
		{
			rate = rate + 2;

			continue;
		}

		if (ItemSanc == 8)// +8
		{
			rate = rate + 4;

			continue;
		}

		if (ItemSanc == 9)// +9
		{
			rate = rate + 10;

			continue;
		}

		if (ItemSanc_2 >= 230 && ItemSanc_2 <= 233)// +10
		{
			rate = rate + 14;

			continue;
		}

		if (ItemSanc_2 >= 234 && ItemSanc_2 <= 237)// +11
		{
			rate = rate + 16;

			continue;
		}

		if (ItemSanc_2 >= 238 && ItemSanc_2 <= 241)// +12
		{
			rate = rate + 18;

			continue;
		}

		if (ItemSanc_2 >= 242 && ItemSanc_2 <= 245)// +13
		{
			rate = rate + 20;

			continue;
		}

		if (ItemSanc_2 >= 246 && ItemSanc_2 <= 249)// +14
		{
			rate = rate + 22;

			continue;
		}

		if (ItemSanc_2 >= 250 && ItemSanc_2 <= 253)// +15
		{
			rate = rate + 25;

			continue;
		}

		return FALSE;
	}

	return rate;
}

void ReadBossQuests()
{
	for (int i = 0; i < 5; i++)
	{
		FILE *fs = NULL;

		char Directory[1024];
		sprintf(Directory, "C://StoredData/Lists/BOSS_QUEST_%d.txt", i);

		fopen_s(&fs, Directory, "r");

		if (!fs)
		{
			MessageBoxA(NULL, "Erro na leitura da lista de bosses das quests.", "Error", MB_OK);
			ExitProcess(1);
		}

		memset(&QuestList[i].Boss, 0, sizeof BossQuest);

		char line[1024];
		int e = 0;

		while (fgets(line, sizeof(line), fs))
		{
			if (*line == '\n' || *line == '#')
				continue;

			if (e == 0)
				sscanf(line, "%d,%d,%d,%16s",
				&QuestList[i].Boss.CountToBorn, 
				&QuestList[i].Boss.CreationPos.X, &QuestList[i].Boss.CreationPos.Y, 
				&QuestList[i].Boss.Name);
			else if (e < 11)
			{
				int count = e - 1;
				sscanf(line, "%d,%d,%d,%d,%d,%d,%d,%d", 
					&QuestList[i].Boss.Gifts[count].Index, 
					&QuestList[i].Boss.Gifts[count].EF1, &QuestList[i].Boss.Gifts[count].EFV1, 
					&QuestList[i].Boss.Gifts[count].EF2, &QuestList[i].Boss.Gifts[count].EFV2, 
					&QuestList[i].Boss.Gifts[count].EF3, &QuestList[i].Boss.Gifts[count].EFV3, 
					&QuestList[i].Boss.Chances[count]);
			}
			else break;

			e++;
		}

		fclose(fs);
	}
}

void ReadNpcsDeTroca()
{
	for (int i = 0; i < 5; i++)
	{
		FILE *fs = NULL;

		char Directory[1024];
		sprintf(Directory, "C://StoredData/Lists/NPCDETROCA_%d.txt", i);

		fopen_s(&fs, Directory, "r");

		if (!fs)
		{
			MessageBoxA(NULL, "Erro na leitura da lista de itens para as trocas.", "Error", MB_OK);
			ExitProcess(1);
		}

		memset(&Eventos.Trade[i], 0x0, sizeof TradeEvent);

		char line[1024];
		int e = 0;

		while (fgets(line, sizeof(line), fs))
		{
			if (*line == '\n' || *line == '#')
				continue;

			if (e < 5)
				sscanf(line, "%d,%d,%d,%d,%d,%d,%d", 
				&Eventos.Trade[i].RequestItem[e].Index, 
				&Eventos.Trade[i].RequestItem[e].EF1, &Eventos.Trade[i].RequestItem[e].EFV1, 
				&Eventos.Trade[i].RequestItem[e].EF2, &Eventos.Trade[i].RequestItem[e].EFV2, 
				&Eventos.Trade[i].RequestItem[e].EF3, &Eventos.Trade[i].RequestItem[e].EFV3);
			else if (e < 15)
			{
				int count = e - 5;
				sscanf(line, "%d,%d,%d,%d,%d,%d,%d",
					&Eventos.Trade[i].PrizeItem[count].Index,
					&Eventos.Trade[i].PrizeItem[count].EF1, &Eventos.Trade[i].PrizeItem[count].EFV1,
					&Eventos.Trade[i].PrizeItem[count].EF2, &Eventos.Trade[i].PrizeItem[count].EFV2,
					&Eventos.Trade[i].PrizeItem[count].EF3, &Eventos.Trade[i].PrizeItem[count].EFV3);
			}
			else break;

			e++;
		}

		fclose(fs);
	}
}

void ReadQuestBonusItensList()
{
	for (int i = 0; i < 5; i++)
	{
		FILE *fs = NULL;

		char Directory[1024];
		sprintf(Directory, "C://StoredData/Lists/QuestBonusItensList_%d.txt", i);

		fopen_s(&fs, Directory, "r");

		if (!fs)
		{
			MessageBoxA(NULL, "Erro na leitura da lista de itens para as quests.", "Error", MB_OK);
			ExitProcess(1);
		}

		memset(&QuestItensEntered[i], 0x0, sizeof QuestItensOnEnter);

		char line[1024];
		int e = 0;

		while (fgets(line, sizeof(line), fs))
		{
			if (*line == '\n' || *line == '#')
				continue;

			if (e < 10)
				sscanf(line, "%d,%d,%d,%d,%d,%d,%d,%d",
					&QuestItensEntered[i].Itens[e].Index,
					&QuestItensEntered[i].Itens[e].EF1, &QuestItensEntered[i].Itens[e].EFV1,
					&QuestItensEntered[i].Itens[e].EF2, &QuestItensEntered[i].Itens[e].EFV2,
					&QuestItensEntered[i].Itens[e].EF3, &QuestItensEntered[i].Itens[e].EFV3,
					&QuestItensEntered[i].Chances[e]);
			else break;

			e++;
		}

		fclose(fs);
	}
}

void OpenTrade(short ClientID, short MobID)
{
	// Usado após o Request, onde é enviado o trade totalmente vazio
	SendTrade p;
	memset(&p, 0, sizeof p);
	p.Header.ClientId = MobID;
	p.OtherClientid = ClientID;
	p.Header.Size = 156;
	p.Header.PacketId = 0x383;
	for (UINT8 i = 0; i<15; i++)
		p.TradeItemSlot[i] = 255;
	Sendpack((BYTE*)&p, MobID, 156);
}

void SendAllTrade(short ClientID, short MobID)
{
	// Envia o trade completo para o MobID
	SendTrade p;
	memset(&p, 0, sizeof p);
	p.Header.ClientId = MobID;
	p.OtherClientid = ClientID;
	p.Header.Size = 156;
	p.Header.PacketId = 0x383;
	p.Confirma = Trade[ClientID].Confirm;

	memcpy(&p.TradeItem, &Trade[ClientID].Itens, sizeof(st_Item) * 15);
	strncpy(p.TradeItemSlot, Trade[ClientID].TradeItemSlot, 15);

	p.Gold = Trade[ClientID].Gold;
	Sendpack((BYTE*)&p, MobID, 156);
}

void CloseTrade(short ClientID)
{
	// Fecha o trade dos dois usuários
	memset(Trade[ClientID].TradeItemSlot, 255, sizeof(char) * 15);
	memset(Trade[ClientID].Itens, 0, sizeof st_Item * 15);

	Trade[ClientID].isTrading = false;
	Trade[ClientID].Timer = 0;
	Trade[ClientID].Confirm = false;
	Trade[ClientID].WaitingData = false;

	PacketHeader Header;
	Header.ClientId = ClientID;
	Header.Size = 12;
	Header.PacketId = 0x384;
	Sendpack((BYTE*)&Header, ClientID, 12);

	Trade[ClientID].ClientID = 0;
}

void CheckItensOnTrade(short ClientID, short MobID)
{
	// Sua função é checar se os itens colocados no trade realmente existem no inventário do usuário
	// Se não existirem, provavelmente é uma tentativa de hack pois o client não se enganará com isso
	st_Mob *player = GetMobFromIndex(ClientID);
	st_Mob *mob = GetMobFromIndex(MobID);
	for (UINT8 i = 0; i<15; i++)
	{
		if (Trade[ClientID].Itens[i].Index == 255)
			continue;
		if (!memcmp(&Trade[ClientID].Itens[i], &player->Inventory[Trade[ClientID].TradeItemSlot[i]], 8))
		{
			CloseTrade(ClientID);
			break;
		}
		if (!memcmp(&Trade[MobID].Itens[i], &mob->Inventory[Trade[MobID].TradeItemSlot[i]], 8))
		{
			CloseTrade(MobID);
			break;
		}
	}
}

bool CanTrade(UINT16 Index)
{
	// Função que serve para adicionar Itens imóveis
	switch (Index)
	{
	case 508: case 509: case 526:
	case 527: case 528:
		return false;

	default:
		return true;
	}

	return true;
}

bool AddItensOnTrade(SendTrade *p)
{
	INT16 cId = p->Header.ClientId;

	// Serve para a verificação de um item imóvel e para adicionar os itens do trade a struct trade do usuário
	for (UINT8 i = 0; i<15; i++)
	{
		if (!CanTrade(p->TradeItem[i].Index))
		{
			SendClientMessage(cId, "Item imóvel!");
			return false;
		}

		Trade[cId].TradeItemSlot[i] = p->TradeItemSlot[i];

		memcpy(&Trade[cId].Itens[i], &p->TradeItem[i], sizeof st_Item);
	}

	INT16 OtherClientid = p->OtherClientid;
	st_Mob *player = GetMobFromIndex(cId);
	st_Mob *mob = GetMobFromIndex(OtherClientid);

	if (p->Gold > player->Gold)
		return false;

	if ((player->Gold - Trade[cId].Gold) + Trade[OtherClientid].Gold > 2000000000)
	{
		SendClientMessage(cId, "Limite de 2.000.000.000 bilhões de gold.");
		SendClientMessage(OtherClientid, "Limite de 2.000.000.000 bilhões de gold.");

		Trade[cId].Gold = 0;
		Trade[OtherClientid].Gold = 0;
		return false;
	}

	if ((mob->Gold - Trade[OtherClientid].Gold) + Trade[cId].Gold > 2000000000)
	{
		SendClientMessage(cId, "Limite de 2.000.000.000 bilhões de gold.");
		SendClientMessage(OtherClientid, "Limite de 2.000.000.000 bilhões de gold.");

		Trade[cId].Gold = 0;
		Trade[OtherClientid].Gold = 0;
		return false;
	}

	return true;
}

BYTE GetAmountItemOnTrade(short ClientID)
{
	// Retorna quantos itens foram colocados no trade do usuário
	BYTE val = 0;
	for (UINT8 i = 0; i<15; i++)
		if (Trade[ClientID].Itens[i].Index != 0)
			val++;
	return val;
}

BYTE GetAmountItem(WORD clientid)
{
	st_Mob* player = GetMobFromIndex(clientid);
	BYTE j = 0;
	for (BYTE i = 0; i < 64; i++)
		if (player->Inventory[i].Index == 0)
			j++;
	return j;
}

void SendAll(int clientid)
{
	sendinv1(clientid);
	sendinv2(clientid);
	sendstats(clientid);
	sendequip(clientid);
	return;
}

void RefinarItemMais(st_Item *item, int value)
{
	if (item->EF1 == EF_SANC || (item->EF1 >= 116 && item->EF1 <= 125))
		item->EFV1 += value;
	else if (item->EF2 == EF_SANC || (item->EF2 >= 116 && item->EF2 <= 125))
		item->EFV2 += value;
	else if (item->EF3 == EF_SANC || (item->EF3 >= 116 && item->EF3 <= 125))
		item->EFV3 += value;
	else
	{
		if (item->EF1 == 0)
		{
			item->EF1 = EF_SANC;
			item->EFV1 = value;
		}
		else if (item->EF2 == 0)
		{
			item->EF2 = EF_SANC;
			item->EFV2 = value;
		}
		else if (item->EF3 == 0)
		{
			item->EF3 = EF_SANC;
			item->EFV3 = value;
		}
	}

	return;
}

void ReadResetList()
{
	FILE *fs = NULL;
	fopen_s(&fs, "C://StoredData/Lists/ResetList.txt", "r");

	if (!fs)
	{
		MessageBoxA(NULL, "Erro na leitura da lista de resets.", "Error", MB_OK);
		ExitProcess(1);

		return;
	}

	memset(ResetList, 0x0, sizeof(ResetList) / sizeof(Resets));

	char line[1024] = { 0, };
	int i = 0;
	while ((fscanf(fs, "%[^\n]", line)) != EOF)
	{
		if (i < 300)
		{
			fgetc(fs);
			//<TimeInterval>,<X_Min>,<Y_Min>,<X_Max>,<Y_Max>,<Lvl_Min>,<Lvl_Max>
			sscanf(line, "%d,%d,%d,%d,%d,%d,%d", &ResetList[i].Time, &ResetList[i].MinCoords.X,
				&ResetList[i].MinCoords.Y, &ResetList[i].MaxCoords.X, &ResetList[i].MaxCoords.Y,
				&ResetList[i].MinLevel, &ResetList[i].MaxLevel);

			if (!ResetList[i].Time)
				continue;
		}
		else
		{
			//SendLog("system", "Erro: Contagem máxima na lista de resetes atingida.");				
			break;
		}

		i++;
	}

	fclose(fs);
}

void ReadChannel()
{
	FILE *fs = NULL;
	fopen_s(&fs, "Channel.txt", "r");

	if (!fs)
	{
		MessageBoxA(NULL, "Erro na leitura do Canal.", "Error", MB_OK);
		ExitProcess(1);
	}

	char line[1024];
	int i = 0;

	while ((fscanf(fs, "%[^\n]", line)) != EOF)
	{
		char msg[1024] = { 0, };
		fgetc(fs);
		
		sscanf(line, "%d %d %d", &Servidor.Channel, &Servidor.DropBonus, &Servidor.ExpBonus);
	}

	fclose(fs);
}

void ReadTabList()
{
	FILE *fs = NULL;
	fopen_s(&fs, "C://StoredData/Lists/TabList.txt", "r");

	if (!fs)
	{
		MessageBoxA(NULL, "Erro na leitura da lista de Tabs.", "Error", MB_OK);
		ExitProcess(1);
	}

	memset(TabList, 0x0, sizeof(TabList) / sizeof(Tabs));

	char line[1024] = { 0, };
	int i = 0;

	while ((fscanf(fs, "%[^\n]", line)) != EOF)
	{
		if (i >= 50)
			break;

		char msg[1024] = { 0, };
		fgetc(fs);
		//<Dextery>,<msg>
		sscanf(line, "%d,%26s", &TabList[i].Dex, &msg);
		strncpy(TabList[i].Tab, msg, 26);	

		i++;
	}

	fclose(fs);
}

INT32 CheckItems(st_Window *pServer)
{
	INT32 retn = -1;

	static const INT16 ItemJoia[6][4] = {
		{ 413, 2441, 2442, 3200 }, { 413, 2443, 2442, 3201 }, { 4127, 4127, 4127, 3202 },
		{ 4127, 4127, 697, 3203 }, { 412, 2441, 2444, 3204 }, { 412, 2444, 2443, 3205 }
	};

	for (INT8 i = 0; i < sizeof(ItemJoia) / sizeof(INT16); i++)
	{
		if (pServer->ItemId[0].Index == ItemJoia[i][0] && pServer->ItemId[1].Index == ItemJoia[i][1] && pServer->ItemId[2].Index == ItemJoia[i][2])
			retn = ItemJoia[i][3];
	}

	return retn;
}

void Composition(st_Window *pServer, st_Mob *player)
{
	INT16 cId = pServer->Header.ClientId;

	if (CheckItems(pServer) == -1)
		SendClientMessage(cId, "Há algo errado com sua composição.");
	else
	{
		for (char i = 0; i < 3; i++)
			memset(&player->Inventory[pServer->Slot[i]], 0, sizeof st_Item);

		srand(time(NULL) / 5 * (rand() % 500) * 5);

		INT8 Random = Rand() % 100, Chances = 0;

		if (player->Equip[0].EFV2 == MORTAL)
			Chances = 50;
		else if (player->Equip[0].EFV2 >= CELESTIAL)
			Chances = 80;

		if (isVip(cId))
		{
			int VipType = GetVipType(cId);

			if (VipType == FOGO_VIP)
				Chances += 4;
			else
				Chances += 8;
		}

		if (Random > Chances)
			SendClientMessage(cId, "Falha na composição.");
		else
		{
			SendClientMessage(cId, "Composição concluída.");

			st_Item pItem;
			memset(&pItem, 0x0, sizeof(st_Item));

			pItem.Index = CheckItems(pServer);

			GiveItem(cId, &pItem);
		}

		for (char i = 0; i < 3; i++)
			SendItem(cId, INVENTORY, pServer->Slot[i], &player->Inventory[pServer->Slot[i]]);
	}
}

int DeleteItemAmount(int clientId, int itemId, int Amount)
{
	if (clientId < 0 || clientId > 30000)
		return -1;
	if (itemId < 0 || itemId > 6500)
		return -1;

	st_Mob *player = _GetMobFromIndex(clientId);
	INT16 result = 0;
	while (result < Amount)
	{
		int Index = GetFirstSlot(clientId, itemId, INVENTORY);

		if (Index != -1)
		{
			memset(&player->Inventory[Index], 0, sizeof st_Item);

			SendItem(clientId, INVENTORY, Index, 0);

			SendCarry(clientId);

			result++;
		}
	}

	return result;
}

bool SaveServerData()
{
	char szTMP[1024];
	FILE *fs = NULL;
	
	sprintf(szTMP, "C://StoredData/Server.bin");

	fopen_s(&fs, szTMP, "wb");
	if (fs)
	{
		int fileSize = sizeof sInfos;

		fwrite((void*)&Servidor, 1, fileSize, fs);

		fclose(fs);

		return true;
	}

	return false;
}

bool SaveGuilds()
{
	char szTMP[1024];
	FILE *pFile = NULL;

	sprintf_s(szTMP, "C://StoredData/Guilds.bin");

	fopen_s(&pFile, szTMP, "wb");
	if (pFile)
	{
		int fileSize = sizeof GuildInfos * 0xFFFF;

		fwrite((void*)&Guilds, 1, fileSize, pFile);

		fclose(pFile);

		return true;
	}

	return false;
}

void ReadServerFile()
{
	memset(&Servidor, 0x0, sizeof sInfos);

	FILE *hFile = NULL;
	char szTMP[128];
	sprintf_s(szTMP, "C://StoredData/Server.bin");
	fopen_s(&hFile, szTMP, "rb");

	if (hFile != NULL)
	{
		fread(&Servidor, 1, sizeof sInfos, hFile);
		fclose(hFile);
	}
	else
	{
		MessageBoxA(NULL, "Erro na leitura do servidor.", "Error", MB_OK);
		ExitProcess(1);
	}
}

void ReadGuildFile()
{
	memset(&Guilds, 0x0, sizeof(GuildInfos) * 0xFFFF);

	FILE *hFile = NULL;
	char szTMP[128];
	sprintf_s(szTMP, "C://StoredData/Guilds.bin");
	fopen_s(&hFile, szTMP, "rb");

	if (hFile != NULL)
	{
		fread(&Guilds, 1, sizeof(GuildInfos) * 0xFFFF, hFile);
		fclose(hFile);
	}
	else
	{
		MessageBoxA(NULL, "Erro na leitura das guilds.", "Error", MB_OK);
		ExitProcess(1);
	}
}

void UpdateGuildInfo(INT16 cId)
{
	if (cId < 0 || cId > 750)
		return;
	else if (Users[cId].Status != 22)
		return;

	st_Mob *player = GetMobFromIndex(cId);

	AAA packet;
	memset(&packet, 0x0, sizeof AAA);

	packet.Header.ClientId = 0x7530;
	packet.Header.Size = sizeof AAA;
	packet.Header.PacketId = 0xAAA;

	memcpy(&packet.Guild, &Guilds[player->GuildIndex], sizeof GuildInfos);

	Sendpack((BYTE*)&packet, cId, sizeof AAA);
}

static const INT16 MaxMinCoordsCitys[4][4] =
{
	{ 2063, 2062, 2150, 2159 }, // Armia
	{ 2459, 1646, 2665, 1765 }, // Azran
	{ 2449, 1968, 2474, 2023 }, // Erion
	{ 3606, 3101, 3687, 3159 }  // Karden
};

int GetCity(int cId)
{
	if (cId < 0 || cId > 750)
		return -1;

	st_Mob *player = GetMobFromIndex(cId);

	for (int x = 0; x < 4; x++)
		if (player->TargetX >= MaxMinCoordsCitys[x][0] && player->TargetX <= MaxMinCoordsCitys[x][2] &&
			player->TargetY >= MaxMinCoordsCitys[x][1] && player->TargetY <= MaxMinCoordsCitys[x][3])
			return x;

	return -1;
}

int TeleportGuild(int GuildIndex, int DestX, int DestY)
{
	if (DestX < 0 || DestX > 4096 || DestY < 0 || DestY > 4096)
		return 0;

	int count = 0;

	for (int i = 0; i < MAX_PLAYER; i++)
	{
		if (Users[i].Status != 22)
			continue;
		st_Mob* player = GetMobFromIndex(i);

		if (player->GuildIndex == GuildIndex)
		{
			DoTeleport(i, DestX, DestY);
			count++;
		}
	}

	return count;
}

void AtualizeWarCount(int WarId)
{
	if (WarId < 0 || WarId  > 3)
		return;

	WarInfos* guerra = &War[WarId];
	if (guerra->Status == GUERRA_TERMINADA)
		return;

	for (int i = 0; i < MAX_PLAYER; i++)
	{
		if (Users[i].Status != 22)
			continue;

		st_Mob* player = GetMobFromIndex(i);

		if (player->GuildIndex == guerra->Desafiado || player->GuildIndex == guerra->Desafiante)
			NpcsAlive(i, guerra->Desafiado, guerra->Desafiante);
	}
}

void DefineWinner(int WarId)
{
	if (WarId < 0 || WarId > 3)
		return;

	WarInfos* guerra = &War[WarId];
	if (guerra->Status == GUERRA_TERMINADA)
		return;

	if (guerra->TamanhoExercitoDesafiado >  0 && guerra->TamanhoExercitoDesafiante <= 0)
	{
		Guilds[guerra->Desafiado].CidadePossuida = WarId;
		Guilds[guerra->Desafiado].Points += (guerra->TotalBet / 2);

		Servidor.PossuidorIndex[WarId] = guerra->Desafiado;

		char szMsg[120];
		sprintf(szMsg, "Guild [%s] venceu a guerra.", Guilds[guerra->Desafiado].GuildName);

		SendNotice(szMsg);
		sendLog(INFO, 1000, "%s %d", szMsg, WarId);
	}
	else if (guerra->TamanhoExercitoDesafiado <= 0 && guerra->TamanhoExercitoDesafiante > 0)
	{
		Guilds[guerra->Desafiante].CidadePossuida = WarId;
		Guilds[guerra->Desafiante].Points += (guerra->TotalBet / 2);

		Servidor.PossuidorIndex[WarId] = guerra->Desafiante;

		char szMsg[120];
		sprintf(szMsg, "Guild [%s] venceu a guerra.", Guilds[guerra->Desafiante].GuildName);

		SendNotice(szMsg);
		sendLog(INFO, 1000, "%s %d", szMsg, WarId);
	}
	else
	{
		// Nenhum dos dois exercitos conseguiu exterminar o outro, então a guerra precisa ser desempatada
		// Os níveis dos jogadores participantes são somados para assim conseguir desempatar
		// A guild com o maior nível total ganha
		st_Mob *mob = (st_Mob*)(0x0015C69B8);

		INT32 DesafianteLevel = 0,
			DesafiadoLevel = 0;

		for (int i = 0; i < MAX_PLAYER; i++)
		{
			if (Users[i].Status == 22 && mob[i].TargetX >= 1075 && mob[i].TargetY >= 1969 && mob[i].TargetX <= 1225 && mob[i].TargetY <= 2126)
			{
				// Mob está na área da guerra
				if (mob[i].GuildIndex == guerra->Desafiado)
					DesafiadoLevel += mob[i].bStatus.Level;
				else if (mob[i].GuildIndex == guerra->Desafiante)
					DesafianteLevel += mob[i].bStatus.Level;
			}
		}

		if (DesafianteLevel > DesafiadoLevel)
		{
			Guilds[guerra->Desafiante].CidadePossuida = WarId;
			Guilds[guerra->Desafiante].Points += (guerra->TotalBet / 2);

			Servidor.PossuidorIndex[WarId] = guerra->Desafiante;

			char szMsg[120];
			sprintf(szMsg, "Guild [%s] venceu a guerra.", Guilds[guerra->Desafiante].GuildName);

			SendNotice(szMsg);
			sendLog(INFO, 1000, "%s %d", szMsg, WarId);

		}
		else if (DesafiadoLevel > DesafianteLevel)
		{
			Guilds[guerra->Desafiado].CidadePossuida = WarId;
			Guilds[guerra->Desafiado].Points += (guerra->TotalBet / 2);

			Servidor.PossuidorIndex[WarId] = guerra->Desafiado;

			char szMsg[120];
			sprintf(szMsg, "Guild [%s] venceu a guerra.", Guilds[guerra->Desafiado].GuildName);

			SendNotice(szMsg);
			sendLog(INFO, 1000, "%s %d", szMsg, WarId);
		}
		else
		{
			SendNotice("Guerra não teve vencedor em tempo hábil.");
			sendLog(INFO, 1000, "Guerra %d terminou empatada.", WarId);
		}
	}

	ClearArea(1075, 1969, 1225, 2126);

	guerra->Status = GUERRA_TERMINADA;
}

void WINAPI FinishWar()
{
	time_t rawnow = time(NULL);
	struct tm *now = localtime(&rawnow);

	int ActualWar = 0;
	if (now->tm_hour == 18)
		ActualWar = KARDE_CITYID;
	else if (now->tm_hour == 19)
		ActualWar = ERION_CITYID;
	else if (now->tm_hour == 20)
		ActualWar = AZRAN_CITYID;
	else
		ActualWar = ARMIA_CITYID;

	if (now->tm_min < 55)
		Sleep(300000);
	else
	{
		int tempoRestante = (60 - now->tm_min - 1);
		if (tempoRestante < 0)
			tempoRestante = 0;

		Sleep(tempoRestante * 60 * 1000);
	}

	DefineWinner(ActualWar);
}

void StartWar(int WarId)
{
	if (WarId < 0 || WarId > 3)
	{
		sendLog(ERROR, 1000, "unknow WarId");
		return;
	}

	WarInfos* guerra = &War[WarId];
	if (guerra->Status == GUERRA_TERMINADA)
		return;

	// Zera todas as infos da guerra para setar em paz
	memset(guerra, 0, sizeof WarInfos);

	INT64 DefensorBet[2] = { 0, 0 };
	INT16 DefensorIndex[2] = { 0, 0 };
	// Acha o defensor e o desafiante
	for (int i = 0; i < 64000; i++)
	{
		// Pega somente as apostas na cidade atual
		if (Guilds[i].CidadeApostada != WarId)
			continue;
		else if (Guilds[i].Kingdom != 7 && Guilds[i].Kingdom != 8)
			continue;

		guerra->TotalBet += Guilds[i].ValorApostado;

		if (Guilds[i].ValorApostado > DefensorBet[0])
		{
			DefensorBet[0] = Guilds[i].ValorApostado;
			DefensorIndex[0] = i;
		}
		else if (Guilds[i].ValorApostado > DefensorBet[1])
		{
			DefensorBet[1] = Guilds[i].ValorApostado;
			DefensorIndex[1] = i;
		}
	}

	if (!DefensorIndex[0])
	{
		// Não houve apostantes...
		SendNotice("Não houveram desafiadores.");
		sendLog(INFO, 1000, "Não houve defensores na cidade %d.", WarId);

		guerra->Status = GUERRA_TERMINADA;

		return;
	}
	else if (!DefensorIndex[1])
	{
		// Houve defensor mas não desafiante
		SendNotice("Não houveram desafiadores.");
		guerra->Status = GUERRA_TERMINADA;

		sendLog(INFO, 1000, "Não houveram desafiantes na cidade %d.", WarId);

		return;
	}

	guerra->Desafiado = DefensorIndex[0];
	guerra->Desafiante = DefensorIndex[1];

	if (Guilds[guerra->Desafiado].Kingdom == 8)
	{
		guerra->TamanhoExercitoDesafiado  = TeleportGuild(guerra->Desafiado,  1223, 1969);
		guerra->TamanhoExercitoDesafiante = TeleportGuild(guerra->Desafiante, 1075, 2126);
	}
	else
	{
		guerra->TamanhoExercitoDesafiado  = TeleportGuild(guerra->Desafiado,  1075, 2126);
		guerra->TamanhoExercitoDesafiante = TeleportGuild(guerra->Desafiante, 1223, 1969);
	}

	AtualizeWarCount(WarId);

	guerra->Status = GUERRA_INICIADA;

	sendLog(INFO, 1000, "Guerra %d iniciada %d(%d) x %d(%d).", WarId, guerra->Desafiado, guerra->TamanhoExercitoDesafiado, guerra->Desafiante, guerra->TamanhoExercitoDesafiante);
}

void GuildProcess()
{
	time_t rawnow = time(NULL);
	struct tm *now = localtime(&rawnow);

	INT32 LOCAL_11 = 0;

}

static void(*BASE_GetGuildName)(int, unsigned short, char *) = (void f_ptr(int, unsigned short, char *)) 0x004013B1; // Lib: Basedef.obj Class : (null)

void DecideWinner()
{
	for (int LOCAL_1 = 0; LOCAL_1 < 0x004CBC44; LOCAL_1++)
	{
		if (LOCAL_1 == 3 || LOCAL_1 == 4)
			continue;

		INT32 LOCAL_2 = 0,
			  LOCAL_3 = 0,
			  DefensorId = *(INT32*)(0x004C7BC0 * LOCAL_1 * 0x50),
			  DesafianteId = *(INT32*)(0x004C7BC4 * LOCAL_1 * 0x50);

		if (!DesafianteId)
			continue;

		char GuildName1[256], GuildName2[256];
		BASE_GetGuildName(0x004CBBC4, DefensorId, GuildName1);
		BASE_GetGuildName(0x004CBBC4, DesafianteId, GuildName2);

		for (int LOCAL_134 = *(int*)(0x004C7BEC * 0x50 * LOCAL_1); LOCAL_134 <= *(int*)(0x004C7BF4); LOCAL_134++)
		{
			for (int LOCAL_135 = *(int*)(0x004C7BE8 * 0x50 * LOCAL_1); LOCAL_135 < *(int*)(0x004C7BF0 * 0x50 * LOCAL_1); LOCAL_135++)
			{
				INT32 cId = ((LOCAL_134 << 0xD) * LOCAL_135 * 2 + 0x003B07D14);
				if (cId < 0 || cId > MAX_PLAYER)
					continue;
				else if (Users[cId].Status != 22)
					continue;
				//else if (!cMob[LOCAL_136].Mode)
				//	continue;
				//else if (cMob[LOCAL_136].Status.curHP <= 0)
				//	continue;
				st_Mob *tmp = GetMobFromIndex(cId);

				INT32 LOCAL_137 = tmp->GuildIndex,
					  LOCAL_138 = tmp->Status.Level;

				if (!LOCAL_137)
					continue;
				else if (LOCAL_137 != DefensorId && LOCAL_137 != DesafianteId)
					continue;

				//sprintf(szMsg, "sys, guild war survive %s(%d-%d)");
				//Log(0x008B7E678, "-system", szMsg);

				if (LOCAL_137 == DefensorId)
					LOCAL_2 += LOCAL_138;
				else if (LOCAL_137 == DesafianteId)
					LOCAL_3 += LOCAL_138;
				//else
					//err,more than 3 guild at same guild zone
			}
		}

		//Log(... "sys, guild war %d result %d(%d) %d(%d)"
		if (LOCAL_2 >= LOCAL_3)
		{
			//sprintf(szMsg, "...");
			SendGuildNotice(DefensorId, "");//szMsg
			SendGuildNotice(DesafianteId, "");//szMsg
		}
		else
		{
			//sprintf(szMsg, "...");
			SendGuildNotice(DefensorId, "");//szMsg
			SendGuildNotice(DesafianteId, "");//szMsg

			*(int*)(0x004C7BC0 * 0x50 * LOCAL_1) = DesafianteId;
		}

		*(int*)(0x004C7BC4 * 0x50 * LOCAL_1) = 0;
	}
}

char *NumberToString(INT64 number)
{
	char st[32];
	char result[32];
	memset(result, 0, sizeof result);
	
	sprintf(st, "%I64d", number);
	
	int len = strlen(st);
	int sum = ((len - 1) / 3);

	for (int i = (len - 1), count = 0, index = (len - 1) + sum; i >= 0; i--, count++)
	{
		if (!(count % 3) && count != 0)
		{
			result[index] = '.';
			index--;
		}

		result[index] = st[i];

		count++;
		index--;
	}

	return result;
}

void ReadEventFile()
{
	memset(&Eventos, 0x0, sizeof Events);

	FILE *hFile = NULL;
	char szTMP[128];
	sprintf_s(szTMP, "C://StoredData/eventData.bin");
	fopen_s(&hFile, szTMP, "rb");

	if (hFile != NULL)
	{
		fread(&Eventos.Drop, 1, sizeof Events, hFile);
		fclose(hFile);
	}
	else
		printf("\n Error: loadEventFile() \n");
		//MessageBoxA(NULL, "Erro na leitura dos npcs de evento.", "Error", MB_OK);
}
