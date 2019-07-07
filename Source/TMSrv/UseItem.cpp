#include "DLLMain.h"

char temp[256];

#define _30DAYS 329040
#define _15DAYS 164520
#define _7DAYS 76776

void UseItens(BYTE *m_PacketBuffer, bool *pRetn)
{
	p373 *p = (p373*)m_PacketBuffer;

	INT cId = p->Header.ClientId;

	*pRetn = true;

	if (cId <= NULL || cId > MAX_PLAYER)
		return; // ClientId inválido
	else if (Users[cId].Status != 22)
		return; // Não está no mundo
	else if (p->SrcType != INVENTORY)
	{
		if (p->SrcType == CARGO)
		{
			SendClientMessage(cId, "Passe o item para o inventário para utilizá-lo.");
			SendItem(cId, p->SrcType, p->SrcSlot, &Users[cId].Storage.Item[p->SrcSlot]);
		}

		return;
	}
	else if (p->PosX < 0 || p->PosY < 0 || p->PosX > 4096 || p->PosY > 4096)
		return; // Coordenadas inválidas
	else if (p->SrcSlot < 0 || p->SrcSlot > 63 || p->DstSlot < 0 || p->DstSlot > 63 || p->DstType < 0 || p->DstType > 2)
	{
		CloseUser(cId);

		return;
	}

	st_Mob *player = GetMobFromIndex(cId);

	st_Item *srcItem = GetItemPointer(player, Users[cId].Storage.Item, p->SrcType, p->SrcSlot),
		*dstItem = GetItemPointer(player, Users[cId].Storage.Item, p->DstType, p->DstSlot);

	if (srcItem == NULL)
		return;

	UINT32 CurHp = player->Status.curHP;

	if (!CurHp)
	{
		SendItem(cId, p->SrcType, p->SrcSlot, srcItem);

		SendClientMessage(cId, "Desculpe.");

		return;
	}

	INT16 ItemIndex = srcItem->Index,
		  ItemVolatile = BASE_GetItemAbility(srcItem, EF_VOLATILE);

	switch (ItemVolatile)
	{
#pragma region POTS
		case 1: // Poções HP/Mp
		{
			for (INT8 i = 0; i <= MAX_AFFECTS; i++)
				if (mBuffer[cId].Affects[i].Index == 47)
				{
					SendItem(cId, p->SrcType, p->SrcSlot, srcItem);
					return;
				}

			int pCalc = 0; //0e8c
			int pHP = BASE_GetItemAbility(srcItem, EF_HP); //0e90
			if (pHP != 0)
			{
				pCalc = Users[cId].Potion.CountHp;
				pCalc = pCalc + pHP;
				if (pCalc > player->Status.maxHP)
					pCalc = player->Status.maxHP;

				Users[cId].Potion.CountHp = pCalc;
			}

			//0043A7A8
			int pMP = BASE_GetItemAbility(srcItem, EF_MP); //0e94
			if (pMP != 0)
			{
				pCalc = Users[cId].Potion.CountMp;
				pCalc = pCalc + pMP;
				if (pCalc > player->Status.maxMP)
					pCalc = player->Status.maxMP;

				Users[cId].Potion.CountMp = pCalc;
			}

			//0043A832
			Users[cId].Potion.bQuaff = 1;

			SendSetHpMp(cId);
			AmountMinus(srcItem);

			break;
		}
#pragma endregion
#pragma region POEIRAS
		case 4: case 5:
		{
			OriLac(p, pRetn);
			break;
		}
#pragma endregion
#pragma region POEIRA_FADA
		case 7:
		{
			if (player->bStatus.Level < 99)
			{
				player->Exp = exp_table[player->bStatus.Level + 1] + 1;
				__asm MOV ECX, player
				HKD_CheckGetLevel();
				SendClientMessage(cId, "+++ LEVEL UP! +++");
				GetCurrentScore(cId);
				SendScore(cId);
				SendEtc(cId);
				AmountMinus(srcItem);
			}
			else
			{
				SendClientMessage(cId, "Não há mais níveis para subir.");
				SendItem(cId, p->SrcType, p->SrcSlot, srcItem);
			}

			break;
		}
#pragma endregion
#pragma region PILULA_PODER
		case 6: // Pilula do poder
		{
			player->StatusPoint += 9;

			SendEmotion(cId, 14, 3);
			SendEtc(cId);

			AmountMinus(srcItem);

			break;
		}
#pragma endregion
#pragma region PEDRAS_LE
		case 9: // Pedras Le
		{
			if (dstItem->Index == NULL)
			{
				SendClientMessage(cId, "Desculpe.");

				SendItem(cId, p->SrcType, p->SrcSlot, srcItem);

				return;
			}

			//0043B663
			int pSrcItemID = srcItem->Index, //0ec8
				pDestItemID = dstItem->Index, //0ecc
				StoneID = pSrcItemID - 575; //0ed0

			if (StoneID < 0 || StoneID >= 4)
			{
				SendItem(cId, p->SrcType, p->SrcSlot, srcItem);

				return;
			}

			//0043B6D1 - Atualizado - agora os Sets E de mortal podem ser transformados em Le
			int flag = -1; //0ed4
			int ItemUnique = itemlistBuffer[pDestItemID].Unique; //0ed8
			if (ItemUnique == 5 || ItemUnique == 14 || ItemUnique == 24 || ItemUnique == 34) //Sets A
				flag = 0; //Pedra_de_Spinner
			if (ItemUnique == 6 || ItemUnique == 15 || ItemUnique == 25 || ItemUnique == 35) //Sets B
				flag = 1; //Pedra_de_Beril
			if (ItemUnique == 7 || ItemUnique == 16 || ItemUnique == 26 || ItemUnique == 36) //Sets C
				flag = 2; //Pedra_de_Tectita
			if (ItemUnique == 8 || ItemUnique == 17 || ItemUnique == 27 || ItemUnique == 37) //Sets D
				flag = 3; //Pedra_de_Adamantita
			if (ItemUnique == 10 || ItemUnique == 20 || ItemUnique == 30 || ItemUnique == 40) //Sets E
				flag = 3; //Pedra_de_Adamantita

			//0043B7A9
			if (ItemUnique == -1)
			{
				SendItem(cId, p->SrcType, p->SrcSlot, srcItem);
				SendClientMessage(cId, "Uso incorreto.");

				return;
			}

			//0043B800
			if (flag != StoneID)
			{
				SendItem(cId, p->SrcType, p->SrcSlot, srcItem);

				SendClientMessage(cId, "Uso incorreto.");

				return;
			}

			//0043B85C
			int ItemGrade = itemlistBuffer[pDestItemID].Grade; //0edc
			if (ItemGrade <= 0 || ItemGrade >= 4)
			{
				SendItem(cId, p->SrcType, p->SrcSlot, srcItem);

				SendClientMessage(cId, "Não é possível utilizar neste item.");

				return;
			}

			//0043B8C1
			int _random = rand() % 100; //0ee0
			int rate = 50; //0ee4
			if (_random > rate)
			{
				int Body = player->Equip[0].Index / 10; //0ee8
				if (Body == 0)
					SendEmotion(cId, 20, 0);
				else if (Body == 1)
					SendEmotion(cId, 15, 0);
				else if (Body == 2)
					SendEmotion(cId, 15, 0);
				else
				{
					if (Body == 3)
						SendEmotion(cId, 15, 0);
				}

				//sprintf(temp, "etc, regendary fail %d - (%d,%d,%d)", pDestPointer->Index, pSrcPointer->EFV1, pSrcPointer->EFV2, pSrcPointer->EFV3);
				//Log(temp, pUser[conn].AccountName, pUser[conn].IP);
				sprintf(temp, "%s(%d/%d)", "Refinação falhou", _random, rate);
				SendClientMessage(cId, temp);

				AmountMinus(srcItem);

				return;
			}

			//0043BA60
			int ItemExtreme = itemlistBuffer[pDestItemID].Extreme; //0eec

			dstItem->Index = ItemExtreme;

			SendItem(cId, p->DstType, p->DstSlot, dstItem);

			AmountMinus(srcItem);

			SendScore(cId);
			SendEmotion(cId, 14, 0);
			//sprintf(temp, "etc, regendary success %d - (%d,%d,%d)", pDestPointer->Index, pSrcPointer->EFV1, pSrcPointer->EFV2, pSrcPointer->EFV3);
			//Log(temp, pUser[conn].AccountName, pUser[conn].IP);
			sprintf(temp, "%s(%d/%d)", "Refinação concluida com sucesso", _random, rate);
			SendClientMessage(cId, temp);

			break;
		}
#pragma endregion
#pragma region NATIVAS
		case 10: // Antídoto, Poção Kappa
		case 11: // Pergaminho retorno
		case 12: // Gema Estelar
		case 13: // Pergas Teleporte
		case 41: // \/ Contratos
		case 42:
		case 43:
		case 44:
		case 45:
		case 46:
		case 47:
		case 48:
		case 49:
		case 50: // /\ Contratos
		{
			// Libera o uso da função nativa
			*pRetn = false;

			break;
		}
#pragma endregion

		default:
		{
			// Funções que serão feitas via index do item
			switch (ItemIndex)
			{
#pragma region ANDARILHO
				case 3467:
					Andarilho(p);
					break;
#pragma endregion
#pragma region ATIVA_VIP
				case 5155:
				case 5156:
				{
					int ItemType      = ItemIndex - 5155,
						PlayerVipType = GetVipType(cId);

					// Pega os dados do tempo atual
					time_t rawnow = time(NULL);
					struct tm *now = localtime(&rawnow);

					if (!isVip(cId))
						memcpy(&nBuffer[cId].Vip.Validade, now, sizeof tm);
					else if (PlayerVipType != ItemType)
					{
						SendItem(cId, p->SrcType, p->SrcSlot, srcItem);
						SendClientMessage(cId, "Para trocar de tipo espere o atual acabar.");

						return;
					}
					
					PutMoreTime(&nBuffer[cId].Vip.Validade, srcItem->EFV1);

					memset(srcItem, 0, sizeof st_Item);
					SendItem(cId, p->SrcType, p->SrcSlot, srcItem);

					nBuffer[cId].Vip.Type = ItemType;

					SendClientMsg(cId, "Vip %s ativado até [%02d/%02d/%04d].", ItemType > FOGO_VIP ? "Gelo" : "Fogo", nBuffer[cId].Vip.Validade.tm_mday, nBuffer[cId].Vip.Validade.tm_mon + 1, nBuffer[cId].Vip.Validade.tm_year + 1900);
					sendLog(INFO, cId, "%s %s Vip %s ativado até [%02d/%02d/%04d].", nBuffer[cId].Login, player->Name, ItemType > FOGO_VIP ? "Gelo" : "Fogo", nBuffer[cId].Vip.Validade.tm_mday, nBuffer[cId].Vip.Validade.tm_mon + 1, nBuffer[cId].Vip.Validade.tm_year + 1900);

					return;
				}
				break;
#pragma endregion
#pragma region AGUA
				case 777:  case 778:  case 779:  case 780:
				case 781:  case 782:  case 783:  case 784:
				case 3173: case 3174: case 3175: case 3176:
				case 3177: case 3178: case 3179: case 3180:
				case 3182: case 3183: case 3184: case 3185:
				case 3186: case 3187: case 3188: case 3189:
					Water(p);
					break;
				case 785: case 3181: case 3190:
					BossWater(p);
					break;
#pragma endregion
#pragma region PESA
				case 3324: case 3325: case 3326:
					NightMare(p, srcItem);
					break;
#pragma endregion
#pragma region CATALIZAR_RECUPERAR_MOUNT
				case 3315:
					CurarMontaria(p);
					break;

				case 3316: case 3317:
					CatalizarLevel(p);
					break;
#pragma endregion
#pragma region BARRA_MITHRIL
				case 3027: case 3028:
				case 3029: case 3030:
					BarrasMithril(p);
					break;
#pragma endregion
#pragma region BARRA_BI
				case 4010: case 4011: case 4026:
				case 4027: case 4028: case 4029:
				case 5359: case 5360:
					Barras(p);
					break;
#pragma endregion
#pragma region QUEST_ITENS
				case 4117: case 4118: case 4119:
				case 4120: case 4121:
					QuestItens(p);
					break;
#pragma endregion
#pragma region FOGOS_ARTIFICIO
				case 1728:
					Fogos(p);
					break;
#pragma endregion
#pragma region EXTRACOES
				case 3021: case 3022:
				case 3023: case 3024:
				case 3025: case 3026:
					Extracoes(p);
					break;
#pragma endregion
#pragma region GEMAS
				case 3386: case 3387: case 3388: case 3389:
					Gemas(p);
					break;
#pragma endregion
#pragma region TINTURAS
				case 3407: case 3408: case 3409:
				case 3410: case 3411: case 3412:
				case 3413: case 3414: case 3415: case 3416:
					Tinturas(p);
					break;

				case 3417:
					RemoveTinturas(p);
					break;
#pragma endregion
#pragma region BOOKSSEPH
				case 667: case 668: case 669:
				case 670: case 671:
					ProcessSephiraBook(m_PacketBuffer, pRetn);
					break;
#pragma endregion
#pragma region HUNTSCROLL
				case 3432: case 3433: case 3434:
				case 3435: case 3436: case 3437:
					ProcessHuntingScrolls(m_PacketBuffer, pRetn);
					break;
#pragma endregion
#pragma region AGRUPPO
				case 3445: case 3446:
					AgruparPoeiras(m_PacketBuffer, pRetn);
					break;
#pragma endregion
#pragma region DESAGRUPPO
				case 3447: case 3448:
					DesagruparPoeiras(m_PacketBuffer, pRetn);
					break;
#pragma endregion
#pragma region REPLATIONS
				case 4016: case 4017: case 4018:
				case 4019: case 4020: case 4021:
				case 4022: case 4023: case 4024:
					Replations(m_PacketBuffer, pRetn);
					break;
#pragma endregion
#pragma region CRISTAIS
				case 4106: case 4107: case 4108: case 4109:
					*pRetn = true;
					SendItem(cId, p->SrcType, p->SrcType, srcItem);
					//Cristais(m_PacketBuffer, pRetn);
					break;
#pragma endregion
#pragma region MOLARGARGULA
				case 4122:
					MolarGargula(m_PacketBuffer, pRetn);
					break;
#pragma endregion
#pragma region MONEYCUBE
				case 4905:
					*pRetn = true;
					SendItem(cId, p->SrcType, p->SrcType, srcItem);
					//MoneyCube(m_PacketBuffer, pRetn);
					break;
#pragma endregion
#pragma region BAUEXP
				case 4140:
				case 4144:
					BauExp(p);
					break;
#pragma endregion
#pragma region LANS
				case 4111: case 4112: case 4113: // Lans
					Lans(p);
					break;
#pragma endregion
#pragma region IDEAL
				case 5338: // Ideal
					*pRetn = true;
					Ideal(p);
					break;
#pragma endregion
#pragma region CHOCAMOR
				case 1739:
					*pRetn = true;
					SendItem(cId, p->SrcType, p->SrcType, srcItem);
					//ChocAmor(m_PacketBuffer, pRetn);
					break;
#pragma endregion
#pragma region JOIAS
				case 3200: case 3201:
				case 3202: case 3204:
				case 3205: case 3206:
				case 3208: case 3209:
					Joias(p);
					break;
#pragma endregion
#pragma region FRANGO
				case 3314:
					Frango(m_PacketBuffer, pRetn);
					break;
#pragma endregion
#pragma region PERDAO
				case 3343:
				{

					player->Inventory[63].EF1 = 150;

					SendCreateMob(cId, cId, 1);
					AmountMinus(&player->Inventory[p->SrcSlot]);

					SendClientMessage(cId, "Você obteve perdão por seus pecados.");

					break;
				}
#pragma endregion
#pragma region RETORNO_HABILIDADES
				case 3336:
					ResetDeHabilidades(p);
					break;
#pragma endregion
#pragma region CATALIZADORES
				case 3344: case 3345: case 3346: case 3347: case 3348: case 3349: case 3350:
					Catalizadores(p);
					break;
#pragma endregion
#pragma region RESTAURADORES
				case 3351: case 3352: case 3353: case 3354: case 3355: case 3356: case 3357:
					Restauradores(p);
					break;
#pragma endregion
				case 5152:
					PergaminhoMissao1(p);
					break;
				case 5153:
					PergaminhoMissao2(p);
					break;

				default:
				{
#pragma region RACOES_AMAGOS
					if (srcItem->Index >= 2390 && srcItem->Index <= 2419)
					{
						Amagos(p, pRetn);

						return;
					}
					else if (srcItem->Index > 2419 && srcItem->Index <= 2439)
					{
						*pRetn = false;

						return;
					}
#pragma endregion

					SendClientMessage(cId, "Desculpe, esta função não está pronta ainda.");
					SendItem(cId, p->SrcType, p->SrcType, srcItem);

					return;
				}
			}
		}
	}
}


