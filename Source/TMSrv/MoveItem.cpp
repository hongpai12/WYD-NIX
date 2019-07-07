#include "DLLMain.h"

static const INT16 NotDesequip[] = {508, 509, 526, 527, 528, 3503, 3504, 3505, 3506};

void VerifyItemMovement(BYTE *m_PacketBuffer, bool *pRetn)
{
	p376 *p = (p376*)m_PacketBuffer;

	INT16 cId = p->Header.ClientId;

	if(cId < 0 || cId > 750)
		return;

	st_Mob *player = GetMobFromIndex(cId);

	st_Item *srcItem = GetItemPointer(player, Users[cId].Storage.Item, p->SrcType, p->SrcSlot),
		    *dstItem = GetItemPointer(player, Users[cId].Storage.Item, p->DstType, p->DstSlot);

	if (srcItem == NULL || srcItem->Index < 0 || srcItem->Index > 6500)
		return;

	time_t rawnow = time(NULL);
	struct tm *now = localtime(&rawnow);

	if(srcItem != NULL)
	{
		if (CheckArchItem(srcItem->Index) && p->DstType == EQUIP && player->Equip[0].EFV2 < SEMIDEUS)
		{
			*pRetn = true;

			SendCharList(cId);

			return;
		}

		for(INT8 i = 0; i < sizeof(NotDesequip) / sizeof(INT16); i++)
		{
			if(srcItem->Index == NotDesequip[i])
			{
				SendClientMessage(cId, "Não é possível movimentar este item.");

				sendLog(WARN, cId, "%s %s tentou mover %s", Users[cId].Username, player->Name, itemlistBuffer[srcItem->Index].Name);

				*pRetn = true;

				return;
			}
		}

		if((srcItem->Index >= 4152 && srcItem->Index <= 4183) && ((player->Equip[0].Index >= 22 && player->Equip[0].Index <= 25) || player->Equip[0].Index == 32)) // Trajes
		{	
			*pRetn = true;

			SendClientMessage(cId, "Impossível equipar traje enquanto transformado.");

			sendLog(WARN, cId, "%s %s tentou ativar traje transformado.", Users[cId].Username, player->Name);

			return;
		}

		if(p->DstType == EQUIP && (!srcItem->EF1 && !srcItem->EFV1 && !srcItem->EF2 && !srcItem->EFV2 && !srcItem->EF3 && !srcItem->EFV3))
		{
			UINT8 Time = 0;

			if(p->DstSlot == 13)
			{
				if(srcItem->Index >= 3900 || srcItem->Index <= 3908) // Fadas
				{
					if(srcItem->Index > 3899 && srcItem->Index < 3903)
						Time = 2;// 3 Dias
					else if(srcItem->Index > 3902 && srcItem->Index < 3906)
						Time = 4;// 5 Dias
					else if(srcItem->Index > 3905 && srcItem->Index < 3909)
						Time = 6;// 7 Dias

					srcItem->EF1 = 106;
					srcItem->EFV1 = Time;
					srcItem->EF2 = 107;
					srcItem->EFV2 = 23;
					srcItem->EF3 = 108;
					srcItem->EFV3 = 59;

					sendLog(INFO, cId, "%s %s ativou %s.", Users[cId].Username, player->Name, itemlistBuffer[srcItem->Index].Name);

					return;
				}
				else if(srcItem->Index >= 4152 && srcItem->Index <= 4180)// Trajes
					Time = 30;
			}
			else if(p->DstSlot == 14 && srcItem->Index >= 3980 && srcItem->Index <= 3999) // Esferas
			{
				if(srcItem->Index > 3979 && srcItem->Index < 3983)
					Time = 3;
				else if(srcItem->Index > 3982 && srcItem->Index < 3986)
					Time = 15;
				else if(srcItem->Index > 3985 && srcItem->Index < 3990)
					Time = 30;
			}

			if(Time)
			{
				srcItem->EF1 = 106;
				srcItem->EFV1 = now->tm_mday + Time; // Dia atual + tempo da mount
				srcItem->EF2 = 110;
				srcItem->EFV2 = ++now->tm_mon; // Aqui incrementamos a variável do mês para depois atribuirmos ao EFV2 o valor dela
				srcItem->EF3 = 109;
				srcItem->EFV3 = now->tm_year - 100; // Ano com um decrescimo de 100 pq o client é bagudo e lê essa porra toda doida

				CheckItemTime(srcItem);

				sendLog(INFO, cId, "%s %s ativou %s.", Users[cId].Username, player->Name, itemlistBuffer[srcItem->Index].Name);
			}
		}
	}

	if (dstItem == NULL)
		return;

	for(INT8 i = 0; i < sizeof(NotDesequip) / sizeof(INT16); i++)
	{
		if(dstItem->Index == NotDesequip[i])
		{
			SendClientMessage(cId, "Não é possível movimentar este item.");

			sendLog(WARN, cId, "%s %s tentou mover %s", Users[cId].Username, player->Name, itemlistBuffer[srcItem->Index].Name);

			*pRetn = true;

			return;
		}
	}

	if (p->SrcType == p->DstType && p->SrcType == INVENTORY && srcItem->Index == dstItem->Index)
	{
		if (!CanAgroup(srcItem))
			return;

		*pRetn = true;

		int srcAmount = GetItemAmount(srcItem),
			dstAmount = GetItemAmount(dstItem),
			maxAmount = GetMaxAmount(srcItem->Index),
			totalAmount = srcAmount + dstAmount;

		if (dstAmount == maxAmount)
			return;

		if (totalAmount > maxAmount)
		{
			// Caso a soma dos dois pacotes seja maior do que o limite
			// Faz com que seja montado um pacote com o limite e um outro com o resto
			srcAmount = totalAmount - maxAmount;
			totalAmount = maxAmount;

			PutAmount(srcItem, srcAmount);
		}
		else
			memset(srcItem, 0, sizeof st_Item);

		PutAmount(dstItem, totalAmount);

		SendItem(cId, p->SrcType, p->SrcSlot, srcItem);
		SendItem(cId, p->DstType, p->DstSlot, dstItem);
	}
}

