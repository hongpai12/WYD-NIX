#include "SendAffect.h"
#include "Functions.h"
#include "Send.h"

void FixSendAffect_SERVER(BYTE *m_PacketBuffer, BYTE *packetBuffer)
{
	*(DWORD*)&m_PacketBuffer[44] = *(DWORD*)&packetBuffer[40];
	*(DWORD*)&m_PacketBuffer[40] = 0x0;
}

static int(*BASE_GetDoubleCritical)(st_Mob*, unsigned short *, unsigned short *, unsigned char *) = (int(__cdecl*)(st_Mob *, unsigned short *, unsigned short *, unsigned char *)) 0x004015FA; // Lib: Basedef.obj Class : (null)

void OitavaHT(int ClientID, int OtherID)
{
	if (ClientID < 0 || OtherID < 0 || ClientID > 30000 || OtherID > 30000)
		return;

	st_Mob *player = (st_Mob*)GetMobFromIndex(ClientID);
	st_Mob *mob = (st_Mob*)GetMobFromIndex(OtherID);

	p36C p;
	memset(&p, 0, sizeof p36C);

	p.Header.ClientId = 30000;
	p.Header.Size = sizeof p36C;
	p.Header.PacketId = 0x36C;

	p.m_AffectedPosition.X = mob->TargetX;
	p.m_AffectedPosition.Y = mob->TargetY;
	p.m_AttackerPosition.X = player->TargetX;
	p.m_AttackerPosition.Y = player->TargetY;

	p.m_ClientId = ClientID;

	p.m_CurrentExp = player->Exp;

	p.m_FlagLocal = 1;
	p.m_Motion = -1;
	p.m_SkillId = 79;

	for (int i = 0; i < 8; i++)
	{
		// Pega a evasão do personagem
		INT32 parry = GetParryRate(mob, mob->Parry);
		INT32 randInterator = rand() % 1000;

		if (randInterator <= (parry / 2))
		{
			// Caso o rand seja <= a evasão é configurado o miss
			p.m_Target[i].Index = OtherID;
			p.m_Target[i].Damage = -1;

			continue;
		}

		int RemoveDamage = BASE_GetDamage(player->Status.Attack, (OtherID <= MAX_PLAYER) ? (mob->Status.Defense << 1) : mob->Status.Defense, 0) / 2;
		int Damage = RemoveDamage;

		if (OtherID < 750)
			RemoveDamage = Damage * 4 / 9;

	    Damage = ControllDamage(ClientID, OtherID, RemoveDamage, 3);

		if (!strcmp(mob->Name, "Arvore") && OtherID > MAX_PLAYER)
		{
			Damage = 0;
		}
		if ((!strcmp(mob->Name, "Kefra") || !strcmp(mob->Name, "Quione")) && OtherID > MAX_PLAYER)
		{
			int newDamage = Damage / 1000;

			if (mob->Status.curHP - newDamage <= 0)
			{
				p.m_Target[i].Damage = Damage;
				p.m_Target[i].Index = OtherID;

				mob->Status.curHP = 0;

				break;
			}
			else
			{
				mob->Status.curHP -= newDamage;

				//Users[OtherID].Potion.CountHp -= Damage;
			}
		}
		else
		{
			if (mob->Status.curHP - Damage <= 0)
			{
				p.m_Target[i].Damage = Damage;
				p.m_Target[i].Index = OtherID;

				mob->Status.curHP = 0;

				break;
			}
			else
			{
				mob->Status.curHP -= Damage;

				//Users[OtherID].Potion.CountHp -= Damage;
			}
		}

		p.m_Target[i].Damage = Damage;
		p.m_Target[i].Index = OtherID;
	}

	p.m_CurrentMp = player->Status.curMP;

	GridMulticast(ClientID, player->TargetX, player->TargetY, &p);

	p.m_FlagLocal = 0;

	Sendpack((BYTE*)&p, ClientID, sizeof p36C);

	if (!mob->Status.curHP)
		MobKilled(OtherID, ClientID, 0, 0);

	SendScore(OtherID);
}