void PergaminhoMissao2(p373* p)
{
	int cId = p->Header.ClientId;

	st_Mob *player = GetMobFromIndex(cId);

	st_Item *srcItem = GetItemPointer(player, Users[cId].Storage.Item, p->SrcType, p->SrcSlot);

	int LoggedMob = nBuffer[cId].Ingame.LoggedMob;
	if (LoggedMob < 0 || LoggedMob > 3)
	{
		SendClientMessage(cId, "Desculpe.");
		SendItem(cId, p->SrcType, p->SrcSlot, srcItem);

		return;
	}

	else if (nBuffer[cId].Chars[LoggedMob].Quests.Armeiro2)
	{
		SendClientMessage(cId, "Esta quest só pode ser concluída uma vez.");
		SendItem(cId, p->SrcType, p->SrcSlot, srcItem);

		return;
	}

	int AmountCount = 0;

	for (int i = 0; i < 60; i++)
	{
		if (player->Inventory[i].Index == 5150)
			AmountCount += GetItemAmount(&player->Inventory[i]);
	}

	if (AmountCount < 10)
	{
		SendClientMessage(cId, "Traga-me 10 Blocos de aço para progredir.");
		SendItem(cId, p->SrcType, p->SrcSlot, srcItem);

		return;
	}

	for (int i = 0; i < 10; i++)
	{
		int slot = GetFirstSlot(cId, 5150, INVENTORY);

		AmountMinus(&player->Inventory[slot]);
		SendItem(cId, INVENTORY, slot, &player->Inventory[slot]);
	}

	st_Item premio;
	memset(&premio, 0, sizeof st_Item);
	premio.Index = 5151;
	GiveItem(cId, &premio);
	AmountMinus(srcItem);

	nBuffer[cId].Chars[LoggedMob].Quests.Armeiro2 = 1;

	SendClientMessage(cId, "Segunda missão concluída com sucesso.");
	return;
}

