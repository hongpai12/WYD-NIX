#include "Funcoes.h"

int BASE_GetHpMp(st_Mob* mob) //00402110 - ok - atualizada(reconhece evolucao)
{
	int clsInfo = mob->ClassInfo;
	if (clsInfo < 0 || clsInfo > 3)
		return FALSE;

	int baseHP = BaseSIDCHM[clsInfo][4];
	int calcHP = ((mob->Status.Points[CONST] - BaseSIDCHM[clsInfo][2]) << 1);
	int calcHPLvl = 0;

	int hpPerLevel = g_pBASE_IncrementStatus[clsInfo][m_HP] * 4;
	int mpPerLevel = g_pBASE_IncrementStatus[clsInfo][m_MP] * 4;

	calcHPLvl = ((mob->bStatus.Level + 1) * hpPerLevel);

	int baseMP = BaseSIDCHM[clsInfo][5];
	int calcMP = ((mob->Status.Points[INTEL] - BaseSIDCHM[clsInfo][3]) << 1);
	int calcMPLvl = 0;

	calcMPLvl = ((mob->bStatus.Level + 1) * (mpPerLevel * 4));

	INT16 cId = GetUserFromMob(mob);

	if (cId < MAX_PLAYER)
	{
		nInfo* nP = &nBuffer[cId];

		if (mob->Equip[0].EFV2 >= SEMIDEUS)
		{
			int sdlevel = (mob->bStatus.Level - nP->Chars[nP->Ingame.LoggedMob].SemiDeusLevel);

			calcHPLvl += sdlevel * (g_pBASE_IncrementStatus[clsInfo][2] * 4);
			calcMPLvl += sdlevel * (g_pBASE_IncrementStatus[clsInfo][3] * 4);
		}
		else if (mob->Equip[0].EFV2 == DEUS)
		{
			int dlevel = (mob->bStatus.Level - nP->Chars[nP->Ingame.LoggedMob].DeusLevel);

			calcHPLvl += dlevel * (g_pBASE_IncrementStatus[clsInfo][4] * 4);
			calcMPLvl += dlevel * (g_pBASE_IncrementStatus[clsInfo][5] * 4);
		}
	}

	UINT32 fHP = baseHP + calcHP + calcHPLvl,
		   fMP = baseMP + calcMP + calcMPLvl;

	INT16 hpBonus = GetBonusFromColor(mob, 119),
		  mpBonus = GetBonusFromColor(mob, 125);

	fHP = (fHP / 100) * (100 + hpBonus);
	fMP = (fMP / 100) * (100 + mpBonus);

	if (fHP > MAX_HPMP)
		fHP = MAX_HPMP;
	if (fHP <= 0)
		fHP = BaseSIDCHM[clsInfo][4];

	if (fMP > MAX_HPMP)
		fMP = MAX_HPMP;
	if (fMP <= 0)
		fMP = BaseSIDCHM[clsInfo][5];

	mob->bStatus.maxHP = fHP;
	mob->Status.maxHP  = fHP;

	mob->bStatus.maxMP = fMP;
	mob->Status.maxMP  = fMP;

	return TRUE;
}
