#include "DLLMain.h"

void GetCurScore_CapeInfo(st_Mob* player)
{
	if (player->CapeInfo != 4)
	{
		INT16 cape = player->Equip[15].Index;
		switch (cape)
		{
			case  543: case  545: case  734: case  736: case 3191:
			case 3194: case 3197: case 3300: case 3303: case 3306:
				cape = 7;
				break;
			case  544: case  546: case  735: case  737: case 3192:
			case 3195: case 3198: case 3301: case 3304: case 3307:
				cape = 8;
				break;
		}

		player->CapeInfo = cape;
	}
}

void GetCurScore_Affects(st_Mob *player, int clientid)
{
	for (INT8 i = 0; i < 16; i++)
	{
		switch (mBuffer[clientid].Affects[i].Index)
		{
			case 8: // Joias
			{
				switch (mBuffer[clientid].Affects[i].Master)
				{
					case 8:
					{
						player->Status.Defense += player->Status.Defense / 100 * 20;
						player->Status.maxHP += player->Status.maxHP / 100 * 10;

						break;
					}
					case 16:
					{
						player->Status.Attack += player->Status.Attack / 100 * 20;
						player->Status.maxHP += player->Status.maxHP / 100 * 10;

						break;
					}
					case 128:
					{
						int fHP = player->Status.maxHP + player->Status.maxMP / 2;

						if (fHP > 32000)
							fHP = 32000;

						player->Status.maxHP = fHP;
						player->Status.maxMP /= 2;

						break;
					}
				}

				break;
			}
			case 13: // Assalto
			{
				if (player->ClassInfo == 0)
				{
					player->Status.Attack += player->Status.Defense / 4;
					player->Status.Defense = (player->Status.Defense / 5) * 4;
				}

				break;
			}
			case 14: // Samaritano
			{
				if (player->ClassInfo == 0)
				{
					player->Status.Points[CONST] += (player->bStatus.Points[CONST] / 3) * 2;
					player->Status.maxHP += (player->bStatus.Points[CONST] / 3) * 4;
				}

				break;
			}
			case 24: // Possuido
			{
				if (player->ClassInfo == 0)
				{
					if (player->Equip[7].Index > 0)
					{
						INT32 itemUnique = itemlistBuffer[player->Equip[7].Index].Unique;

						if (itemUnique == 51) //escudo					
							player->Status.Defense += player->Status.Defense / 10;
					}

					player->Status.Defense += (player->Status.Defense / 100) * 70;
				}

				break;
			}
			case 29: // Soul
			{
				if (player->Equip[0].EFV2 == MORTAL)
					break;

				int bonus[4] = { 0, 0, 0, 0 };

				nInfo* nPlayer = &nBuffer[clientid];
				int soulInfo = nPlayer->Chars[nPlayer->Ingame.LoggedMob].SoulInfo;

				if (player->Equip[0].EFV2 == SEMIDEUS)
				{
					bonus[INTEL] = MortalStatsBonuse[soulInfo][INTEL];
					bonus[FORCA] = MortalStatsBonuse[soulInfo][FORCA];
					bonus[CONST] = MortalStatsBonuse[soulInfo][CONST];
					bonus[DEXTE] = MortalStatsBonuse[soulInfo][DEXTE];
				}
				else
				{
					bonus[INTEL] = CeleStatsBonuse[soulInfo][INTEL];
					bonus[FORCA] = CeleStatsBonuse[soulInfo][FORCA];
					bonus[CONST] = CeleStatsBonuse[soulInfo][CONST];
					bonus[DEXTE] = CeleStatsBonuse[soulInfo][DEXTE];
				}
				
				bonus[INTEL] = ((player->bStatus.Points[INTEL] / 100) * bonus[INTEL]);
				bonus[FORCA] = ((player->bStatus.Points[FORCA] / 100) * bonus[FORCA]);
				bonus[DEXTE] = ((player->bStatus.Points[DEXTE] / 100) * bonus[DEXTE]);
				bonus[CONST] = ((player->bStatus.Points[CONST] / 100) * bonus[CONST]);

				player->Status.Points[FORCA] += bonus[FORCA];
				player->Status.Points[DEXTE] += bonus[DEXTE];
				player->Status.Points[INTEL] += bonus[INTEL];
				player->Status.Points[CONST] += bonus[CONST];

				player->Status.Attack += bonus[FORCA] / 3;
				player->Status.maxHP += bonus[CONST] * 2;
				player->Status.maxMP += bonus[INTEL] * 2;

				break;
			}
		}
	}
}

