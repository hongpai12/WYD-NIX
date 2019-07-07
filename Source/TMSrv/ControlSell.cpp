#include "PacketControl.h"

void Control37A(BYTE *m_PacketBuffer, bool *pRetn)
{
	p37A *p = (p37A*)m_PacketBuffer;

	if (p->Header.ClientId < 0 || p->Header.ClientId > 750)
		return;

	INT16 cId = p->Header.ClientId;

	st_Mob *player = GetMobFromIndex(cId);
	CUser *user = GetUserFromIndex(cId);

	if (p->type != INVENTORY)
	{
		*pRetn = true;

		sendLog(ERROR, cId, "%s %s enviou 37A de fora do inventário.", user->Username, player->Name);

		return;
	}

	st_Item *SellItem = (st_Item*)GetItemPointer(player, user->Storage.Item, p->type, p->sellSlot);

	if (SellItem == NULL)
	{
		*pRetn = true;

		return;
	}

	for (INT8 i = 0; i < sizeof(NotSell) / sizeof(INT16); i++)
	{
		if (SellItem->Index == NotSell[i])
		{
			*pRetn = true;

			SendClientMessage(cId, "Este item não pode ser vendido.");

			sendLog(WARN, cId, "%s %s tentou vender o item %s.", user->Username, player->Name, itemlistBuffer[SellItem->Index].Name);

			return;
		}
	}

	sendLog(INFO, cId, "%s %s vendeu o item %s.", user->Username, player->Name, itemlistBuffer[SellItem->Index].Name);
}

void Control398(BYTE *m_PacketBuffer, bool *pRetn)
{
	p398 *p = (p398*)m_PacketBuffer;

	if (p->mobId < 1 || p->mobId > MAX_PLAYER)
		return;

	else if (p->Header.ClientId < 1 || p->Header.ClientId > MAX_PLAYER)
		return;

	else if (p->Slot < 0 || p->Slot > 12)
		return;

	INT16 cId = p->Header.ClientId;

	st_Mob *buyer = GetMobFromIndex(cId),
		   *seller = GetMobFromIndex(p->mobId);

	CUser *user = GetUserFromIndex(cId),
		  *bUser = GetUserFromIndex(p->mobId);


	time_t rawnow = time(NULL);
	struct tm *now = localtime(&rawnow);

	char szMsg[256];

	for (INT8 i = 0; i < sizeof(NotSell) / sizeof(INT16); i++)
	{
		if (user->AutoTrade.Item[p->Slot].Index == NotSell[i])
		{
			*pRetn = true;

			SendClientMessage(cId, "Impossível negociar este item.");

			sendLog(WARN, cId, "%s %s tentou negociar o item %s", user->Username, buyer->Name, itemlistBuffer[NotSell[i]].Name);

			return;
		}
	}

	// Bloqueia o usuário ter mais que 2kkk no baú na loja.
	if (bUser->AutoTrade.Price[p->Slot] <= 100)
	{
		*pRetn = true;

		SendClientMessage(cId, "Este item não está disponível para venda.");

		return;
	}

	else if ((bUser->Storage.Gold + bUser->AutoTrade.Price[p->Slot]) > 2000000000)
	{
		*pRetn = true;

		SendClientMessage(cId, "Não é possível carregar mais que 2 bilhões de gold.");

		return;
	}

	sprintf_s(szMsg, "![%s] comprou [%s] por [%d] às [%d:%d:%d] de [%d/%d/%d]", buyer->Name, itemlistBuffer[p->Item.Index].Name, bUser->AutoTrade.Price[p->Slot], now->tm_hour, now->tm_min, now->tm_sec, now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);

	SendClientMessage(p->mobId, szMsg);

	sendLog(INFO, cId, szMsg);

	int CityId = GetCity(cId);
	if (CityId >= 0 && CityId <= 3)
	{
		INT32 taxa = (bUser->AutoTrade.Price[p->Slot] / 100) * Servidor.TaxaImposto[CityId];
		if (taxa > 0)
			Servidor.ImpostoAcumulado[CityId] += taxa;
	}
}