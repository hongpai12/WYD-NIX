#include "DLLMain.h"

void ReqDeleteItem(UINT8* m_PacketBuffer, bool* pRetn)
{
	*pRetn = true;

	p2E4* p = (p2E4*)m_PacketBuffer;

	INT16 cId = p->Header.ClientId;

	if (p->Slot < 0 || p->Slot > 60)
		return;
	else if (p->Index < 0 || p->Index > 6500)
		return;
	else if (cId < 0 || cId > MAX_PLAYER)
		return;

	st_Mob *player = GetMobFromIndex(cId);

	if (p->Index != player->Inventory[p->Slot].Index)
	{
		SendItem(cId, INVENTORY, p->Slot, &player->Inventory[p->Slot]);

		return;
	}

	memset(&player->Inventory[p->Slot], 0, sizeof st_Item);

	sendLog(INFO, cId, "%s %s deletou item %d.", nBuffer[cId].Login, player->Name, p->Index);
}

bool RequestUngroupItem(BYTE *m_PacketBuffer)
{
	p2E5 *p = (p2E5*)m_PacketBuffer;
	INT16 cId = p->Header.ClientId;

	if (p->SlotID < 0 || p->SlotID > 60 || p->ItemID < 0 || p->ItemID > 6500 || p->Amount < 0 || p->Amount > 120)
	{
		CloseUser_OL2(cId);

		return true;
	}

	st_Item *item = &GetMobFromIndex(cId)->Inventory[p->SlotID];
	st_Mob *player = GetMobFromIndex(cId);

	int amount = 0;
	for (int i = 0; i < 3; i++)
		if (item->Effect[i].Index == EF_AMOUNT)
			amount += item->Effect[i].Value;

	if (amount < p->Amount)
	{
		CloseUser_OL2(cId);

		return true;
	}

	if (p->ItemID != item->Index)
	{
		CloseUser_OL2(cId);

		return true;
	}

	int newSlot = GetFirstSlot(cId, 0, INVENTORY);

	if (newSlot == -1)
	{
		SendClientMessage(cId, "Não há espaço para separar");

		return true;
	}

	int i = 0;
	for (i = 0; i < 3; i++)
	{
		if (item->Effect[i].Index == EF_AMOUNT)
		{
			if (item->Effect[i].Value > p->Amount)
			{
				item->Effect[i].Value -= p->Amount;
				break;
			}
		}
	}

	// Caso aconteça alguma falha na função anterior
	if (i == 3)
		return true;
	st_Item *newItem = &GetMobFromIndex(cId)->Inventory[newSlot];

	memset(newItem, 0, sizeof st_Item);

	newItem->Index = p->ItemID;

	newItem->Effect[0].Index = EF_AMOUNT;
	newItem->Effect[0].Value = p->Amount;

	SendItem(cId, INVENTORY, p->SlotID, item);
	SendItem(cId, INVENTORY, newSlot, newItem);

	return false;
}