void PergaminhoMissao1(p373 *p)
{
	int cId = p->Header.ClientId;

	st_Mob *player = GetMobFromIndex(cId);

	st_Item *srcItem = GetItemPointer(player, Users[cId].Storage.Item, p->SrcType, p->SrcSlot);

	int LoggedMob = nBuffer[cId].Ingame.LoggedMob;
	if (LoggedMob < 0 || LoggedMob > 3)
	{
		SendClientMessage(cId, "Desculpe.");
		SendItem(cId, p->SrcType, p->SrcSlot, srcItem);

		return;
	}
	else if (nBuffer[cId].Chars[LoggedMob].Quests.Armeiro1)
	{
		SendClientMessage(cId, "Esta quest só pode ser concluída uma vez.");
		SendItem(cId, p->SrcType, p->SrcSlot, srcItem);

		return;
	}

	static const int armas_Armeiro[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

	for (int i = 0; i < sizeof(armas_Armeiro) / sizeof(int); i++)
	{
		if (player->Equip[6].Index == armas_Armeiro[i])
		{
			st_Item premio;
			memset(&premio, 0, sizeof st_Item);
			premio.Index = srcItem->Index + 1;

			if (GiveItem(cId, &premio) != -1)
			{
				AmountMinus(srcItem);

				nBuffer[cId].Chars[LoggedMob].Quests.Armeiro1 = 1;
				SendClientMessage(cId, "Primeira missão concluída com sucesso !");
			}
			else
			{
				SendClientMessage(cId, "Não há espaço suficiente.");

				SendItem(cId, p->SrcType, p->SrcSlot, srcItem);
			}

			return;
		}
	}

	SendClientMessage(cId, "Compre uma arma do Armeiro e equipe-a antes de prosseguir.");
	SendItem(cId, p->SrcType, p->SrcSlot, srcItem);

	return;
}

static const INT16 Crias[][6] = {
	{    0,    0,    0, 2333, 2334, 2335 }, // Kapel
	{ 2336, 2337, 2338, 2341, 2342, 2343 }, // Acuban
	{    0,    0, 2339, 2340, 2345, 2344 }, // Mencar
	{    0,    0,    0, 2346, 2347, 2348 }, // Birago
	{    0,    0,    0, 2351, 2352, 2353 }, // Yus
	{    0,    0,    0, 2354, 2355, 2356 }, // Makav
	{    0,    0,    0,    0, 2349, 2350 } // Alperath
};

void Restauradores(p373* p)
{
	INT16 cId = p->Header.ClientId;

	st_Mob *player = GetMobFromIndex(cId);

	st_Item *srcItem = GetItemPointer(player, Users[cId].Storage.Item, p->SrcType, p->SrcSlot),
		*dstItem = GetItemPointer(player, Users[cId].Storage.Item, p->DstType, p->DstSlot);

	if (dstItem == NULL)
		return;

	else if ((dstItem->Index < 2333 || dstItem->Index > 2359) && (dstItem->Index < 2363 || dstItem->Index > 2389))
	{
		SendClientMessage(cId, "Uso incorreto.");
		SendItem(cId, p->SrcType, p->SrcSlot, srcItem);

		return;
	}

	srand(time(NULL) / 5 * (rand() % 500) * 5);
	int Vita = (rand() % 3) + 1;

	if (dstItem->EFV2 >= 60)
	{
		SendClientMessage(cId, "Limite máximo de 60 vitalidades.");
		SendItem(cId, p->SrcType, p->SrcSlot, srcItem);

		return;
	}

	int count = srcItem->Index - 3351;

	for (int i = 0; i < 6; i++)
	{
		if (dstItem->Index == Crias[count][i] || dstItem->Index == (Crias[count][i] + 30))
		{
			dstItem->EFV2 += Vita;
			if (dstItem->EFV2 > 60)
				dstItem->EFV2 = 60;

			SendClientMessage(cId, "Sua vitalidade cresceu.");
			SendItem(cId, p->DstType, p->DstSlot, dstItem);

			AmountMinus(srcItem);

			return;
		}
	}

	SendClientMessage(cId, "Uso incorreto.");
	SendItem(cId, p->SrcType, p->SrcSlot, srcItem);

	return;
}

void Catalizadores(p373* p)
{
	INT16 cId = p->Header.ClientId;

	st_Mob *player = GetMobFromIndex(cId);

	st_Item *srcItem = GetItemPointer(player, Users[cId].Storage.Item, p->SrcType, p->SrcSlot),
			*dstItem = GetItemPointer(player, Users[cId].Storage.Item, p->DstType, p->DstSlot);

	if (dstItem == NULL)
		return;

	if (dstItem->Index < 2333 || dstItem->Index > 2359)
	{
		SendClientMessage(cId, "Uso incorreto.");
		SendItem(cId, p->SrcType, p->SrcSlot, srcItem);

		return;
	}

	int count = srcItem->Index - 3344;

	for (int i = 0; i < 6; i++)
	{
		if (dstItem->Index == Crias[count][i])
		{
			int face = dstItem->Index - 2015;

			dstItem->Index += 30;
			dstItem->EF2 = 1;
			srand(time(NULL) / 5 * (rand() % 500) * 5);
			dstItem->EFV2 = (rand() % 15) + 10;

			INT16 LeaderCid = cId;

			if (player->Leader > 0 && player->Leader < MAX_PLAYER)
				LeaderCid = player->Leader;

			st_Mob *Lider = GetMobFromIndex(LeaderCid);

			for (int e = 0; e < 12; e++)
			{
				if (Lider->PartyList[e] == cId)
					continue;

				if (Lider->PartyList[e] <= 1000)
					continue;

				st_Mob *tmpMob = (st_Mob*)GetMobFromIndex(Lider->PartyList[e]);

				for (int z = 0; z < 8; z++)
				{
					if (tmpMob->Equip[0].Index == face)
					{
						if (tmpMob->Summoner != cId)
							continue;

						RemoveParty(Lider->PartyList[i]);
						SendRemoveParty(Lider->Leader, Lider->PartyList[i]);
					}
				}
			}
			
			SendClientMessage(cId, "Sua montaria nasceu.");
			SendItem(cId, p->DstType, p->DstSlot, dstItem);
			
			AmountMinus(srcItem);

			return;
		}
	}

	SendClientMessage(cId, "Uso incorreto.");
	SendItem(cId, p->SrcType, p->SrcSlot, srcItem);

	return;
}

void ResetDeHabilidades(p373* p)
{
	INT16 cId = p->Header.ClientId;
	st_Mob *player = GetMobFromIndex(cId);

	st_Item *srcItem = GetItemPointer(player, Users[cId].Storage.Item, p->SrcType, p->SrcSlot);

	INT32 PointsCount = 0,
		ClassInfo = player->ClassInfo;

	for (int i = 0; i < 4; i++)
	{
		if (player->bStatus.Points[i] > (100 + BaseSIDCHM[ClassInfo][i]))
		{
			player->bStatus.Points[i] -= 100;
			PointsCount += 100;
		}
		else
		{
			PointsCount += player->bStatus.Points[i] - BaseSIDCHM[ClassInfo][i];
			player->bStatus.Points[i] = BaseSIDCHM[ClassInfo][i];
		}
	}

	if (PointsCount == 0)
	{
		SendClientMessage(cId, "Não possui pontos para retornar.");
		SendItem(cId, p->SrcType, p->SrcSlot, srcItem);

		return;
	}

	AmountMinus(srcItem);

	SendClientMsg(cId, "%d pontos foram retornados.", PointsCount);
	player->StatusPoint += PointsCount;

	BASE_GetBonusScorePoint(player);
	GetCurrentScore(cId);
	SendScore(cId);
}

int	g_pSancRate[4][MAX_MAINREFINELEVEL] = {
	{ 100, 100, 80, 60, 40, 20, -1, -1, -1, -1, -1, -1, -1, -1, -1 },	// Poeira de Ori
	{ 100, 100, 100, 85, 75, 65, 50, 35, 20, -1, -1, -1, -1, -1, -1 },	// Poeira de Lac
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, -1, 100, -1, -1, -1, -1 },	// Poeira de Lac 100%
	{ 100, 100, 90, 80, 70, 60, 50, 40, 30, 15, 10, 5, -1, -1, -1 }    // Amagos
};

void Amagos(p373* p, bool* pRetn)
{
	INT16 cId = p->Header.ClientId;
	
	st_Mob *player = GetMobFromIndex(cId);

	st_Item *srcItem = GetItemPointer(player, Users[cId].Storage.Item, p->SrcType, p->SrcSlot),
			*dstItem = GetItemPointer(player, Users[cId].Storage.Item, p->DstType, p->DstSlot);

	if (dstItem == NULL)
		return;
	else if (dstItem->Index < 2330 || dstItem->Index > 2389)
	{
		SendClientMessage(cId, "Não pode fazer isso.");
		SendItem(cId, p->SrcType, p->SrcSlot, srcItem);
		return;
	}
	else if (dstItem->EFV2 <= 3)
	{
		SendClientMessage(cId, "Somente montarias com mais de 3 vitalidades.");
		SendItem(cId, p->SrcType, p->SrcSlot, srcItem);
		return;
	}
	else if (dstItem->Index >= 2330 && dstItem->Index < 2360)
	{
		int LastLevel = 99;
		if (dstItem->Index >= 2330 && dstItem->Index <= 2335)
			LastLevel = 24 + ((dstItem->Index - 2330) * 10);

		if (dstItem->EF2 == LastLevel)
		{
			// Cria -> Adulto
			*pRetn = false;
			return;
		}
	}
	else if (dstItem->EF2 >= 120)
	{
		SendClientMessage(cId, "Já atingiu o nível máximo.");
		SendItem(cId, p->SrcType, p->SrcSlot, srcItem);
		return;
	}

	int rate = dstItem->EF2 / 10;
	rate = g_pSancRate[3][rate];

	srand(time(NULL) / 5 * (rand() % 500) * 5);
	int SuccessRate = rand() % 100;

	if (SuccessRate <= rate)
	{
		dstItem->EF2++;
		SendClientMsg(cId, "Refinação bem sucedida. (%d/%d)", SuccessRate, rate);
	}
	else
	{
		srand(time(NULL) / 5 * (rand() % 500) * 5);
		int chance = rand() % 15;
		if (!chance)
		{
			dstItem->EFV2--;
			SendClientMsg(cId, "Refinação falhou, vitalidade diminuida. (%d/%d)", SuccessRate, rate);
		}
		else
			SendClientMsg(cId, "Refinação falhou. (%d/%d)", SuccessRate, rate);
	}

	AmountMinus(srcItem);
	SendItem(cId, p->DstType, p->DstSlot, dstItem);
}

void MolarGargula(UINT8* m_PacketBuffer, bool *pRetn)
{
	*pRetn = true;

	p373 *p = (p373*)m_PacketBuffer;

	INT16 cId = p->Header.ClientId;

	st_Mob *player = GetMobFromIndex(cId);

	if (player->Equip[0].EFV2 != MORTAL)// Mortais apenas
	{
		SendItem(cId, INVENTORY, p->SrcSlot, &player->Inventory[p->SrcSlot]);

		SendClientMessage(cId, "Item exclusivo para Mortais.");

		return;
	}

	if (player->Status.Level < 40 || player->Status.Level > 44)// 201 ao 205
	{
		SendItem(cId, INVENTORY, p->SrcSlot, &player->Inventory[p->SrcSlot]);

		SendClientMessage(cId, "Possível utilizar este item apenas entre os níveis 41 - 45.");

		return;

	}

	if (nBuffer[cId].Chars[nBuffer[cId].Ingame.LoggedMob].Quests.MolarGargula)// Já completou a quest
	{
		SendItem(cId, INVENTORY, p->SrcSlot, &player->Inventory[p->SrcSlot]);

		SendClientMessage(cId, "Você já participou desta quest.");

		return;
	}

	if (player->Equip[1].Index >= 3500 && player->Equip[1].Index <= 3507)// Não pode refinar tiaras
	{
		SendItem(cId, INVENTORY, p->SrcSlot, &player->Inventory[p->SrcSlot]);

		SendClientMessage(cId, "Nesta quest é impossível refinar Cythera.");

		return;
	}

	nBuffer[cId].Chars[nBuffer[cId].Ingame.LoggedMob].Quests.MolarGargula = TRUE;

	for (INT8 i = 1; i < 6; i++)
	{
		SetItemSanc(&player->Equip[i], 6);

		SendItem(cId, EQUIP, i, &player->Equip[i]);
	}

	AmountMinus(&player->Inventory[p->SrcSlot]);

	SendEffect(cId, 0xE, 3);

	SendClientMessage(cId, "Quest Molar do Gargula concluída.");
}

void Frango(BYTE *m_PacketBuffer, bool *pRetn)
{
	p373 *p = (p373*)m_PacketBuffer;

	INT16 cId = p->Header.ClientId;

	st_Mob *player = (st_Mob*)GetMobFromIndex(cId);

	st_Item *srcItem = GetItemPointer(player, Users[cId].Storage.Item, p->SrcType, p->SrcSlot);

	AmountMinus(srcItem);

	for (INT8 i = 0; i <= MAX_AFFECTS; i++)
	{
		if (mBuffer[cId].Affects[i].Index == 30)
		{
			mBuffer[cId].Affects[i].Index = 30;
			mBuffer[cId].Affects[i].Master = 30;
			mBuffer[cId].Affects[i].Time = 1800.4;
			mBuffer[cId].Affects[i].Value = 30;
			
			GetCurrentScore(cId);
			SendScore(cId);

			return;
		}
	}

	for (INT8 i = 0; i <= MAX_AFFECTS; i++)
	{
		if (!mBuffer[cId].Affects[i].Index)
		{
			mBuffer[cId].Affects[i].Index = 30;
			mBuffer[cId].Affects[i].Master = 30;
			mBuffer[cId].Affects[i].Time = 1800.4;
			mBuffer[cId].Affects[i].Value = 30;

			GetCurrentScore(cId);
			SendScore(cId);

			return;
		}
	}
}

void Ideal(p373 *p)
{
	INT16 cId = p->Header.ClientId;

	st_Mob *player = (st_Mob*)GetMobFromIndex(cId);

	st_Item *srcItem = GetItemPointer(player, Users[cId].Storage.Item, p->SrcType, p->SrcSlot);

	if (player->Equip[0].EFV2 != SEMIDEUS || player->bStatus.Level < 94)
	{
		SendClientMessage(cId, "Somente Semideuses a partir do nível 95.");
		SendItem(cId, p->SrcType, p->SrcSlot, srcItem);
		return;
	}
	else if (player->Equip[1].Index != 3500 || GetItemSanc(&player->Equip[1]) != 9)
	{
		SendClientMessage(cId, "Equipe uma Cythera Prateada +9.");
		SendItem(cId, p->SrcType, p->SrcSlot, srcItem);
		return;
	}

	AmountMinus(srcItem);

	nInfo* nP = &nBuffer[cId];

	nP->Chars[nP->Ingame.LoggedMob].DeusLevel = player->bStatus.Level;
	player->Equip[0].EFV2 = DEUS;

	memset(&player->Equip[1], 0, sizeof st_Item);
	memset(&player->Equip[15], 0x0, sizeof st_Item); // Zera a capa 
	
	player->Equip[1].Index = 3501;

	if (player->CapeInfo == 7) // Blue
		player->Equip[15].Index = 3197;
	else if (player->CapeInfo == 8) // Red
		player->Equip[15].Index = 3198;
	else // White
		player->Equip[15].Index = 3199;

	char szMsg[120];
	sprintf(szMsg, "[%s] acaba de se tornar um Deus!", player->Name);
	SendNotice(szMsg);

	BASE_GetBonusScorePoint(player);
	BASE_GetHpMp(player);

	SendCharList(cId);

	SendClientSignalParm(cId, cId, 0x3B4, nBuffer[cId].Ingame.LoggedMob);
}

