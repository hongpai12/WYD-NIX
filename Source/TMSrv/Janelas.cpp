#include "DLLMain.h"

void Compounder(BYTE *packetBuffer, bool *pRetn)
{
	*pRetn = true;

	st_Window *p = (st_Window*)packetBuffer;
	
	INT16 cId = p->Header.ClientId;

	if (cId <= 0 || cId >= MAX_PLAYER)
		return;

	st_Mob *player = (st_Mob*)GetMobFromIndex(cId);

	for (INT8 i = 0; i < 6; i++)
	{
		if (p->Slot[i] == 0xFF)
			return;

		else if (memcmp(&p->ItemId[i], &player->Inventory[p->Slot[i]], sizeof st_Item))
			return;

		else if (p->ItemId[i].Index != player->Inventory[p->Slot[i]].Index)
			return;

		else if (p->ItemId[i].Index < 0 || p->ItemId[i].Index > 6500)
			return;

		else if (i >= 2)
		{
			if (player->Inventory[p->Slot[i]].Index && GetItemSanc(&p->ItemId[i]) < 7)
			{
				SendClientMessage(cId, "Somente itens com refinação superior ou igual a 7.");

				return;
			}
		}
	}

	
	INT16 ItemExtreme = itemlistBuffer[p->ItemId[0].Index].Extreme,
		  ItemIndex = p->ItemId[1].Index - 2441;

	if (ItemExtreme <= 0 || ItemExtreme > 6500)
	{
		SendClientMessage(cId, "Este item já é Anct.");

		return;
	}

	if (ItemIndex < 0 || ItemIndex > 3)
	{
		SendClientMessage(cId, "Necessário 1 jóia Garnet, Coral, Esmeralda ou Diamente.");

		return;
	}

	for (INT8 i = 1; i < 6; i++)
	{
		memset(&player->Inventory[p->Slot[i]], 0, sizeof(st_Item));
		SendItem(cId, INVENTORY, p->Slot[i], &player->Inventory[p->Slot[i]]);
	}

	srand(time(NULL) / 5 * (rand() % 500) * 5);

	INT8 Chance = rand() % 100,
		 Percent = 0;

	char Message[245];

	int RealChance = GetMatchCombine(p->ItemId),
		VipType = GetVipType(cId);

	if (VipType == FOGO_VIP)
		RealChance += 4;
	else if (VipType == GELO_VIP)
		RealChance += 8;

	if (Chance <= RealChance)
	{
		player->Inventory[p->Slot[0]].Index = (INT16)(ItemExtreme + ItemIndex);

		SetItemSanc(&player->Inventory[p->Slot[0]], 7);

		sprintf(Message, "%s obteve sucesso na composição do item %s (%d/%d).", player->Name, itemlistBuffer[p->ItemId[0].Index].Name, Chance, RealChance);

		SendEffect(cId, 100, (Rand() % 6));
		SendEffect(cId, 100, (Rand() % 6));
		SendEffect(cId, 100, (Rand() % 6));
	}
	else
	{
		memset(&player->Inventory[p->Slot[0]], 0, sizeof(st_Item));

		sprintf(Message, "Houve falha na composição do item.");
	}

	SendItem(cId, INVENTORY, p->Slot[0], &player->Inventory[p->Slot[0]]);

	SendClientMsg(cId, "%s (%d/%d)", Message, Chance, RealChance);
	sendLog(INFO, cId, "%s %s %s", nBuffer[cId].Login, player->Name, Message);
	
	if (Chance <= RealChance)
	{ 
		sprintf(Message, "%s obteve sucesso na composição ancient do item [%s]. Parabéns!", player->Name, itemlistBuffer[player->Inventory[p->Slot[0]].Index].Name);
		SendNotice(Message);
	}
		
	SendClientSignalParm(cId, 0x7530, 0x3A7, 2);
}

