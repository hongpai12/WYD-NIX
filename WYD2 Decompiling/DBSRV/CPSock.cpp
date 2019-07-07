#include "stdafx.h"

CPSock::CPSock()
{
	Sock = INVALID_SOCKET;
	Init = 0;
	pRecvBuffer = new uint8_t[RECV_BUFFER_SIZE];
	pSendBuffer = new uint8_t[SEND_BUFFER_SIZE];
	nSendPosition = nSentPosition = 0;
	nRecvPosition = nProcPosition = 0;
}
CPSock::~CPSock()
{
	if (pRecvBuffer)
		delete[] pRecvBuffer;

	if (pSendBuffer)
		delete[] pSendBuffer;
}
bool CPSock::WSAInitialize()
{
	WSADATA wsa;
	return WSAStartup(MAKEWORD(2, 2), &wsa) == 0 ? true : false;
}
void CPSock::CloseSocket()
{
	nSendPosition = nSentPosition = 0;
	nRecvPosition = nProcPosition = 0;
	Init = 0;

	if (Sock != INVALID_SOCKET)
		closesocket(Sock);

	Sock = INVALID_SOCKET;
}
bool CPSock::StartListen(HWND hwnd, const char* ip, int32_t port, int32_t wsa)
{
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (sock == INVALID_SOCKET)
	{
		Log("Erro: socket() falhou com erro: %d", WSAGetLastError());
		return false;
	}

	sockaddr_in localaddr { 0 };
	localaddr.sin_addr.s_addr = inet_addr(ip);
	localaddr.sin_family = AF_INET;
	localaddr.sin_port = htons(port);

	if (bind(sock, (sockaddr*)&localaddr, sizeof(localaddr)) == SOCKET_ERROR)
	{
		Log("Erro: bind() falhou com erro: %d", WSAGetLastError());
		closesocket(sock);
		return false;
	}
	if (listen(sock, MAX_PENDING_CONNECTS) == SOCKET_ERROR)
	{
		Log("Erro: listen() falhou com erro: %d", WSAGetLastError());
		closesocket(sock);
		return false;
	}
	if (WSAAsyncSelect(sock, hwnd, wsa, FD_ACCEPT) == SOCKET_ERROR)
	{
		Log("Erro: WSAAsyncSelect() falhou com erro: %d", WSAGetLastError());
		closesocket(sock);
		return false;
	}

	Sock = sock;
	return true;
}
bool CPSock::ConnectServer(HWND hwnd, const char* host, int32_t port, int32_t wsa)
{
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (sock == INVALID_SOCKET)
		return false;

	nSendPosition = nSentPosition = 0;
	nRecvPosition = nProcPosition = 0;

	if (Sock != INVALID_SOCKET)
		CloseSocket();


	sockaddr_in remoteaddr;
	remoteaddr.sin_addr.s_addr = inet_addr(host);
	remoteaddr.sin_port = htons(port);
	remoteaddr.sin_family = AF_INET;

	if (connect(sock, (sockaddr*)&remoteaddr, sizeof(remoteaddr)) == SOCKET_ERROR)
	{
		Log("Erro: connect() falhou com erro: %d", WSAGetLastError());
		closesocket(sock);
		return false;
	}
	if (WSAAsyncSelect(sock, hwnd, wsa, FD_READ | FD_CLOSE) == SOCKET_ERROR)
	{
		Log("Erro: WSAAsyncSelect() falhou com erro: %d", WSAGetLastError());
		closesocket(sock);
		return false;
	}
	Sock = sock;
	uint32_t helloPacket = INITCODE;
	send(Sock, (char*)&helloPacket, 4, 0);
	Init = TRUE;
	return true;
}
void CPSock::RefreshRecvBuffer()
{
	int32_t left = nRecvPosition - nProcPosition;
	if (left <= 0)
		return;

	memcpy(pRecvBuffer, pRecvBuffer + nProcPosition, left);
	nProcPosition = 0;
	nRecvPosition -= left;
}
void CPSock::RefreshSendBuffer()
{
	int32_t left = nSendPosition - nSentPosition;
	if (left <= 0)
		return;

	memcpy(pSendBuffer, pSendBuffer + nSentPosition, left);
	nSentPosition = 0;
	nSendPosition -= left;
}
bool CPSock::SendMsg()
{
	if (Sock == INVALID_SOCKET)
	{
		nSendPosition = nSentPosition = 0;
		return false;
	}
	if (nSentPosition > 0)
		RefreshSendBuffer();

	if (nSendPosition > SEND_BUFFER_SIZE || nSendPosition < 0 || Sock == INVALID_SOCKET)
	{
		nSendPosition = nSentPosition = 0;
		return false;
	}
	if (nSentPosition > nSendPosition || nSentPosition >= SEND_BUFFER_SIZE || nSentPosition < 0)
	{
		nSendPosition = nSentPosition = 0;
		return false;
	}

	int32_t err = 0;
	int32_t left = nSendPosition - nSentPosition;
	int32_t nSend = send(Sock, (char*)(pSendBuffer + nSentPosition), left, 0);
	if (nSend != SOCKET_ERROR)
	{
		nSentPosition += nSend;

		if (nSentPosition >= nSendPosition)
			nSendPosition = nSentPosition = 0;

		return true;
	}
	else
	{
		err = WSAGetLastError();

		return false;
	}
}
bool CPSock::SendOneMessage(uint8_t* pMsg, int32_t pSize)
{
	if (Sock == INVALID_SOCKET)
		return false;

	AddMessage(pMsg, pSize);

	return SendMsg();
}
uint8_t* CPSock::ReadMessage(int32_t *errorCode, int32_t *errorType)
{
	*errorCode = 0;

	if (nProcPosition >= nRecvPosition)
	{
		nProcPosition = nRecvPosition = 0;
		return 0;
	}
	if (!Init)
	{
		if ((nRecvPosition - nProcPosition) < 4)
			return 0;

		uint32_t helloPacket = *(uint32_t*)(pRecvBuffer + nProcPosition);
		if (helloPacket != INITCODE)
		{
			*errorCode = 2;
			*errorType = helloPacket;
			CloseSocket();
			return 0;
		}
		Init = TRUE;
		nProcPosition += sizeof(helloPacket);
	}

	if ((nRecvPosition - nProcPosition) < 12)
		return 0;

	uint16_t size = *(uint16_t*)(pRecvBuffer + nProcPosition);
	uint8_t rndkey = *(uint8_t*)(pRecvBuffer + nProcPosition + 2);
	uint8_t csum = *(uint8_t*)(pRecvBuffer + nProcPosition + 3);

	if (size > MAX_MESSAGE_SIZE || size < sizeof(MSG_STANDARD))
	{
		nRecvPosition = nProcPosition = 0;
		*errorCode = 2;
		*errorType = size;
		return 0;
	}
	uint16_t rest = nRecvPosition - nProcPosition;

	if (size > rest)
		return 0;

	uint8_t* pMsg = &pRecvBuffer[nProcPosition];

	nProcPosition += size;
	if (nRecvPosition <= nProcPosition)
		nProcPosition = nRecvPosition = 0;

	int32_t pos = g_pKeyword[rndkey * 2];
	int32_t sum1 = 0, sum2 = 0, key = 0;

	for (uint32_t i = 4; i < size; i++, pos++)
	{
		sum1 += pMsg[i];
		key = g_pKeyword[((pos & 0xFF) * 2) + 1];

		switch (i & 3)
		{
		case 0:
			key <<= 1;
			key &= 0xFF;
			pMsg[i] -= key;
			break;
		case 1:
			key >>= 3;
			key &= 0xFF;
			pMsg[i] += key;
			break;
		case 2:
			key <<= 2;
			key &= 0xFF;
			pMsg[i] -= key;
			break;
		case 3: default:
			key >>= 5;
			key &= 0xFF;
			pMsg[i] += key;
			break;
		}
		sum2 += pMsg[i];
	}

	sum1 &= 0xFF;
	sum2 &= 0xFF;
	uint8_t difference = (uint8_t)(sum1 - sum2);
	if (difference != csum)
	{
		*errorCode = 1;
		*errorType = size;
	}
	return pMsg;
}
bool CPSock::AddMessage(uint8_t *pMsg, int32_t size)
{
	if (size < 0 || size > MAX_MESSAGE_SIZE)
		return false;

	if ((nSendPosition + size) >= SEND_BUFFER_SIZE)
	{
		RefreshSendBuffer();
		if ((nSendPosition + size) >= SEND_BUFFER_SIZE)
			return false;
	}
	if (Sock == INVALID_SOCKET)
		return false;

	MSG_STANDARD* pHeader = (MSG_STANDARD*)pMsg;

	int32_t rndkey = (rand() % 255);
	int32_t sum1 = 0, sum2 = 0, key = 0;
	int32_t pos = g_pKeyword[rndkey * 2];

	pHeader->Key = rndkey;
	pHeader->Size = size;

	for (int32_t i = 4; i < size; i++, pos++)
	{
		key = g_pKeyword[((pos & 0xFF) * 2) + 1];

		switch (i & 3)
		{
		case 0:
			pSendBuffer[nSendPosition + i] = pMsg[i] + ((key * 2) & 0xFF);
			break;
		case 1:
			pSendBuffer[nSendPosition + i] = pMsg[i] - ((key >> 3) & 0xFF);
			break;
		case 2:
			pSendBuffer[nSendPosition + i] = pMsg[i] + ((key * 4) & 0xFF);
			break;
		case 3: default:
			pSendBuffer[nSendPosition + i] = pMsg[i] - ((key >> 5) & 0xFF);
			break;
		}

		sum1 += pMsg[i];
		sum2 += pSendBuffer[nSendPosition + i];
	}
	sum1 &= 0xFF;
	sum2 &= 0xFF;

	pHeader->CheckSum = ((sum2 - sum1) & 0xFF);

	memcpy(pSendBuffer + nSendPosition, pMsg, 4);

	nSendPosition += size;
	return true;
}
bool CPSock::Receive()
{
	int32_t rest = RECV_BUFFER_SIZE - nRecvPosition;
	int32_t nRecv = recv(Sock, (char*)(pRecvBuffer + nRecvPosition), rest, 0);
	if (nRecv == SOCKET_ERROR)
		return false;

	nRecvPosition += nRecv;
	if (nRecv == rest)
	{
		RefreshRecvBuffer();
		rest = RECV_BUFFER_SIZE - nRecvPosition;
		nRecv = recv(Sock, (char*)(pRecvBuffer + nRecvPosition), rest, 0);
		nRecvPosition += nRecv;
	}
	return true;
}
bool CPSock::ConnectBillServer(HWND hwnd, const char* host, int32_t port, int32_t wsa)
{
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (sock == INVALID_SOCKET)
		return false;

	nSendPosition = nSentPosition = 0;
	nRecvPosition = nProcPosition = 0;

	if (Sock != INVALID_SOCKET)
		CloseSocket();


	sockaddr_in remoteaddr;
	remoteaddr.sin_addr.s_addr = inet_addr(host);
	remoteaddr.sin_port = htons(port);
	remoteaddr.sin_family = AF_INET;

	if (connect(sock, (sockaddr*)&remoteaddr, sizeof(remoteaddr)) == SOCKET_ERROR)
	{
		Log("Erro: connect() falhou com erro: %d", WSAGetLastError());
		closesocket(sock);
		Sock = INVALID_SOCKET;
		return false;
	}
	if (WSAAsyncSelect(sock, hwnd, wsa, FD_READ | FD_CLOSE) == SOCKET_ERROR)
	{
		Log("Erro: WSAAsyncSelect() falhou com erro: %d", WSAGetLastError());
		closesocket(sock);
		Sock = INVALID_SOCKET;
		return false;
	}
	Sock = sock;
	return true;
}
uint8_t* CPSock::ReadBillMessage(int32_t* errorCode, int32_t* errorType)
{
	*errorCode = 0, *errorType = 0;

	if (nProcPosition >= nRecvPosition)
	{
		nProcPosition = nRecvPosition = 0;
		return 0;
	}

	if (nRecvPosition - nProcPosition < sizeof(_AUTH_GAME2))
		return 0;

	uint8_t* pMsg = &pRecvBuffer[nProcPosition];

	nProcPosition += sizeof(_AUTH_GAME2);

	if (nRecvPosition <= nProcPosition)
		nProcPosition = nRecvPosition = 0;

	return pMsg;
}
bool CPSock::SendBillMessage(uint8_t* pMsg)
{
	if (nSendPosition + sizeof(_AUTH_GAME) >= SEND_BUFFER_SIZE)
		return false;

	for (int i = 0; i < sizeof(_AUTH_GAME); i++)
		pSendBuffer[nSendPosition + i] = pMsg[i];
	
	nSendPosition += sizeof(_AUTH_GAME);
	return SendMsg();
}
