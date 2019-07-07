#include "stdafx.h"

CFileDB::CFileDB()
{
	for (int i = 0; i < MAX_DBACCOUNT; i++)
	{
		pAccountList[i].Login = FALSE;
		pAccountList[i].Slot = -1;
		memset(&pAccountList[i].File, 0, sizeof(STRUCT_ACCOUNTFILE));
	}
}
CFileDB::~CFileDB()
{

}
int32_t CFileDB::GetIndex(int32_t server, int32_t id)
{
	return (server * MAX_USER) + id;
}
int32_t CFileDB::GetIndex(char* account)
{
	for (int i = 0; i < MAX_DBACCOUNT; i++)
	{
		if (pAccountList[i].Login == FALSE)
			continue;

		if (!strcmp(pAccountList[i].File.AccountName, account))
			return i;
	}
	return 0;
}
bool CFileDB::DBWriteAccount(STRUCT_ACCOUNTFILE* account)
{
	if (*account->AccountName == '\0')
	{
		Log("err, DBWriteAccount, invalid accountname %s", account->AccountName);
		return false;
	}

	char accName[ACCOUNTNAME_LENGTH] {0, };
	strncpy_s(accName, account->AccountName, ACCOUNTNAME_LENGTH);
	_strupr_s(accName);

	if (accName[0] == 'C' && accName[1] == 'O' && accName[2] == 'M' && accName[3] >= '0' && accName[3] <= '9' && accName[4] == 0)
		return false;
	if (accName[0] == 'L' && accName[1] == 'P' && accName[2] == 'T' && accName[3] >= '0' && accName[3] <= '9' && accName[4] == 0)
		return false;

	char first[5] {0, };
	char fileName[128] {0, };
	BASE_GetFirstKey(accName, first);
	sprintf_s(fileName, "./account/%s/%s", first, accName);

	FILE* fs = 0;
	fopen_s(&fs, fileName, "wb");
	if (fs == 0)
	{
		Log("err, DBWriteAccount, failed to open file [%s]", accName);
		return false;
	}
	size_t numWrite = fwrite(account, 1, sizeof(STRUCT_ACCOUNTFILE), fs);
	if (numWrite != sizeof(STRUCT_ACCOUNTFILE))
	{
		Log("err, DBWriteAccount, could not write all bytes [%s %d]", accName, numWrite);
		return false;
	}
	fclose(fs);
	return true;
}
bool CFileDB::DBReadAccount(STRUCT_ACCOUNTFILE* account)
{
	account->AccountName[ACCOUNTNAME_LENGTH - 1] = '\0';
	account->AccountName[ACCOUNTNAME_LENGTH - 2] = '\0';
	account->AccountPass[ACCOUNTPASS_LENGTH - 1] = '\0';
	account->AccountPass[ACCOUNTPASS_LENGTH - 2] = '\0';

	char accName[ACCOUNTNAME_LENGTH] {0, };
	strncpy_s(accName, account->AccountName, ACCOUNTNAME_LENGTH);
	_strupr_s(accName);

	if (accName[0] == 'C' && accName[1] == 'O' && accName[2] == 'M' && accName[3] >= '0' && accName[3] <= '9' && accName[4] == 0)
		return false;
	if (accName[0] == 'L' && accName[1] == 'P' && accName[2] == 'T' && accName[3] >= '0' && accName[3] <= '9' && accName[4] == 0)
		return false;

	
	char first[5] {0, };
	char fileName[128] {0, };
	BASE_GetFirstKey(accName, first);
	sprintf_s(fileName, "./account/%s/%s", first, accName);

	FILE* fs = 0;
	fopen_s(&fs, fileName, "rb");
	if (fs == 0)
	{
		Log("err, DBReadAccount, failed to open file [%s]", accName);
		return false;
	}
	fseek(fs, 0, SEEK_END);
	size_t fsize = ftell(fs);
	fseek(fs, 0, SEEK_SET);

	assert(sizeof(STRUCT_ACCOUNTFILE) == 4292);

	if (fsize < sizeof(STRUCT_ACCOUNTFILE))
	{
		Log("err, DBReadAccount, file is too small [%s]", accName);
		fclose(fs);
		return false;
	}
	size_t numRead = fread(account, 1, sizeof(STRUCT_ACCOUNTFILE), fs);
	if (numRead < sizeof(STRUCT_ACCOUNTFILE))
	{
		Log("err, DBReadAccount, could not read all bytes [%s %d]", accName, numRead);
	}
	fclose(fs);
	return true;
}
bool CFileDB::DBExportAccount(STRUCT_ACCOUNTFILE* account)
{
	char check[20] = { 0, };
	strncpy_s(check, account->AccountName, ACCOUNTNAME_LENGTH);
	_strupr_s(check);

	if (check[0] == 'C' && check[1] == 'O' && check[2] == 'M' && check[3] >= '0' && check[3] <= '9' && check[4] == 0)
		return false;
	if (check[0] == 'L' && check[1] == 'P' && check[2] == 'T' && check[3] >= '0' && check[3] <= '9' && check[4] == 0)
		return false;

	char temp[256] = { 0, };
	sprintf_s(temp, "../export/account/srv%2.2d/%s", ServerGroup, account->AccountName);

	FILE* fs = 0;
	fopen_s(&fs, temp, "wb");
	if (fs == 0)
		return false;

	fwrite(account, sizeof(STRUCT_ACCOUNTFILE), 1, fs);
	fclose(fs);
	return true;
}
bool CFileDB::SendDBSignal(int32_t svr, uint16_t id, uint16_t signal)
{
	MSG_STANDARD msg;
	msg.ClientId = id;
	msg.PacketId = signal;
	msg.Size = sizeof(MSG_STANDARD);
	return pUser[svr].cSock.SendOneMessage((uint8_t*)&msg, msg.Size);
}
bool CFileDB::SendDBSignalParm(int32_t svr, uint16_t id, uint16_t signal, int32_t parm)
{
	MSG_STANDARDPARM msg;
	msg.Header.ClientId = id;
	msg.Header.PacketId = signal;
	msg.Header.Size = sizeof(MSG_STANDARDPARM);
	msg.Parm = parm;
	return pUser[svr].cSock.SendOneMessage((uint8_t*)&msg, msg.Header.Size);;
}
bool CFileDB::SendDBSignalParm2(int32_t svr, uint16_t id, uint16_t signal, int32_t parm1, int32_t parm2)
{
	MSG_STANDARDPARM2 msg;
	msg.Header.ClientId = id;
	msg.Header.PacketId = signal;
	msg.Header.Size = sizeof(MSG_STANDARDPARM2);
	msg.Param[0] = parm1;
	msg.Param[1] = parm2;
	return pUser[svr].cSock.SendOneMessage((uint8_t*)&msg, sizeof(MSG_STANDARDPARM2));
}
bool CFileDB::SendDBSignalParm3(int32_t svr, uint16_t id, uint16_t signal, int32_t parm1, int32_t parm2, int32_t parm3)
{
	MSG_STANDARDPARM3 msg;
	msg.Header.ClientId = id;
	msg.Header.PacketId = signal;
	msg.Header.Size = sizeof(MSG_STANDARDPARM3);
	msg.Param[0] = parm1;
	msg.Param[1] = parm2;
	msg.Param[2] = parm3;
	return pUser[svr].cSock.SendOneMessage((uint8_t*)&msg, sizeof(MSG_STANDARDPARM3));
}
void CFileDB::SendDBSavingQuit(int32_t Idx, int32_t mode)
{
	int32_t conn = Idx / MAX_USER;
	int32_t id = Idx % MAX_USER;

	MSG_DBSavingQuit msg; 
	msg.Header.PacketId = _MSG_DBSavingQuit;
	msg.Header.ClientId = id;
	msg.Header.Size = sizeof(MSG_DBSavingQuit);
	msg.Mode = mode;
	memcpy(&msg.AccountName[0], &pAccountList[Idx].File.AccountName[0], ACCOUNTNAME_LENGTH);
	pUser[conn].cSock.SendOneMessage((uint8_t*)&msg, sizeof(MSG_DBSavingQuit));
	return;
}
bool CFileDB::AddAccount(char* account, char* password, char* numericToken, bool bonus)
{
	char accName[ACCOUNTNAME_LENGTH] {0, };
	strncpy_s(accName, account, ACCOUNTNAME_LENGTH);
	_strupr_s(accName);

	if (accName[0] == 'C' && accName[1] == 'O' && accName[2] == 'M' && accName[3] >= '0' && accName[3] <= '9' && accName[4] == 0)
		return false;
	if (accName[0] == 'L' && accName[1] == 'P' && accName[2] == 'T' && accName[3] >= '0' && accName[3] <= '9' && accName[4] == 0)
		return false;

	char temp[256] = { 0, }, first[5] = { 0, };
	BASE_GetFirstKey(account, first);
	sprintf_s(temp, "./account/%s/%s", first, account);

	FILE* fs = 0;
	fopen_s(&fs, temp, "rb");
	if (fs != 0)
	{
		fclose(fs);
		return false;
	}
	STRUCT_ACCOUNTFILE file;
	strcpy_s(file.AccountName, account);
	strcpy_s(file.AccountPass, password);
	memset(&file.NotUsed, 0, 180);
	memset(&file.Cargo[0], 0, sizeof(STRUCT_ITEM) * MAX_CARGO);
	memset(&file.Character[0], 0, sizeof(STRUCT_MOB) * MOB_PER_ACCOUNT);
	memset(&file.Gold, 0, sizeof(int32_t));
	memset(&file.SkillBar, -1, 8 * MOB_PER_ACCOUNT);

	if (bonus)
	{
		for (int i = 0; i < 100; i++)
		{
			if (i < 50)
				file.Cargo[i].Index = 401;
			else
				file.Cargo[i].Index = 406;

			file.Cargo[i].EF1 = EF_AMOUNT;
			file.Cargo[i].EFV1 = 100;
		}
	}
	return DBWriteAccount(&file);
}
bool CFileDB::UpdateAccount(char* account, char* password, char* numericToken)
{
	char accName[ACCOUNTNAME_LENGTH] {0, };
	strncpy_s(accName, account, ACCOUNTNAME_LENGTH);
	_strupr_s(accName);

	if (accName[0] == 'C' && accName[1] == 'O' && accName[2] == 'M' && accName[3] >= '0' && accName[3] <= '9' && accName[4] == 0)
		return false;
	if (accName[0] == 'L' && accName[1] == 'P' && accName[2] == 'T' && accName[3] >= '0' && accName[3] <= '9' && accName[4] == 0)
		return false;

	char temp[256] = { 0, }, first[5] = { 0, };
	BASE_GetFirstKey(account, first);
	sprintf_s(temp, "./account/%s/%s", first, account);

	FILE* fs = nullptr;
	fopen_s(&fs, temp, "rb");
	if (fs == nullptr)
		return false;
	
	fclose(fs);

	STRUCT_ACCOUNTFILE file;

	int32_t idx = GetIndex(account);
	if (idx != 0)
		strcpy_s(pAccountList[idx].File.AccountPass, password);

	memcpy(file.AccountName, account, ACCOUNTNAME_LENGTH);
	if (!DBReadAccount(&file))
	{
		Log("err, UpdateAccount, readfail [%s]", account);
		return false;
	}
	strcpy_s(file.AccountPass, password);
	if (!DBWriteAccount(&file))
	{
		Log("err, UpdateAccount, writefail [%s]", account);
		return false;
	}

	idx = GetIndex(file.AccountName);
	if (idx > 0 && idx < MAX_DBACCOUNT)
		strcpy_s(pAccountList[idx].File.AccountPass, password);

	return true;
}
bool CFileDB::CreateCharacter(char* account, char* character)
{
	char check[20] = { 0, };
	strncpy_s(check, account, ACCOUNTNAME_LENGTH);
	_strupr_s(check);

	if (check[0] == 'C' && check[1] == 'O' && check[2] == 'M' && check[3] >= '0' && check[3] <= '9' && check[4] == 0)
		return false;
	if (check[0] == 'L' && check[1] == 'P' && check[2] == 'T' && check[3] >= '0' && check[3] <= '9' && check[4] == 0)
		return false;

	strncpy_s(check, character, MOBNAME_LENGTH);
	_strupr_s(check);
	if (check[0] == 'C' && check[1] == 'O' && check[2] == 'M' && check[3] >= '0' && check[3] <= '9' && check[4] == 0)
		return false;
	if (check[0] == 'L' && check[1] == 'P' && check[2] == 'T' && check[3] >= '0' && check[3] <= '9' && check[4] == 0)
		return false;


	char temp[128] = { 0, }, first[5] = { 0, };
	BASE_GetFirstKey(character, first);
	sprintf_s(temp, "./char/%s/%s", first, character);

	FILE* fs = 0;
	fopen_s(&fs, temp, "rb");
	if (fs != 0)
	{
		fclose(fs);
		Log("err, createchar already exist [%s]", character);
		return false;
	}
	fopen_s(&fs, temp, "wb");
	if (fs == nullptr)
		return false;

	fwrite(account, ACCOUNTNAME_LENGTH, 1, fs);
	fclose(fs);
	return true;
}
bool CFileDB::DeleteCharacter(char* account, char* character)
{
	char check[20] = { 0, };
	strncpy_s(check, character, MOBNAME_LENGTH);
	_strupr_s(check);

	if (check[0] == 'C' && check[1] == 'O' && check[2] == 'M' && check[3] >= '0' && check[3] <= '9' && check[4] == 0)
		return false;
	if (check[0] == 'L' && check[1] == 'P' && check[2] == 'T' && check[3] >= '0' && check[3] <= '9' && check[4] == 0)
		return false;

	char first[5] = { 0, };
	BASE_GetFirstKey(character, first);
	char tmp[256] = { 0, };
	sprintf_s(tmp, "./char/%s/%s", first, character);
	return DeleteFile(tmp) == FALSE ? false : true;
}
void CFileDB::AddAccountList(int32_t idx)
{
	if (pAccountList[idx].Login == TRUE)
	{
		Log("err, AddAccountList - already added [%s]", pAccountList[idx].File.AccountName);
		return;
	}
	int32_t conn = idx / MAX_USER;
	pUser[conn].Count++;
	pAccountList[idx].Login = TRUE;
	pAccountList[idx].Slot = -1;
	return;
}
void CFileDB::RemoveAccountList(int32_t idx)
{
	if (pAccountList[idx].Login)
	{
		int32_t conn = idx / MAX_USER;
		pUser[conn].Count--;

		pAccountList[idx].Login = FALSE;
		pAccountList[idx].Slot = -1;
		memset(&pAccountList[idx].File, 0, sizeof(STRUCT_ACCOUNTFILE));
	}
}
void CFileDB::GetAccountByChar(char* account, char* character)
{
	*account = '\0';

	char check[20] = { 0, };
	strncpy_s(check, character, MOBNAME_LENGTH);
	_strupr_s(check);

	if (check[0] == 'C' && check[1] == 'O' && check[2] == 'M' && check[3] >= '0' && check[3] <= '9' && check[4] == 0)
		return;
	if (check[0] == 'L' && check[1] == 'P' && check[2] == 'T' && check[3] >= '0' && check[3] <= '9' && check[4] == 0)
		return;

	char temp[256] = { 0, }, first[5] = { 0, };
	BASE_GetFirstKey(character, first);
	sprintf_s(temp, "./char/%s/%s", first, character);

	FILE* fs = 0;
	fopen_s(&fs, temp, "rb");
	if (fs == 0)
		return;

	fread(account, ACCOUNTNAME_LENGTH, 1, fs);
	fclose(fs);
}
void CFileDB::DBGetSelChar(STRUCT_SELCHAR* sel, STRUCT_ACCOUNTFILE* file)
{
	for (int i = 0; i < MOB_PER_ACCOUNT; i++)
	{
		memcpy(&sel->Name[i][0], &file->Character[i].Name[0], MOBNAME_LENGTH);
		memcpy(&sel->Equipment[i][0], &file->Character[i].Equip[0], sizeof(STRUCT_ITEM) * MAX_EQUIPMENT);
		memcpy(&sel->Score[i], &file->Character[i].BaseScore, sizeof(STRUCT_SCORE));
		sel->LastPosition[i] = file->Character[i].LastPosition;
		sel->Exp[i] = file->Character[i].Exp;
		sel->Gold[i] = file->Character[i].Gold;
		sel->GuildIndex[i] = file->Character[i].GuildIndex;

		if (file->Character[i].Equip[0].EFV2 > 1)
			sel->Equipment[i][0].Index = file->Character[i].Equip[0].EFV3;
	}
}
bool CFileDB::ProcessMessage(int32_t user, uint8_t* pMsg)
{
	MSG_STANDARD* header = (MSG_STANDARD*)pMsg;
	INT16 cId = header->ClientId;

	switch (header->PacketId)
	{
		case _MSG_DBSavenQuit:
		{
			//00412F05
			MSG_DBSavenQuit* packet = (MSG_DBSavenQuit*)pMsg;
			int32_t idx = GetIndex(user, cId);
			int32_t slot = pAccountList[idx].Slot;
			pAccountList[idx].File.AccountName[ACCOUNTNAME_LENGTH - 1] = '\0';
			pAccountList[idx].File.AccountName[ACCOUNTNAME_LENGTH - 2] = '\0';
			char acc[ACCOUNTNAME_LENGTH] = { 0, };
			strncpy_s(acc, pAccountList[idx].File.AccountName, ACCOUNTNAME_LENGTH);
			packet->AccountName[ACCOUNTNAME_LENGTH - 1] = '\0';
			packet->AccountName[ACCOUNTNAME_LENGTH - 2] = '\0';
			
			if (strcmp(packet->AccountName, acc))
			{
				Log("err, savenquit1 [%d %d %d %s %s]", cId, slot, packet->SlotId,
					acc, packet->AccountName);

				RemoveAccountList(idx);
				break;
			}
			if (slot < 0 || slot >= MOB_PER_ACCOUNT || slot != packet->SlotId)
			{
				Log("err, savenquit2 [%d %d %d %s %s]", cId, slot, packet->SlotId,
					acc, packet->AccountName);

				RemoveAccountList(idx);
				break;
			}
			if (!pAccountList[idx].Login)
			{
				Log("err, savenquit3 [%d %d %d %s %s]", cId, slot, packet->SlotId,
					packet->AccountName, pAccountList[idx].File.AccountName);
				break;
			}
			memcpy(&pAccountList[idx].File.Cargo[0], &packet->Cargo[0], sizeof(STRUCT_ITEM) * MAX_CARGO);
			memcpy(&pAccountList[idx].File.Character[slot], &packet->Mob, sizeof(STRUCT_MOB));
			memcpy(&pAccountList[idx].File.SkillBar[slot], &packet->SkillBar, 8);
			pAccountList[idx].File.Gold = packet->Gold;

			DBWriteAccount(&pAccountList[idx].File);
			DBExportAccount(&pAccountList[idx].File);
			RemoveAccountList(idx);
			SendDBSignal(user, cId, _MSG_DBCNFSavenQuit);

			for (int i = 0; i < MAX_ADMIN; i++)
			{
				if (pAdmin[i].Level <= 2 || pAdmin[i].DisableID == 0 || pAdmin[i].DisableID != idx)
					continue;

				DisableAccount(i, acc);
				break;
			}
			break;
		}
		case _MSG_DBSaveMob:
		{
			//00412C14 
			MSG_DBSaveMob* packet = (MSG_DBSaveMob*)pMsg;
			int32_t idx = GetIndex(user, cId);
			pAccountList[idx].File.AccountName[ACCOUNTNAME_LENGTH - 1] = '\0';
			pAccountList[idx].File.AccountName[ACCOUNTNAME_LENGTH - 2] = '\0';
			char acc[ACCOUNTNAME_LENGTH] = { 0, };
			strncpy_s(acc, pAccountList[idx].File.AccountName, ACCOUNTNAME_LENGTH);
			packet->AccountName[ACCOUNTNAME_LENGTH - 1] = '\0';
			packet->AccountName[ACCOUNTNAME_LENGTH - 2] = '\0';

			if (strcmp(packet->AccountName, pAccountList[idx].File.AccountName))
			{
				Log("err, savemob account does not match [%d %d %s %s]", cId,
					packet->SlotId, acc, packet->AccountName);
				break;
			}
			int32_t slot = pAccountList[idx].Slot;
			if (slot < 0 || slot >= MOB_PER_ACCOUNT)
			{
				Log("err, savemob1 [%d %d %d %s %s]", cId, slot, packet->SlotId,
					acc, packet->AccountName);
				break;
			}
			if (slot != packet->SlotId)
			{
				Log("err, savemob2 [%d %d %d %s %s]", cId, slot, packet->SlotId,
					acc, packet->AccountName);
				break;
			}
			if (!pAccountList[idx].Login)
			{
				Log("err, savemob3 [%d %d %d %s %s]", cId, slot, packet->SlotId,
					acc, packet->AccountName);
				break;
			}

			memcpy(&pAccountList[idx].File.Cargo[0], &packet->Cargo[0], sizeof(STRUCT_ITEM) * MAX_CARGO);
			memcpy(&pAccountList[idx].File.Character[slot], &packet->Mob, sizeof(STRUCT_MOB));
			memcpy(&pAccountList[idx].File.SkillBar[slot], &packet->SkillBar, 8);
			pAccountList[idx].File.Gold = packet->Gold;

			DBWriteAccount(&pAccountList[idx].File);
			if (packet->Export)
				DBExportAccount(&pAccountList[idx].File);

			break;
		}
		case _MSG_GuildZoneReport:
		{
			MSG_GuildZoneReport* packet = (MSG_GuildZoneReport*)pMsg;
			int32_t server = cId;
			if (server < 0 || server > MAX_SERVERNUMBER)
				break;

			for (int i = 0; i < MAX_GUILDZONE; i++)
				ChargedGuildList[i][server] = packet->Guild[i];
		
			MSG_DBChargedGuildList response;
			response.Header.PacketId = _MSG_DBChargedGuildList;
			response.Header.Size = sizeof(MSG_DBChargedGuildList);
			response.Header.ClientId = 0;
			memcpy(&response.ChargedGuildList, &ChargedGuildList, sizeof(int32_t) * MAX_GUILDZONE * MAX_SERVERNUMBER);

			if (pUser[server].Mode && pUser[server].cSock.Sock)
				pUser[server].cSock.SendOneMessage((uint8_t*)&response, sizeof(response));

			break;
		}
		case _MSG_UpdateSapphire:
		{
			MSG_UpdateSapphire* packet = (MSG_UpdateSapphire*)pMsg;
			if (packet->Sapphire == 1)
				Sapphire *= 2;
			else
				Sapphire /= 2;

			if (Sapphire < 1)
				Sapphire = 1;
			if (Sapphire > 30)
				Sapphire = 30;

			if (pUser[user].Mode && pUser[user].cSock.Sock)
				g_pFileDB.SendDBSignalParm3(user, 0, _MSG_DBSendSapphireCount, -1, Sapphire, user);

			WriteConfig();
			break;
		}
		case _MSG_MessageDBRecord:
		{
			MSG_MessageDBImple* packet = (MSG_MessageDBImple*)pMsg;
			packet->String[MESSAGE_LENGTH - 1] = 0;
			packet->String[MESSAGE_LENGTH - 2] = 0;
			bool ret = ProcessRecord(user, packet->String);
			Log("-system, %s", packet->String);
			if (!ret)
				Log("-system, DBRecord Fail");
			break;
		}
		case _MSG_DBDeleteCharacter:
		{
			MSG_DBDeleteCharacter* packet = (MSG_DBDeleteCharacter*)pMsg;
			int32_t idx = GetIndex(user, cId);

			if (packet->SlotId < 0 || packet->SlotId >= MOB_PER_ACCOUNT)
			{
				SendDBSignal(user, cId, _MSG_DBDeleteCharacterFail);
				Log("err, deletechar, slot invalid [%d]", packet->SlotId);
				break;
			}
			char* accountName = pAccountList[idx].File.AccountName;
			STRUCT_MOB* mob = &pAccountList[idx].File.Character[packet->SlotId];
			int32_t level = pAccountList[idx].File.Character[packet->SlotId].BaseScore.Level;
			if (level >= MaxDeleteLevel)
			{
				SendDBSignal(user, cId, _MSG_DBDeleteCharacterFail);
				Log("err, deletechar, level too high [%s - %d]", mob->Name, level);
				break;
			}
			if (strcmp(pAccountList[idx].File.AccountPass, packet->Password))
			{
				SendDBSignal(user, cId, _MSG_DBDeleteCharacterFail);
				Log("err, deletechar, wrong password [%s - %s]", accountName, mob->Name);
				break;
			}
			DeleteCharacter(accountName, mob->Name);
			Log("-system, deletechar [%s -> %s]", accountName, mob->Name);
			BASE_ClearMob(&pAccountList[idx].File.Character[packet->SlotId]);

			DBWriteAccount(&pAccountList[idx].File);

			MSG_CNFDeleteCharacter response;
			response.Header.Size = sizeof(MSG_CNFDeleteCharacter);
			response.Header.PacketId = _MSG_DBCNFDeleteCharacter;
			response.Header.ClientId = cId;
			
			DBGetSelChar(&response.SelChar, &pAccountList[idx].File);

			pUser[user].cSock.SendOneMessage((uint8_t*)&response, sizeof(response));
			break;
		}
		case _MSG_DBNoNeedSave:
		{
			MSG_STANDARD* packet = (MSG_STANDARD*)pMsg;
			int32_t idx = GetIndex(user, cId);
			char acc[ACCOUNTNAME_LENGTH] = { 0, };
			strncpy_s(acc, pAccountList[idx].File.AccountName, ACCOUNTNAME_LENGTH);
			RemoveAccountList(idx);

			for (int i = 0; i < MAX_ADMIN; i++)
			{
				if (pAdmin[i].Level <= 2 || pAdmin[i].DisableID == 0)
					continue;

				if (pAdmin[i].DisableID == idx)
				{
					DisableAccount(i, acc);
					break;
				}
			}
			break;
		}
		case _MSG_DBNewCharacter:
		{
			MSG_NewCharacter* packet = (MSG_NewCharacter*)pMsg;
			int32_t	slot = packet->Slot;
			int32_t classId = packet->ClassId;
			int32_t idx = GetIndex(user, cId);
			if (slot < 0 || slot >= MOB_PER_ACCOUNT)
			{
				Log("err, newchar slot out of range [%s]", pAccountList[idx].File.AccountName);
				SendDBSignal(user, cId, _MSG_DBNewCharacterFail);
				break;
			}
			char name[MOBNAME_LENGTH] = { 0, };
			strncpy_s(name, packet->CharacterName, MOBNAME_LENGTH);
			_strupr_s(name);

			if (classId < 0 || classId >= MAX_CLASS)
			{
				Log("err, newchar class out of range [%s -> %s -> %d]", pAccountList[idx].File.AccountName, name, classId);
				SendDBSignal(user, cId, _MSG_DBNewCharacterFail);
				break;
			}
			if (name[0] == 'C' && name[1] == 'O' && name[2] == 'M' && name[3] >= '0' && name[3] <= '9' && name[4] == 0)
			{
				Log("err, newchar com [%s -> %s]", pAccountList[idx].File.AccountName, name);
				SendDBSignal(user, cId, _MSG_DBNewCharacterFail);
				break;
			}
			if (name[0] == 'L' && name[1] == 'P' && name[2] == 'T' && name[3] >= '0' && name[3] <= '9' && name[4] == 0)
			{
				Log("err, newchar lpt [%s -> %s]", pAccountList[idx].File.AccountName, name);
				SendDBSignal(user, cId, _MSG_DBNewCharacterFail);
				break;
			}
			STRUCT_MOB* mob = &pAccountList[idx].File.Character[slot];
			mob->Name[MOBNAME_LENGTH - 1] = '\0';
			mob->Name[MOBNAME_LENGTH - 2] = '\0';
			packet->CharacterName[MOBNAME_LENGTH - 1] = '\0';
			packet->CharacterName[MOBNAME_LENGTH - 2] = '\0';

			if (*mob->Name)
			{
				Log("err, newchar already charged [%s -> %s]", pAccountList[idx].File.AccountName, mob->Name);
				SendDBSignal(user, cId, _MSG_DBNewCharacterFail);
				break;
			}
			int32_t len = strnlen_s(packet->CharacterName, MOBNAME_LENGTH);

			for (int i = 0; i < len; i++)
			{
				if (packet->CharacterName[i] == '_' || packet->CharacterName[i + 1] == '_')
				{
					SendDBSignal(user, cId, _MSG_DBNewCharacterFail);
					return true;
				}
			}
			if (!CreateCharacter(pAccountList[idx].File.AccountName, packet->CharacterName))
			{
				Log("err, newchar createcharacter failed [%s -> %s]", pAccountList[idx].File.AccountName, packet->CharacterName);
				SendDBSignal(user, cId, _MSG_DBNewCharacterFail);
				break;
			}

			memcpy(mob, &g_pBaseSet[classId], sizeof(STRUCT_MOB));
			memcpy(mob->Name, packet->CharacterName, MOBNAME_LENGTH);

			bool ret = DBWriteAccount(&pAccountList[idx].File);
			if (!ret)
			{
				Log("err, newchar fail - create file [%s -> %s]", pAccountList[idx].File.AccountName, packet->CharacterName);
				SendDBSignal(user, cId, _MSG_DBNewCharacterFail);
				break;
			}
			Log("sys, create character [%s -> %s]", pAccountList[idx].File.AccountName, packet->CharacterName);
			MSG_CNFNewCharacter response;
			response.Header.ClientId = cId;
			response.Header.Size = sizeof(MSG_CNFNewCharacter);
			response.Header.PacketId = _MSG_DBCNFNewCharacter;
			DBGetSelChar(&response.SelChar, &pAccountList[idx].File);
			pUser[user].cSock.SendOneMessage((uint8_t*)&response, sizeof(response));
			break;
		}
		case _MSG_DBCharacterLogin:
		{
			MSG_STANDARDPARM2* packet = (MSG_STANDARDPARM2*)pMsg;
			int32_t idx = GetIndex(user, cId);
			int32_t slot = packet->Param[0];
			if (slot < 0 || slot >= MOB_PER_ACCOUNT)
			{
				Log("err, charlogin, slot illegal [%s]", pAccountList[idx].File.AccountName);
				break;
			}
			if (*pAccountList[idx].File.Character[slot].Name == '\0')
			{
				Log("err, charlogin, mobname empty [%s]", pAccountList[idx].File.AccountName);
				break;
			}
			pAccountList[idx].Slot = slot;
			MSG_CNFCharacterLogin response;
			response.Header.PacketId = _MSG_DBCNFCharacterLogin;
			response.Header.Size = sizeof(MSG_CNFCharacterLogin);
			response.Header.ClientId = cId;
			response.ClientIndex = cId;
			response.SlotIndex = slot;
			memcpy(&response.LastPosition, &pAccountList[idx].File.Character[slot].LastPosition, sizeof(STRUCT_POSITION));
			memcpy(&response.Mob, &pAccountList[idx].File.Character[slot], sizeof(STRUCT_MOB));
			memcpy(&response.SkillBar, &pAccountList[idx].File.SkillBar[slot], 8);
			pUser[user].cSock.SendOneMessage((uint8_t*)&response, sizeof(response));
			break;
		}
		case _MSG_DBAccountLogin:
		{
			MSG_AccountLogin* packet = (MSG_AccountLogin*)pMsg;
			_strupr_s(packet->AccountName);
			if (packet->AccountName[0] == 'C' && packet->AccountName[1] == 'O' && packet->AccountName[2] == 'M'
				&& (packet->AccountName[3] >= '0' && packet->AccountName[3] <= '9') 
				&& !packet->Header.PacketId)
			{
				SendDBSignal(user, cId, _MSG_DBAccountLoginFail_Account);
				break;
			}
			if (packet->AccountName[0] == 'L' && packet->AccountName[1] == 'P' && packet->AccountName[2] == 'T'
				&& (packet->AccountName[3] >= '0' && packet->AccountName[3] <= '9')
				&& !packet->Header.PacketId)
			{
				SendDBSignal(user, packet->Header.ClientId, _MSG_DBAccountLoginFail_Account);
				break;
			}
			int32_t idx = GetIndex(user, cId);
			int32_t accIdx = GetIndex(packet->AccountName);

			STRUCT_ACCOUNTFILE account;
			memcpy(&account.AccountName[0], &packet->AccountName[0], ACCOUNTNAME_LENGTH);

			bool ret = DBReadAccount(&account);
			if (!ret)
			{
				if (!CreateAccountAtLogin)
				{
					SendDBSignal(user, cId, _MSG_DBAccountLoginFail_Account);
					break;
				}
				if (!AddAccount(packet->AccountName, packet->Password, 0, false))
				{
					SendDBSignal(user, cId, _MSG_DBAccountLoginFail_Account);
					break;
				}
				if (!DBReadAccount(&account))
				{
					SendDBSignal(user, cId, _MSG_DBAccountLoginFail_Account);
					break;
				}
			}
			if (strcmp(packet->Password, account.AccountPass))
			{
				if (account.AccountPass[0] == '_')
					SendDBSignal(user, cId, _MSG_DBAccountLoginFail_Account);
				else if (account.AccountPass[0] == '@')
					SendDBSignal(user, cId, _MSG_DBAccountLoginFail_Block);
				else if (account.AccountPass[0] == '#')
					SendDBSignal(user, cId, _MSG_DBAccountLoginFail_Disable);
				else
					SendDBSignal(user, cId, _MSG_DBAccountLoginFail_Pass);

				break;
			}
			if (idx == accIdx)
				break;
			
			if (accIdx)
			{
				Log("err, disconnect previous connection [%s]", packet->AccountName);

				if (packet->Unknown)
				{
					SendDBSignal(user, cId, _MSG_DBStillPlaying);
					SendDBSavingQuit(accIdx, 0);
				}
				else
					SendDBSignal(user, cId, _MSG_DBAlreadyPlaying);
				
				break;
			}
			_strupr_s(account.AccountName);
			int32_t srcSwap = -1, dstSwap = -1;

			for (int i = 0; i < MOB_PER_ACCOUNT; i++)
			{
				if (account.Character[i].Name[0] && account.Character[i].Equip[13].Index == 774)
					srcSwap = i;

				if (account.Character[i].Name[0] && account.Character[i].Equip[13].Index == 775)
					dstSwap = i;
			}

			if (srcSwap != -1 && dstSwap != -1)
			{
				char nameTemp[MOBNAME_LENGTH] = { 0, };
				memcpy(&nameTemp[0], &account.Character[dstSwap].Name[0], MOBNAME_LENGTH);
				memcpy(&account.Character[dstSwap].Name[0], &account.Character[srcSwap].Name[0], MOBNAME_LENGTH);
				memcpy(&account.Character[srcSwap].Name[0], &nameTemp[0], MOBNAME_LENGTH);
				BASE_ClearItem(&account.Character[srcSwap].Equip[13]);
				BASE_ClearItem(&account.Character[dstSwap].Equip[13]);

				Log("etc, name swap [%s][%s - %s]", account.AccountName, 
					account.Character[srcSwap].Name, account.Character[dstSwap].Name);

				DBWriteAccount(&account);
			}
			STRUCT_ACCOUNTFILE* pFile = &pAccountList[idx].File;
			memcpy(pFile, &account, sizeof(STRUCT_ACCOUNTFILE));

			AddAccountList(idx);

			MSG_CNFAccountLogin response;
			DBGetSelChar(&response.SelChar, &account);

			response.Header.ClientId = cId;
			response.Header.Size = sizeof(MSG_CNFAccountLogin);
			response.Header.PacketId = _MSG_DBCNFAccountLogin;
			strcpy_s(response.AccountName, account.AccountName);
			memcpy(&response.Cargo[0], &account.Cargo[0], sizeof(STRUCT_ITEM) * MAX_CARGO);
			response.Gold = account.Gold;
			pUser[user].cSock.SendOneMessage((uint8_t*)&response, sizeof(response));
			break;
		}
		case _MSG_MessageDBImple: // 0x411C8E
		{
			MSG_MessageDBImple* m = (MSG_MessageDBImple*)pMsg;
			m->Header.ClientId = 0;
			m->String[MESSAGE_LENGTH - 1] = '\0';
			m->String[MESSAGE_LENGTH - 2] = '\0';
			for (int i = 0; i < MAX_SERVERNUMBER; i++)
			{
				if (pUser[i].Mode == USER_EMPTY)
					continue;

				if (pUser[i].cSock.Sock == INVALID_SOCKET)
					continue;

				pUser[i].cSock.SendOneMessage((uint8_t*)m, m->Header.Size);
			}
			break;
		}
		case _MSG_Unknown: // 0x411C1C
		{
			for (int i = 0; i < MAX_ADMIN; i++)
			{
				if (pAdmin[i].cSock.Sock && pAdmin[i].Mode)
					pAdmin[i].cSock.SendOneMessage(pMsg, header->Size);
			}
			break;
		}
		case _MSG_ReqGuildAlly: // 0x411734
		{
			MSG_STANDARDPARM2* m = (MSG_STANDARDPARM2*)pMsg;

			time_t now;
			time(&now);
			tm when;
			localtime_s(&when, &now);

			uint32_t guildId = m->Param[0];
			Log("sys, guildally request %u %u", m->Param[0], m->Param[1]);
			if (guildId > 0 && guildId < 65536)
			{
				g_pGuildAlly[guildId] = when.tm_hour;

				for (int i = 0; i < MAX_SERVERNUMBER; i++)
				{
					if (pUser[i].cSock.Sock && pUser[i].Mode)
						g_pFileDB.SendDBSignalParm2(i, 0, _MSG_ReqGuildAlly, m->Param[0], m->Param[1]);
				}
			}
			break;
		}
		case _MSG_ReqGuildWar: // 0x411605
		{
			MSG_STANDARDPARM2* m = (MSG_STANDARDPARM2*)pMsg;

			time_t now;
			time(&now);
			tm when;
			localtime_s(&when, &now);

			uint32_t guildId = m->Param[0];
			Log("sys, guildwar request %u %u", m->Param[0], m->Param[1]);
			if (guildId > 0 && guildId < 65536)
			{
				g_pGuildWar[guildId] = when.tm_hour;

				for (int i = 0; i < MAX_SERVERNUMBER; i++)
				{
					if (pUser[i].cSock.Sock && pUser[i].Mode)
						g_pFileDB.SendDBSignalParm2(i, 0, _MSG_ReqGuildWar, m->Param[0], m->Param[1]);
				}
			}
			break;
		}
		case _MSG_ReqTransper: // 0x411105
		{
			MSG_ReqTransper* m = (MSG_ReqTransper*)pMsg;
			int32_t svr = user / MAX_USER;
			int32_t id = user % MAX_USER;
			m->OldName[MOBNAME_LENGTH - 1] = '\0';
			m->OldName[MOBNAME_LENGTH - 2] = '\0';
			m->NewName[MOBNAME_LENGTH - 1] = '\0';
			m->NewName[MOBNAME_LENGTH - 2] = '\0';
			int32_t slot = m->Slot;

			if (m->Header.ClientId < 0 && cId >= MAX_USER && slot < 0 && slot >= MOB_PER_ACCOUNT)
			{
				Log("err, transper [id: %d slot: %d old: %s new: %s]", cId, slot, m->OldName, m->NewName);
				MSG_ReqTransper sm;
				sm.Header.PacketId = _MSG_ReqTransper;
				sm.Header.Size = sizeof(MSG_ReqTransper);
				sm.Header.ClientId = id;
				sm.Result = MOB_PER_ACCOUNT;
				if (user)
					pUser[user].cSock.SendOneMessage((uint8_t*)&sm, sm.Header.Size);

				return true;
			}
			int32_t idx = GetIndex(user, cId);

			if (TransperCharacter && AdminClient.Sock)
			{
				STRUCT_MOB* mob = &pAccountList[idx].File.Character[slot];
				mob->Name[MOBNAME_LENGTH - 1] = '\0';
				mob->Name[MOBNAME_LENGTH - 2] = '\0';
				char tmpName[MOBNAME_LENGTH] = { 0, };
				strcpy_s(tmpName, mob->Name);
				strncpy_s(mob->Name, m->NewName, MOBNAME_LENGTH);
				mob->Name[MOBNAME_LENGTH - 1] = '\0';
				mob->Name[MOBNAME_LENGTH - 2] = '\0';
				_strupr_s(tmpName);
				_strupr_s(m->OldName);
				if (strcmp(tmpName, m->OldName))
				{
					Log("err, transper [id: %d slot: %d old: %s current: %s] account [%s]", cId, slot,
						m->OldName, tmpName, pAccountList[idx].File.AccountName);

					MSG_ReqTransper sm;
					sm.Header.PacketId = _MSG_ReqTransper;
					sm.Header.Size = sizeof(MSG_ReqTransper);
					sm.Header.ClientId = id;
					sm.Result = MOB_PER_ACCOUNT;
					if (user)
						pUser[user].cSock.SendOneMessage((uint8_t*)&sm, sm.Header.Size);

					return true;
				}
				else
				{
					DisableAccount(-1, pAccountList[idx].File.AccountName);
					MSG_NPCreateCharacter sm;
					sm.Header.PacketId = _MSG_NPCreateCharacter;
					sm.Header.Size = sizeof(MSG_NPCreateCharacter);
					sm.Slot = slot;
					strncpy_s(sm.AccountName, pAccountList[idx].File.AccountName, ACCOUNTNAME_LENGTH);
					memcpy(&sm.Mob, mob, sizeof(STRUCT_MOB));

					AdminClient.SendOneMessage((uint8_t*)&sm, sm.Header.Size);
					return true;
				}
			}
			else
			{
				Log("err, transper but no transper server [%s]", pAccountList[idx].File.AccountName);
				MSG_ReqTransper sm;
				sm.Header.PacketId = _MSG_ReqTransper;
				sm.Header.Size = sizeof(MSG_ReqTransper);
				sm.Header.ClientId = id;
				sm.Result = MOB_PER_ACCOUNT;
				if (user)
					pUser[user].cSock.SendOneMessage((uint8_t*)&sm, sm.Header.Size);

				return true;
			}
		}
		default:
			Log("sys, Unknown Packet [0x%X ID:%d Size:%d KeyWord:%d]", header->PacketId, cId,
				header->Size, header->Key);
			break;
	}
	return true;
}
bool ProcessRecord(int32_t conn, const char* str)
{
	char dest[256] = { 0, };
	time_t now;
	time(&now);
	tm when;
	localtime_s(&when, &now);
	strftime(dest, 256, "RC_%d_%m_%y__%H_%M_%S", &when);

	char fileName[256] = { 0, };
	sprintf_s(fileName, "../export/%d/%s__SV_%d.log", ServerGroup, dest, conn);

	FILE* fs = 0;
	fopen_s(&fs, fileName, "wt");
	if (fs == 0)
		return false;

	fprintf_s(fs, "%s", str);
	fclose(fs);
	return true;
}