void Agatha(BYTE *m_PacketBuffer, bool *pRetn)
{
	*pRetn = true;

	st_Window *p = (st_Window*)m_PacketBuffer;

	INT16 cId = p->Header.ClientId;

	st_Mob *player = (st_Mob*)GetMobFromIndex(p->Header.ClientId);

	if (cId <= 0 || cId >= MAX_PLAYER)
		return;

	for (INT8 i = 0; i < 6; i++)
	{
		if (p->Slot[i] == 0xFF)
			return;

		else if (memcmp(&p->ItemId[i], &player->Inventory[p->Slot[i]], sizeof st_Item))
			return;

		else if (p->ItemId[i].Index != player->Inventory[p->Slot[i]].Index)
			return;

		else if (p->ItemId[i].Index < 0 || p->ItemId[i].Index > 6500)
			return;
	}

	INT16 SancSlot1 = GetItemSanc(&p->ItemId[0]), SancSlot2 = GetItemSanc(&p->ItemId[1]);

	if (SancSlot1 < 9 || SancSlot1 > 253 || SancSlot2 < 9 || SancSlot2 > 253)
	{
		SendClientMessage(cId, "O item deve ter a refinação acima de +9.");

		return;
	}

	INT8 Percent = 0;
	INT16 grade = itemlistBuffer[p->ItemId[1].Index].Grade;

	if (grade == 1)
		Percent = 12;
	else if (grade == 2)
		Percent = 18;
	else if (grade == 3)
		Percent = 22;
	else if (grade == 4)
		Percent = 29;
	else
		Percent = 10;

	if (SancSlot2 >= 230 && SancSlot2 <= 233)
		Percent += 18;
	else if (SancSlot2 >= 234 && SancSlot2 <= 237)
		Percent += 26;
	else if (SancSlot2 >= 238 && SancSlot2 <= 241)
		Percent += 28;
	else if (SancSlot2 >= 242 && SancSlot2 <= 245)
		Percent += 30;
	else if (SancSlot2 >= 246 && SancSlot2 <= 249)
		Percent += 32;
	else if (SancSlot2 >= 250 && SancSlot2 <= 253)
		Percent += 34;
	else
		Percent += 10;

	if (isVip(cId))
	{
		int VipType = GetVipType(cId);

		if (VipType == FOGO_VIP)
			Percent += 4;
		else
			Percent += 8;
	}

	if (Percent < 0 || Percent > 100)
		Percent = 100;

	srand(time(NULL) / 5 * (rand() % 500) * 5);

	INT8 Chance = rand() % 100;

	char Message[260];

	for (INT8 i = 2; i < 6; i++)
		memset(&player->Inventory[p->Slot[i]], 0, sizeof(st_Item));

	if (Chance <= Percent)
	{
		memcpy(&player->Inventory[p->Slot[0]].EF1, &player->Inventory[p->Slot[1]].EF1, 6);

		SetItemSanc(&player->Inventory[p->Slot[0]], 7);

		memset(&player->Inventory[p->Slot[1]], 0x0, sizeof(st_Item));

		sprintf(Message, "%s obteve sucesso na composição do item %s. (%d/%d)", player->Name, itemlistBuffer[p->ItemId[0].Index].Name, Chance, Percent);
	
		SendEffect(cId, 100, (Rand() % 6));
		SendEffect(cId, 100, (Rand() % 6));
		SendEffect(cId, 100, (Rand() % 6));
	}
	else
	{
		memset(&player->Inventory[p->Slot[0]], 0, sizeof(st_Item));

		sprintf(Message, "Houve falha na composição do item.");
	}

	for (INT8 i = 0; i < 6; i++)
		SendItem(cId, INVENTORY, p->Slot[i], &player->Inventory[p->Slot[i]]);

	SendClientMessage(cId, Message);
	sendLog(INFO, cId, "%s %s %s", nBuffer[cId].Login, player->Name, Message);

	SendClientSignalParm(cId, 0x7530, 0x3A7, 2);
}

