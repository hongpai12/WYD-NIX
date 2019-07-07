#include "DLLMain.h"

int BASE_GetBonusScorePoint(st_Mob* mob)
{
	if (mob == NULL)
		return FALSE;

	int clsInfo = mob->ClassInfo;
	if (clsInfo < 0 || clsInfo > 3)
		return FALSE;

	INT16 cId = GetUserFromMob(mob);
	if (cId < 0 || cId > MAX_PLAYER)
		return FALSE;

	int LoggedMob = nBuffer[cId].Ingame.LoggedMob;
	if (LoggedMob < 0 || LoggedMob > 3)
		return FALSE;

	int decStr = mob->bStatus.Points[FORCA] - BaseSIDCHM[clsInfo][FORCA];
	int decInt = mob->bStatus.Points[INTEL] - BaseSIDCHM[clsInfo][INTEL];
	int decDes = mob->bStatus.Points[DEXTE] - BaseSIDCHM[clsInfo][DEXTE];
	int decCon = mob->bStatus.Points[CONST] - BaseSIDCHM[clsInfo][CONST];

	int calcDecs = decStr + decInt + decDes + decCon;
	int lvl = mob->bStatus.Level;
	int totalStats = lvl * 20;

	INT16 semiDeusLevel = nBuffer[cId].Chars[LoggedMob].SemiDeusLevel,
		DeusLevel = nBuffer[cId].Chars[LoggedMob].DeusLevel;

	if (lvl > 30)
		totalStats += (lvl - 30) * 10;

	if (mob->Equip[0].EFV2 >= SEMIDEUS)
	{
		totalStats += 600;
		totalStats += (lvl - semiDeusLevel) * 30;

		if (mob->Equip[0].EFV2 == DEUS)
		{
			totalStats += (lvl - DeusLevel) * 40;

			if (DeusLevel == 94)
				totalStats += 1500;
			else if (DeusLevel == 95)
				totalStats += 1000;
			else if (DeusLevel == 96)
				totalStats += 800;
			else if (DeusLevel == 97)
				totalStats += 600;
			else if (DeusLevel == 98)
				totalStats += 400;
			else
				totalStats += 300;
		}
	}

	if (calcDecs > 6020)
	{
		int classInfo = mob->ClassInfo,
			dif = calcDecs - 6020;
		// Máximo de pontos que pode ser ganho upando
		for (int i = 0; i < 4; i++)
		{
			if (mob->bStatus.Points[i] > BaseSIDCHM[classInfo][i])
			{
				int points = mob->bStatus.Points[i] - BaseSIDCHM[classInfo][i];
				if (dif > points)
				{
					dif -= points;
					mob->bStatus.Points[i] = BaseSIDCHM[classInfo][i];
				}
				else
				{
					mob->bStatus.Points[i] -= dif;
					dif = 0;
					break;
				}
			}
		}

		calcDecs = 6020;
	}

	mob->StatusPoint = totalStats - calcDecs;

	return TRUE;
}