void MSG_SWAPITEM(BYTE* msg, bool* pRetn)
{
	*pRetn = true;

	st_Mob* pMob = (st_Mob*)0x0015C69B8;

	//0043FE71
	p376* pMoveItem = (p376*)msg;

	INT16 conn = pMoveItem->Header.ClientId,
		pSrcType = pMoveItem->SrcType,
		pSrcSlot = pMoveItem->SrcSlot,
		pDstType = pMoveItem->DstType,
		pDstSlot = pMoveItem->DstSlot;

	if (pMob[conn].Status.curHP == 0 || Users[conn].Status != 22)
	{
		SendHpMode(conn);
		
		return;
	}

	//0043FEC1
	if (Trade[conn].ClientID)
	{
		//RemoveTrade(conn);
		return;
	}

	//0043FEE8
	if (Users[conn].IsAutoTrading)
	{
		SendClientMessage(conn, "Não é possível movimentar com a loja aberta."); //007CF428
		return;
	}

	//0043FF30

	if (pSrcType == CARGO && pDstType == EQUIP)
	{
		SendClientMessage(conn, "Não é possível equipar a partir do baú.");
		return;
	}

	if (pSrcType == STORAGE_TYPE || pDstType == STORAGE_TYPE)
	{
		//0043FF86
		int alvo = pMoveItem->Unknown;
		if (alvo <= MAX_PLAYER || alvo >= 30000)
		{
			SendClientSignalParm(conn, 0x7530, 0x3A7, 2);
			return;
		}

		//0043FFC8
		if (pMob[alvo].Mode != MOB_PEACE || pMob[alvo].CapeInfo != 2)
		{
			SendClientSignalParm(conn, 0x7530, 0x3A7, 2);
			return;
		}

		//00440010
		int tX = pMob[conn].TargetX;
		int tY = pMob[conn].TargetY;
		int AlvoTx = pMob[alvo].TargetX;
		int AlvoTy = pMob[alvo].TargetY;
		if (tX < AlvoTx - 23 || tX > AlvoTx + 23 || tY < AlvoTy - 23 || tY > AlvoTy + 23)
		{
			SendClientSignalParm(conn, 0x7530, 0x3A7, 2);
			return;
		}
	}

	//004400C9
	st_Item pMount;
	memcpy(&pMount, &pMob[conn].Equip[MOUNT], sizeof st_Item);

	int isEquip = FALSE;
	int isGuild = FALSE;

	st_Item* pSrcPtr = NULL;
	st_Item* pDestPtr = NULL;

	//004400FC
	if (pSrcType == EQUIP_TYPE)
	{
		//0044010F
		if (pSrcSlot <= FACE || pSrcSlot >= MAX_EQUIP)
		{
			//Log("err, trading fails.SwapItem-Equip", pUser[conn].AccountName, pUser[conn].IP);
			return;
		}

		pSrcPtr = &pMob[conn].Equip[pSrcSlot];
		isEquip = TRUE;
		isGuild = TRUE;
	}
	else if (pSrcType == INV_TYPE) //0044019A
	{
		//004401AA
		if (pSrcSlot <= 0 || pSrcSlot >= MAX_INVEN)
		{
			//Log("err, trading fails.SwapItem-Carry", pUser[conn].AccountName, pUser[conn].IP);
			return;
		}

		pSrcPtr = &pMob[conn].Inventory[pSrcSlot];
	}
	else if (pSrcType == STORAGE_TYPE) //00440220
	{
		//00440230
		if (pSrcSlot <= 0 || pSrcSlot >= MAX_CARGO)
		{
			//Log("err, trading fails.SwapItem-Cargo", pUser[conn].AccountName, pUser[conn].IP);
			return;
		}

		pSrcPtr = &Users[conn].Storage.Item[pSrcSlot];
	}
	else //004402A7
	{
		//Log("err, trading fails.SwapItem source type", pUser[conn].AccountName, pUser[conn].IP);
		return;
	}

	//004402D9
	if (pDstType == EQUIP_TYPE)
	{
		//004402EC
		if (pDstSlot <= FACE || pDstSlot >= MAX_EQUIP)
		{
			//Log("err, trading fails.SwapItem-Equip", pUser[conn].AccountName, pUser[conn].IP);
			return;
		}

		pDestPtr = &pMob[conn].Equip[pDstSlot];
		isEquip = 1;
		isGuild = 1;
	}
	else if (pDstType == INV_TYPE) //00440377
	{
		//00440387
		if (pDstSlot <= 0 || pDstSlot >= MAX_INVEN)
		{
			//Log("err, trading fails.SwapItem-Carry", pUser[conn].AccountName, pUser[conn].IP);
			return;
		}

		pDestPtr = &pMob[conn].Inventory[pDstSlot];
	}
	else if (pDstType == STORAGE_TYPE) //004403FD
	{
		//0044040D
		if (pDstSlot <= 0 || pDstSlot >= MAX_CARGO)
		{
			//Log("err, trading fails.SwapItem-Cargo", pUser[conn].AccountName, pUser[conn].IP);
			return;
		}

		pDestPtr = &Users[conn].Storage.Item[pDstSlot];
	}
	else //00440484
	{
		//Log("err, trading fails.SwapItem source type", pUser[conn].AccountName, pUser[conn].IP);
		return;
	}

	if (pMob[conn].Equip[0].EFV2 >= SEMIDEUS && pDstType == EQUIP && pDstSlot == HELMET)
	{
		if (pSrcPtr->Index < 3500 || pSrcPtr->Index > 3507)
		{
			SendClientMessage(conn, "Movimento inválido.");
			return;
		}
	}

	bool ArchItem = CheckArchItem(pSrcPtr->Index),
		 HardItem = CheckHardItem(pSrcPtr->Index);

	if (ArchItem && pMob[conn].Equip[0].EFV2 < SEMIDEUS)
	{
		SendClientMessage(conn, "Somente para Semideuses ou superior.");

		return;
	}
	else if (HardItem && pMob[conn].Equip[0].EFV2 < DEUS)
	{
		SendClientMessage(conn, "Somente para Deuses.");

		return;
	}

	for (INT8 i = 0; i < sizeof(NotDesequip) / sizeof(INT16); i++)
	{
		if (pSrcPtr->Index == NotDesequip[i] || pDestPtr->Index == NotDesequip[i])
		{
			SendClientMessage(conn, "Não é possível movimentar este item.");

			return;
		}
	}

	if ((pSrcPtr->Index >= 4152 && pSrcPtr->Index <= 4183) && ((pMob[conn].Equip[0].Index >= 22 && pMob[conn].Equip[0].Index <= 25) || pMob[conn].Equip[0].Index == 32)) // Trajes
	{
		SendClientMessage(conn, "Não é possível equipar traje enquanto transformado.");

		return;
	}

	if (pDstType == EQUIP && 
		(!pSrcPtr->EF1 && !pSrcPtr->EFV1 && !pSrcPtr->EF2 && !pSrcPtr->EFV2 && !pSrcPtr->EF3 && !pSrcPtr->EFV3))
	{
		UINT8 Time = 0;

		if (pDstSlot == 13)
		{
			if (pSrcPtr->Index >= 3900 || pSrcPtr->Index <= 3908) // Fadas
			{
				if (pSrcPtr->Index > 3899 && pSrcPtr->Index < 3903)
					Time = 2;// 3 Dias
				else if (pSrcPtr->Index > 3902 && pSrcPtr->Index < 3906)
					Time = 4;// 5 Dias
				else if (pSrcPtr->Index > 3905 && pSrcPtr->Index < 3909)
					Time = 6;// 7 Dias

				pSrcPtr->EF1 = 106;
				pSrcPtr->EFV1 = Time;
				pSrcPtr->EF2 = 107;
				pSrcPtr->EFV2 = 23;
				pSrcPtr->EF3 = 108;
				pSrcPtr->EFV3 = 59;

				//sendLog(INFO, cId, "%s %s ativou %s.", Users[cId].Username, player->Name, itemlistBuffer[srcItem->Index].Name);

				return;
			}
			else if (pSrcPtr->Index >= 4152 && pSrcPtr->Index <= 4180)// Trajes
				Time = 30;
		}
		else if (pDstSlot == 14 && pSrcPtr->Index >= 3980 && pSrcPtr->Index <= 3999) // Esferas
		{
			if (pSrcPtr->Index > 3979 && pSrcPtr->Index < 3983)
				Time = 3;
			else if (pSrcPtr->Index > 3982 && pSrcPtr->Index < 3986)
				Time = 15;
			else if (pSrcPtr->Index > 3985 && pSrcPtr->Index < 3990)
				Time = 30;
		}

		if (Time)
		{
			time_t rawnow = time(NULL);
			struct tm *now = localtime(&rawnow);

			pSrcPtr->EF1 = 106;
			pSrcPtr->EFV1 = now->tm_mday + Time; // Dia atual + tempo da mount
			pSrcPtr->EF2 = 110;
			pSrcPtr->EFV2 = ++now->tm_mon; // Aqui incrementamos a variável do mês para depois atribuirmos ao EFV2 o valor dela
			pSrcPtr->EF3 = 109;
			pSrcPtr->EFV3 = now->tm_year - 100; // Ano com um decrescimo de 100 pq o client é bagudo e lê essa porra toda doida

			CheckItemTime(pSrcPtr);

			//sendLog(INFO, cId, "%s %s ativou %s.", Users[cId].Username, player->Name, itemlistBuffer[srcItem->Index].Name);
		}
	}

	if (pSrcType == pDstType && pSrcType == INVENTORY && pSrcPtr->Index == pDestPtr->Index)
	{
		if (!CanAgroup(pSrcPtr))
			return;

		int srcAmount = GetItemAmount(pSrcPtr),
			dstAmount = GetItemAmount(pDestPtr),
			maxAmount = GetMaxAmount(pSrcPtr->Index),
			totalAmount = srcAmount + dstAmount;

		if (dstAmount >= maxAmount)
			return;

		if (totalAmount > maxAmount)
		{
			// Caso a soma dos dois pacotes seja maior do que o limite
			// Faz com que seja montado um pacote com o limite e um outro com o resto
			srcAmount = totalAmount - maxAmount;
			totalAmount = maxAmount;

			PutAmount(pSrcPtr, srcAmount);
		}
		else
			memset(pSrcPtr, 0, sizeof st_Item);

		PutAmount(pDestPtr, totalAmount);

		SendItem(conn, pSrcType, pSrcSlot, pSrcPtr);
		SendItem(conn, pDstType, pDstSlot, pDestPtr);

		return;
	}

	//004404B6
	if (pSrcPtr->Index == 747 || pDestPtr->Index == 747) //Coroa de Atila
		//004404D7
		if (pSrcType == STORAGE_TYPE || pDstType == STORAGE_TYPE)
			return;

	//004404FC
	int retSrcItem = FALSE;
	if (pSrcPtr->Index == 508 || pSrcPtr->Index == 522 || (pSrcPtr->Index >= 526 && pSrcPtr->Index <= 537)) //Medalhas
		return;

	//00440570
	int retDestItem = FALSE;
	if (pDestPtr->Index == 508 || pDestPtr->Index == 522 || (pDestPtr->Index >= 526 && pDestPtr->Index <= 537)) //Medalhas
		return;

	//0044062C
	st_Item SrcItem;
	st_Item DestItem;
	memcpy(&SrcItem, pSrcPtr, sizeof st_Item);
	memcpy(&DestItem, pDestPtr, sizeof st_Item);

	BASE_ClearItem(pDestPtr);
	BASE_ClearItem(pSrcPtr);

	//00440678
	int canEquipDest = TRUE;
	int canEquipSrc = TRUE;
	if (DestItem.Index != 0)
	{
		//0044069B
		int destError = -2;
		if (pDstType == EQUIP_TYPE && !ArchItem && !HardItem)
		{
			//004406B4
			canEquipDest = BASE_CanEquip(&DestItem, &pMob[conn].Status, pMoveItem->DstSlot, pMob[conn].ClassInfo, pMob[conn].Equip);
		}
		else if (pDstType == INV_TYPE)
		{
			//0044071A
			canEquipDest = BASE_CanCarry(&DestItem, pMob[conn].Inventory, (pMoveItem->DstSlot % 9), (pMoveItem->DstSlot / 9), &destError);
			if (canEquipDest == 0 && destError > 0 && destError <= MAX_INVEN)
			{
				//00440789
				destError--;
				SendItem(conn, INV_TYPE, destError, &pMob[conn].Inventory[destError]);
			}
		}
		else if (pDstType == STORAGE_TYPE)
		{
			//004407D4
			canEquipDest = BASE_CanCargo(&DestItem, Users[conn].Storage.Item, (pMoveItem->DstSlot % 9), (pMoveItem->DstSlot / 9));
		}
	}

	//00440823
	if (SrcItem.Index != 0)
	{
		//00440832
		int srcError = -2;
		if (pSrcType == EQUIP_TYPE)
		{
			//0044084B
			canEquipSrc = BASE_CanEquip(&SrcItem, &pMob[conn].Status, pMoveItem->SrcSlot, pMob[conn].ClassInfo, pMob[conn].Equip);
		}
		else if (pSrcType == INV_TYPE)
		{
			//004408B1
			canEquipSrc = BASE_CanCarry(&SrcItem, pMob[conn].Inventory, (pMoveItem->SrcSlot % 9), (pMoveItem->SrcSlot / 9), &srcError);
			if (canEquipSrc == 0 && srcError > 0 && srcError <= MAX_INVEN)
			{
				//00440920
				srcError--;
				SendItem(conn, INV_TYPE, srcError, &pMob[conn].Inventory[srcError]);
			}
		}
		else if (pSrcType == STORAGE_TYPE)
		{
			//0044096B
			canEquipSrc = BASE_CanCargo(&SrcItem, Users[conn].Storage.Item, (pMoveItem->SrcSlot % 9), (pMoveItem->SrcSlot / 9));
		}
	}

	SendClientMsg(pMoveItem->Header.ClientId, "canEquipDest: %d canEquipSrc: %d", canEquipDest, canEquipSrc);
	//004409BA
	if (canEquipDest == 0 && canEquipSrc == 1)
	{
		memcpy(pSrcPtr, &DestItem, sizeof st_Item);
		memcpy(pDestPtr, &SrcItem, sizeof st_Item);
		return;
	}
	else
	{
		memcpy(pSrcPtr, &SrcItem, sizeof st_Item);
		memcpy(pDestPtr, &DestItem, sizeof st_Item);
	}

	//00440A2D
	if (pSrcPtr->Index <= 40) //ID das Faces
		BASE_ClearItem(pSrcPtr);

	if (pDestPtr->Index <= 40) //ID das Faces
		BASE_ClearItem(pDestPtr);

	//00440A58
	GetCurrentScore(conn);
	Sendpack((BYTE*)pMoveItem, conn, sizeof p376);

	//00440A9C
	if (pMob[conn].Equip[WEAPON].Index == 0 && pMob[conn].Equip[SHIELD].Index != 0)
	{
		int ItemUnique = BASE_GetItemAbility(&pMob[conn].Equip[SHIELD], EF_POS);
		if (ItemUnique != 128)
		{
			//00440AFC
			pMoveItem->DstType = INV_TYPE;
			pMoveItem->DstSlot = WEAPON;

			pMoveItem->SrcType = INV_TYPE;
			pMoveItem->SrcSlot = SHIELD;

			Sendpack((BYTE*)pMoveItem, conn, sizeof p376);

			memcpy(&pMob[conn].Equip[WEAPON], &pMob[conn].Equip[SHIELD], sizeof st_Item);
			memset(&pMob[conn].Equip[SHIELD], 0, sizeof st_Item);
		}
	}

	//00440BA9
	if (isEquip == TRUE)
		SendEquip(conn);

	//00440BE3
	MountProcess(conn, &pMount);

	st_Item* penultimo = &Users[conn].Storage.Item[126];
	st_Item* ultimo    = &Users[conn].Storage.Item[127];

	//00440C1F
	if (pMoveItem->SrcType == STORAGE_TYPE || pMoveItem->DstType == STORAGE_TYPE)
	{
		if (penultimo->Index == 0 || ultimo->Index == 0)
			return;

		//00440C61
		for (int i = 0; i < 126; i++)
		{
			//00440C89
			if (penultimo->Index != 0)
			{
				int canCargo = BASE_CanCargo(penultimo, Users[conn].Storage.Item, i % 9, i / 9);
				if (canCargo != 0)
				{
					memcpy(&Users[conn].Storage.Item[i], penultimo, sizeof st_Item);
					memset(penultimo, 0, sizeof st_Item);
					SendItem(conn, STORAGE_TYPE, i, &Users[conn].Storage.Item[i]);
				}
			}

			if (ultimo->Index != 0) //00440D57
			{
				int canCargo = BASE_CanCargo(ultimo, Users[conn].Storage.Item, i % 9, i / 9);
				if (canCargo != 0)
				{
					memcpy(&Users[conn].Storage.Item[i], ultimo, sizeof st_Item);
					memset(ultimo, 0, sizeof st_Item);
					SendItem(conn, STORAGE_TYPE, i, &Users[conn].Storage.Item[i]);
				}
			}
		}
	}
}