void GetPassivas(st_Mob *player, INT32 clientid)
{
	GetCurScore_CapeInfo(player);
	GetCurScore_Affects(player, clientid);

	INT16 calcCriticalIncrement = ((GetMobAbility(player, EF_CRITICAL) / 10) * 2.5) + ((GetMobAbility(player, EF_CRITICAL2) / 10) * 2.5);
	INT16 calcMagicIncrement = (GetMobAbility(player, EF_MAGIC) + GetMobAbility(player, EF_MAGICADD));
	INT16 def_inc = 0;
	INT16 atk_inc = 0;

	int resist1 = GetMobAbility(player, EF_RESIST1) + GetMobAbility(player, EF_RESISTALL);
	int resist2 = GetMobAbility(player, EF_RESIST2) + GetMobAbility(player, EF_RESISTALL);
	int resist3 = GetMobAbility(player, EF_RESIST3) + GetMobAbility(player, EF_RESISTALL);
	int resist4 = GetMobAbility(player, EF_RESIST4) + GetMobAbility(player, EF_RESISTALL);

	if (resist1 > 100)
		resist1 = 100;
	else if (resist1 < 0)
		resist1 = 0;

	if (resist2 > 100)
		resist2 = 100;
	else if (resist2 < 0)
		resist2 = 0;

	if (resist3 > 100)
		resist3 = 100;
	else if (resist3 < 0)
		resist3 = 0;

	if (resist4 > 100)
		resist4 = 100;
	else if (resist4 < 0)
		resist4 = 0;

	player->Resists[0] = resist1;
	player->Resists[1] = resist2;
	player->Resists[2] = resist3;
	player->Resists[3] = resist4;

	// Skills passívas
	if (player->ClassInfo == 0) // TK
	{
		atk_inc += (player->bStatus.Points[FORCA] + player->bStatus.Points[DEXTE]) / 4;
		def_inc += player->Status.Defense / 2;

 		if ((player->Learn & (1 << 9)))
		{// Bonus ao usar 2 armas equipadas  (mestre das armas)
			if (player->Equip[6].Index && player->Equip[7].Index)
			{
				INT32 itemUnique = itemlistBuffer[player->Equip[7].Index].Unique;

				if (itemUnique != 51) //escudo	
					atk_inc += ((player->Status.Mastery[2] * 3) / 2) + 100;
			}
		}

		if ((player->Learn & (1 << 14)))
			atk_inc += (player->Status.Mastery[2] / 2) + 100;

		if ((player->Learn & (1 << 15)))
		{//Defesa e critico (Armadura critica)   
			def_inc += (player->Status.Mastery[2] * 5) / 2;
			calcCriticalIncrement += 25;
		}
	}

	else if (player->ClassInfo == 2) //BM
	{
		player->Status.maxHP += player->bStatus.maxHP / 100 * 30;

		if ((player->Equip[0].Index >= 22 && player->Equip[0].Index <= 25) || player->Equip[0].Index == 32)
		{
			for (int i = 0; i < 4; i++)
			{
				player->Resists[i] += 20;

				if (player->Resists[i] > 100)
					player->Resists[i] = 100;
			}
		}

		if ((player->Learn & (1 << 51)))
		{
			if (player->Equip[7].Index > 0)
			{
				INT32 itemUnique = itemlistBuffer[player->Equip[7].Index].Unique;

				if (itemUnique == 51) //escudo					
					def_inc += player->Status.Mastery[3] + 25;
			}
		}
	}

	else if (player->ClassInfo == 3) // HT
	{
		if ((player->Learn & (1 << 34)))
		{
			INT32 itemUnique = itemlistBuffer[player->Equip[6].Index].Unique;

			if (itemUnique == 44 || itemUnique == 42) //arcos ou garras
				atk_inc += 300 + (player->Status.Mastery[1] / 100) * 240;
		}

		if ((player->Learn & (1 << 42)))
		{//Pericia de caçador (aumenta o atk se tiver com armas de 1 mao)
			if (player->Equip[6].Index && !player->Equip[7].Index)
				atk_inc += 300 + (player->Status.Mastery[2] / 100) * 240;
		}

		if ((player->Learn & (1 << 50)))
		{//Visao de caçadora (aumenta o critico proporcional a velocidade)
			int bonus = (player->Status.Points[DEXTE] / 50);
			if (bonus > 30)
				bonus = 30;

			calcCriticalIncrement += bonus;
		}

		if ((player->Learn & (1 << 54)))
		{ //proteção das sombras (+def com garras)
			INT32 itemUnique = itemlistBuffer[player->Equip[6].Index].Unique;

			if (itemUnique == 44)
				def_inc += 150 + (player->Status.Mastery[3] / 100) * 240;
		}

		if ((player->Learn & (1 << 39))) //tempestade de raios (8 skill)
			atk_inc += 200;
	}

	if (player->Equip[6].Index > 0)
	{
		INT16 weaponUnique = itemlistBuffer[player->Equip[6].Index].Unique;

		if (weaponUnique == 47)
			calcMagicIncrement += 8;
		else
			atk_inc += 40;
	}

	if (player->Equip[7].Index > 0)
	{
		INT16 shieldUnique = itemlistBuffer[player->Equip[7].Index].Unique;

		if (shieldUnique == 47)
			calcMagicIncrement += 8;
		else
			atk_inc += 40;
	}

	calcMagicIncrement = ((calcMagicIncrement + 1) >> 2);

	if (calcMagicIncrement > 255)
		calcMagicIncrement = 255;
	if (calcMagicIncrement < 0)
		calcMagicIncrement = 0;

	if (calcCriticalIncrement > 255)
		calcCriticalIncrement = 255;

	player->Critical = calcCriticalIncrement;
	player->MagicIncrement = calcMagicIncrement;

	atk_inc += (player->Status.Points[DEXTE] / 6);
	player->Status.Attack += atk_inc;
	player->Status.Defense += def_inc;
}