static const INT16 NotCharms[] =
{
	540, 541, 551, 552, 553, 554, 555, 556, 557, 558, 559,
	560, 561, 562, 563, 564, 565, 566, 567, 568, 569, 570,
	591, 592, 593, 594, 595, 633, 1738, 661, 662, 663
};

void Ailyn(BYTE *m_PacketBuffer, bool *pRetn)
{
	*pRetn = true;

	st_Window *p = (st_Window*)m_PacketBuffer;

	INT16 cId = p->Header.ClientId;

	st_Mob *player = GetMobFromIndex(p->Header.ClientId);

	if (cId < 0 || cId > MAX_PLAYER)
		return;

	for (INT8 i = 0; i < 7; i++)
	{
		if (p->Slot[i] == 0xFF)
			return;

		else if (memcmp(&p->ItemId[i], &player->Inventory[p->Slot[i]], sizeof st_Item))
			return;

		else if (p->ItemId[i].Index != player->Inventory[p->Slot[i]].Index)
			return;

		if (p->ItemId[i].Index < 0 || p->ItemId[i].Index > 6500)
			return;
	}

	for (INT8 i = 0; i < sizeof(NotCharms) / sizeof(INT16); i++)
	{
		if (player->Inventory[p->Slot[0]].Index == NotCharms[i])
		{
			SendClientMessage(cId, "Você não pode compor este item.");
			return;
		}
	}

	if (player->Gold < 50000000)
	{
		SendClientMessage(cId, "É necessário possuir 50 Milhões(50kk) de Gold.");
		return;
	}

	if (GetItemSanc(&player->Inventory[p->Slot[0]]) != 9 || GetItemSanc(&player->Inventory[p->Slot[1]]) != 9)
	{
		SendClientMessage(cId, "A refinação do item deve ser +9.");
		return;
	}

	if (p->ItemId[2].Index != 1774)
		return;

	for (INT8 i = 3; i < 7; i++)
	{
		if (p->ItemId[i].Index < 2441 || p->ItemId[i].Index > 2444)
			return;
	}

	INT16 AddItem = p->ItemId[3].Index - 2441;

	srand(time(NULL) / 5 * (rand() % 500) * 5);

	INT8 Random = rand() % 100, Chances = 40;

	if (isVip(cId))
	{
		int VipType = GetVipType(cId);

		if (VipType == FOGO_VIP)
			Chances += 4;
		else
			Chances += 8;
	}

	char Message[260];

	if (Random <= Chances)
	{
		if (rand() % 2)
		{
			SetItemSanc(&player->Inventory[p->Slot[0]], (230 + AddItem));

			SendItem(cId, INVENTORY, p->Slot[0], &player->Inventory[p->Slot[0]]);

			memset(&player->Inventory[p->Slot[1]], 0, sizeof(st_Item));

			SendItem(cId, INVENTORY, p->Slot[1], &player->Inventory[p->Slot[1]]);
		}
		else
		{
			SetItemSanc(&player->Inventory[p->Slot[1]], (230 + AddItem));

			SendItem(cId, INVENTORY, p->Slot[1], &player->Inventory[p->Slot[1]]);

			memset(&player->Inventory[p->Slot[0]], 0, sizeof(st_Item));

			SendItem(cId, INVENTORY, p->Slot[0], &player->Inventory[p->Slot[0]]);
		}

		sprintf(Message, "%s obteve sucesso na composição do item %s. (%d/%d)", player->Name, itemlistBuffer[player->Inventory[p->Slot[0]].Index].Name, Random, Chances);
	
		SendEffect(cId, 100, (Rand() % 6));
		SendEffect(cId, 100, (Rand() % 6));
		SendEffect(cId, 100, (Rand() % 6));
	}
	else
		sprintf(Message, "Houve falha na composição do item.");

	for (INT8 o = 2; o < 7; o++)
	{
		memset(&player->Inventory[p->Slot[o]], 0, sizeof(st_Item));

		SendItem(cId, INVENTORY, p->Slot[o], &player->Inventory[p->Slot[o]]);
	}

	player->Gold -= 50000000;

	SendEtc(cId);

	SendClientMessage(cId, Message);
	sendLog(INFO, cId, "%s %s %s", nBuffer[cId].Login, player->Name, Message);

	SendClientSignalParm(cId, 0x7530, 0x3A7, 2);
}
    
