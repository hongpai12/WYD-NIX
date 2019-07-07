#include "DLLMain.h"

void __stdcall NKD_SystemTrade(BYTE *m_PacketBuffer, int conn)
{
	NKD_SystemTradeFunc(m_PacketBuffer, conn);

	ZeroMemory(m_PacketBuffer, 156);
}

void NKD_SystemTradeFunc(BYTE *m_PacketBuffer, int ClientID)
{
	SendTrade *p = (SendTrade*)m_PacketBuffer;

	int OtherClientid = p->OtherClientid;

	if (ClientID < 0 || OtherClientid < 0 || OtherClientid > 750 || ClientID > 750)
		return;

	if (Users[ClientID].Status != 22)
		return;
	else if (Users[OtherClientid].Status != 22)
	{
		SendClientMessage(ClientID, "Jogador indisponível para a troca.");

		return;
	}

	if (!Trade[OtherClientid].isTrading && !Trade[ClientID].isTrading)
	{
		if (Users[ClientID].AllStatus.PK)
		{
			SendClientMessage(ClientID, "Você está em modo PK.");
			
			return;
		}
		else if (Users[OtherClientid].AllStatus.PK)
		{
			SendClientMessage(ClientID, "O outro usuário está em modo PK.");

			return;
		}
		// Verifica se nenhum dos dois jogadores está com trade
		// Se os dois não estiverem, quer dizer que foi enviado um pedido para Trade
	
		OpenTrade(ClientID, p->OtherClientid);

		Trade[ClientID].WaitingData = true;
		Trade[ClientID].Timer = clock();
		Trade[ClientID].isTrading = true;
		Trade[ClientID].ClientID = OtherClientid;
		Trade[ClientID].Confirm = false;

		Trade[OtherClientid].Timer = clock();
		Trade[OtherClientid].isTrading = true;
		Trade[OtherClientid].ClientID = ClientID;
		Trade[OtherClientid].Confirm = false;
	}
	else if (Trade[OtherClientid].isTrading && Trade[OtherClientid].ClientID != ClientID)
	{
		// Se você está sem o modo trade e o usuário que foi pedido estiver em negociação
		// a mensagem abaixo é mostrada
		SendClientMessage(ClientID, "O outro jogador já está em negociação!");

		return;
	}
	else
	{ // Caso não seja um request, será feito as verificações para envio de item/confirm etc.
		if (Trade[ClientID].isTrading && Trade[ClientID].ClientID != p->OtherClientid)
		{
			// Para evitar qualquer tipo de bug se o jogador já está em troca
			// o trade do mesmo é fechado
			SendClientMessage(ClientID, "Você já está em modo troca!");
			CloseTrade(ClientID);

			return;
		}
		else if (Trade[OtherClientid].isTrading && !Trade[ClientID].isTrading && Trade[OtherClientid].WaitingData)
		{
			// Checa se o usuário que foi pedido o trade está sem trade novamente e se
			// o usuário que enviou o trade está realmente esperando o trade
			Trade[ClientID].isTrading = true;
			Trade[ClientID].WaitingData = true;
			Trade[ClientID].Confirm = false;
			Trade[ClientID].Timer = clock();
			Trade[ClientID].ClientID = OtherClientid;
			OpenTrade(ClientID, OtherClientid);
		}
		else if (p->Confirma)
		{
			if (clock() - Trade[ClientID].Timer < 2 * CLOCKS_PER_SEC)
			{
				// O próprio cliente existe proteção, mas como o packet pode ser manipulado e/ou enviado
				// pelo próprio usuário como forma de hack, é feito a proteção no servidor também
				// Caso queira, coloque um CloseTrade ao final dessa condição porque o client provavelmente não se enganará
				SendClientMessage(ClientID, "Aguarde 2 segundos e confirme.");
				Trade[ClientID].Timer = clock();
			}
			else
			{
				Trade[ClientID].Confirm = p->Confirma;
				if (Trade[OtherClientid].Confirm)
				{
					int amount_TradeOID = GetAmountItemOnTrade(OtherClientid);
					int amount_TradeCID = GetAmountItemOnTrade(ClientID);
					if (GetAmountItem(ClientID) < amount_TradeOID)
					{
						// Checa se a quantidade de itens nulos no inventário do usuário é igual ou maior
						// que a quantidade de itens colocadas no trade
						SendClientMessage(ClientID, "Você não possui espaço no inventário");
						SendClientMessage(OtherClientid, "O outro jogador não possui espaço no inventário");
						CloseTrade(ClientID);

						return;
					}
					else if (GetAmountItem(OtherClientid) < amount_TradeCID)
					{
						// Checa se a quantidade de itens nulos no inventário do usuário é igual ou maior
						// que a quantidade de itens colocadas no trade
						SendClientMessage(OtherClientid, "Você não possui espaço no inventário");
						SendClientMessage(ClientID, "O outro jogador não possui espaço no inventário");
						CloseTrade(ClientID);

						return;
					}
					else
					{
						st_Mob *player = GetMobFromIndex(ClientID);
						st_Mob *mob = GetMobFromIndex(OtherClientid);

						if (Trade[ClientID].Gold < 0 || Trade[ClientID].Gold > 2000000000)
						{
							CloseTrade(ClientID);
							CloseTrade(OtherClientid);

							return;
						}

						if (Trade[OtherClientid].Gold < 0 || Trade[OtherClientid].Gold > 2000000000)
						{
							CloseTrade(ClientID);
							CloseTrade(OtherClientid);

							return;
						}

						int calcGold = (player->Gold - Trade[ClientID].Gold) + Trade[OtherClientid].Gold;

						if (calcGold > 2000000000 || calcGold < 0)
						{
							SendClientMessage(ClientID, "Limite máximo de 2bilhões de gold no invetário!");
							SendClientMessage(OtherClientid, "O outro jogador não possui espaço para guardar gold.");

							CloseTrade(ClientID);
							CloseTrade(OtherClientid);

							return;
						}

						int calcOpponentGold = (mob->Gold - Trade[OtherClientid].Gold) + Trade[ClientID].Gold;

						if (calcOpponentGold > 2000000000 || calcOpponentGold < 0)
						{
							SendClientMessage(OtherClientid, "Limite máximo de 2bilhões de gold no invetário!");
							SendClientMessage(ClientID, "O outro jogador não possui espaço para guardar gold.");

							CloseTrade(ClientID);
							CloseTrade(OtherClientid);

							return;
						}

						if (Trade[ClientID].Gold > player->Gold)
						{
							SendClientMessage(ClientID, "Você não possui esse gold.");
							SendClientMessage(OtherClientid, "O outro jogador não possui a quantia de gold.");

							CloseTrade(ClientID);
							CloseTrade(OtherClientid);

							return;
						}

						if (Trade[OtherClientid].Gold > mob->Gold)
						{
							SendClientMessage(OtherClientid, "Você não possui esse gold.");
							SendClientMessage(ClientID, "O outro jogador não possui a quantia de gold.");

							CloseTrade(ClientID);
							CloseTrade(OtherClientid);

							return;
						}

						player->Gold = calcGold;
						mob->Gold = calcOpponentGold;

						SendCarry(ClientID);
						SendCarry(OtherClientid);

						int SlotID = 0;
						int SlotOID = 0;

						for (UINT8 i = 0; i<15; i++)
						{
							// Faz o loop para todos os itens do trade
							// Slot livre do inventário
							if (Trade[ClientID].Itens[i].Index)
								SlotID = GetFirstSlot(OtherClientid, 0, INVENTORY);
							// Slot livre do inventário
							if (Trade[OtherClientid].Itens[i].Index)
								SlotOID = GetFirstSlot(ClientID, 0, INVENTORY);
							// Checa se os valores retornados são válidos
							// Se retornar -1 quer dizer que a funçaõ anterior (GetAmountItem) aconteceu algum erro
							if (SlotID == -1 || SlotOID == -1)
							{
								// O erro aconteceu então a mensagem é enviada aos dois players e o trade dos mesmos é fechado
								SendClientMessage(ClientID, "Ocorreu um erro!");
								SendClientMessage(OtherClientid, "Ocorreu um erro!");
								CloseTrade(ClientID);
							}
							// Checa se existe algum item no slot correspondente do trade
							if (Trade[ClientID].Itens[i].Index)
							{
								// Proteção contra criação de itens
								if (memcmp(&Trade[ClientID].Itens[i], &player->Inventory[Trade[ClientID].TradeItemSlot[i]], sizeof st_Item))
								{
									CloseTrade(ClientID);
									CloseTrade(OtherClientid);

									SendClientMessage(ClientID, "Ocorreu um erro!");
									SendClientMessage(OtherClientid, "Ocorreu um erro!");
								}
								else
								{
									// Copia o item que está no trade do outro usuário
									memcpy(&mob->Inventory[SlotID], &Trade[ClientID].Itens[i], sizeof st_Item);
									// Apaga o item do inventário do outro usuário
									memset(&player->Inventory[Trade[ClientID].TradeItemSlot[i]], 0, sizeof st_Item);
									// Atualiza o slot do personagem em que o item foi apagado
									SendItem(ClientID, INVENTORY, Trade[ClientID].TradeItemSlot[i], &player->Inventory[Trade[ClientID].TradeItemSlot[i]]);
									SendItem(OtherClientid, INVENTORY, SlotID, &mob->Inventory[SlotID]);

									memset(&Trade[ClientID].Itens[i], 0, sizeof st_Item);
								}
							}
							// Checa se existe algum item no slot correspondente do trade
							if (Trade[OtherClientid].Itens[i].Index)
							{
								// Proteção contra criação de itens
								if (memcmp(&Trade[OtherClientid].Itens[i], &mob->Inventory[Trade[OtherClientid].TradeItemSlot[i]], sizeof st_Item))
								{
									CloseTrade(ClientID);
									CloseTrade(OtherClientid);
									SendClientMessage(ClientID, "Ocorreu um erro!");
									SendClientMessage(OtherClientid, "Ocorreu um erro!");
								}
								else
								{
									// Copia o item que está no trade do outro usuário
									memcpy(&player->Inventory[SlotOID], &Trade[OtherClientid].Itens[i], sizeof st_Item);
									// Apaga o item do inventário do outro usuário
									memset(&mob->Inventory[Trade[OtherClientid].TradeItemSlot[i]], 0, sizeof st_Item);
									// Atualiza o slot do personagem em que o item foi apagado
									SendItem(p->OtherClientid, INVENTORY, Trade[OtherClientid].TradeItemSlot[i], &mob->Inventory[Trade[OtherClientid].TradeItemSlot[i]]);
									SendItem(ClientID, INVENTORY, SlotOID, &player->Inventory[SlotOID]);

									memset(&Trade[OtherClientid].Itens[i], 0, sizeof st_Item);
								}
							}
						}

						CloseTrade(ClientID);
					}
				}
				else
					// Se o outro usuário não confirmou ainda o trade, apenas é enviado para o outro usuário
					// o botão de confirmação juntamente com os itens/gold do trade
					SendAllTrade(ClientID, OtherClientid);
			}
		}
		else
		{
			// Aqui o usuário apenas colocou os itens no slot
			if (!AddItensOnTrade(p))  // Aconteceu algum erro ao adicionar os itens ao trade
			{
				CloseTrade(ClientID);
				CloseTrade(OtherClientid);
			}
			else
			{
				// Não aconteceu erro, então é setado tudo normalmente
				Trade[ClientID].Gold = p->Gold;
				Trade[ClientID].Confirm = false;
				Trade[Trade[ClientID].ClientID].Confirm = false;
				Trade[ClientID].Timer = clock();
				Trade[ClientID].Confirm = false;

				SendAllTrade(ClientID, OtherClientid);
			}
		}
	}

	ZeroMemory(m_PacketBuffer, 156);
}

