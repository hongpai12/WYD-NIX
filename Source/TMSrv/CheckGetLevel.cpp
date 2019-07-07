#include "DLLMain.h"

int __stdcall HKD_CheckGetLevel()
{
	st_Mob *player;

	__asm MOV player, ECX

	INT16 cId = GetUserFromMob(player);

	int retnValue = 0;
	int Class = player->ClassInfo;
	int LOCAL4 = 0;

	int ActualLevel = player->bStatus.Level;
	if (ActualLevel >= 99)
		return 0;

	int PlayerExp = player->Exp;
	int ActualLevelExp = exp_table[ActualLevel];
	int NextLevelExp = exp_table[ActualLevel + 1];

	int LOCAL9 = (NextLevelExp - ActualLevelExp) + (NextLevelExp & 3) >> 2;
	int LOCAL10 = ActualLevelExp + LOCAL9;
	int LOCAL11 = (LOCAL9 * 2) + ActualLevelExp;
	int LOCAL12 = (LOCAL9 * 3) + ActualLevelExp;
	int LOCAL13 = NextLevelExp;

	if (PlayerExp > LOCAL12)
		LOCAL4 = 3;
	else if (PlayerExp > LOCAL11)
		LOCAL4 = 2;
	else if (PlayerExp > LOCAL10)
		LOCAL4 = 1;

	if (PlayerExp > NextLevelExp)
	{
		player->bStatus.Level++;
		player->bStatus.Defense += 4;

		if (player->Equip[0].EFV2 >= SEMIDEUS)
			player->bStatus.Defense += 2;
		if (player->Equip[0].EFV2 == DEUS)
			player->bStatus.Defense += 2;

		player->SkillPoint += 12;
		player->MasterPoint += 8;

		int bonusHp = g_pBASE_IncrementStatus[Class][m_HP],
			bonusMp = g_pBASE_IncrementStatus[Class][m_MP];

		if (player->GuildIndex)
		{
			Guilds[player->GuildIndex].Points += 10;

			if (player->Equip[0].EFV2 >= SEMIDEUS)
				Guilds[player->GuildIndex].Points += 15;
			if (player->Equip[0].EFV2 == DEUS)
				Guilds[player->GuildIndex].Points += 25;
		}

		if (player->Equip[0].EFV2 >= SEMIDEUS)
		{
			player->StatusPoint += 30;

			bonusHp = g_pBASE_IncrementStatus[Class][2];
			bonusMp = g_pBASE_IncrementStatus[Class][3];

			if (player->Equip[0].EFV2 == DEUS)
			{
				bonusHp = g_pBASE_IncrementStatus[Class][4];
				bonusMp = g_pBASE_IncrementStatus[Class][5];

				player->StatusPoint += 40;
			}
		}

		if (player->Status.Level >= 30)
			player->StatusPoint += 10;
		
		player->bStatus.maxHP += bonusHp * 4;
		player->bStatus.maxMP += bonusMp * 4;

		player->StatusPoint += 20;

		player->bStatus.curHP = player->bStatus.maxHP;
		player->bStatus.curMP = player->bStatus.maxMP;

		BASE_GetBonusScorePoint(player);
		BASE_GetBonusSkillPoint(player);
		BASE_GetHpMp(player);

		GetCurrentScore(cId);

		player->Status.curHP = player->Status.maxHP;
		player->Status.curMP = player->Status.maxMP;

		retnValue = 4;
		player->Segment = 0;

		SendStats(cId);
		SendScore(cId);
		SendEtc(cId);

		sendLog(INFO, cId, "%s %s subiu para o nível %d evolução %d.", nBuffer[cId].Login, player->Name, player->bStatus.Level, player->Equip[0].EFV2);
	}
	else if (PlayerExp >= LOCAL12 && player->Segment < 3)
	{
		retnValue = 3;
		player->Segment = 3;
	}
	else if (PlayerExp >= LOCAL11 && player->Segment < 2)
	{
		retnValue = 2;
		player->Segment = 2;
	}
	else if (PlayerExp >= LOCAL10 && player->Segment < 1)
	{
		retnValue = 1;
		player->Segment = 1;
	}
	else
		return retnValue;

	if (retnValue >= 1 && retnValue <= 3)
	{
		player->bStatus.curHP = player->bStatus.maxHP;
		player->bStatus.curMP = player->bStatus.maxMP;

		player->Status.curHP = player->Status.maxHP;
		player->Status.curMP = player->Status.maxMP;

		GetCurrentScore(cId);
		SendStats(cId);
		SendScore(cId);
		SendEtc(cId);

		return retnValue;
	}

	return retnValue;
}

int __stdcall HKD_FixLevelBonus(st_Mob* player)
{
	int level = player->bStatus.Level * 4;

	if (player->Equip[0].EFV2 >= SEMIDEUS)
		level += 200;
	if (player->Equip[0].EFV2 == DEUS)
		level += 100;

	if (level > 700)
		level = 700;
	if (level < 0)
		level = 0;

	return level;
}

int __stdcall HKD_FixLevelBonusFromCid(DWORD cId)
{
	int level = 0;

	if (cId < 0)
		return 0;
	else if (cId > 30000)
		return 0;

	st_Mob *mob = (st_Mob*)(0x0015C69B8);

	level = mob[cId].bStatus.Level + 50;

	if (cId < MAX_PLAYER)
	{
		level *= 4;

		if (mob[cId].Equip[0].EFV2 >= SEMIDEUS)
			level += 200;
		if (mob[cId].Equip[0].EFV2 == DEUS)
			level += 100;

		if (level > 700)
			level = 700;
		if (level < 0)
			level = 0;
	}

	return level;
}