void Joias(p373 *p)
{
	INT16 cId = p->Header.ClientId;

	st_Mob *player = (st_Mob*)GetMobFromIndex(cId);

	st_Item *srcItem = GetItemPointer(player, Users[cId].Storage.Item, p->SrcType, p->SrcSlot);

	if (srcItem->Index < 3200 || srcItem->Index > 3209)
		return;

	INT8 isJoia = -1;

	for (INT8 i = 0; i <= MAX_AFFECTS; i++)
	{
		if (mBuffer[cId].Affects[i].Index == 8)
		{
			isJoia = i;
			break;
		}
	}

	if (isJoia >= 0)
	{
		mBuffer[cId].Affects[isJoia].Index = 8;
		mBuffer[cId].Affects[isJoia].Master &= Joia[srcItem->Index - 3200];
		mBuffer[cId].Affects[isJoia].Time = 420;
		mBuffer[cId].Affects[isJoia].Value &= Joia[srcItem->Index - 3200];
	}
	else
	{
		for (INT8 i = 0; i <= MAX_AFFECTS; i++)
		{
			if (mBuffer[cId].Affects[i].Index == 0)
			{
				mBuffer[cId].Affects[i].Index = 8;
				mBuffer[cId].Affects[i].Master &= Joia[srcItem->Index - 3200];
				mBuffer[cId].Affects[i].Time = 420;
				mBuffer[cId].Affects[i].Value &= Joia[srcItem->Index - 3200];
				break;
			}
		}
	}

	GetCurrentScore(cId);

	SendScore(cId);

	AmountMinus(srcItem);
}

void Lans(p373 *p)
{
	INT16 cId = p->Header.ClientId;

	st_Mob *player = GetMobFromIndex(cId);

	st_Item *srcItem = GetItemPointer(player, Users[cId].Storage.Item, p->SrcType, p->SrcSlot);

	static const INT16 Infos[][3] =
	{
		{ 3683, 3640, MORTAL },
		{ 3810, 3525, SEMIDEUS },
		{ 3908, 3652, DEUS }
	};

	INT8 Index = srcItem->Index - 4111;
	
	if (player->Equip[0].EFV2 >= Infos[Index][2])
	{
		AmountMinus(srcItem);

		DoTeleport(cId, Infos[Index][0], Infos[Index][1]);
	}
	else
	{
		SendClientMessage(cId, "Desculpe.");

		SendItem(cId, INVENTORY, p->SrcSlot, srcItem);
	}
}

void CurarMontaria(p373* p)
{
	INT16 cId = p->Header.ClientId;

	st_Mob *player = GetMobFromIndex(cId);

	st_Item *srcItem = GetItemPointer(player, Users[cId].Storage.Item, p->SrcType, p->SrcSlot),
		*dstItem = GetItemPointer(player, Users[cId].Storage.Item, p->DstType, p->DstSlot);

	if (!dstItem)
	{
		printf("NULL\n");

		return;
	}

	if (p->DstSlot != 14 || (dstItem->Index < 2360 || dstItem->Index > 2388))
	{
		SendItem(cId, INVENTORY, p->SrcSlot, srcItem);

		SendClientMessage(cId, "Usado somente em montarias.");

		return;
	}

	if (dstItem->EFV2 < 4 || dstItem->EFV2 > 59)
	{
		SendClientMessage(cId, "Possível usar apenas com vitalidades entre o 4~60.");

		SendItem(cId, INVENTORY, p->SrcSlot, &player->Inventory[p->SrcSlot]);

		return;
	}

	dstItem->EFV2++;

	SendClientMessage(p->Header.ClientId, "Foi restaurado 1 de vida da montaria.");

	AmountMinus(srcItem);

	SendItem(cId, EQUIP, p->DstSlot, dstItem);

	SendEffect(cId, 0xE, 3);
}

void CatalizarLevel(p373* p)
{
	INT16 cId = p->Header.ClientId;

	st_Mob *player = GetMobFromIndex(cId);

	st_Item *srcItem = GetItemPointer(player, Users[cId].Storage.Item, p->SrcType, p->SrcSlot),
		*dstItem = GetItemPointer(player, Users[cId].Storage.Item, p->DstType, p->DstSlot);

	if (!dstItem || p->DstType != EQUIP)
	{
		printf("NULL\n");

		return;
	}

	if (p->DstSlot != 14 || (dstItem->Index < 2360 || dstItem->Index > 2388))
	{
		SendItem(cId, INVENTORY, p->SrcSlot, srcItem);

		SendClientMessage(cId, "Usado somente em montarias.");

		return;
	}

	UINT8 Level = 0,
		MinLevel = 0;

	if (srcItem->Index == 3316)
		Level = 100;
	else if (srcItem->Index == 3317)
	{
		Level = 120;
		MinLevel = 100;
	}

	if (dstItem->EF2 >= Level || dstItem->EF2 < MinLevel || dstItem->EFV2 < 3)
	{
		SendItem(cId, INVENTORY, p->SrcSlot, srcItem);

		SendClientMessage(cId, "Não é possível melhorar sua montaria.");

		return;
	}

	AmountMinus(srcItem);

	dstItem->EF2 = Level;

	SendItem(cId, EQUIP, p->DstSlot, dstItem);

	SendClientMessage(cId, "Sua montaria foi melhorada.");
}

void BossWater(p373* p)
{
	INT16 cId = p->Header.ClientId;

	st_Mob *player = GetMobFromIndex(cId);
	st_Item *Item = GetItemPointer(player, Users[cId].Storage.Item, p->SrcType, p->SrcSlot);

	if ((player->TargetX >> 2) != 491 || (player->TargetY >> 2) != 443)
	{
		if (player->TargetX <= 1040 || player->TargetX > 1392 || player->TargetY < 3470 || player->TargetY > 3695)
		{
			SendItem(cId, p->SrcType, p->SrcSlot, Item);
			SendClientMessage(cId, "Use somente na zona elemental da água.");

			return;
		}
	}

	int Type = -1;

	if (Item->Index == 3181)
		Type = 0;
	else if (Item->Index == 785)
		Type = 1;
	else if (Item->Index == 3190)
		Type = 2;
	else
	{
		SendItem(cId, p->SrcType, p->SrcSlot, Item);

		SendClientMessage(cId, "Erro de typing, favor entrar em contato com a administração.");

		return;
	}

	if (Agua[Type].Rooms[8].isActive)
	{
		SendItem(cId, p->SrcType, p->SrcSlot, Item);

		SendClientMessage(cId, "Esta sala já está ocupada.");

		return;
	}

	if (player->Leader != 0 && player->Leader != -1)
	{
		SendClientMessage(cId, "Uso exclusivo do líder do grupo.");

		SendItem(cId, p->SrcType, p->SrcSlot, Item);

		return;
	}

	AmountMinus(Item);

	st_Position Dest;

	// Zero a var pra evitar dados desconhecidos atrapalhando o funcionamento
	memset(&Dest, 0x0, sizeof(st_Position));

	// Pega as coords pela struct
	Dest.X = BossCoords[Type][0];
	Dest.Y = BossCoords[Type][1];

	// Marca a sala como ativa e fixa o tempo restante
	Agua[Type].Rooms[8].isActive = true;
	Agua[Type].Rooms[8].Leaders = p->Header.ClientId;
	Agua[Type].Rooms[8].Cids[0] = p->Header.ClientId;
	Agua[Type].Rooms[8].TimeLeft = 120;
	Agua[Type].Rooms[8].MobsLeft = (BossNum[Type][0] + BossNum[Type][1]);

	DoTeleport(cId, Dest.X, Dest.Y);

	// Manda o tempo verde
	SendClientSignalParm(cId, 0x7530, 0x3A1, 120);
	// Manda o número de Mobs
	NpcsAlive(cId, Agua[Type].Rooms[8].MobsLeft, Agua[Type].Rooms[8].MobsLeft);

	SendClientMessage(cId, "Bem vindo a zona elemental da água.");

	int e = 0;

	while (e < BossNum[Type][0])
	{
		e++;

		CreateMob(BossWaterMobs[Type][0], Dest.X, Dest.Y, "npc");
	}

	e = 0;

	while (e < BossNum[Type][1])
	{
		e++;

		CreateMob(BossWaterMobs[Type][1], Dest.X, Dest.Y, "npc");
	}

	for (int i = 0; i < 12; i++)
	{
		if (player->PartyList[i] > 740)
			continue;

		if (Users[player->PartyList[i]].Status != 22)
			continue;

		// Caso tenha pessoas no grupo poe na array
		// i + 1 pq o 0 é sempre o lider
		Agua[Type].Rooms[8].Cids[i + 1] = player->PartyList[i];
		// Teleporta o pessoal
		DoTeleport(player->PartyList[i], Dest.X, Dest.Y);
		// Msg de gay
		SendClientMessage(player->PartyList[i], "Bem vindo a zona elemental da água.");
		// Tempo verde
		SendClientSignalParm(player->PartyList[i], 0x7530, 0x3A1, 120);
		// Manda o número de Mobs
		NpcsAlive(player->PartyList[i], Agua[Type].Rooms[8].MobsLeft, Agua[Type].Rooms[8].MobsLeft);
	}
}

void BauExp(p373 *p)
{
	INT16 cId = p->Header.ClientId;

	st_Mob *player = GetMobFromIndex(cId);

	st_Item *srcItem = GetItemPointer(player, Users[cId].Storage.Item, p->SrcType, p->SrcSlot);

	AmountMinus(srcItem);

	for (INT8 i = 0; i <= MAX_AFFECTS; i++)
	{
		if (mBuffer[cId].Affects[i].Index == 39)
		{
			stAffects* buff = &mBuffer[cId].Affects[i];
			buff->Index = 39;
			buff->Time += 900.4;
			buff->Master = 39;
			buff->Value = 39;

			GetCurrentScore(cId);
			SendScore(cId);

			return;
		}
	}

	for (INT8 i = 0; i <= MAX_AFFECTS; i++)
	{
		if (!mBuffer[cId].Affects[i].Index)
		{
			mBuffer[cId].Affects[i].Index = 39;
			mBuffer[cId].Affects[i].Time = 900.4;
			mBuffer[cId].Affects[i].Master = 39;
			mBuffer[cId].Affects[i].Value = 39;

			GetCurrentScore(cId);
			SendScore(cId);

			return;
		}
	}
}