void HKD_BASE_GetCurrentScore_BlockStatus(INT32 clientid)
{
	st_Mob *player = GetMobFromIndex(clientid);

	if (player->Status.Attack > 32000)
		player->Status.Attack = 32000;

	if (player->bStatus.Attack > 32000)
		player->bStatus.Attack = 32000;

	if (player->Status.Defense > 32000)
		player->Status.Defense = 32000;

	if (player->bStatus.Defense > 32000)
		player->bStatus.Defense = 32000;

	if (player->Status.curHP > MAX_HPMP)
		player->Status.curHP = MAX_HPMP;

	if (player->bStatus.curHP > MAX_HPMP)
		player->bStatus.curHP = MAX_HPMP;

	if (player->Status.maxHP > MAX_HPMP)
		player->Status.maxHP = MAX_HPMP;

	if (player->bStatus.maxHP > MAX_HPMP)
		player->bStatus.maxHP = MAX_HPMP;

	if (player->Status.curMP > MAX_HPMP)
		player->Status.curMP = MAX_HPMP;

	if (player->bStatus.curMP > MAX_HPMP)
		player->bStatus.curMP = MAX_HPMP;

	if (player->Status.maxMP > MAX_HPMP)
		player->Status.maxMP = MAX_HPMP;

	if (player->bStatus.maxMP > MAX_HPMP)
		player->bStatus.maxMP = MAX_HPMP;
}

void __stdcall HKD_GetCurrentScore(st_Mob *player, int cId)
{
#pragma region EQUIP_BONUSES
	for (int i = 0; i < 16; i++)
	{
		INT32 LOCAL_17 = player->Equip[i].Index;

		if (LOCAL_17 > 0 && LOCAL_17 < 6500)
		{
			INT32 sanc = GetItemSanc(&player->Equip[i]);
			int Extreme = itemlistBuffer[i].Extreme,
				mCALC = sanc % 4;

			if (Extreme == 5 || mCALC == 2)
				player->DropBonus += 8;
			if (Extreme == 6 || mCALC == 3)
				player->ForceDamage += 20;
			if (Extreme == 7 || !mCALC)
				player->ExpBonus += 2;
			if (Extreme == 8 || mCALC == 1)
				player->ReflectDamage += 20;
		}
	}

	if (player->Equip[13].Index > 0) //fadas
	{
		switch (player->Equip[13].Index)
		{
			case 3900: case 3903: case 3906: case 3911: case 3912: case 3913: //fada verde
				player->ExpBonus += 16;
				break;

			case 3901: case 3904: case 3907: //azul
				player->DropBonus += 32;
				break;

			case 3902: case 3905: case 3908: //vermelha
				player->ExpBonus += 32;
				player->DropBonus += 16;
				break;

			case 3914: //prateada
				player->ExpBonus += 16;
				player->DropBonus += 32;
				break;

			case 3915:
				player->ExpBonus += 18;
				player->DropBonus += 50;
				break;
		}
	}

	for (int i = 0; i < 16; i++)
	{
		switch (mBuffer[cId].Affects[i].Index)
		{
			case 39:
				player->ExpBonus *= 2;
				break;
		}
	}

	player->DropBonus += GetBonusFromColor(player, 118);
#pragma endregion

	player->ExpBonus += Servidor.ExpBonus;
	player->DropBonus += Servidor.DropBonus;

	if (cId < MAX_PLAYER)
	{
		GetPassivas(player, cId);
		HKD_BASE_GetCurrentScore_BlockStatus(cId);
	}
}

