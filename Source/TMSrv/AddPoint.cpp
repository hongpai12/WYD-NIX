#include "PacketControl.h"

void FixAddPoint_CLIENT(BYTE *m_PacketBuffer, bool *pRetn)
{
	p277 *p = (p277*)m_PacketBuffer;

	INT16 cId = p->Header.ClientId;

	if (cId < 0 || cId > MAX_PLAYER)
		return;

	st_Mob *player = GetMobFromIndex(cId);

	nInfo *nP = &nBuffer[cId];

	switch (p->Mode)
	{
		case 0:
		{
			if (player->StatusPoint <= 0)
			{
				sendLog(ERROR, cId, "%s %s tentou distribuir pontos de status sem os possuir.", nP->Login, player->Name);

				SendClientMessage(cId, "Não possui pontos a distribuir.");

				*pRetn = true;

				return;
			}
			else if (player->StatusPoint >= 200)
			{
				INT16 pontosDistribuir = 100;

				if (player->bStatus.Points[p->Info] + pontosDistribuir > MAX_STATUS_POINTS)
					pontosDistribuir = MAX_STATUS_POINTS - player->bStatus.Points[p->Info];
				if (pontosDistribuir <= 0)
				{
					sendLog(ERROR, cId, "%s %s tentou distribuir pontos em %d além do limite.", nP->Login, player->Name, p->Info);
					SendClientMessage(cId, "Limite de pontos neste status alcançado.");

					*pRetn = true;

					return;
				}
				else
				{
					player->bStatus.Points[p->Info] += pontosDistribuir;
					player->StatusPoint -= pontosDistribuir;

					BASE_GetHpMp(player);

					SendScore(cId);

					sendLog(INFO, cId, "%s %s distribuiu 100 pontos em %d.", nP->Login, player->Name, p->Info);
				}
			}
			else
			{
				if (player->bStatus.Points[p->Info] + 1 > MAX_STATUS_POINTS)
				{
					sendLog(ERROR, cId, "%s %s tentou distribuir pontos em %d além do limite.", nP->Login, player->Name, p->Info);

					SendClientMessage(cId, "Limite de pontos neste status já atingido.");

					*pRetn = true;

					return;
				}
				else
					sendLog(INFO, cId, "%s %s distribuiu 1 ponto em %d.", nP->Login, player->Name, p->Info);
			}

			BASE_GetBonusScorePoint(player);
		}
		break;
		case 2:
		{
			int skillID = (p->Info - 5000) % 24,
				skillDiv = ((p->Info - 5000) % 24 / 8) + 1;

			if ((skillID % 24) % 8 == 7)
			{
				for (int i = 1; i<8; i++)
				{
					int Have = player->Learn & (1 << (skillID - i));

					if (Have == 0)
					{
						SendClientMessage(cId, "Necessário todas as skills da linhagem.");
						sendLog(ERROR, cId, "%s %s tentou adquirir oitava skill sem possuir todas da linhagem.", nP->Login, player->Name);

						*pRetn = true;

						return;
					}
				}

				for (int i = 1; i<4; i++)
				{
					if (i == skillDiv)
						continue;

					int Have = player->Learn & (1 << ((i * 8) - 1));
					if (Have != 0)
					{
						SendClientMessage(cId, "Só é possível obter uma oitava skill por personagem.");
						sendLog(ERROR, cId, "%s %s tentou adquirir segunda oitava skill.", nP->Login, player->Name);

						*pRetn = true;

						return;
					}
				}
				if (player->Gold < 50000000)
				{
					SendClientMessage(cId, "São necessários 50 Milhões(50.000.000) de gold.");
					sendLog(ERROR, cId, "%s %s tentou adquirir oitava skill sem possuir 50KK.", nP->Login, player->Name);

					*pRetn = true;

					return;
				}

				// Não possui a skill e possui todas as funções passaram, então, retira o gold
				// TODO : Falta verificação de level
				player->Gold -= 50000000;

				SendEtc(cId);
				SendScore(cId);
				sendLog(INFO, cId, "%s %s adquiriu oitava skill.", nP->Login, player->Name);
			}			
		}

		break;
	}
}

INT32 HKD_ProcessClientMessage_ApplyBonus(BYTE *buffer, INT32 clientid)
{
	p277 *p = (p277*)buffer;

	INT16 cId = p->Header.ClientId;

	st_Mob *player = (st_Mob*)GetMobFromIndex(cId);

	if (!p->Info)
	{
		if (player->bStatus.Mastery[p->Info] >= 200)
			return 0;

		return 1;
	}

	if (player->Equip[0].EFV2 >= SEMIDEUS)
	{
		if (player->bStatus.Mastery[p->Info] < 200)
			return 1;
	}

	int Have = player->Learn & (1 << (8 * p->Info - 1));

	if (Have)
	{
		if (player->bStatus.Mastery[p->Info] >= 255)
		{
			SendClientMessage(cId, "Limite máximo de pontos.");

			return 0;
		}
	}
	else
	{
		if (player->bStatus.Mastery[p->Info] >= 200)
		{
			SendClientMessage(cId, "Limite máximo de pontos");

			return 0;
		}
	}

	return 1;
}