void Andarilho(p373* p)
{
	INT16 cId = p->Header.ClientId;
	
	nInfo* nP = &nBuffer[cId];
	st_Mob *player = (st_Mob*)GetMobFromIndex(cId);
	st_Item *srcItem = GetItemPointer(player, Users[cId].Storage.Item, p->SrcType, p->SrcSlot);
	
	if (nP->Chars[nP->Ingame.LoggedMob].Inventarios.hasBuyed[0] && nP->Chars[nP->Ingame.LoggedMob].Inventarios.hasBuyed[1])
	{
		SendClientMessage(cId, "Limite de 2 inventários extras.");
		SendItem(cId, p->SrcType, p->SrcSlot, srcItem);

		return;
	}

	AmountMinus(srcItem);
	SendItem(cId, p->SrcType, p->SrcSlot, srcItem);

	if (!nP->Chars[nP->Ingame.LoggedMob].Inventarios.hasBuyed[0])
	{
		nP->Chars[nP->Ingame.LoggedMob].Inventarios.hasBuyed[0] = true;
		memset(nP->Chars[nP->Ingame.LoggedMob].Inventarios.sInv1, 0, sizeof(st_Item) * 64);
	}
	else
	{
		nP->Chars[nP->Ingame.LoggedMob].Inventarios.hasBuyed[1] = true;
		memset(nP->Chars[nP->Ingame.LoggedMob].Inventarios.sInv2, 0, sizeof(st_Item) * 64);
	}

	SendClientMessage(cId, "Novo inventário ativado, utilize /changeinv para realizar as trocas. Bom uso!");
}

void Fogos(p373* p)
{
	INT16 cId = p->Header.ClientId;

	st_Mob *player = (st_Mob*)GetMobFromIndex(cId);

	st_Item *srcItem = GetItemPointer(player, Users[cId].Storage.Item, p->SrcType, p->SrcSlot);

	SendEffect(cId, 100, (Rand() % 6));

	AmountMinus(srcItem);
}

INT16 cantUse[] = {1, 2, 3, 4, 5, 6, 7, 8};

void OriLac(p373* p, bool* pRetn)
{
	INT16 cId = p->Header.ClientId;

	st_Mob* player = GetMobFromIndex(cId);
	nInfo* nB = &nBuffer[cId];

	st_Item *srcItem = GetItemPointer(player, Users[cId].Storage.Item, p->SrcType, p->SrcSlot),
		    *dstItem = GetItemPointer(player, Users[cId].Storage.Item, p->DstType, p->DstSlot);

#pragma region ITEM_INVALID
	if (dstItem == NULL || !dstItem->Index)
	{
		sendLog(WARN, cId, "%s %s dstItem null, OriLac func.", nB->Login, player->Name);
		return;
	}
	else if (p->DstType != EQUIP)
	{
		sendLog(WARN, cId, "%s %s dstItem null, OriLac func.", nB->Login, player->Name);
		return;
	}

	int dstVolatile = BASE_GetItemAbility(dstItem, EF_VOLATILE);
	if (dstVolatile)
	{
		sendLog(WARN, cId, "%s %s volatile on dstItem, OriLac func.", nB->Login, player->Name);
		return;
	}
#pragma endregion

#pragma region CANT_USE
	for (int i = 0; i < sizeof cantUse / sizeof INT16; i++)
	{
		if (dstItem->Index == cantUse[i])
		{
			SendItem(cId, p->SrcType, p->SrcSlot, srcItem);
			SendItem(cId, p->DstType, p->DstSlot, dstItem);

			SendClientMessage(cId, "Não pode fazer isso.");
			sendLog(WARN, cId, "%s %s tentou refinar %s.", nB->Login, player->Name, itemlistBuffer[dstItem->Index].Name);

			return;
		}
	}
#pragma endregion

//#pragma region CYTHERAS_BLOCK
//	if (dstItem->Index >= 3500 && dstItem->Index <= 3507)
//	{
//		SendClientMessage(cId, "Cytheras não podem ser refinadas desta forma.");
//		SendItem(cId, p->SrcType, p->SrcSlot, srcItem);
//
//		return;
//	}
//#pragma endregion

#pragma region PEDRAS_SAGRADAS
	if (dstItem->Index >= 1752 && dstItem->Index <= 1759)
	{
		int rand = Rand() % 100;

		if (srcItem->Index == 4141)
			rand = 0;

		sprintf(tmp, "Composição %s. ", (rand <= 55) ? "efetuada com sucesso" : "falhou");

		if (rand <= 55)
			dstItem->Index -= 8;

		// Remove a pedra refinada
		AmountMinus(srcItem);
		SendItem(cId, p->DstType, p->DstSlot, dstItem);

		SendClientMessage(cId, tmp);

		return;
	}
#pragma endregion

#pragma region OVOS
	else if (dstItem->Index >= 2300 && dstItem->Index < 2330)
	{
		*pRetn = false;

		return;
	}
#pragma endregion

	int srcItemType = srcItem->Index - 412;
	if (srcItem->Index == 4141)
		srcItemType = 2;
 
	int dstSanc    = GetItemSanc(dstItem),
		dstChances = g_pSancRate[srcItemType][dstSanc];

	if (dstSanc >= 11)
	{
		SendItem(cId, p->SrcType, p->SrcSlot, srcItem);
		SendItem(cId, p->DstType, p->DstSlot, dstItem);

		SendClientMessage(cId, "Não pode fazer isso.");
		sendLog(WARN, cId, "%s %s tentou refinar %s acima do +9 com poeira.", nB->Login, player->Name, itemlistBuffer[dstItem->Index].Name);

		return;

	}

	if ((dstItem->Index < 591 || dstItem->Index > 595) && dstSanc == 9)
	{
		SendItem(cId, p->SrcType, p->SrcSlot, srcItem);
		SendItem(cId, p->DstType, p->DstSlot, dstItem);

		SendClientMessage(cId, "Não pode fazer isso.");
		sendLog(WARN, cId, "%s %s tentou refinar %s acima do +9 com poeira.", nB->Login, player->Name, itemlistBuffer[dstItem->Index].Name);

		return;
	}

	if (dstItem->Index >= 591 && dstItem->Index <= 595 && dstSanc == 9)
		dstChances = 10;
	else if (dstChances <= 0)
	{
		SendItem(cId, p->SrcType, p->SrcSlot, srcItem);
		SendItem(cId, p->DstType, p->DstSlot, dstItem);

		SendClientMessage(cId, "Não pode fazer isso.");
		sendLog(WARN, cId, "%s %s tentou refinar %s acima do +9 com poeira.", nB->Login, player->Name, itemlistBuffer[dstItem->Index].Name);

		return;
	}

	srand(time(NULL) / 5 * (rand() % 500) * 5);
	int SuccessRate = rand() % 100;

#pragma region FALHA
	if (SuccessRate > dstChances)
	{
		AmountMinus(srcItem);

		// Falha
		if (dstSanc == 6 || (dstItem->Index >= 591 && dstItem->Index <= 595 && dstSanc == 9))
		{
			sendLog(INFO, cId, "%s %s quebrou %s na refinação para +%d.", nB->Login, player->Name, itemlistBuffer[dstItem->Index].Name, ++dstSanc);

			if (p->DstSlot == CAPE_SLOT)
				SendClientMsg(cId, "Falha na refinação (%d/%d)", SuccessRate, dstChances); 
			else if (dstItem->Index >= 3500 && dstItem->Index <= 3506)
				SendClientMsg(cId, "Falha na refinação (%d/%d)", SuccessRate, dstChances);

			else{
				memset(dstItem, 0, sizeof st_Item);

				SendClientMessage(cId, "Item quebrou!!");
			}
			SendItem(cId, p->DstType, p->DstSlot, dstItem);

			return;
		}

		if ((dstItem->Index < 591 || dstItem->Index > 595) && dstSanc < 230)
		{
			int dstItemSancSuccess = 0;

			if (rand() % 4 <= 2) //incrementa a rate de refinação
			{
				if (srcItemType == 1)
					dstItemSancSuccess = dstItemSancSuccess + 1;
				else
				{
					if (dstSanc <= 5 && srcItemType == 0)
						dstItemSancSuccess = dstItemSancSuccess + 1;
				}
			}

			if (dstItem->Index >= 2300 && dstItem->Index < 2330)
			{
				dstItem->EF3 = EF_INCUDELAY;
				dstItem->EFV3 = (rand() % 4);
			}

			BASE_SetItemSanc(dstItem, dstSanc, dstItemSancSuccess);
		}

		SendClientMsg(cId, "Falha na refinação (%d/%d)", SuccessRate, dstChances);
		SendItem(cId, p->DstType, p->DstSlot, dstItem);
		sendLog(INFO, cId, "%s %s falhou na refinação de %s.", nB->Login, player->Name, itemlistBuffer[dstItem->Index].Name);

		int Body = player->Equip[0].Index / 10; 
		if (Body == 0)
			SendEmotion(cId, 20, 0);
		else if (Body == 1)
			SendEmotion(cId, 15, 0);
		else if (Body == 2)
			SendEmotion(cId, 15, 0);
		else if (Body == 3)
			SendEmotion(cId, 15, 0);

		return;
	}
#pragma endregion
	
	else if (dstSanc < 9)
		dstSanc = dstSanc + 1;

	else if (dstSanc == 9)
	{
		if (player->Inventory[0].Index < 2441 || player->Inventory[0].Index > 2444)
		{
			SendItem(cId, p->SrcType, p->SrcSlot, srcItem);
			SendClientMessage(cId, "Posicione uma joia no primeiro slot.");

			return;
		}

		dstSanc = 230 + (player->Inventory[0].Index - 2441);

		AmountMinus(&player->Inventory[0]);

		SendItem(cId, INVENTORY, 0, &player->Inventory[0]);
	}

	else // dstSanc == 10 
		dstSanc = GetItemSanc_OL2(dstItem) + 4;

	if (!SetItemSanc(dstItem, dstSanc))
	{
		sendLog(WARN, cId, "Erro desconhecido OriLac Func %s %s", nB->Login, player->Name);
		SendClientMessage(cId, "Impossível Refinar.");
		SendItem(cId, p->SrcType, p->SrcSlot, srcItem);

		return;
	}

	AmountMinus(srcItem);

	SendItem(cId, p->DstType, p->DstSlot, dstItem);

	SendEmotion(cId, 14, 3);

	SendClientMsg(cId, "Sucesso na refinação (%d/%d)", SuccessRate, dstChances);
	sendLog(INFO, cId, "%s %s refinou %s para %d.", nB->Login, player->Name, itemlistBuffer[dstItem->Index].Name, dstSanc);
}