int __stdcall GetNewSpeed(int speed, st_Mob *mob)
{
	if (speed < 0)
		speed = 0;
	if (speed > 6)
		speed = 6;

	if (mob->Equip[14].Index >= 3980 && mob->Equip[14].Index <= 3994)
		speed = 6;

	return speed;
}

void HKD_GetCurScore_ControlAffects(st_Mob *player, int loopId)
{
	int mobId = GetUserFromMob(&player);


	switch (mBuffer[mobId].Affects[loopId].Index)
	{
		case 8: // Joias
		{
			switch (mBuffer[mobId].Affects[loopId].Master)
			{
				case 8:
				{
					player->Status.Defense += player->Status.Defense / 100 * 20;
					player->Status.maxHP += player->Status.maxHP / 100 * 10;

					break;
				}
				case 16:
				{
					player->Status.Attack += player->Status.Attack / 100 * 20;
					player->Status.maxHP += player->Status.maxHP / 100 * 10;

					break;
				}
				case 128:
				{
					int fHP = player->Status.maxHP + player->Status.maxMP / 2;

					if (fHP > 32000)
						fHP = 32000;

					player->Status.maxHP = fHP;
					player->Status.maxMP /= 2;

					break;
				}
			}

			break;
		}
		case 29: // Soul
		{
			if (player->Equip[0].EFV2 == MORTAL)
				break;

			int bonus[4] = { 0, 0, 0, 0 };

			nInfo* nPlayer = &nBuffer[mobId];
			int soulInfo = nPlayer->Chars[nPlayer->Ingame.LoggedMob].SoulInfo;

			if (player->Equip[0].EFV2 == SEMIDEUS)
			{
				bonus[INTEL] = MortalStatsBonuse[soulInfo][INTEL];
				bonus[FORCA] = MortalStatsBonuse[soulInfo][FORCA];
				bonus[CONST] = MortalStatsBonuse[soulInfo][CONST];
				bonus[DEXTE] = MortalStatsBonuse[soulInfo][DEXTE];
			}
			else
			{
				bonus[INTEL] = CeleStatsBonuse[soulInfo][INTEL];
				bonus[FORCA] = CeleStatsBonuse[soulInfo][FORCA];
				bonus[CONST] = CeleStatsBonuse[soulInfo][CONST];
				bonus[DEXTE] = CeleStatsBonuse[soulInfo][DEXTE];
			}

			bonus[INTEL] = ((player->bStatus.Points[INTEL] / 100) * bonus[INTEL]);
			bonus[FORCA] = ((player->bStatus.Points[FORCA] / 100) * bonus[FORCA]);
			bonus[DEXTE] = ((player->bStatus.Points[DEXTE] / 100) * bonus[DEXTE]);
			bonus[CONST] = ((player->bStatus.Points[CONST] / 100) * bonus[CONST]);

			player->Status.Points[FORCA] += bonus[FORCA];
			player->Status.Points[DEXTE] += bonus[DEXTE];
			player->Status.Points[INTEL] += bonus[INTEL];
			player->Status.Points[CONST] += bonus[CONST];

			player->Status.Attack += bonus[FORCA] / 3;
			player->Status.maxHP += bonus[CONST] * 2;
			player->Status.maxMP += bonus[INTEL] * 2;

			break;
		}
		case 38: //8 skill ht troca
		{
			player->Status.maxMP >>= 1;
			player->Status.maxHP += player->Status.maxMP + (500 * ((player->Status.Mastery[2] << 1) / 100)) + 100;

			if (player->Status.curHP > player->Status.maxHP)
				player->Status.curHP = player->Status.maxHP;

			if (player->Status.curMP > player->Status.maxMP)
				player->Status.curMP = player->Status.maxMP;
		}
			break;
	}
}

bool __stdcall FixSkillsGetCurrentScore(const int loopIndex, st_Mob &mob, stAffects* affects)
{
	stAffects& affect = affects[loopIndex];

	if (affect.Index == 38)
	{
		mob.Status.maxMP >>= 1;
		mob.Status.maxHP += mob.Status.maxMP + (500 * ((mob.Status.Mastery[2] << 1) / 100)) + 100;

		if (mob.Status.curHP > mob.Status.maxHP)
			mob.Status.curHP = mob.Status.maxHP;

		if (mob.Status.curMP > mob.Status.maxMP)
			mob.Status.curMP = mob.Status.maxMP;
		return true;
	}

	return false;
}