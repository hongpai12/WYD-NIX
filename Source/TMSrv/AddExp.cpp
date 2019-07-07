#include "AddExp.h"

void FixSendExp_SERVER(BYTE *m_PacketBuffer, BYTE *packetBuffer)
{
	*(DWORD*)&m_PacketBuffer[20] = *(DWORD*)&packetBuffer[16];
	*(DWORD*)&m_PacketBuffer[16] = *(DWORD*)&packetBuffer[12];
	*(DWORD*)&m_PacketBuffer[12] = 0x0;
}

int __stdcall ControlExp(DWORD exp, DWORD Killer, DWORD Killed)
{
	// Função desativada
	return 0;
}

INT32 HKD_ExpInPesaAllArea(UINT32 ClientID, INT32 X, INT32 Y)
{
	// retornar 0 caso não queira que dê a experiência
	// retornar 1 caso queira que dê a experiência
	INT32 retn = 0;

	st_Mob *player = GetMobFromIndex(ClientID);

	if ((X >= 1150 && Y >= 130 && X <= 1270 && Y <= 220) || // Pesa A
		(X >= 1040 && Y >= 260 && X <= 1135 && Y <= 350) || // Pesa M
		(X >= 1295 && Y >= 290 && X <= 1325 && Y <= 365))   // Pesa N
		retn = 1;
	
	return retn;
}

int __stdcall HKD_GetExpApply(int Exp, int attackerId, int mobId)
{
	if (attackerId >= 30000 || mobId >= 30000 || attackerId < 0 || mobId < 0)
		return Exp;

	if (attackerId > MAX_PLAYER)
	{
		st_Mob *evock = GetMobFromIndex(attackerId);
		if (evock->Summoner > 0 && evock->Summoner < MAX_PLAYER)
			attackerId = evock->Summoner;
	}
	
	st_Mob *mobs = (st_Mob*)0x0015C69B8;

	int attackerLevel = mobs[attackerId].bStatus.Level + 1,
	    mobLevel      = mobs[mobId].bStatus.Level + 1;

	INT32 LOCAL_1 = (mobLevel * 400) / attackerLevel;

	if (LOCAL_1 < 320 && attackerLevel >= 10)
		LOCAL_1 = (LOCAL_1 * 2) - 400;
	else if (LOCAL_1 > 800)
		LOCAL_1 = 800;
	
	if (LOCAL_1 < 0)
		LOCAL_1 = 0;

	Exp = Exp * LOCAL_1 / 400;

	int newExp = Exp;

	newExp *= 5;

	if (mobId < MAX_PLAYER)
		newExp += (newExp / 100) * mobs[attackerId].ExpBonus;

	time_t rawnow = time(NULL);
	struct tm *now = localtime(&rawnow);

	if (now->tm_wday == 6 || !now->tm_wday)
		newExp *= 2;

	if (mobs[attackerId].Exp + newExp > 4100000000)
		newExp = 4100000000 - mobs[attackerId].Exp;

	//if (mobs[attackerId].bStatus.Level >= 99)
	//	newExp = 0;

	if (mobs[attackerId].Equip[0].EFV2 >= SEMIDEUS)
		newExp /= 10;
	if (mobs[attackerId].Equip[0].EFV2 == DEUS)
		newExp /= 10;

	Servidor.ExpCount += newExp;
	if (Servidor.ExpCount >= 2000000000)
	{
		char szTmp[120];
		sprintf(szTmp, "Jogador [%s] bateu a meta de experiência.", mobs[attackerId].Name);

		st_Item prize;
		memset(&prize, 0, sizeof st_Item);

		prize.Index = 413;
		prize.EF1 = 61;
		srand(time(NULL) / 5 * (rand() % 500) * 5);
		prize.EFV1 = rand() % 6;

		if (GiveItem(attackerId, &prize) == -1)
			SendClientMessage(attackerId, "!Não há espaço para receber a premiação.");

		SendNotice(szTmp);

		Servidor.ExpCount -= 2000000000;

		if (Servidor.ExpBonus < 100)
			Servidor.ExpBonus += 1;
	}

	return newExp;
}