void Water(p373* p)
{
	INT16 cId = p->Header.ClientId;

	st_Mob *player = GetMobFromIndex(cId);
	nInfo*  user = &nBuffer[cId];

	st_Item *Item = &player->Inventory[p->SrcSlot];

	if ((player->TargetX >> 2) != 491 || (player->TargetY >> 2) != 443)
	{
		if (player->TargetX < 1038 || player->TargetX > 1389 || player->TargetY < 3470 || player->TargetY > 3694)
		{
			SendItem(cId, p->SrcType, p->SrcSlot, Item);
			SendClientMessage(cId, "Use somente na zona elemental da água.");

			return;
		}
	}

	INT8 Type = -1;
	UINT8 Sala = -1;

	switch (Item->Index)
	{
		case 3173: case 3174: case 3175: case 3176:
		case 3177: case 3178: case 3179: case 3180:
		{
			Type = 0;
			Sala = Item->Index - 3173;
			break;
		}

		case 777:  case 778:  case 779:  case 780:
		case 781:  case 782:  case 783:  case 784:
		{
			Type = 1;
			Sala = Item->Index - 777;
			break;
		}

		case 3182: case 3183: case 3184: case 3185:
		case 3186: case 3187: case 3188: case 3189:
		{
			Type = 2;
			Sala = Item->Index - 3182;
			break;
		}

		default:
			break;
	}

	if (Type == -1 || Sala == -1 || Type > 2 || Sala > 7)
	{
		SendItem(cId, INVENTORY, p->SrcSlot, &player->Inventory[p->SrcSlot]);

		SendClientMessage(cId, "Entre em contato com a administração, erro de typing.");

		return;
	}

	if (Agua[Type].Rooms[Sala].isActive)
	{
		// Alguém já entrou nesta sala

		SendItem(cId, INVENTORY, p->SrcSlot, &player->Inventory[p->SrcSlot]);

		SendClientMessage(cId, "Esta sala já está ocupada.");

		return;
	}

	if (player->Leader != 0 && player->Leader != -1)
	{
		SendClientMessage(cId, "Uso exclusivo do líder do grupo.");

		SendItem(cId, INVENTORY, p->SrcSlot, &player->Inventory[p->SrcSlot]);

		return;
	}

	AmountMinus(&player->Inventory[p->SrcSlot]);

	st_Position Dest;

	// Zero a var pra evitar dados desconhecidos atrapalhando o funcionamento
	memset(&Dest, 0x0, sizeof(st_Position));

	// Pega as coords pela struct
	Dest.X = WaterCoords[Type][Sala][0];
	Dest.Y = WaterCoords[Type][Sala][1];

	// Marca a sala como ativa e fixa o tempo restante
	Agua[Type].Rooms[Sala].isActive = true;
	Agua[Type].Rooms[Sala].Leaders = cId;
	Agua[Type].Rooms[Sala].Cids[0] = cId;
	Agua[Type].Rooms[Sala].TimeLeft = 120;
	Agua[Type].Rooms[Sala].MobsLeft = WaterNum[Type][Sala];

	DoTeleport(cId, Dest.X, Dest.Y);

	// Manda o tempo verde
	SendClientSignalParm(cId, 0x7530, 0x3A1, 120);

	// Mata os possíveis mobs que já estejam lá
	KillMobsOnArea(WaterMMCoords[Type][Sala][0], WaterMMCoords[Type][Sala][1], WaterMMCoords[Type][Sala][2], WaterMMCoords[Type][Sala][3]);

	// Manda o número de Mobs
	NpcsAlive(cId, WaterNum[Type][Sala], WaterNum[Type][Sala]);

	int e = 0;

	while (e < WaterNum[Type][Sala])
	{
		e++;

		CreateMob(WaterMobs[Type][Sala], Dest.X, Dest.Y, "npc");
	}

	memcpy(Agua[Type].Rooms[Sala].Cids, player->PartyList, 24);

	for (INT8 i = 0; i < 12; i++)
	{
		int mcId = player->PartyList[i];

		if (mcId < 0 || mcId > MAX_PLAYER)
			continue;

		if (Users[mcId].Status != 22)
			continue;

		Agua[Type].Rooms[Sala].Cids[i] = mcId;

		// Teleporta o pessoal
		DoTeleport(mcId, Dest.X, Dest.Y);

		// Tempo verde
		SendClientSignalParm(mcId, 0x7530, 0x3A1, 120);
		// Manda o número de Mobs
		NpcsAlive(mcId, WaterNum[Type][Sala], WaterNum[Type][Sala]);
	}
}

void NightMare(p373* p, st_Item* Item)
{
	if (Item->Index < 3324 || Item->Index > 3326)
		return;

	INT8  PesaType = Item->Index - 3324;
	INT16 cId      = p->Header.ClientId;

	st_Mob* player = GetMobFromIndex(cId);
	nInfo*  user   = &nBuffer[cId];
	Pesas*  pesa   = &Pesa[PesaType];

	// Pega os dados do tempo atual
	time_t rawnow = time(NULL);
	struct tm *now = localtime(&rawnow);

	if (now->tm_min && now->tm_min != 15 && now->tm_min != 30 && now->tm_min != 45)
	{
		SendClientMessage(cId, "Somente nos minutos 00, 15, 30 e 45.");
		SendItem(cId, p->SrcType, p->SrcSlot, Item);

		return;
	}
	else if (player->Leader || player->Leader == -1)
	{
		SendClientMessage(cId, "Uso exclusivo do líder do grupo.");
		SendItem(cId, p->SrcType, p->SrcSlot, Item);

		return;
	}
	else if (pesa->Status)
	{
		SendClientMessage(cId, "O pesadelo já começou!");
		SendItem(cId, p->SrcType, p->SrcSlot, Item);

		return;
	}
	else if (pesa->Groups >= 3)
	{
		SendClientMessage(cId, "Máximo de grupos atingido!");
		SendItem(cId, p->SrcType, p->SrcSlot, Item);

		return;
	}
	
	st_Position FinalPosition;
	static const INT16 Destinys[3][2] =
	{
		{ 1310, 313 },
		{ 1093, 310 },
		{ 1205, 173 }
	};

	FinalPosition.X = Destinys[PesaType][0];
	FinalPosition.Y = Destinys[PesaType][1];

	AmountMinus(Item);

	pesa->Groups++;

	for (int i = 0; i < 3; i++)
	{
		if (!pesa->cId[i][0])
		{
			pesa->cId[i][0] = cId;
			pesa->TimeLeft  = 60 - now->tm_sec;

			DoTeleport(cId, FinalPosition.X, FinalPosition.Y);
			SendClientSignalParm(cId, 0x7530, 0x3A1, pesa->TimeLeft);

			for (int e = 0; e < 12; e++)
			{
				int mcId = player->PartyList[e];

				if (mcId > 740)
					continue;
				else if (Users[mcId].Status != 22)
					continue;

				pesa->cId[i][e + 1] = mcId;

				DoTeleport(mcId, FinalPosition.X, FinalPosition.Y);

				SendClientSignalParm(mcId, 0x7530, 0x3A1, pesa->TimeLeft);
			}

			break;
		}
	}
}

void BarrasMithril(p373* p)
{
	INT16 cId = p->Header.ClientId;

	st_Mob *player = GetMobFromIndex(cId);

	st_Item *srcItem = GetItemPointer(player, Users[cId].Storage.Item, p->SrcType, p->SrcSlot),
		*dstItem = GetItemPointer(player, Users[cId].Storage.Item, p->DstType, p->DstSlot);

	static const UINT8 Add[4][2] = {
		{ 6, 9 },
		{ 2, 4 },
		{ 6, 0 },
		{ 5, 0 }
	};

	static const UINT8 AddTypes[2][4] = {
		{ EF_DAMAGE, EF_MAGIC, EF_AC, EF_CRITICAL },
		{ EF_DAMAGEADD, EF_MAGICADD, EF_ACADD, EF_CRITICAL2 }
	};

	static const UINT8 MaxAdds[4][2] = {
		{ 42, 81 },
		{ 14, 36 },
		{ 70, 0 },
		{ 90, 30 }
	};

	if (srcItem->Index < 3027 || srcItem->Index > 3030)
		return;

	UINT8 Type = srcItem->Index - 3027;

	if (Type < 0 || Type > 3)
	{
		SendClientMessage(cId, "Erro no Type do Item, entre em contato com à Administração.");

		SendItem(cId, p->SrcType, p->SrcSlot, srcItem);

		return;
	}

	UINT8 Armor = 0;

	INT8 pos = itemlistBuffer[dstItem->Index].Pos;

	if (pos == ARMA_TYPE1 || pos == ARMA_TYPE2)
		Armor = 1;
	else if (pos != ELMO_TYPE && pos != PEITO_TYPE && pos != LUVA_TYPE && pos != BOTA_TYPE  && pos != CALCA_TYPE)
		Armor = -1;

	if (Armor == -1)
	{
		SendClientMessage(cId, "Possível usar somente em armas ou armaduras.");

		SendItem(cId, p->SrcType, p->SrcSlot, srcItem);

		return;
	}

	INT8 place = -1;

	for (int e = 0; e < 3; e++)
	{
		if (dstItem->Effect[e].Index == AddTypes[0][Type] || dstItem->Effect[e].Index == AddTypes[1][Type])
		{
			// Acha em qual slot dos efs no item está o add
			place = e;

			break;
		}
	}

	if (place == -1)
	{
		SendClientMessage(cId, "Necessário que o item tenha o adicional que deseja aprimorar.");
		// O item não tem o adicional para o item que ele usou
		SendItem(cId, p->SrcType, p->SrcSlot, srcItem);

		return;
	}
	else if (dstItem->Effect[place].Value <= Add[Type][Armor] ||
		(dstItem->Effect[place].Value + Add[Type][Armor]) > MaxAdds[Type][Armor] ||
		GetItemSanc(dstItem) <= 9 ||
		(!CheckArchItem(dstItem->Index) && !CheckHardItem(dstItem->Index) && !CheckEItem(dstItem->Index)))
	{
		SendClientMessage(cId, "Não é possível aprimorar este item.");
		// Adicional já passou do limite ou é muito pequeno
		SendItem(cId, p->SrcType, p->SrcSlot, srcItem);

		return;
	}

	AmountMinus(srcItem);

	srand(time(NULL) / 5 * (rand() % 500) * 5);

	INT8 Chance = rand() % 10;

	if (Chance < 2)
	{
		// Falha na composição, item volta o add
		dstItem->Effect[place].Value -= Add[Type][Armor];

		SendClientMessage(cId, "Falha na composição, houve queda do adicional.");
	}
	else if (Chance <= 6)
	{
		// Acertou, add sobe
		dstItem->Effect[place].Value += Add[Type][Armor];

		SendClientMessage(cId, "Houve sucesso na refinação.");
	}
	else
		SendClientMessage(cId, "Falha na composição.");

	SendItem(cId, p->DstType, p->DstSlot, dstItem);
}
void Barras(p373* p)
{
	INT16 cId = p->Header.ClientId;

	st_Mob *player = (st_Mob*)GetMobFromIndex(cId);

	st_Item *srcItem = GetItemPointer(player, Users[cId].Storage.Item, p->SrcType, p->SrcSlot),
		*dstItem = GetItemPointer(player, Users[cId].Storage.Item, p->DstType, p->DstSlot);

	unsigned long int valor_gold;

	if (srcItem->Index == 4010)
		valor_gold = 100000000;
	else if (srcItem->Index == 4011)
		valor_gold = 1000000000;
	else if (srcItem->Index == 4026)
		valor_gold = 1000000;
	else if (srcItem->Index == 4027)
		valor_gold = 5000000;
	else if (srcItem->Index == 4028)
		valor_gold = 10000000;
	else if (srcItem->Index == 4029)
		valor_gold = 50000000;
	else if (srcItem->Index == 5359)
		valor_gold = 500000000;
	else if (srcItem->Index == 5360)
		valor_gold = 2000000000;
	else
		valor_gold = 0;

	if (player->Gold > 2000000000 - valor_gold)
	{
		SendItem(cId, INVENTORY, p->SrcSlot, srcItem);

		SendClientMessage(cId, "Limite máximo de 2.000.000.000 (Dois bilhões) de gold.");

		return;
	}

	player->Gold += valor_gold;

	AmountMinus(srcItem);

	SendEtc(cId);

	return;
}

