#ifndef CFILEDB_H
#define CFILEDB_H

struct STRUCT_ACCOUNTLIST
{
	int32_t Login;
	int32_t Slot;
	STRUCT_ACCOUNTFILE File;
};

class CFileDB
{
public:
	STRUCT_ACCOUNTLIST pAccountList[MAX_DBACCOUNT];

public:
	CFileDB();
	virtual ~CFileDB();


	bool	AddAccount(char* account, char* password, char* numericToken, bool bonus);
	bool	UpdateAccount(char* account, char* password, char* numericToken);
	bool	ProcessMessage(int32_t user, uint8_t* pMsg);
	bool	SendDBSignal(int32_t svr, uint16_t id, uint16_t signal);
	bool	SendDBSignalParm(int32_t svr, uint16_t id, uint16_t signal, int32_t parm);
	bool	SendDBSignalParm2(int32_t svr, uint16_t id, uint16_t signal, int32_t parm1, int32_t parm2);
	bool	SendDBSignalParm3(int32_t svr, uint16_t id, uint16_t signal, int32_t parm1, int32_t parm2, int32_t parm3);
	bool	SendDBMessage(int32_t svr, uint16_t id, uint8_t* pMsg);
	void	AddAccountList(int32_t idx);
	void	RemoveAccountList(int32_t idx);
	bool	CreateCharacter(char* account, char* character);
	bool	DeleteCharacter(char* account, char* character);
	int		GetIndex(char* account);
	int		GetIndex(int32_t server, int32_t id);
	void	SendDBSavingQuit(int32_t Idx, int32_t mode);
	bool	DBWriteAccount(STRUCT_ACCOUNTFILE* account);
	bool	DBExportAccount(STRUCT_ACCOUNTFILE* account);
	bool	DBReadAccount(STRUCT_ACCOUNTFILE* account);
	void	DBGetSelChar(STRUCT_SELCHAR* sel, STRUCT_ACCOUNTFILE* file);
	void	GetAccountByChar(char* account, char* character);
};

bool ProcessRecord(int32_t conn, const char* str);

#endif // !CFILEDB_H
