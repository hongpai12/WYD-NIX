#include "DLLMain.h"

void FixBuyItem_SERVER(BYTE *m_PacketBuffer, bool *pRetn)
{
	*pRetn = true;

	p379 *p = (p379*)m_PacketBuffer;

	INT16 cId = p->Header.ClientId;

	if(cId < 0 || cId > MAX_PLAYER)
		return;

	else if(p->mobID > 30000 || p->mobID < MAX_PLAYER)
		return;

	else if(p->sellSlot < 0 || p->sellSlot > 64)
		return;

	st_Mob *Npc    = GetMobFromIndex(p->mobID),
		   *player = GetMobFromIndex(cId);

	st_Item *item = &Npc->Inventory[p->sellSlot];

#pragma region COMPOSICOES
	if (!strcmp(Npc->Name, "Jeffi"))
	{
#pragma region POEIRA_LAC
		if (item->Index == 5700)
		{
			if (player->Gold < 2000000)
			{
				SendClientMsg(cId, "São necessários 2.000.000 (Dois milhões) de gold para a composição.");

				return;
			}

#pragma region FIND_RESTOS
			int AmountCount = 0;

			for (int i = 0; i < 60; i++)
			{
				if (player->Inventory[i].Index == 420)
					AmountCount += GetItemAmount(&player->Inventory[i]);
			}

			if (AmountCount < 10)
			{
				SendClientMessage(cId, "Traga-me 10 restos de Lac para a composição.");

				return;
			}

			for (int i = 0; i < 10; i++)
			{
				int slot = GetFirstSlot(cId, 420, INVENTORY);

				AmountMinus(&player->Inventory[slot]);
				SendItem(cId, INVENTORY, slot, &player->Inventory[slot]);
			}
#pragma endregion
			
			st_Item Item;
			memset(&Item, 0, sizeof st_Item);

			Item.Index = 413;

			GiveItem(cId, &Item);

			player->Gold -= 2000000;
			SendEtc(cId);

			sendLog(INFO, cId, "%s %s compôs Poeira de Lac.", nBuffer[cId].Login, player->Name);

			return;
		}
#pragma endregion
#pragma region POEIRA_ORI
		else if (item->Index == 5701)
		{
			if (player->Gold < 1000000)
			{
				SendClientMsg(cId, "São necessários 1.000.000 (Um milhão) de gold para a composição.");

				return;
			}

#pragma region FIND_RESTOS
			int AmountCount = 0;

			for (int i = 0; i < 60; i++)
			{
				if (player->Inventory[i].Index == 419)
					AmountCount += GetItemAmount(&player->Inventory[i]);
			}

			if (AmountCount < 10)
			{
				SendClientMessage(cId, "Traga-me 10 restos de Ori para a composição.");

				return;
			}

			for (int i = 0; i < 10; i++)
			{
				int slot = GetFirstSlot(cId, 419, INVENTORY);

				AmountMinus(&player->Inventory[slot]);
				SendItem(cId, INVENTORY, slot, &player->Inventory[slot]);
			}
#pragma endregion

			st_Item Item;
			memset(&Item, 0, sizeof st_Item);
            
			Item.Index = 412;

			GiveItem(cId, &Item);

			player->Gold -= 1000000;
			SendEtc(cId);

			sendLog(INFO, cId, "%s %s compôs Poeira de Ori.", nBuffer[cId].Login, player->Name);

			return;
		}
#pragma endregion
#pragma region LANHOUSES
		else if (item->Index >= 5702 && item->Index <= 5704)
		{
			int IndexToDeliver = item->Index - 1591,
				TotalToBring   = (IndexToDeliver - 4110) * 5;

			long int RequestedGold = (IndexToDeliver - 4110) * 250000;

			if (player->Gold < RequestedGold)
			{
				SendClientMsg(cId, "Gold insuficiente para a composição.");

				return;
			}

#pragma region FIND_SAFIRAS
			int AmountCount = 0;

			for (int i = 0; i < 60; i++)
			{
				if (player->Inventory[i].Index == 697)
					AmountCount += GetItemAmount(&player->Inventory[i]);
			}

			if (AmountCount < TotalToBring)
			{
				SendClientMsg(cId, "Traga-me %d safiras para a composição.", TotalToBring);

				return;
			}

			for (int i = 0; i < TotalToBring; i++)
			{
				int slot = GetFirstSlot(cId, 697, INVENTORY);

				AmountMinus(&player->Inventory[slot]);
				SendItem(cId, INVENTORY, slot, &player->Inventory[slot]);
			}
#pragma endregion

			st_Item Item;
			memset(&Item, 0, sizeof st_Item);
			Item.Index = IndexToDeliver;
			Item.EF1 = EF_AMOUNT;
			Item.EFV1 = 5;

			GiveItem(cId, &Item);

			player->Gold -= RequestedGold;
			SendEtc(cId);

			sendLog(INFO, cId, "%s %s compôs entrada lanhouse.", nBuffer[cId].Login, player->Name);
			return;
		}
#pragma endregion
#pragma region PESA_N

#pragma endregion
#pragma region IDEAL
		else if (item->Index == 5706)
		{
			INT16 Agua = GetFirstSlot(cId, 5334, INVENTORY),
				  Terra = GetFirstSlot(cId, 5335, INVENTORY),
				  Sol = GetFirstSlot(cId, 5336, INVENTORY),
				  Vento = GetFirstSlot(cId, 5337, INVENTORY);

			if (Agua == -1 || Terra == -1 || Sol == -1 || Vento == -1)
			{
				SendClientMessage(cId, "Traga-me as quatro pedras secretas para a composição.");

				return;
			}
			else if (player->Equip[10].Index != 1742)
			{
				SendClientMessage(cId, "Favor equipar a Imortalidade para a composição.");

				return;
			}
			else if (player->Equip[11].Index < 1760 || player->Equip[11].Index > 1763)
			{
				SendClientMessage(cId, "Traga-me um Sephirot para a composição.");

				return;
			}

			AmountMinus(&player->Equip[10]);
			AmountMinus(&player->Equip[11]);
			AmountMinus(&player->Inventory[Agua]);
			AmountMinus(&player->Inventory[Terra]);
			AmountMinus(&player->Inventory[Sol]);
			AmountMinus(&player->Inventory[Vento]);

			SendItem(cId, EQUIP, 10, &player->Equip[10]);
			SendItem(cId, EQUIP, 11, &player->Equip[11]);
			SendItem(cId, INVENTORY, Agua,  &player->Inventory[Agua]);
			SendItem(cId, INVENTORY, Terra, &player->Inventory[Terra]);
			SendItem(cId, INVENTORY, Sol,   &player->Inventory[Sol]);
			SendItem(cId, INVENTORY, Vento, &player->Inventory[Vento]);

			st_Item Item;
			memset(&Item, 0, sizeof st_Item);

			Item.Index = 5338;
			GiveItem(cId, &Item);

			SendClientMessage(cId, "Composição concluida com sucesso!");

			sendLog(INFO, cId, "%s %s compôs ideal.", nBuffer[cId].Login, player->Name);
			return;
		}
		else if (item->Index == 3194 || item->Index == 3195)
		{
			player->Equip[15].Index = item->Index;

			if (player->Equip[0].EFV2 == DEUS)
				player->Equip[15].Index += 3;

			SendItem(cId, EQUIP, 15, &player->Equip[15]);
			SendClientMessage(cId, "Seja bem vindo a nosso reino.");

			return;
		}
#pragma endregion
#pragma region ESPIRITUAL_ABS_OR_PERFU
		else if (item->Index == 5709)
		{
			if (player->Gold < 15000000)
			{
				SendClientMessage(cId, "São necessários 15.000.000 de gold para a composição.");

				return;
			}

#pragma region FIND_SELOS
			int AmountCount = 0;

			for (int i = 0; i < 60; i++)
			{
				if (player->Inventory[i].Index == 5135)
					AmountCount += GetItemAmount(&player->Inventory[i]);
			}

			if (AmountCount < 5)
			{
				SendClientMessage(cId, "Traga-me 5 Selos Mágicos para a composição.");

				return;
			}

			for (int i = 0; i < 5; i++)
			{
				int slot = GetFirstSlot(cId, 5135, INVENTORY);

				AmountMinus(&player->Inventory[slot]);
				SendItem(cId, INVENTORY, slot, &player->Inventory[slot]);
			}
#pragma endregion

			player->Gold -= 15000000;
			SendEtc(cId);

			st_Item Item;
			memset(&Item, 0, sizeof st_Item);

			srand(time(NULL) / 5 * (rand() % 500) * 5);
			Item.Index = 631 + rand() % 2;

			GiveItem(cId, &Item);

			SendClientMessage(cId, "Item composto com sucesso.");

			sendLog(INFO, cId, "%s %s compôs abs || perfu simples", nBuffer[cId].Login, player->Name);
			return;
		}
#pragma endregion
#pragma region ESPIRITUAL_ABS_PERFU
		else if (item->Index == 5710)
		{
			if (player->Gold < 30000000)
			{
				SendClientMessage(cId, "São necessários 30.000.000 de gold para a composição.");

				return;
			}

#pragma region FIND_SELOS
			int AmountCount = 0;

			for (int i = 0; i < 60; i++)
			{
				if (player->Inventory[i].Index == 5135)
					AmountCount += GetItemAmount(&player->Inventory[i]);
			}

			if (AmountCount < 5)
			{
				SendClientMessage(cId, "Traga-me 5 Selos Mágicos para a composição.");

				return;
			}
#pragma endregion
#pragma region GET_ESPIRITUAL
			int abs   = FindFirstItembyRefination(cId, 631, 9),
				perfu = FindFirstItembyRefination(cId, 632, 9);

			if (abs != -1)
			{
				AmountMinus(&player->Inventory[abs]);
				SendItem(cId, INVENTORY, perfu, &player->Inventory[abs]);
			}
			else if (perfu != -1)
			{
				AmountMinus(&player->Inventory[perfu]);
				SendItem(cId, INVENTORY, perfu, &player->Inventory[perfu]);
			}
			else
			{
				SendClientMessage(cId, "Traga-me uma Pedra Espiritual + 9 para a composição.");

				return;
			}
#pragma endregion
#pragma region REMOVE_SELOS
			for (int i = 0; i < 5; i++)
			{
				int slot = GetFirstSlot(cId, 5135, INVENTORY);

				AmountMinus(&player->Inventory[slot]);
				SendItem(cId, INVENTORY, slot, &player->Inventory[slot]);
			}
#pragma endregion

			player->Gold -= 30000000;
			SendEtc(cId);

			st_Item Item;
			memset(&Item, 0, sizeof st_Item);
			Item.Index = 633;

			GiveItem(cId, &Item);
			
			SendClientMessage(cId, "Composição concluída com sucesso.");

			sendLog(INFO, cId, "%s %s compôs abs/perfu dupla.", nBuffer[cId].Login, player->Name);
			return;
		}
#pragma endregion
#pragma region CIRCULO_DIVINO
		else if (item->Index == 5707)
		{
			int Slot = GetFirstSlot(cId, 447, INVENTORY);

			if (Slot == -1)
			{
				SendClientMsg(cId, "Traga-me um %s", itemlistBuffer[447].Name);

				return;
			}

			else if (player->Gold < 1000000)
			{
				SendClientMessage(cId, "Traga-me 1.000.000 de gold para a composição.");

				return;
			}

			player->Gold -= 1000000;
			SendEtc(cId);

			memset(&player->Inventory[Slot].Effect, 0, 6);
			player->Inventory[Slot].Index += (rand() % 3) + 1;
			SendItem(cId, INVENTORY, Slot, &player->Inventory[Slot]);
		}
		else if (item->Index == 5708)
		{
			int Slot = GetFirstSlot(cId, 692, INVENTORY);

			if (Slot == -1)
			{
				SendClientMsg(cId, "Traga-me um %s", itemlistBuffer[692].Name);

				return;
			}

			else if (player->Gold < 5000000)
			{
				SendClientMessage(cId, "Traga-me 5.000.000 de gold para a composição.");

				return;
			}

			player->Gold -= 5000000;
			SendEtc(cId);

			memset(&player->Inventory[Slot].Effect, 0, 6);
			player->Inventory[Slot].Index += (rand() % 3) + 1;
			SendItem(cId, INVENTORY, Slot, &player->Inventory[Slot]);
		}
#pragma endregion
	}
#pragma endregion
#pragma region DONATE
	else if(Npc->Status.Level == 987 && Npc->Status.Points[DEXTE] >= 600)
	{
		// Função do Donate		
		for(int i = 0; i < sizeof(DonateList) / sizeof(Donate); i++)
		{
			if(!DonateList[i].Item.Index)
				continue;

			// memcmp retorna 0 quando os dois tipos comparados são iguais
			// quando o retorno é diferente de 0 quer dizer que não são totalmente compatíveis
			// se o retorno é diferente de 0, então é true
			if(memcmp(item, &DonateList[i].Item, sizeof(st_Item)))
				continue;

			char tmp[108];

			if(DonateList[i].Store <= 0)
			{
				SendClientMessage(cId, "Não há unidades disponíveis deste item para a venda.");

				sendLog(ERROR, cId, "%s %s tentou comprar item %s sem unidades disponíveis no donate.", nBuffer[cId].Login, player->Name, itemlistBuffer[DonateList[i].Item.Index].Name);

				return;
			}

			else if(nBuffer[cId].Ingame.LastClicked != item->Index)
			{
				// Confirmação de compra do item, para evitar aquilo de "Comprei sem querer"
				// Dessa forma é necessário clicar duas vezes para comprar o item
				// Posteriormente tentar adicionar clock junto a essa info
				// Para colocar para sumir 1 minuto depois a confirmação
				sprintf(tmp, "Deseja adquirir [%s] por [%d] NP's? %d unidades disponíveis.", itemlistBuffer[item->Index].Name, DonateList[i].Price, DonateList[i].Store);

				SendClientMessage(cId, tmp);

				nBuffer[cId].Ingame.LastClicked = item->Index;

				return;
			}

			nBuffer[cId].Ingame.LastClicked = 0;

			if (DonateList[i].Price > nBuffer[cId].Donate)
			{
				SendClientMessage(cId, "Não possui NP's suficientes.");

				sendLog(ERROR, cId, "%s %s tentou comprar item %s sem NPs suficientes.", nBuffer[cId].Login, player->Name, itemlistBuffer[DonateList[i].Item.Index].Name);

				return;
			}

			int freeSlot = -1;

			for(int e = 0; e < sizeof(Users[0].Storage.Item) / sizeof(st_Item); e++)
			{
				// Procura por slot vazio no Baú

				if(Users[cId].Storage.Item[e].Index)
					continue;
				else
				{
					freeSlot = e;

					break;
				}
			}

			if(freeSlot == -1 || freeSlot < 0 || freeSlot > 127)
			{
				SendClientMessage(cId, "Não possui espaço suficiente no cargo.");

				sendLog(ERROR, cId, "%s %s tentou comprar item %s sem espaço suficiente.",  nBuffer[cId].Login, player->Name, itemlistBuffer[DonateList[i].Item.Index].Name);

				return;
			}
			
			nBuffer[cId].Donate -= DonateList[i].Price;

			SaveAccount(cId);

			DonateList[i].Store--;

			memcpy(&Users[p->Header.ClientId].Storage.Item[freeSlot], &DonateList[i].Item, sizeof(st_Item));

			sprintf(tmp, "Item [%s] adquirido por [%d] Nix Points.", itemlistBuffer[item->Index].Name, DonateList[i].Price);

			SendClientMessage(cId, tmp);

			SendItem(cId, CARGO, freeSlot, &DonateList[i].Item);
			sendLog(INFO, cId, "%s %s %s", nBuffer[cId].Login, player->Name, tmp);

			return;
		}
	}
#pragma endregion
#pragma region COMPRA
	else
	{
		if(player->Gold < itemlistBuffer[item->Index].Price)
		{
			SendClientMessage(cId, "Gold insuficiente.");

			return;
		}

		// Tenta adicionar nos inventários
		if(GiveItem(cId, item) == -1)
		{
			SendClientMessage(cId, "Espaço insuficiente.");

			return;
		}

		player->Gold -= itemlistBuffer[item->Index].Price;

		SendEtc(cId);

		sendLog(INFO, cId, "%s %s Item %s adquirido por %d no npc %s.", nBuffer[cId].Login, player->Name, itemlistBuffer[item->Index].Name, itemlistBuffer[item->Index].Price, Npc->Name);

		int CityId = GetCity(cId);
		if (CityId >= 0 && CityId <= 3)
		{
			INT32 taxa = (itemlistBuffer[item->Index].Price / 100) * Servidor.TaxaImposto[CityId];
			if (taxa > 0)
				Servidor.ImpostoAcumulado[CityId] += taxa;
		}
	}
#pragma endregion
}