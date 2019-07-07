#include "stdafx.h"

CUser::CUser()
{
	Mode = USER_EMPTY;
	IP = 0;
	Count = 0;
	DisableID = 0;
	Level = -1;
}
CUser::~CUser()
{

}
bool CUser::AcceptUser(SOCKET listenSocket, int32_t wsa)
{
	sockaddr_in addr;
	int32_t size = sizeof(addr);

	SOCKET nSocket = accept(listenSocket, (sockaddr*)&addr, &size);

	if (nSocket == INVALID_SOCKET)
		return false;

	if (WSAAsyncSelect(nSocket, hWndMain, wsa, FD_READ | FD_CLOSE) == SOCKET_ERROR)
	{
		closesocket(nSocket);
		return false;
	}

	cSock.Sock = nSocket;
	cSock.nSendPosition = cSock.nSentPosition = 0;
	cSock.nRecvPosition = cSock.nProcPosition = 0;
	IP = addr.sin_addr.S_un.S_addr;
	Mode = USER_ACCEPT;
	return true;
}