void FixUseSkill(BYTE *m_PacketBuffer, bool* pRetn)
{
	p39D_754 *p = (p39D_754*)m_PacketBuffer;

	INT16 cId = p->Header.ClientId;

	p754_SendAffect *oldp = (p754_SendAffect*)m_PacketBuffer;

	INT16 oldcId = oldp->Header.ClientId;

	st_Mob *player = GetMobFromIndex(oldcId);

	if (!p->m_SkillId) // Se não for skill 
		return;

	if (p->m_SkillId == 103) // Skill Kefra sem ser mob
	{
		*pRetn = true;

		CloseUser(cId);

		return;
	}

	else if (p->Header.PacketId == 0x36C && p->m_SkillId == 79)
	{
		OitavaHT(cId, p->m_Target.Index);

		*pRetn = true;

		return;
	}

	switch (p->m_SkillId)
	{
		case 27: case 29:
			*(DWORD*)&m_PacketBuffer[12] = 0x0;
			break;
#pragma region RENASCIMENTO
		case 31: // Renascimento
		{
			*pRetn = true;

			INT16 alvo = p->m_Target.Index;

			if (cId == alvo)
				return;
			else if (alvo < 0 || alvo > 750)
				return;

			bool HasOnGroup = false;

			if (player->Leader <= 0) // Lider do grupo ou sem grupo
			{
				for (int i = 0; i < 12; i++)
				{
					if (player->PartyList[i] == alvo)
					{
						HasOnGroup = true;
						break;
					}
				}
			}
			else if (player->Leader != -1)
			{
				st_Mob *leader = GetMobFromIndex(player->Leader);

				for (int i = 0; i < 12; i++)
				{
					if (leader->PartyList[i] == alvo)
					{
						HasOnGroup = true;
						break;
					}
				}
			}

			if (!HasOnGroup)
				return;

			st_Mob *deadPerson = GetMobFromIndex(alvo);

			if (deadPerson->Status.curHP > 0)
				return;
			if (!player->Status.curHP || !player->Status.curMP)
				return;

			deadPerson->Status.curHP = deadPerson->Status.maxHP / 2;
			deadPerson->Status.curMP = deadPerson->Status.maxMP / 2;

			deadPerson->bStatus.curHP = deadPerson->bStatus.maxHP / 2;
			deadPerson->bStatus.curMP = deadPerson->bStatus.maxMP / 2;

			Users[alvo].Potion.CountMp = deadPerson->Status.curMP;
			Users[alvo].Potion.CountHp = deadPerson->Status.curHP;

			player->Status.curMP = 0;
			player->bStatus.curMP = 0;

			Users[cId].Potion.CountMp = player->Status.curMP;
			SetReqMp(cId);
			SendScore(cId);

			GetCurrentScore(alvo);

			BASE_GetHpMp(deadPerson);

			SendScore(alvo);
			SendCreateMob(alvo, alvo, 1);

			DoTeleport(alvo, deadPerson->TargetX, deadPerson->TargetY);

			break;
		}
#pragma endregion
#pragma region EVOCACOES
		case 56: case 57: case 58:
		case 59: case 60: case 61:
		case 62: case 63: // Torna a evocação compatível com a 75x
		{
			*pRetn = true;

			UINT8 summonId = p->m_SkillId - 56;

			INT16 LeaderCid = p->Header.ClientId;

			if (player->Leader > 0 && player->Leader < MAX_PLAYER)
				LeaderCid = player->Leader;

			st_Mob *Lider = GetMobFromIndex(LeaderCid);

			// Verifica se já existem evocações no grupo
			// Caso elas já existam, verifica se são do mesmo personagem que evocou
			// Caso seja, remove e evoca novas
			// Caso não seja, retorna e impede que evoque por cima
			for (int i = 0; i < 12; i++)
			{
				if (Lider->PartyList[i] == cId)
					continue;

				if (Lider->PartyList[i] <= 1000)
					continue;

				st_Mob *tmpMob = (st_Mob*)GetMobFromIndex(Lider->PartyList[i]);

				for (int e = 0; e < 8; e++)
				{
					if (tmpMob->Equip[0].Index == MobFaces[e])
					{
						if (tmpMob->Summoner != cId)
							return;

						RemoveParty(Lider->PartyList[i]);
						SendRemoveParty(Lider->Leader, Lider->PartyList[i]);
					}
				}
			}

			static const UINT8 MobDivisor[8] = { 31, 36, 42, 51, 42, 85, 127, 255 };

			// Essa conta pega o número de mobs que a skill deve criar
			INT16 numMob = player->Status.Mastery[2] / MobDivisor[summonId];

			if (!numMob)
				numMob += 1;

			for (int i = 0; i < numMob; i++)
				GenerateSummon(cId, summonId, 0);

			break;
		}
#pragma endregion
#pragma region TRANSFORMACOES
		case 64: case 66: case 68:
		case 70: case 71:
		{
			if (player->Equip[13].Index > 4151 && player->Equip[13].Index <= 4183)
			{
				// Está de traje
				SendClientMessage(cId, "Impossível utilizar esta skill enquanto trajado.");

				*pRetn = true;

				return;
			}

			break;
		}
#pragma endregion
#pragma region VINHAS
		case 98: // Vinha - Skill sephira
			CreateMob("Vinha", p->m_AffectedPosition.X, p->m_AffectedPosition.Y, "npc");
			break;
#pragma endregion
#pragma region RESSUREICAO
		case 99: // Ressureição - Skill sephira
		{
			*pRetn = true;

			if (nBuffer[cId].Ingame.hasFailed)
			{
				SendClientMsg(cId, "Espere [%d] segundos para utilizar novamente.", nBuffer[cId].Ingame.hasFailed);

				return;
			}
			else if (player->TargetX >= 1050 && player->TargetY >= 1945 &&
					 player->TargetX <= 1250 && player->TargetY <= 2150)
			{
				SendClientMessage(cId, "Impossível utilizar esta skill nessa área.");

				return;
			}

			INT16 Level = player->bStatus.Level + 1;

			if (player->Equip[0].EFV2 > SEMIDEUS)
				Level += 100;
			if (player->Equip[0].EFV2 == DEUS)
				Level += 100;

			INT8 resChances = Level / 4;

			srand(time(NULL) / 5 * (rand() % 500) * 5);

			if (rand() % 100 <= resChances)
			{
				player->Status.curHP = player->Status.maxHP / 2;
				player->Status.curMP = player->Status.maxMP / 2;
				player->bStatus.curHP = player->bStatus.maxHP / 2;
				player->bStatus.curMP = player->bStatus.maxMP / 2;

				SetReqHp(cId);
				SetReqMp(cId);

				GetCurrentScore(cId);

				BASE_GetHpMp(player);

				SendScore(cId);
				SendCreateMob(cId, cId, 1);
			}
			else
				if (!nBuffer[cId].Ingame.hasFailed)
					nBuffer[cId].Ingame.hasFailed = 60;

			break;
		}
#pragma endregion
#pragma region SOUL
		case 102: // Soul - Melhor buff ever
		{
			*pRetn = true;

			if (player->Equip[0].EFV2 == MORTAL)
				return;

			bool hasSoulOn = false;

			int calcTime = (player->Equip[0].EFV2 * player->Status.Level) / 3;

			for (int i = 0; i < 16; i++)
				if (mBuffer[cId].Affects[i].Index == 29)
				{
					hasSoulOn = true;
					mBuffer[cId].Affects[i].Master = nBuffer[cId].Chars[nBuffer[cId].Ingame.LoggedMob].SoulInfo;
					mBuffer[cId].Affects[i].Time = calcTime;
					mBuffer[cId].Affects[i].Value = 4;
					break;
				}

			if (!hasSoulOn)
			{
				for (int i = 0; i < 16; i++)
					if (!mBuffer[cId].Affects[i].Index)
					{
						mBuffer[cId].Affects[i].Index = 29;
						mBuffer[cId].Affects[i].Master = nBuffer[cId].Chars[nBuffer[cId].Ingame.LoggedMob].SoulInfo;
						mBuffer[cId].Affects[i].Time = calcTime;
						mBuffer[cId].Affects[i].Value = 4;
						break;
					}
			}

			GetCurrentScore(cId);
			SendStats(cId);
			SendScore(cId);
			break;
		}
#pragma endregion
	}
}

