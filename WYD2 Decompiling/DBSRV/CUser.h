#ifndef CUSER_H
#define CUSER_H

#define USER_EMPTY 0
#define USER_ACCEPT 1

class CUser
{
public:
	uint32_t	IP;
	int32_t		Mode;
	CPSock		cSock;
	int32_t		Count;
	int32_t		Level;
	int32_t		Encode1;
	int32_t		Encode2;
	char		Name[ACCOUNTNAME_LENGTH];
	int32_t		DisableID;

public:
	CUser();
	virtual ~CUser();

	bool AcceptUser(SOCKET listenSocket, int32_t wsa);
};
#endif // !CUSER_H
