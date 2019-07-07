#include "DLLMain.h"

void RecruitPerson(UINT8* m_PacketBuffer, bool* pRetn)
{
	*pRetn = true;

	p3D5* p = (p3D5*)m_PacketBuffer;

	if (p->Header.ClientId < 0 || p->Header.ClientId > 750 ||
		p->RecruitCid      < 0 || p->RecruitCid      > 750)
		return;

	INT16 LidercId = p->Header.ClientId,
		RecruitcId = p->RecruitCid;

	if (Users[p->RecruitCid].Status != 22)
		return;

	st_Mob *Lider = GetMobFromIndex(LidercId),
		*Member = GetMobFromIndex(RecruitcId);

	GetCurScore_CapeInfo(Lider);
	GetCurScore_CapeInfo(Member);

	if (Lider->Equip[12].Index != 509 && (Lider->Equip[12].Index < 526 || Lider->Equip[12].Index > 528))
	{
		SendClientMessage(LidercId, "Para recrutar é necessário ser Lider ou SubLider.");

		return;
	}

	else if (Member->Equip[12].Index)
	{
		SendClientMessage(LidercId, "Este jogador já tem guild.");

		return;
	}

	else if (Member->CapeInfo != Lider->CapeInfo)
	{
		SendClientMessage(LidercId, "Jogador deve pertencer ao mesmo reino para ser recrutado.");

		return;
	}

	else if (Lider->Gold < 4000000)
	{
		SendClientMessage(LidercId, "São necessários 4.000.000 (4 Milhões) de gold para recrutar.");

		return;
	}

	else if (Guilds[Lider->GuildIndex].Members > 64000)
	{
		SendClientMessage(LidercId, "Sua guild já atingiu o tamanho máximo.");

		return;
	}

	else if (nBuffer[RecruitcId].Chars[nBuffer[RecruitcId].Ingame.LoggedMob].Cidadania != Guilds[Lider->GuildIndex].Cidadania)
	{
		SendClientMessage(LidercId, "O novo membro deve ter a mesma cidadania da guild.");

		return;
	}

	Guilds[Lider->GuildIndex].Members++;

	Lider->Gold -= 4000000;

	memcpy(&Member->GuildIndex, &Lider->GuildIndex, sizeof(unsigned short));
	memcpy(&Member->Equip[12], &Lider->Equip[12], sizeof st_Item);

	Member->Equip[12].Index = 508;

	SendItem(RecruitcId, EQUIP, 12, &Member->Equip[12]);

	SendScore(LidercId);
	SendScore(RecruitcId);
	SendEtc(LidercId);
	SendEtc(RecruitcId);

	BYTE packet[108];

	BASE_GetCreateMob(RecruitcId, (BYTE*)&packet);
	GridMulticast(RecruitcId, Member->TargetX, Member->TargetY, (void*)&packet);

	SendCreateMob(RecruitcId, RecruitcId, 1);

	char szMsg[108];
	sprintf_s(szMsg, "! Bem vindo a guild %s!!", Guilds[Lider->GuildIndex].GuildName);
	SendClientMessage(p->RecruitCid, szMsg);

	sprintf(szMsg, "Jogador %s acaba de entrar para a guild.", Member->Name);

	SendGuildMessage(Lider->GuildIndex, szMsg, 0xFF00BFFF);
}