void FixSendAffect_CLIENT(BYTE *m_PacketBuffer, bool* pRetn)
{
	PacketHeader *p = (PacketHeader*)m_PacketBuffer;

	if (p->ClientId < 750)
		FixUseSkill((BYTE*)m_PacketBuffer, pRetn);

	*(DWORD*)&m_PacketBuffer[40] = *(DWORD*)&m_PacketBuffer[44];
	*(DWORD*)&m_PacketBuffer[44] = 0x0;
}

void FixSendAffect39E_SERVER(BYTE *m_PacketBuffer, BYTE *packetBuffer)
{
	if (*(WORD*)m_PacketBuffer < 50)
		return;

	*(DWORD*)&m_PacketBuffer[48] = *(DWORD*)&packetBuffer[44];
	*(DWORD*)&m_PacketBuffer[44] = *(DWORD*)&packetBuffer[40];
	*(DWORD*)&m_PacketBuffer[40] = 0x0;
}

void FixSendAffect39E_CLIENT(BYTE *m_PacketBuffer, bool* pRetn)
{
	if (*(WORD*)m_PacketBuffer < 50)
		return;

	*(DWORD*)&m_PacketBuffer[40] = *(DWORD*)&m_PacketBuffer[44];
	*(DWORD*)&m_PacketBuffer[44] = *(DWORD*)&m_PacketBuffer[48];
	*(DWORD*)&m_PacketBuffer[48] = 0x0;
}

