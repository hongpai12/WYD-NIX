#include "DLLMain.h"

int BASE_GetBonusSkillPoint(st_Mob* mob)
{
	int multLevel = mob->Status.Level * 12;

	int qstInfo = mob->QuestInfo & 1;
	if (qstInfo != 0)
		multLevel = multLevel + 9;

	int clsInfo = mob->ClassInfo;
	if (clsInfo < 0 || clsInfo > 3)
		return FALSE;

	int baseSkillPoint = 0;
	int skillPos = 1;

	for (int i = 0; i < 24; i++)
	{
		if ((mob->Learn & skillPos) != 0)
			baseSkillPoint = g_pSpell[(clsInfo * 24) + i].SkillPoint;

		skillPos = skillPos << 1;
	}

	int skillPoint = multLevel - baseSkillPoint;
	if (skillPoint >= 0)
		mob->SkillPoint = skillPoint;

	return TRUE;
}