void QuestItens(p373* p)
{
	INT16 cId = p->Header.ClientId;

	st_Mob *player = GetMobFromIndex(cId);

	st_Item *Item = GetItemPointer(player, Users[cId].Storage.Item, p->SrcType, p->SrcSlot);

	if (Item->Index < 4117 || Item->Index > 4121)
		return;

	INT8 ActualQuest = -1;

	char szMsg[108];

	ActualQuest = Item->Index - 4117;

	if (ActualQuest < 0 || ActualQuest > 4)
	{
		SendItem(cId, INVENTORY, p->SrcSlot, Item);

		SendClientMessage(cId, "Erro no Type do item, entre em contato com à Administração.");

		return;
	}

	if (player->bStatus.Level < Quests_Level[ActualQuest][0] || player->bStatus.Level > Quests_Level[ActualQuest][1] || player->Equip[0].EFV2 != MORTAL)
	{
		sprintf(szMsg, "Somente mortais entre os níveis: %d e %d", Quests_Level[ActualQuest][0] + 1, Quests_Level[ActualQuest][1] + 1);

		SendClientMessage(cId, szMsg);

		SendItem(cId, INVENTORY, p->SrcSlot, Item);

		return;
	}

	AmountMinus(Item);

	player->Exp += ExpQuests[ActualQuest];

	int gold = player->bStatus.Level * 50;

	if ((player->Gold + gold) > 2000000000)
		gold = (2000000000 - gold);

	player->Gold += gold;

	INT16 lcId = 0;
	if (player->Leader && player->Leader != -1)
		lcId = player->Leader;
	else
		lcId = cId;
	
	st_Mob *Lider = GetMobFromIndex(lcId);

	if (lcId != cId)
	{
		// Caso o líder do grupo não seja quem usou o item
		Lider->Exp += ExpQuests[ActualQuest] / 10;

		if (Lider->Gold + (ExpQuests[ActualQuest] / 10) > 2000000000)
			Lider->Gold = 2000000000;
		else
			Lider->Gold + (ExpQuests[ActualQuest] / 10);

		SendEtc(lcId);
		SendClientMsg(lcId, "++++ %d ++++", ExpQuests[ActualQuest] / 10);
	}

	for (int i = 0; i < 12; i++)
	{
		if (!Lider->PartyList[i])
			continue;

		else if (Lider->PartyList[i] < 0 || Lider->PartyList[i] > MAX_PLAYER)
			continue;

		else if (Lider->PartyList[i] == cId)
			continue;

		else if (Users[Lider->PartyList[i]].Status != 22)
			continue;

		st_Mob *tmpMob = GetMobFromIndex(Lider->PartyList[i]);

		tmpMob->Exp += ExpQuests[ActualQuest] / 10;

		SendClientMsg(Lider->PartyList[i], "++++ %d ++++", ExpQuests[ActualQuest] / 10);

		SendEtc(Lider->PartyList[i]);

		SendEffect(Lider->PartyList[i], 0x0E, 1);
	}

	sprintf(szMsg, "++++ %d ++++", ExpQuests[ActualQuest]);

	SendClientMessage(cId, szMsg);

	SendEtc(cId);

	SendEffect(cId, 0x0E, 1);
}

void Extracoes(p373* p)
{
	INT16 cId = p->Header.ClientId;

	st_Mob *player = (st_Mob*)GetMobFromIndex(cId);

	st_Item *srcItem = GetItemPointer(player, Users[cId].Storage.Item, p->SrcType, p->SrcSlot);
		    
	int ExtractionType = srcItem->Index - 3020;

	if (!player->Equip[ExtractionType].Index)
	{
		SendClientMessage(cId, "Equipe um item para utilizar a extração.");

		SendItem(cId, p->SrcType, p->SrcSlot, srcItem);

		return;
	}

	int sanc = GetItemSanc(&player->Equip[ExtractionType]);
	if (sanc > 9)
	{
		SendClientMessage(cId, "Somente em itens com refinação igual ou inferior a +9.");
		SendItem(cId, p->SrcType, p->SrcSlot, srcItem);

		return;
	}

	if (!srcItem->EF1 && !srcItem->EF2 && !srcItem->EF3)
	{
		memcpy(&srcItem->EF1, &player->Equip[ExtractionType].EF1, 6);
		memset(&player->Equip[ExtractionType].EF1, 0, 6);
		
		SendClientMessage(cId, "Adicional extraido com sucesso.");
	}
	else
	{
		memcpy(&player->Equip[ExtractionType].EF1, &srcItem->EF1, 6);
		AmountMinus(srcItem);

		SendClientMessage(cId, "Adicional entregue com sucesso.");
	}

	SendItem(cId, EQUIP, ExtractionType, &player->Equip[ExtractionType]);
	SendItem(cId, p->SrcType, p->SrcSlot, srcItem);
}

static const int szRefis[6][2] =
{
	{ 230, 233 },
	{ 234, 237 },
	{ 238, 241 },
	{ 242, 245 },
	{ 246, 249 },
	{ 250, 253 }
};

void Gemas(p373 *p)
{
	INT16 cId = p->Header.ClientId;

	st_Mob *player = (st_Mob*)GetMobFromIndex(cId);

	st_Item *srcItem = GetItemPointer(player, Users[cId].Storage.Item, p->SrcType, p->SrcSlot),
		*dstItem = GetItemPointer(player, Users[cId].Storage.Item, p->DstType, p->DstSlot);

	if (p->DstType != EQUIP || dstItem == NULL)
		return;

	if (srcItem->Index < 3386 || srcItem->Index > 3389)
	{
		SendItem(cId, INVENTORY, p->SrcSlot, srcItem);

		SendClientMessage(cId, "Erro, entre em contato com a Administração.");

		return;
	}

	if (GetItemSanc_OL2(dstItem) < 230 || GetItemSanc_OL2(dstItem) > 253)
	{
		SendItem(cId, INVENTORY, p->SrcSlot, srcItem);

		SendClientMessage(cId, "Não há como usar em refinações abaixo de +10.");

		return;
	}

	if (dstItem->Index >= 3500 && dstItem->Index <= 3507)
	{
		SendItem(cId, INVENTORY, p->SrcSlot, srcItem);

		SendClientMessage(cId, "Não é possível utilizar em Cytheras.");

		return;
	}

	int Pos = itemlistBuffer[dstItem->Index].Pos;
	int Sanc = GetItemSanc_OL2(dstItem);

	if (Pos == ELMO_TYPE  || Pos == PEITO_TYPE ||
		Pos == CALCA_TYPE || Pos == LUVA_TYPE ||
		Pos == BOTA_TYPE  || Pos == ESCUDO_TYPE)
	{
		for (int i = 0; i < 6; i++)
		{
			if (Sanc >= szRefis[i][0] && Sanc <= szRefis[i][1])
			{
				SetItemSanc(dstItem, szRefis[i][0] + srcItem->Index - 3386);

				AmountMinus(srcItem);

				SendItem(cId, EQUIP, p->DstSlot, dstItem);

				SendEffect(cId, 0xE, 3);

				break;
			}
		}
	}
	else
	{
		if (Pos == ARMA_TYPE1 || Pos == ARMA_TYPE2)
		{
			if (itemlistBuffer[dstItem->Index].Grade < 5 || itemlistBuffer[dstItem->Index].Grade > 8)
			{
				SendItem(cId, INVENTORY, p->SrcSlot, srcItem);

				SendClientMessage(cId, "Há algo errado com este item.");

				return;
			}

			for (int i = 0; i < 6; i++)
			{
				if (Sanc >= szRefis[i][0] && Sanc <= szRefis[i][1])
				{
					if (itemlistBuffer[dstItem->Index].Grade >= 5 && itemlistBuffer[dstItem->Index].Grade <= 8)
						dstItem->Index += (srcItem->Index - 3386 + 5 - itemlistBuffer[dstItem->Index].Grade);

					SetItemSanc(dstItem, szRefis[i][0] + srcItem->Index - 3386);

					AmountMinus(srcItem);

					SendItem(cId, EQUIP, p->DstSlot, dstItem);

					SendEffect(cId, 0xE, 3);

					break;
				}
			}
		}
	}
}

void Tinturas(p373 *p)
{
	INT16 cId = p->Header.ClientId;

	st_Mob *player = (st_Mob*)GetMobFromIndex(cId);

	st_Item *srcItem = GetItemPointer(player, Users[cId].Storage.Item, p->SrcType, p->SrcSlot),
		*dstItem = GetItemPointer(player, Users[cId].Storage.Item, p->DstType, p->DstSlot);

	if (p->DstType != EQUIP || dstItem == NULL)
		return;

	if (GetItemSanc_OL2(dstItem) < 230)
	{
		SendItem(cId, INVENTORY, p->SrcSlot, srcItem);

		SendClientMessage(cId, "Você só pode utilizar em equipamentos superiores a +9.");

		return;
	}

	for (int i = 0; i < 3; i++)
	{
		if (srcItem->Effect[i].Index == EF_SANC)
		{
			SendItem(cId, INVENTORY, p->SrcSlot, srcItem);

			SendClientMessage(cId, "Você só pode utilizar em equipamentos sem tintura.");

			return;
		}
	}

	if (p->DstSlot < 1 || p->DstSlot > 6)
	{
		SendItem(cId, INVENTORY, p->SrcSlot, srcItem);

		SendClientMessage(cId, "Você só pode utilizar em equipamentos de defesa.");

		return;
	}

	for (int i = 0; i < 3; i++)
		if (dstItem->Effect[i].Index == 43 || (dstItem->Effect[i].Index >= 116 && dstItem->Effect[i].Index <= 125))
			dstItem->Effect[i].Index = srcItem->Index - 3291;

	AmountMinus(srcItem);

	SendItem(cId, EQUIP, p->DstSlot, dstItem);

	return;
}

void RemoveTinturas(p373 *p)
{
	INT16 cId = p->Header.ClientId;
	
	st_Mob *player = (st_Mob*)GetMobFromIndex(cId);

	st_Item *srcItem = GetItemPointer(player, Users[cId].Storage.Item, p->SrcType, p->SrcSlot),
		*dstItem = GetItemPointer(player, Users[cId].Storage.Item, p->DstType, p->DstSlot);

	if (p->DstType != EQUIP || dstItem == NULL)
		return;

	if (GetItemSanc_OL2(dstItem) < 230)
	{
		SendItem(cId, INVENTORY, p->SrcSlot, srcItem);

		SendClientMessage(cId, "Você só pode utilizar em equipamentos superiores a +9.");

		return;
	}

	if (p->DstSlot < 1 || p->DstSlot > 6)
	{
		SendItem(cId, INVENTORY, p->SrcSlot, srcItem);

		SendClientMessage(cId, "Você só pode utilizar em equipamentos de defesa.");

		return;
	}

	for (int i = 0; i < 3; i++)
		if (dstItem->Effect[i].Index >= 116 && dstItem->Effect[i].Index <= 125)
			dstItem->Effect[i].Index = 43;

	AmountMinus(srcItem);

	SendItem(cId, EQUIP, p->DstSlot, dstItem);

	return;
}