void __stdcall SendAffect(UINT32 ClientID)
{
	if (ClientID < 0 || ClientID > 740)
		return;

	p3B9 pack_3B9;

	pack_3B9.Header.PacketId = 0x3B9;
	pack_3B9.Header.Size = 140;
	pack_3B9.Header.ClientId = ClientID;

	memset(&pack_3B9.Affect[0], 0x0, 140 - 12);

	for (short i = 0; i <= 15; i++)
	{
		if (mBuffer[ClientID].Affects[i].Time != 0)
		{
			pack_3B9.Affect[i].Index = (BYTE)mBuffer[ClientID].Affects[i].Index;
			pack_3B9.Affect[i].Master = (BYTE)mBuffer[ClientID].Affects[i].Master;
			pack_3B9.Affect[i].Value = (DWORD)mBuffer[ClientID].Affects[i].Value;
			pack_3B9.Affect[i].Time = (WORD)mBuffer[ClientID].Affects[i].Time;
		}
	}

	Sendpack((BYTE*)&pack_3B9, ClientID, sizeof pack_3B9);
}

void SkillAlchemy(UINT8 *m_PacketBuffer, bool *pRetn)
{
	*pRetn = true;

	st_Window *pServer = (st_Window*)m_PacketBuffer;

	INT16 cId = pServer->Header.ClientId;

	if (cId <= NULL || cId >= MAX_PLAYER)
		return;

	st_Mob *player = GetMobFromIndex(cId);

	if (pServer->Slot[0] == 0xFF || pServer->Slot[1] == 0xFF || pServer->Slot[2] == 0xFF)
		SendClientMessage(cId, "Coloque os três itens necessários.");
	else
	{
		for (char i = 0; i < 3; i++)
		{
			if (memcmp(&pServer->ItemId[i], &player->Inventory[pServer->Slot[i]], sizeof st_Item))
				return;

			else if (pServer->ItemId[i].Index != player->Inventory[pServer->Slot[i]].Index)
				return;
		}

		Composition(pServer, player);

		SendClientSignalParm(cId, 0x7530, 0x3A7, 2);
	}
}

int GetManaControlStats(DWORD Target, DWORD Damage)
{
	st_Mob *mob = GetMobFromIndex(Target);

	for (int i = 0; i < 16; i++)
	{
		if (mBuffer[Target].Affects[i].Index == 18)
		{
			UINT16 manaSpent = ((Damage / 100) * 60) / 3;

			if (manaSpent > mob->Status.curMP)
				manaSpent = mob->Status.curMP;

			mob->Status.curMP -= manaSpent;
			Users[Target].Potion.CountMp -= manaSpent;

			SetReqMp(Target);
			SendScore(Target);

			Damage -= manaSpent * 3;

			break;
		}
	}

	return Damage;
}