int AbencoadaChances[] = { 2, 3, 4, 5, 6, 7, 8, 9, 12, 15, 16, 17, 18, 19, 20, 21 };

void Odin(BYTE *m_PacketBuffer, bool *pRetn)
{
	*pRetn = true;

	st_Window *p = (st_Window*)m_PacketBuffer;

	INT16 cId = p->Header.ClientId;

	st_Mob *player = (st_Mob*)GetMobFromIndex(cId);

	if (cId <= 0 || cId >= MAX_PLAYER)
		return;

	for (INT8 i = 0; i < 7; i++)
	{
		if (p->Slot[i] == 0xFF)
			return;

		else if (memcmp(&p->ItemId[i], &player->Inventory[p->Slot[i]], sizeof st_Item))
			return;

		else if (p->ItemId[i].Index != player->Inventory[p->Slot[i]].Index)
			return;

		else if (p->ItemId[i].Index < 0 || p->ItemId[i].Index > 6500)
			return;
	}

	if (p->ItemId[0].Index == 4043 && p->ItemId[1].Index == 4043)
	{
		if ((p->ItemId[3].Index != 3338 && p->ItemId[3].Index) || (p->ItemId[4].Index != 3338 && p->ItemId[4].Index) ||
			(p->ItemId[5].Index != 3338 && p->ItemId[5].Index) || (p->ItemId[6].Index != 3338 && p->ItemId[6].Index))
		{
			SendClientMessage(cId, "Item inválido na composição.");

			return;
		}
			
		for (INT8 i = 0; i < sizeof(NotCharms) / sizeof(INT16); i++)
		{
			if (player->Inventory[p->Slot[2]].Index == NotCharms[i])
			{
				SendClientMessage(cId, "Algo está errado com a combinação.");
				return;
			}
		}

		INT16 RefDoItem = GetItemSanc_OL2(&p->ItemId[2]);

		if (RefDoItem <= 233 || RefDoItem >= 250)
		{
			SendClientMessage(cId, "A refinação do item deve estar entre +11 e +14.");
			return;
		}

		UINT8 chances = 0;
		chances += (AbencoadaChances[BASE_GetItemSanc(&p->ItemId[3])] + 2);
		chances += (AbencoadaChances[BASE_GetItemSanc(&p->ItemId[4])] + 2);
		chances += (AbencoadaChances[BASE_GetItemSanc(&p->ItemId[5])] + 2);
		chances += (AbencoadaChances[BASE_GetItemSanc(&p->ItemId[6])] + 2);
		chances += GetChanceRefOdin(&p->ItemId[2]);

		for (INT8 e = 3; e < 7; e++)
			memset(&player->Inventory[p->Slot[e]], 0, sizeof st_Item);

		memset(&player->Inventory[p->Slot[0]], 0, sizeof st_Item);
		memset(&player->Inventory[p->Slot[1]], 0, sizeof st_Item);

		if (isVip(cId))
			chances += 15;

		srand(time(NULL) / 5 * (rand() % 500) * 5);

		if ((rand() % 100) <= chances)
		{
			SendNotice(" !!! ");

			RefinarItemMais(&player->Inventory[p->Slot[2]], 4);
		}
		else
			RefinarItemMais(&player->Inventory[p->Slot[2]], -4);

		SendALL(cId);

		SendClientSignalParm(cId, 0x7530, 0x3A7, 2);
		return;
	}

	SendClientMessage(p->Header.ClientId, "Há algo de errado com sua composição.");
}

