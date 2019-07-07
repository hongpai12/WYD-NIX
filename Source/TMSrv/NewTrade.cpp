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
		SendClientMessage(ClientID, "Jogador indispon�vel para a troca.");

		return;
	}

	if (!Trade[OtherClientid].isTrading && !Trade[ClientID].isTrading)
	{
		if (Users[ClientID].AllStatus.PK)
		{
			SendClientMessage(ClientID, "Voc� est� em modo PK.");
			
			return;
		}
		else if (Users[OtherClientid].AllStatus.PK)
		{
			SendClientMessage(ClientID, "O outro usu�rio est� em modo PK.");

			return;
		}
		// Verifica se nenhum dos dois jogadores est� com trade
		// Se os dois n�o estiverem, quer dizer que foi enviado um pedido para Trade
	
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
		// Se voc� est� sem o modo trade e o usu�rio que foi pedido estiver em negocia��o
		// a mensagem abaixo � mostrada
		SendClientMessage(ClientID, "O outro jogador j� est� em negocia��o!");

		return;
	}
	else
	{ // Caso n�o seja um request, ser� feito as verifica��es para envio de item/confirm etc.
		if (Trade[ClientID].isTrading && Trade[ClientID].ClientID != p->OtherClientid)
		{
			// Para evitar qualquer tipo de bug se o jogador j� est� em troca
			// o trade do mesmo � fechado
			SendClientMessage(ClientID, "Voc� j� est� em modo troca!");
			CloseTrade(ClientID);

			return;
		}
		else if (Trade[OtherClientid].isTrading && !Trade[ClientID].isTrading && Trade[OtherClientid].WaitingData)
		{
			// Checa se o usu�rio que foi pedido o trade est� sem trade novamente e se
			// o usu�rio que enviou o trade est� realmente esperando o trade
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
				// O pr�prio cliente existe prote��o, mas como o packet pode ser manipulado e/ou enviado
				// pelo pr�prio usu�rio como forma de hack, � feito a prote��o no servidor tamb�m
				// Caso queira, coloque um CloseTrade ao final dessa condi��o porque o client provavelmente n�o se enganar�
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
						// Checa se a quantidade de itens nulos no invent�rio do usu�rio � igual ou maior
						// que a quantidade de itens colocadas no trade
						SendClientMessage(ClientID, "Voc� n�o possui espa�o no invent�rio");
						SendClientMessage(OtherClientid, "O outro jogador n�o possui espa�o no invent�rio");
						CloseTrade(ClientID);

						return;
					}
					else if (GetAmountItem(OtherClientid) < amount_TradeCID)
					{
						// Checa se a quantidade de itens nulos no invent�rio do usu�rio � igual ou maior
						// que a quantidade de itens colocadas no trade
						SendClientMessage(OtherClientid, "Voc� n�o possui espa�o no invent�rio");
						SendClientMessage(ClientID, "O outro jogador n�o possui espa�o no invent�rio");
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
							SendClientMessage(ClientID, "Limite m�ximo de 2bilh�es de gold no invet�rio!");
							SendClientMessage(OtherClientid, "O outro jogador n�o possui espa�o para guardar gold.");

							CloseTrade(ClientID);
							CloseTrade(OtherClientid);

							return;
						}

						int calcOpponentGold = (mob->Gold - Trade[OtherClientid].Gold) + Trade[ClientID].Gold;

						if (calcOpponentGold > 2000000000 || calcOpponentGold < 0)
						{
							SendClientMessage(OtherClientid, "Limite m�ximo de 2bilh�es de gold no invet�rio!");
							SendClientMessage(ClientID, "O outro jogador n�o possui espa�o para guardar gold.");

							CloseTrade(ClientID);
							CloseTrade(OtherClientid);

							return;
						}

						if (Trade[ClientID].Gold > player->Gold)
						{
							SendClientMessage(ClientID, "Voc� n�o possui esse gold.");
							SendClientMessage(OtherClientid, "O outro jogador n�o possui a quantia de gold.");

							CloseTrade(ClientID);
							CloseTrade(OtherClientid);

							return;
						}

						if (Trade[OtherClientid].Gold > mob->Gold)
						{
							SendClientMessage(OtherClientid, "Voc� n�o possui esse gold.");
							SendClientMessage(ClientID, "O outro jogador n�o possui a quantia de gold.");

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
							// Slot livre do invent�rio
							if (Trade[ClientID].Itens[i].Index)
								SlotID = GetFirstSlot(OtherClientid, 0, INVENTORY);
							// Slot livre do invent�rio
							if (Trade[OtherClientid].Itens[i].Index)
								SlotOID = GetFirstSlot(ClientID, 0, INVENTORY);
							// Checa se os valores retornados s�o v�lidos
							// Se retornar -1 quer dizer que a fun�a� anterior (GetAmountItem) aconteceu algum erro
							if (SlotID == -1 || SlotOID == -1)
							{
								// O erro aconteceu ent�o a mensagem � enviada aos dois players e o trade dos mesmos � fechado
								SendClientMessage(ClientID, "Ocorreu um erro!");
								SendClientMessage(OtherClientid, "Ocorreu um erro!");
								CloseTrade(ClientID);
							}
							// Checa se existe algum item no slot correspondente do trade
							if (Trade[ClientID].Itens[i].Index)
							{
								// Prote��o contra cria��o de itens
								if (memcmp(&Trade[ClientID].Itens[i], &player->Inventory[Trade[ClientID].TradeItemSlot[i]], sizeof st_Item))
								{
									CloseTrade(ClientID);
									CloseTrade(OtherClientid);

									SendClientMessage(ClientID, "Ocorreu um erro!");
									SendClientMessage(OtherClientid, "Ocorreu um erro!");
								}
								else
								{
									// Copia o item que est� no trade do outro usu�rio
									memcpy(&mob->Inventory[SlotID], &Trade[ClientID].Itens[i], sizeof st_Item);
									// Apaga o item do invent�rio do outro usu�rio
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
								// Prote��o contra cria��o de itens
								if (memcmp(&Trade[OtherClientid].Itens[i], &mob->Inventory[Trade[OtherClientid].TradeItemSlot[i]], sizeof st_Item))
								{
									CloseTrade(ClientID);
									CloseTrade(OtherClientid);
									SendClientMessage(ClientID, "Ocorreu um erro!");
									SendClientMessage(OtherClientid, "Ocorreu um erro!");
								}
								else
								{
									// Copia o item que est� no trade do outro usu�rio
									memcpy(&player->Inventory[SlotOID], &Trade[OtherClientid].Itens[i], sizeof st_Item);
									// Apaga o item do invent�rio do outro usu�rio
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
					// Se o outro usu�rio n�o confirmou ainda o trade, apenas � enviado para o outro usu�rio
					// o bot�o de confirma��o juntamente com os itens/gold do trade
					SendAllTrade(ClientID, OtherClientid);
			}
		}
		else
		{
			// Aqui o usu�rio apenas colocou os itens no slot
			if (!AddItensOnTrade(p))  // Aconteceu algum erro ao adicionar os itens ao trade
			{
				CloseTrade(ClientID);
				CloseTrade(OtherClientid);
			}
			else
			{
				// N�o aconteceu erro, ent�o � setado tudo normalmente
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