void ProcessSephiraBook(UINT8* m_PacketBuffer, bool* pRetn)
{
	p373* p = (p373*)m_PacketBuffer;

	INT16 cId = p->Header.ClientId;

	st_Mob *player = GetMobFromIndex(cId);

	const WORD BooksSkill[5] = { 0, 36, 35, 40, 39 };
	int _index = player->Inventory[p->SrcSlot].Index - 666;

	if (_index <= 0 || _index > 5)
	{
		SendItem(cId, INVENTORY, p->SrcSlot, &player->Inventory[p->SrcSlot]);
		*pRetn = true;

		return;
	}

	int has = (player->Learn & (1 << (24 + _index)));
	if (has)
	{
		SendClientMessage(cId, "Já aprendeu esta skill anteriormente.");

		SendItem(cId, INVENTORY, p->SrcSlot, &player->Inventory[p->SrcSlot]);
		*pRetn = true;

		return;
	}

	if (player->SkillPoint < BooksSkill[_index - 1])
	{
		SendClientMessage(cId, "Não possui pontos de skill suficientes.");

		SendItem(cId, INVENTORY, p->SrcSlot, &player->Inventory[p->SrcSlot]);
		*pRetn = true;

		return;
	}

	if (player->Equip[0].EFV2 <= ARCH && player->bStatus.Level < 50)
	{
		SendClientMessage(cId, "Level inadequado.");

		SendItem(cId, INVENTORY, p->SrcSlot, &player->Inventory[p->SrcSlot]);
		*pRetn = true;

		return;
	}

	player->SkillPoint -= BooksSkill[_index - 1];
	player->Learn |= 1 << (_index + 24);

	SendClientMessage(cId, "Você aprendeu a skill com sucesso!");

	GetCurrentScore(cId);
	SendStats(cId);

	AmountMinus(&player->Inventory[p->SrcSlot]);

}

void ProcessHuntingScrolls(UINT8* m_PacketBuffer, bool* pRetn)
{
	p373* p = (p373*)m_PacketBuffer;

	INT16 cId = p->Header.ClientId;

	st_Mob *player = GetMobFromIndex(cId);

	if ((player->Inventory[p->SrcSlot].Index < 3432 || player->Inventory[p->SrcSlot].Index > 3437) ||
		p->warp > 10 || (p->warp - 1 < 0))
	{
		*pRetn = true;
		SendItem(cId, INVENTORY, p->SrcSlot, &player->Inventory[p->SrcSlot]);
		return;
	}

	p->warp--;
	int ID = player->Inventory[p->SrcSlot].Index - 3432;
	int tX = HuntingScrolls[ID][p->warp][0];
	int tY = HuntingScrolls[ID][p->warp][1];

	AmountMinus(&player->Inventory[p->SrcSlot]);
	DoTeleport(cId, tX, tY);

	*pRetn = true;
}

void AgruparPoeiras(UINT8* m_PacketBuffer, bool* pRetn)
{
	p373* p = (p373*)m_PacketBuffer;

	INT16 cId = p->Header.ClientId;

	st_Mob *player = GetMobFromIndex(cId);

	int i = 0, amount = 0, index = (player->Inventory[p->SrcSlot].Index - 3033);

	for (; i < 63; i++){
		if (player->Inventory[i].Index == index && (player->Inventory[i].EF1 != EF_AMOUNT || player->Inventory[i].EFV1 <= 1))
		{
			amount++;
			AmountMinus(&player->Inventory[i]);
		}
	}

	if (amount != 0){
		int slot = GetFirstSlot(cId, 0, INVENTORY);
		player->Inventory[slot].Index = (index + 3035);
		player->Inventory[slot].EF1 = EF_AMOUNT;
		player->Inventory[slot].EFV1 = amount;
		AmountMinus(&player->Inventory[p->SrcSlot]);
	}

	SendAll(cId);

	*pRetn = true;
}

void DesagruparPoeiras(UINT8* m_PacketBuffer, bool* pRetn)
{
	p373* p = (p373*)m_PacketBuffer;

	INT16 cId = p->Header.ClientId;

	st_Mob *player = GetMobFromIndex(cId);

	if (GetFirstSlot(cId, 0, INVENTORY) == -1)
	{
		SendAll(cId);
		*pRetn = true;

		return;
	}

	int amount = 0, index = (player->Inventory[p->SrcSlot].Index - 3035), slot = 0;

	amount = player->Inventory[p->SrcSlot].EFV1;

	while (amount > 0 && GetFirstSlot(cId, 0, INVENTORY) != -1){
		amount--;
		slot = GetFirstSlot(cId, 0, INVENTORY);
		player->Inventory[slot].Index = index;
		AmountMinus(&player->Inventory[p->SrcSlot]);
	}

}

void Replations(UINT8* m_PacketBuffer, bool* pRetn)
{
	p373* p = (p373*)m_PacketBuffer;

	INT16 cId = p->Header.ClientId;

	st_Mob *player = GetMobFromIndex(cId);

	*pRetn = true;

	st_Item *destItem = (st_Item*)&player->Inventory[p->DstSlot],
		*srcItem = (st_Item*)&player->Inventory[p->SrcSlot];

	if (BASE_GetItemSanc(destItem) > 4)
	{
		SendClientMessage(cId, "Somente itens com refinação igual ou inferior a 4");
		SendItem(cId, INVENTORY, p->SrcSlot, srcItem);
		return;
	}

	int CheckRefine = CheckReplation(destItem->Index);

	for (int i = 0; i < sizeof(NotReplation) / sizeof(int); i++){
		if (destItem->Index == NotReplation[i])
		{
			SendItem(cId, INVENTORY, p->SrcSlot, srcItem);
			return;
		}
	}

	if (CheckRefine == REPLATION_ERROR)
	{
		SendItem(cId, INVENTORY, p->SrcSlot, srcItem);
		return;
	}

	int successRate = Rand() % 100;
	BOOL doRefine = FALSE;

	switch (srcItem->Index)
	{
		case 4016: case 4021:
			if (CheckRefine == REPLATION_A_TYPE)
			{
				if (successRate <= 95)
					doRefine = true;
			}
			else
			{
				SendItem(cId, INVENTORY, p->SrcSlot, srcItem);

				return;
			}
			break;
		case 4017: case 4022:
			if (CheckRefine == REPLATION_B_TYPE)
			{
				if (successRate <= 90)
					doRefine = true;
			}
			else
			{
				SendItem(cId, INVENTORY, p->SrcSlot, srcItem);

				return;
			}
			break;
		case 4018: case 4023:
			if (CheckRefine == REPLATION_C_TYPE)
			{
				if (successRate <= 85)
					doRefine = true;
			}
			else
			{
				SendItem(cId, INVENTORY, p->SrcSlot, srcItem);

				return;
			}
			break;
		case 4019: case 4024:
			if (CheckRefine == REPLATION_D_TYPE)
			{
				if (successRate <= 80)
					doRefine = true;
			}
			else
			{
				SendItem(cId, INVENTORY, p->SrcSlot, srcItem);

				return;
			}
			break;
		case 4020:
			if (CheckRefine == REPLATION_E_TYPE)
			{
				if (successRate <= 60)
					doRefine = true;
			}
			else
			{
				SendItem(cId, INVENTORY, p->SrcSlot, srcItem);

				return;
			}
			break;
	}

	if (doRefine)
	{
		int ef2 = 0, efv2 = 0,
			ef3 = 0, efv3 = 0,

			effs_elmo[5] = { EF_NONE, EF_ATTSPEED, EF_MAGIC, EF_HP, EF_ACADD },
			effs_armadura[5] = { EF_NONE, EF_DAMAGE, EF_CRITICAL, EF_MAGIC, EF_ACADD },
			effs_calca[5] = { EF_NONE, EF_DAMAGE, EF_CRITICAL, EF_MAGIC, EF_ACADD },
			effs_manopla[5] = { EF_NONE, EF_DAMAGE, EF_SPECIALALL, EF_MAGIC, EF_ACADD },
			effs_bota[4] = { EF_NONE, EF_DAMAGE, EF_SPECIALALL, EF_MAGIC };

		INT16 type = GetItemlistBuffer()[destItem->Index].Pos;
		if (type == ELMO_TYPE)
		{
			ef2 = effs_elmo[Rand() % 5];
			ef3 = effs_elmo[Rand() % 5];
		}
		else if (type == PEITO_TYPE)
		{
			ef2 = effs_armadura[Rand() % 5];
			ef3 = effs_armadura[Rand() % 5];
		}
		else if (type == CALCA_TYPE)
		{
			ef2 = effs_calca[Rand() % 5];
			ef3 = effs_calca[Rand() % 5];
		}
		else if (type == LUVA_TYPE)
		{
			ef2 = effs_manopla[Rand() % 5];
			ef3 = effs_manopla[Rand() % 5];
		}
		else if (type == BOTA_TYPE)
		{
			ef2 = effs_bota[Rand() % 4];
			ef3 = effs_bota[Rand() % 4];
		}

		if (ef2 == EF_NONE)
			ef2 = ef3;
		if (ef2 == ef3)
			ef3 = EF_NONE;

		if (ef2 != EF_NONE)
		{
			if (ef2 == EF_ATTSPEED)
				efv2 = add_attspeed[Rand() % (sizeof(add_attspeed) / sizeof(int))];
			else if (ef2 == EF_CRITICAL)
				efv2 = add_critical[Rand() % (sizeof(add_critical) / sizeof(int))];
			else if (ef2 == EF_ACADD)
				efv2 = add_ac[Rand() % (sizeof(add_ac) / sizeof(int))];
			else if (ef2 == EF_DAMAGE)
				efv2 = add_damage[Rand() % (sizeof(add_damage) / sizeof(int))];
			else if (ef2 == EF_MAGIC)
				efv2 = add_magic[Rand() % (sizeof(add_magic) / sizeof(int))];
			else if (ef2 == EF_HP)
				efv2 = add_hp[Rand() % (sizeof(add_hp) / sizeof(int))];
			else if (ef2 == EF_SPECIALALL)
				efv2 = add_skill[Rand() % (sizeof(add_skill) / sizeof(int))];
		}
		if (ef3 != EF_NONE)
		{
			if (ef3 == EF_ATTSPEED)
				efv3 = add_attspeed[Rand() % (sizeof(add_attspeed) / sizeof(int))];
			else if (ef3 == EF_CRITICAL)
				efv3 = add_critical[Rand() % (sizeof(add_critical) / sizeof(int))];
			else if (ef3 == EF_ACADD)
				efv3 = add_ac[Rand() % (sizeof(add_ac) / sizeof(int))];
			else if (ef3 == EF_DAMAGE)
				efv3 = add_damage[Rand() % (sizeof(add_damage) / sizeof(int))];
			else if (ef3 == EF_MAGIC)
				efv3 = add_magic[Rand() % (sizeof(add_magic) / sizeof(int))];
			else if (ef3 == EF_HP)
				efv3 = add_hp[Rand() % (sizeof(add_hp) / sizeof(int))];
			else if (ef3 == EF_SPECIALALL)
				efv3 = add_skill[Rand() % (sizeof(add_skill) / sizeof(int))];
		}

		destItem->EF1 = 43;
		destItem->EFV1 = Rand() % 4;
		destItem->EF2 = ef2;
		destItem->EFV2 = efv2;
		destItem->EF3 = ef3;
		destItem->EFV3 = efv3;

		SendItem(cId, INVENTORY, p->DstSlot, destItem);
	}
	else
		SendClientMessage(cId, "Refinação falhou.");

	AmountMinus(srcItem);
}