static const int Configs[][3] = {
	{ 2442, 2443, 2444 }, { 2442, 2441, 2443 }, { 2444, 2441, 2443 },
	{ 2443, 2442, 2444 }, { 2441, 2441, 2441 }, { 2442, 2442, 2442 },
	{ 2443, 2443, 2443 }, { 2444, 2444, 2444 }, { 2441, 2442, 2443 }, { 2441, 2443, 2444 }
};

void Ehre(BYTE *m_PacketBuffer, bool *pRetn)
{
	*pRetn = true;

	st_Window *p = (st_Window*)m_PacketBuffer;

	INT16 cId = p->Header.ClientId;

	st_Mob *player = (st_Mob*)GetMobFromIndex(cId);

	if (cId <= 0 || cId >= MAX_PLAYER)
		return;

	for (INT8 i = 0; i < 3; i++)
	{
		if (p->Slot[i] == 0xFF)
			return;

		else if (memcmp(&p->ItemId[i], &player->Inventory[p->Slot[i]], sizeof st_Item))
			return;

		else if (p->ItemId[i].Index != player->Inventory[p->Slot[i]].Index)
			return;

		else if (p->ItemId[i].Index < 0 || p->ItemId[i].Index > 6500)
			return;
	}

	for (INT8 i = 0; i < sizeof(Configs) / sizeof(INT16); i++)
	{
		if (Configs[i][0] == p->ItemId[0].Index && Configs[i][1] == p->ItemId[1].Index && Configs[i][2] == p->ItemId[2].Index && player->Equip[0].EFV2 != MORTAL)
		{
			nBuffer[cId].Chars[nBuffer[cId].Ingame.LoggedMob].SoulInfo = i;

			for (INT8 o = 0; o < 3; o++)
			{
				memset(&player->Inventory[p->Slot[o]], 0, sizeof st_Item);

				SendItem(cId, INVENTORY, p->Slot[o], &player->Inventory[p->Slot[o]]);
			}

			SendClientMessage(cId, "Composição concluída.");
			sendLog(INFO, cId, "%s %s configurou soul %d", nBuffer[cId].Login, player->Name, i);

			SaveAccount(cId);

			SendClientSignalParm(cId, 0x7530, 0x3A7, 2);
			return;
		}
	}

	if (p->ItemId[0].Index == 697 && p->ItemId[1].Index == 697)
	{
		if (player->Equip[0].EFV2 != DEUS)
		{
			SendClientMessage(cId, "Necessário ser um Deus.");

			return;
		}

		if (player->Inventory[p->Slot[2]].Index != 3338 || GetItemSanc(&p->ItemId[2]) >= 9)
		{
			SendClientMessage(cId, "Necessário uma Refinação Abençoada inferior a +9.");
			return;
		}

		else if (player->Exp < 10000000)
		{
			SendClientMessage(cId, "Necessário possuir 10.000.000 de experiência.");

			return;
		}

		INT8 chances = 0;
		if (player->Status.Level == 94)
			chances = 20;
		else if (player->Status.Level == 95)
			chances = 40;
		else if (player->Status.Level == 96)
			chances = 60;
		else if (player->Status.Level == 97)
			chances = 70;
		else if (player->Status.Level == 98)
			chances = 80;
		else
			chances = 85;

		if (isVip(cId))
			chances += 15;

		srand(time(NULL) / 5 * (rand() % 500) * 5);

		if ((rand() % 100) <= chances)
		{
			SendClientMessage(cId, "Obteve sucesso na refinação.");

			RefinarItemMais(&player->Inventory[p->Slot[2]], 1);
		}
		else
		{
			SendClientMessage(cId, "Sua refinação falhou.");

			if (GetItemSanc(&p->ItemId[2]) > 1)
				RefinarItemMais(&player->Inventory[p->Slot[2]], -1);
			else
				memset(&player->Inventory[p->Slot[2]], 0, sizeof st_Item);
		}

		for (INT8 j = 0; j < 2; j++)
			memset(&player->Inventory[p->Slot[j]], 0, sizeof st_Item);

		player->Exp -= 10000000;

		SendEtc(cId);
		SendALL(cId);

		SendClientSignalParm(cId, 0x7530, 0x3A7, 2);
	}

	SendClientMessage(cId, "Não pode fazer isso.");
}

