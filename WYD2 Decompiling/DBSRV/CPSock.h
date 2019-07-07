#ifndef CPSOCK_H
#define CPSOCK_H

#define WSA_ACCEPT          (WM_USER + 3) 
#define WSA_ACCEPTADMIN     (WM_USER + 4) 
#define WSA_READADMINCLIENT	(WM_USER + 5) 
#define WSA_READADMIN		(WM_USER + 6) 
#define WSA_READ            (WM_USER + 100) 

#define MAX_PENDING_CONNECTS  8

#define RECV_BUFFER_SIZE	(64 * 1024)
#define SEND_BUFFER_SIZE	(64 * 1024)

#define INITCODE			0x1F11F311

#define MAX_MESSAGE_SIZE	10000

class CPSock
{
public:
	SOCKET Sock;
	uint8_t* pSendBuffer;
	uint8_t* pRecvBuffer;
	int32_t
		nSendPosition,
		nRecvPosition,
		nProcPosition,
		nSentPosition;
	int32_t Init;

public:
	CPSock();
	virtual ~CPSock();
	
	bool		AddMessage(uint8_t* pMsg, int32_t pSize);
	void		CloseSocket();
	bool		ConnectBillServer(HWND hwnd, const char* host, int32_t port, int32_t wsa);
	bool		ConnectServer(HWND hwnd, const char* host, int32_t port, int32_t wsa);
	uint8_t*	ReadBillMessage(int32_t* errorCode, int32_t* errorType);
	uint8_t*	ReadMessage(int32_t* errorCode, int32_t* errorType);
	bool		Receive();
	void		RefreshRecvBuffer();
	void		RefreshSendBuffer();
	bool		SendBillMessage(uint8_t* pMsg);
	bool		SendMsg();
	bool		SendOneMessage(uint8_t* pMsg, int32_t pSize);
	bool		StartListen(HWND hwnd, const char* ip, int32_t port, int32_t wsa);
	bool		WSAInitialize();	

};

struct _AUTH_GAME
{
	uint8_t Unknown[196];
};
struct _AUTH_GAME2
{
	uint8_t Unknown[196];
};

#endif // !CPSOCK_H
