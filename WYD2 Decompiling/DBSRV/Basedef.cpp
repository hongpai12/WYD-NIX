#include "stdafx.h"

bool BASE_CheckPacket(MSG_STANDARD* msg)
{
	return true;
}
bool BASE_InitializeBasedef()
{
	if (!BASE_InitializeServerlist())
	{
		MessageBox(0, "Could not load serverlist!", "Error", MB_OK);
		return false;
	}
	if (!BASE_InitializeBaseSet())
	{
		MessageBox(0, "Could not load base characters", "Error", MB_OK);
		return false;
	}
	if (!BASE_InitializeEffectName())
	{
		MessageBox(0, "Could not load item effect", "Error", MB_OK);
		return false;
	}
	if (!BASE_InitializeItemList())
	{
		MessageBox(0, "Could not load itemlist", "Error", MB_OK);
		return false;
	}
	if (!BASE_InitializeEncryption())
	{
		MessageBox(0, "Could not load encryption keys", "Error", MB_OK);
		return false;
	}
	return true;
}
int32_t BASE_GetSum2(uint8_t* p, int32_t size)
{
	int32_t sum = 0;
	for (int i = 0; i < size; i++)
	{
		int mod = i % 9;
		if (mod == 0) sum += p[i] * 2;
		if (mod == 1) sum += (p[i] ^ 0xFF);
		if (mod == 2) sum += p[i] / 3;
		if (mod == 3) sum += p[i] * 2;
		if (mod == 4) sum -= (p[i] ^ 0x5A);
		if (mod == 5) sum -= p[i];
		else sum += (p[i] / 5);
	}
	return sum;
}
bool BASE_InitializeBaseSet()
{
	const char* charNames[MAX_CLASS] = {
		{ "./npc/TK" },
		{ "./npc/FM" },
		{ "./npc/BM" },
		{ "./npc/HT" },
		{ "./npc/TK_" },
		{ "./npc/FM_" },
		{ "./npc/BM_" },
		{ "./npc/HT_" }
	};

	FILE* fs = 0;

	for (uint8_t i = 0; i < MAX_CLASS; i++)
	{
		fopen_s(&fs, charNames[i], "rb");
		if (fs != 0)
		{
			fread(&g_pBaseSet[i], sizeof(STRUCT_MOB), 1, fs);
			fclose(fs);
			fs = 0;
		}
		else
			return false;
	}
	return true;
}
bool BASE_InitializeItemList()
{
	int32_t size = sizeof(STRUCT_ITEMLIST) * MAX_ITEMLIST;
	memset(g_pItemList, 0, size);

	bool ret = BASE_ReadItemListFile("itemlist.csv", false);

	if (ret)
		BASE_ReadItemListFile("extraitem.csv", false);
	
	return ret;
}
bool BASE_ReadItemListFile(char* filename, bool build)
{
	FILE* fs = 0, *buildFs = 0;
	if (build)
	{
		BASE_WriteItemList(true);

		fopen_s(&buildFs, "extraitem.bin", "wb");

		if (buildFs == 0)
			return false;
	}

	fopen_s(&fs, filename, "rt");
	if (fs == 0)
	{
		char fname[256] = { 0, };
		sprintf_s(fname, "../../TMSRV/run/%s", filename);

		fopen_s(&fs, fname, "rt");

		if (fs == 0)
			return false;
	}
	char line[1024] = { 0, };

	int32_t index = -1;
	char name[128] = { 0, };
	char effName[MAX_STATICEFFECT][128];
	int32_t effVal[MAX_STATICEFFECT] = { 0, };
	char textureTmp[256] = { 0, };
	char reqTemp[256] = { 0, };

	int32_t mesh = 0, texture = 0;
	int32_t rLvl = 0, rStr = 0, rInt = 0, rDex = 0, rCon = 0;
	int32_t unique = 0, price = 0, pos = 0, extra = 0, grade = 0;

	while (fgets(line, 1024, fs))
	{
		memset(effName, 0, sizeof(effName));
		memset(effVal, 0, sizeof(effVal));

		for (int i = 0; i < 1024; i++)
		{
			if (line[i] == ',')
				line[i] = ' ';

			if (line[i] == '\0')
				break;
		}

		*textureTmp = 0;
		*reqTemp = 0;

		sscanf_s(line, "%d %s %s %s %d %d %d %d %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d",
			&index, name, _countof(name), textureTmp, _countof(textureTmp), reqTemp, _countof(reqTemp),
			&unique, &price, &pos, &extra, &grade,
			effName[0], _countof(effName[0]), &effVal[0], effName[1], _countof(effName[1]), &effVal[1], effName[2], _countof(effName[2]), &effVal[2],
			effName[3], _countof(effName[3]), &effVal[3], effName[4], _countof(effName[4]), &effVal[4], effName[5], _countof(effName[5]), &effVal[5],
			effName[6], _countof(effName[6]), &effVal[6], effName[7], _countof(effName[7]), &effVal[7], effName[8], _countof(effName[8]), &effVal[8],
			effName[9], _countof(effName[9]), &effVal[9], effName[10], _countof(effName[10]), &effVal[10], effName[11], _countof(effName[11]), &effVal[11]);

		if (index < 0 || index > MAX_ITEMLIST)
			continue;

		int32_t nameLen = strnlen_s(name, _countof(name));
		if (nameLen >= ITEMNAME_LENGTH - 1)
			name[ITEMNAME_LENGTH - 1] = '\0';

		strcpy_s(g_pItemList[index].Name, name);

		if (*textureTmp == '.')
			*textureTmp = ' ';

		if (*reqTemp == '.')
			*reqTemp = ' ';

		sscanf_s(textureTmp, "%d.%d", &mesh, &texture);
		sscanf_s(reqTemp, "%d.%d.%d.%d.%d", &rLvl, &rStr, &rInt, &rDex, &rCon);

		g_pItemList[index].Mesh = mesh;
		g_pItemList[index].SubMesh = texture;
		g_pItemList[index].Level = rLvl;
		g_pItemList[index].STR = rStr;
		g_pItemList[index].INT = rInt;
		g_pItemList[index].DEX = rDex;
		g_pItemList[index].CON = rCon;
		g_pItemList[index].Unique = unique;
		g_pItemList[index].Price = price;
		g_pItemList[index].Pos = pos;
		g_pItemList[index].Grade = grade;
		g_pItemList[index].Extra = extra;
		g_pItemList[index].Unknown = 0;

		for (int i = 0; i < MAX_STATICEFFECT; i++)
		{
			if (effName[i][0] == '\0') continue;

			int j = 0;
			for (j = 0; j < MAX_ITEMEFFECT; j++)
			{
				if (!strcmp(g_pEffectNameTable[j], effName[i]))
					break;
			}
			if (j == MAX_ITEMEFFECT)
			{
				g_pItemList[index].Effect[i].Index = atoi(effName[i]);
				g_pItemList[index].Effect[i].Value = effVal[i];
				continue;
			}
			g_pItemList[index].Effect[i].Index = j;
			g_pItemList[index].Effect[i].Value = effVal[i];
		}

		if (build)
		{
			fwrite(&index, 2, 1, buildFs);
			fwrite(&g_pItemList[index], sizeof(STRUCT_ITEMLIST), 1, buildFs);
		}
	}
	fclose(fs);
	
	if (build)
	{
		fclose(buildFs);
		BASE_WriteItemList(false);
	}
	return true;
}
bool BASE_WriteItemList(bool write)
{
	int32_t size = sizeof(STRUCT_ITEMLIST) * MAX_ITEMLIST;
	uint8_t* tmp = (uint8_t*)g_pItemList;

	for (int i = 0; i < size; i++)
		tmp[i] ^= 0x5A;

	FILE* fs = 0;
	if (write)
	{
		fopen_s(&fs, "ItemList.bin", "wb");
		if (fs == 0)
			return false;

		fwrite(g_pItemList, sizeof(STRUCT_ITEMLIST), MAX_ITEMLIST, fs);
	}
	int32_t sum = BASE_GetSum2(tmp, size);
	int32_t nRand = rand();
	if (write)
	{
		fwrite(&nRand, sizeof(int32_t), 1, fs);
		fclose(fs);
	}
	FILE* fsCsum = 0;
	if (write)
		fopen_s(&fsCsum, "..\\ItemCSum.h", "wt");
	else
		fopen_s(&fsCsum, "..\\ItemCSum.h", "a+");

	if (fsCsum != 0)
	{
		if (write)
			fprintf_s(fsCsum, "#define ITEM_CHECKSUM %d\n", sum);
		else
			fprintf_s(fsCsum, "#define ITEM_CHECKSUM2 %d\n", sum);

		fclose(fsCsum);
	}
	for (int i = 0; i < size; i++)
		tmp[i] ^= 0x5A;

	return true;
}
bool BASE_InitializeServerlist()
{
	FILE* fs = 0;
	fopen_s(&fs, "./serverlist.txt", "rt");
	if (fs == 0)
	{
		fopen_s(&fs, "../../serverlist.txt", "rt");
		if (fs == 0)
			return false;
	}

	int32_t serverGroup = -1, serverNumber = -1;
	char address[64];
	char str[256];
	while (fgets(str, 256, fs) != 0)
	{
		sscanf_s(str, "%d %d %s", &serverGroup, &serverNumber, address, 64);
		if (serverGroup < 0 || serverGroup >= MAX_SERVERGROUP || serverNumber < 0 || serverNumber >= MAX_SERVERNUMBER)
			break;

		strcpy_s(g_pServerlist[serverGroup][serverNumber], 64, address);
	}
	fclose(fs);
	return true;
}
void BASE_InitializeModuleDir()
{
	char str[256];
	if (GetModuleFileName(0, str, 256) == 0)
		return;

	size_t len = strlen(str);
	for (size_t i = len - 1; i > 0; i--) {
		if (str[i] == '\\') {
			str[i] = 0;
			break;
		}
	}
	SetCurrentDirectory(str);
}
bool BASE_InitializeEffectName()
{
	for (int i = 0; i < MAX_ITEMEFFECT; i++)
		*g_pEffectNameTable[i] = '\0';

	FILE* fs = 0;
	fopen_s(&fs, "ItemEffect.h", "rt");
	if (fs == 0)
	{
		fopen_s(&fs, "../../TMSRV/run/ItemEffect.h", "rt");
		if (fs == 0)
			return false;
	}

	char line[256] = { 0, };
	char value[32] = { 0, };
	char effName[32] = { 0, };
	char firstToken[64] = { 0, };

	while (fgets(line, 256, fs))
	{
		if (*line == '\r')
			continue;

		sscanf_s(line, "%s", firstToken, _countof(firstToken));

		if (!strcmp(firstToken, "#include"))
			continue;

		else if (!strcmp(firstToken, "#define"))
		{
			sscanf_s(line, "%*s %s %s", effName, _countof(effName), value, _countof(value));

			int32_t val = atoi(value);
			if (val <= 0 || val >= MAX_ITEMEFFECT)
				continue;

			if (*g_pEffectNameTable[val] == '\0')
				strcpy_s(g_pEffectNameTable[val], effName);
		}
	}
	fclose(fs);
	return true;
}
bool BASE_InitializeEncryption()
{
	FILE* fs = 0;
	fopen_s(&fs, "encdeckeys.dat", "rb");
	if (fs == 0)
	{
		fopen_s(&fs, "../../encdeckeys.dat", "rb");
		if (fs == 0)
			return false;
	}
	fread(&g_pKeyword[0], 512, 1, fs);
	fclose(fs);
	return true;
}
void BASE_ClearItem(STRUCT_ITEM* item)
{
	item->Index = 0;
	item->EF1 = 0;
	item->EFV1 = 0;
	item->EF2 = 0;
	item->EFV2 = 0;
	item->EF3 = 0;
	item->EFV3 = 0;
}
void BASE_ClearMob(STRUCT_MOB* mob)
{
	memset(mob, 0, sizeof(STRUCT_MOB));
	mob->LastPosition.X = 2112;
	mob->LastPosition.Y = 2112;
	memset(&mob->BaseScore, 0, sizeof(STRUCT_SCORE));
	memset(&mob->CurrentScore, 0, sizeof(STRUCT_SCORE));

	for (int i = 0; i < MAX_EQUIPMENT; i++)
		BASE_ClearItem(&mob->Equip[i]);

	for (int i = 0; i < MAX_INVENTORY; i++)
		BASE_ClearItem(&mob->Inventory[i]);

	for (int i = 0; i < MAX_SKILLBAR; i++)
		mob->SkillBar[i] = -1;
}
int32_t	BASE_GetItemSanc(STRUCT_ITEM* item)
{
	int32_t value = 0;

	/* Montarias */
	if (item->Index >= 2360 && item->Index <= 2389)
		return (item->EF2 / 8);

	/* Crias? */
	else if (item->Index >= 2330 && item->Index <= 2359)
		return 0;

	if (item->EF1 == EF_SANC || item->EF1 >= EF_COLOR && item->EF1 <= EF_COLOR_END)
		value = item->EFV1;
	else if (item->EF2 == EF_SANC || item->EF2 >= EF_COLOR && item->EF2 <= EF_COLOR_END)
		value = item->EFV2;
	else if (item->EF3 == EF_SANC || item->EF3 >= EF_COLOR && item->EF3 <= EF_COLOR_END)
		value = item->EFV3;

	if (value >= 230)
	{
		value = 10 + ((value - 230) / 4);
		if (value > 15)
			value = 15;
	}
	else
		value %= 10;

	return value;
}
int32_t	BASE_GetGuild(STRUCT_ITEM* item)
{
	int32_t high = 0, low = 0;

	for (int i = 0; i < 3; i++)
	{
		if (item->Effects[i].Index == EF_HWORDGUILD)
		{
			high = item->Effects[i].Value;
			break;
		}
	}

	for (int i = 0; i < 3; i++)
	{
		if (item->Effects[i].Index == EF_LWORDGUILD)
		{
			low = item->Effects[i].Value;
			break;
		}
	}
	return low | (high << 8);
}
int32_t	BASE_GetItemAbility(STRUCT_ITEM* item, uint8_t ef)
{
	int32_t retn = 0;
	int32_t index = item->Index;

	if (index < 0 || index > MAX_ITEMLIST)
		return retn;

	int32_t unique = g_pItemList[index].Unique;

	int32_t pos = g_pItemList[index].Pos;

	if (ef == EF_DAMAGEADD || ef == EF_MAGICADD)
	{
		if (unique < 41 || unique >= 50)
			return retn;
	}

	if (ef == EF_CRITICAL)
	{
		if (item->EF1 == EF_CRITICAL2 || item->EF2 == EF_CRITICAL2 || item->EF3 == EF_CRITICAL2)
			ef = EF_CRITICAL2;
	}
	if (ef == EF_DAMAGE && unique == 32)
	{
		if (item->EF1 == EF_DAMAGE2 || item->EF2 == EF_DAMAGE2 || item->EF3 == EF_DAMAGE2)
			ef = EF_DAMAGE2;
	}
	if (ef == EF_MPADD)
	{
		if (item->EF1 == EF_MPADD2 || item->EF2 == EF_MPADD2 || item->EF3 == EF_MPADD2)
			ef = EF_MPADD2;
	}
	if (ef == EF_ACADD)
	{
		if (item->EF1 == EF_ACADD2 || item->EF2 == EF_ACADD2 || item->EF3 == EF_ACADD2)
			ef = EF_ACADD2;
	}
	if (ef == EF_LEVEL && (index >= 2330 || index < 2360))
		retn = item->EF2 - 1;
	else if (ef == EF_LEVEL)
		retn += g_pItemList[index].Level;

	if (ef == EF_REQ_STR)
		retn += g_pItemList[index].STR;
	if (ef == EF_REQ_INT)
		retn += g_pItemList[index].INT;
	if (ef == EF_REQ_DEX)
		retn += g_pItemList[index].DEX;
	if (ef == EF_REQ_CON)
		retn += g_pItemList[index].CON;

	if (ef == EF_POS)
		retn += pos;

	if (ef != EF_INCUBATE)
	{
		for (int i = 0; i < MAX_STATICEFFECT; i++)
		{
			if (g_pItemList[index].Effect[i].Index != ef)
				continue;

			int32_t effVal = g_pItemList[index].Effect[i].Value;

			if (ef == EF_ATTSPEED && effVal == 1)
				effVal = 10;

			retn += effVal;
			break;
		}
		for (int i = 0; i < 3; i++)
		{
			if (item->Effects[i].Index != ef)
				continue;

			int32_t effVal = item->Effects[i].Value;
			if (ef == EF_ATTSPEED && effVal == 1)
				effVal = 10;

			retn += effVal;
			break;
		}
	}

	if (index >= 2330 && index < 2390)
	{
		if (ef == EF_MOUNTHP)
			return item->EF1;

		if (ef == EF_MOUNTSANC)
			return item->EF2;

		if (ef == EF_MOUNTLIFE)
			return item->EFV2;

		if (ef == EF_MOUNTFEED)
			return item->EF3;

		if (ef == EF_MOUNTKILL)
			return item->EFV3;

		if (index >= 2360 && index < 2390 && item->EF1 > 0)
		{
			int32_t ef2 = item->EF2;
			int32_t mountId = index - 2360;

			if (ef == EF_DAMAGE)
				return (((ef2 + 20) * g_pMountBonus[mountId].PhysicalDamage) / 100);

			if (ef == EF_MAGIC)
				return (((ef2 + 15) * g_pMountBonus[mountId].MagicDamage) / 100);

			if (ef == EF_PARRY)
				return g_pMountBonus[mountId].Parry;

			if (ef == EF_RESIST1 || ef == EF_RESIST2 || ef == EF_RESIST3 || ef == EF_RESIST4)
				return g_pMountBonus[mountId].Resist;

			if (ef == EF_RUNSPEED)
				return g_pMountBonus[mountId].Speed;
		}

		return retn;
	}

	if (ef == EF_RESIST1 || ef == EF_RESIST2 || ef == EF_RESIST3 || ef == EF_RESIST4)
	{
		for (int i = 0; i < MAX_STATICEFFECT; i++)
		{
			if (g_pItemList[index].Effect[i].Index != EF_RESISTALL)
				continue;

			retn += g_pItemList[index].Effect[i].Value;
			break;
		}

		for (int i = 0; i < 3; i++)
		{
			if (item->Effects[i].Index != EF_RESISTALL)
				continue;

			retn += item->Effects[i].Value;
			break;
		}
	}
	int32_t itemSanc = BASE_GetItemSanc(item);

	if (item->Index <= 40)
		itemSanc = 0;

	if (itemSanc == 9 && (pos & 3840) != 0)
		itemSanc++;

	if (itemSanc != 0 && ef != EF_GRID && ef != EF_CLASS && ef != EF_POS && ef != EF_WTYPE && ef != EF_RANGE
		&& ef != EF_LEVEL && ef != EF_REQ_STR && ef != EF_REQ_INT && ef != EF_REQ_DEX && ef != EF_REQ_CON &&
		ef != EF_VOLATILE && ef != EF_INCUBATE && ef != EF_INCUDELAY)
	{
		if (itemSanc <= 10)
			retn = (retn * (itemSanc + 10) / 10);
		else
		{
			int32_t plus = itemSanc - 10, mult = 0;
			if (plus == 1) mult = 220;
			else if (plus == 2) mult = 250;
			else if (plus == 3) mult = 280;
			else if (plus == 4) mult = 320;
			else if (plus == 5) mult = 370;

			retn = ((((retn * 10) * mult) / 100) / 10);
		}
	}
	if (ef == EF_RUNSPEED)
	{
		if (retn >= 3)
			retn = 2;

		if (retn > 0 && itemSanc >= 9)
			retn++;
	}

	if (ef == EF_GRID)
	{
		if (retn < 0 || retn > 7)
			retn = 0;
	}
	return retn;
}
bool BASE_CanCargo(STRUCT_ITEM* item, STRUCT_ITEM* cargo, int32_t destX, int32_t destY)
{
	int32_t itemGrid = BASE_GetItemAbility(item, EF_GRID);

	uint8_t destGrid[MAX_ITEMGRID];
	memcpy(destGrid, g_pItemGrid[itemGrid], MAX_ITEMGRID);

	uint8_t cargoSlots[MAX_CARGO];
	memset(cargoSlots, 0, MAX_CARGO);

	uint8_t tmpGrid[MAX_ITEMGRID];

	for (int i = 0; i < MAX_CARGO; i++)
	{
		if (!cargo[i].Index)
			continue;

		itemGrid = BASE_GetItemAbility(&cargo[i], EF_GRID);
		memcpy(tmpGrid, g_pItemGrid[itemGrid], MAX_ITEMGRID);

		int32_t pInvX = i % 9, pInvY = i / 9;

		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 2; x++)
			{
				if (tmpGrid[(y * 2) + x] != 1)
					continue;

				if ((y + pInvY) < 0 || (y + pInvY) >= 14)
					continue;

				if ((x + pInvX) < 0 || (x + pInvX) >= 9)
					continue;

				cargoSlots[(y + pInvY) * 9 + x + pInvX] = i + 1;
			}
		}
	}

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 2; x++)
		{
			if (destGrid[(y * 2) + x] != 1)
				continue;

			if ((y + destY) < 0 || (x + destX) < 0 || (y + destY) >= 14 || (x + destX) >= 9)
				return false;

			if (cargoSlots[(y + destY) * 9 + x + destX] != 1)
				continue;

			return false;
		}
	}
	return true;
}
void BASE_GetFirstKey(char* source, char* dest)
{
	if ((*source >= 'A' && *source <= 'Z') || (*source >= 'a' && *source <= 'z'))
	{
		dest[0] = source[0];
		dest[1] = '\0';
		return;
	}
	/*
	else if (*source < 0)
	{
		
		char		KorFirst[40] = "ㄱㄲㄴㄷㄸㄹㅁㅂㅃㅅㅆㅇㅈㅉㅊㅋㅌㅍㅎ";
		int32_t		KorIndex[19] = { 171, 291, 432, 560, 646, 773, 902, 1031, 1104, 1237, 1353, 1561, 1696, 1770, 1890, 1997, 2103, 2208, 2350 };
		uint8_t* unsource = (uint8_t*)source;
		int32_t point = (unsource[0] - 0xB0) * 94 + unsource[1] - 0xA1;
		if (point >= 0 && point < 2350)
		{
			int i = 0;
			for (i = 0; i < sizeof(KorIndex) / sizeof(int32_t); i++)
				if (point < KorIndex[i])
					break;

			if (i >= 0 && i <= 17)
			{
				dest[0] = KorFirst[i * 2];
				dest[1] = KorFirst[i * 2 + 1];
				dest[2] = 0;
			}
		}
		
	}
	*/
	strcpy_s(dest, 4, "etc");
}