int __stdcall ControllDamage(DWORD Attacker, DWORD Target, DWORD Damage, DWORD TYPE)
{
	if (Attacker < 0 || Target < 0 || Attacker > 30000 || Target > 30000)
		return Damage;

	st_Mob *player = GetMobFromIndex(Attacker),
		   *mob = GetMobFromIndex(Target);

	if (Target < MAX_PLAYER)
	{
#pragma region REFLECTION
		// Reflexão de dano recebido do atacante
		int Reflection = ((Damage / 3) * GetBonusFromColor(mob, 121)) / 100;
		if (Reflection > 0 && Attacker < MAX_PLAYER)
		{
			Reflection = GetManaControlStats(Attacker, Reflection);

			player->Status.curHP -= Reflection;
			Users[Attacker].Potion.CountHp -= Reflection;

			SetReqHp(Attacker);
			SendScore(Attacker);
		}
#pragma endregion

#pragma region CONTROLE_MANA
		Damage = GetManaControlStats(Target, Damage);
#pragma endregion
	}
	
	if (Attacker <= MAX_PLAYER)
	{
		if (Target > MAX_PLAYER)
		{
			if (mob->TargetX >= 1039 && mob->TargetX <= 1138 && mob->TargetY >= 1421 && mob->TargetY <= 1519)
			{
				return 0;
			}

			for (int i = 0; i < 16; i++)
			{
				if (mBuffer[Attacker].Affects[i].Index == 30)
				{
					// Frango
					Damage += 2000;
					break;
				}
			}

			// Bônus de dano PVM vindo de cor dos itens
			Damage += (Damage * GetBonusFromColor(player, 120)) / 100;
		}
		
#pragma region ABSORPTION
		// Absorção do dano inflingido no alvo
		int Absorption = (Damage * GetBonusFromColor(player, 122)) / 100;

		if (Target < MAX_PLAYER)
			Absorption = ((Damage / 3) * GetBonusFromColor(player, 122)) / 100;

		if (player->Status.curHP + Absorption > player->Status.maxHP)
			Absorption = player->Status.maxHP - player->Status.curHP;
		if (Absorption > 0)
		{
			if (Attacker > 0 && Attacker < MAX_PLAYER)
			{
				player->Status.curHP += Absorption;
				Users[Attacker].Potion.CountHp += Absorption;

				SetReqHp(Attacker);
				SendScore(Attacker);
			}
		}
#pragma endregion
	}

	return Damage;
}

INT32 HKD_GetAttack_CriticalHit(INT32 clientId, p39D_613 *p)
{
	// hook by shepher
	st_Mob *mob = (st_Mob*)(0x0015C69B8);
	static unsigned short attackCount = 0;

	if (p->m_SkillId > 0 && p->m_SkillId < 96)
		return mob[clientId].Status.Attack;

	if (clientId > MAX_PLAYER)
	{
		mob[clientId].Critical = 51;

		if (!strcmp(mob[clientId].Name, "Kefra") || !strcmp(mob[clientId].Name, "Quione"))
			mob[clientId].Critical = 127;
	}

	BASE_GetDoubleCritical(&mob[clientId], &attackCount, &p->m_AttackCount, &p->m_Critical);

	INT32 damage = mob[clientId].Status.Attack;
	if (p->m_Critical & 2)
		damage = (((Rand() % 4) + 0x0C) * damage) / 10;

	return damage;
}

bool __stdcall ControllBossDamage(DWORD mobId, DWORD damage)
{
	bool retn = true;
	
	st_Mob* mob = GetMobFromIndex(mobId);

	if ((mob->Equip[13].Index == 786 || mob->Equip[13].Index == 1936 || mob->Equip[13].Index == 1937) && mobId > MAX_PLAYER)
	{
		int sanc = GetItemSanc(&mob->Equip[13]);
		if (sanc < 2)
			sanc = 2;

		if (mob->Equip[13].Index == 1936)
			sanc *= 10;
		else if (mob->Equip[13].Index == 1937)
			sanc *= 1000;

		if (damage <= 0)
			damage = 1;

		int calc = damage / sanc;

		if (calc > mob->Status.curHP)
			calc = mob->Status.curHP;

		mob->Status.curHP = mob->Status.curHP - damage;

		retn = false;
	}

	return retn;
}