void Tiny(BYTE *m_PacketBuffer, bool *pRetn)
{
	*pRetn = true;

	st_Window *p = (st_Window*)m_PacketBuffer;

	INT16 cId = p->Header.ClientId;

	if (cId <= 0 || cId >= MAX_PLAYER)
		return;

	st_Mob *player = (st_Mob*)GetMobFromIndex(cId);

	for (INT8 i = 0; i < 3; i++)
	{
		if (p->Slot[i] == 0xFF)
			return;

		else if (memcmp(&p->ItemId[i], &player->Inventory[p->Slot[i]], sizeof st_Item))
			return;

		else if (p->ItemId[i].Index != player->Inventory[p->Slot[i]].Index)
			return;

		else if (p->ItemId[i].Index < 0 || p->ItemId[i].Index > 6500)
			return;
	}

	INT16 SancSlot1 = GetItemSanc(&p->ItemId[0]), SancSlot2 = GetItemSanc(&p->ItemId[1]);

	if (SancSlot1 < 9 || SancSlot1 > 253 || SancSlot2 < 9 || SancSlot2 > 253)
	{
		SendClientMessage(cId, "O item deve ter a refinação acima de +9.");
		return;
	}

	if (player->Gold < 100000000)
	{
		SendClientMessage(cId, "Você precisa de 100 Milhões(100kk) de Gold.");
		return;
	}

	INT8 Percent = 0;
	if (SancSlot2 >= 230 && SancSlot2 <= 233)
		Percent += 25;
	else if (SancSlot2 >= 234 && SancSlot2 <= 237)
		Percent += 30;
	else if (SancSlot2 >= 238 && SancSlot2 <= 241)
		Percent += 35;
	else if (SancSlot2 >= 242 && SancSlot2 <= 245)
		Percent += 40;
	else if (SancSlot2 >= 246 && SancSlot2 <= 249)
		Percent += 45;
	else if (SancSlot2 >= 250 && SancSlot2 <= 253)
		Percent += 60;
	else
		Percent += 20;

	if (isVip(cId))
		Percent += 15;

	if (Percent < 0 || Percent > 100)
		Percent = 100;

	srand(time(NULL) / 5 * (rand() % 500) * 5);

	INT8 Chance = rand() % 100;

	char Message[260];

	INT8 size = 0;

	if (Chance <= Percent)
	{
		memcpy(&player->Inventory[p->Slot[0]].EF1, &player->Inventory[p->Slot[1]].EF1, 6);

		SetItemSanc(&player->Inventory[p->Slot[0]], 7);

		SendItem(cId, INVENTORY, p->Slot[0], &player->Inventory[p->Slot[0]]);

		size = 1;

		sprintf(Message, "%s obteve sucesso na composição do item %s. (%d/%d)", player->Name, itemlistBuffer[p->ItemId[0].Index].Name, Chance, Percent);
	}
	else
		sprintf(Message, "Houve falha na composição do item.");

	for (INT8 i = size; i < 3; i++)
	{
		memset(&player->Inventory[p->Slot[i]], 0, sizeof(st_Item));

		SendItem(cId, INVENTORY, p->Slot[i], &player->Inventory[p->Slot[i]]);
	}

	SendClientMessage(cId, Message);

	player->Gold -= 100000000;

	SendEtc(cId);

	SendClientSignalParm(cId, 0x7530, 0x3A7, 2);
}