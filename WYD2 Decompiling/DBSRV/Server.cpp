#include "stdafx.h"
#include <fcntl.h>
#include "DrawInfo.h"

bool InitApplication(HINSTANCE hInstance)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = "";
	wc.lpszClassName = "DataServer";

	if (RegisterClassEx(&wc) == 0)
		return false;
	return true;
}
bool InitInstance(HINSTANCE hInstance, int32_t nCmdShow)
{
	hMainMenu = CreateMenu();
	hWndMain = CreateWindowA(
		"DataServer",
		"DBSRV",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN,
		CW_USEDEFAULT, CW_USEDEFAULT,
		640, 480,
		0,
		hMainMenu,
		hInstance,
		0);

	if (hWndMain == 0)
		return false;

	ShowWindow(hWndMain, nCmdShow);
	UpdateWindow(hWndMain);
	return true;
}

void ReadTrandGuildInfo()
{
	memset(&TransGuildConn, 0, sizeof(TransGuildConn));
	memset(&TransGuildIndex, 0, sizeof(TransGuildIndex));

	FILE* fs = nullptr;
	fopen_s(&fs, "Trans.txt", "rt");
	if (fs != nullptr)
	{
		if (fscanf_s(fs, "%d", &CurrentTransInfo))
		{
			int i = 0, first = 0, curConn = 0, curIndex = 0;
			for (i = 0; i < 20480; i++)
			{
				if (!fscanf_s(fs, "%d %d %d", &first, &curConn, &curIndex))
					break;

				if (first != i + 40960)
					break;

				TransGuildConn[i] = curConn;
				TransGuildIndex[i] = curIndex;
			}
			fclose(fs);
			CurrentTransInfo = i;
			return;
		}
	}
	return;
}
void WriteTrandGuildInfo()
{
	FILE* fs = nullptr;
	fopen_s(&fs, "Trans.txt", "wt");
	if (fs == nullptr)
		return;

	if (fprintf_s(fs, "%d\n", CurrentTransInfo) == 0)
		return;

	for (int i = 0; i < CurrentTransInfo; i++)
	{
		if (fprintf_s(fs, "%d %d %d\n", i + 40960, TransGuildConn[i], TransGuildIndex[i]) == 0)
			break;
	}
	fclose(fs);
}
void ConvertGuildNumber(int32_t srv, STRUCT_ITEM* item)
{
	int32_t i = 0;
	int32_t guildId = BASE_GetGuild(item);

	for (int i = 0; i < CurrentTransInfo; i++)
	{
		if (TransGuildConn[i] == srv && TransGuildIndex[i] == (guildId & 0xFFFF))
			break;
	}
	if (i == CurrentTransInfo)
	{
		TransGuildConn[i] = srv;
		TransGuildIndex[i] = (guildId & 0xFFFF);
		CurrentTransInfo++;
		WriteTrandGuildInfo();
	}
	uint16_t idx = i + 40960;
	item->EF1 = EF_HWORDGUILD;
	item->EFV1 = HIBYTE(idx);
	item->EF2 = EF_LWORDGUILD;
	item->EFV2 = LOBYTE(idx);
}
void StartLog(const char* cccc)
{
	AllocConsole();
	*stdout = *_fdopen(_open_osfhandle((intptr_t)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT), "w");
	*stdin = *_fdopen(_open_osfhandle((intptr_t)GetStdHandle(STD_INPUT_HANDLE), _O_TEXT), "r");
	*stderr = *_fdopen(_open_osfhandle((intptr_t)GetStdHandle(STD_ERROR_HANDLE), _O_TEXT), "w");
	SetConsoleTitle("Nix Project DBSRV");

	if (g_pLogFile != nullptr)
	{
		if (fclose(g_pLogFile))
			Log("Logfile close fail!", "-system", 0);
	}
	time_t now;
	time(&now);
	tm when;
	localtime_s(&when, &now);
	char tmp[256] = { 0, };
	strftime(tmp, 256, "./Log/%d_%m_%y__%H_%M_%S.log", &when);

	fopen_s(&g_pLogFile, tmp, "wt");
	assert(g_pLogFile != nullptr);
}
void Log(const char* str1, ...)
{
	char buffer[256] {};
	va_list va;
	va_start(va, str1);
	vsprintf_s(buffer, str1, va);
	va_end(va);
	
	time_t now;
	time(&now);
	tm when;
	localtime_s(&when, &now);

	char tmp[256] = { 0, };
	strftime(tmp, 256, "[%d-%m-%y][%H:%M:%S]:", &when);
	strcat_s(tmp, buffer);
	strcat_s(tmp, "\n");

	if (g_pLogFile)
		fprintf_s(g_pLogFile, tmp);

	printf_s(tmp);
}
bool ReadConfig()
{
	FILE* fs = 0;
	fopen_s(&fs, "Config.txt", "rt");

	if (fs == 0)
	{
		MessageBox(hWndMain, "Can't find Config.txt", "Booting Error", MB_OK);
		return false;
	}
	char line[256] = { 0, };
	char cmd[32] = { 0, };
	char val[108] = { 0, };
	int Version = 0;

	while (fgets(line, 256, fs))
	{
		if (*line == '\n' || *line == '#' || *line == '\0')
			continue;

		sscanf_s(line, "%31[^=]=%107s", cmd, _countof(cmd), val, _countof(val));

		if (!strcmp(cmd, "Sapphire"))
			Sapphire = atoi(val);
		else if (!strcmp(cmd, "MaxDeleteLevel"))
		{
			MaxDeleteLevel = atoi(val);

			if (MaxDeleteLevel > 400)
				MaxDeleteLevel = 400;
			else if (MaxDeleteLevel < 0)
				MaxDeleteLevel = 0;
		}
		else if (!strcmp(cmd, "CreateAccountAtLogin"))
		{
			CreateAccountAtLogin = atoi(val);

			if (CreateAccountAtLogin > 1)
				CreateAccountAtLogin = 1;
			if (CreateAccountAtLogin < 0)
				CreateAccountAtLogin = 0;
		}
	}

	fclose(fs);

	return true;
}
bool WriteConfig()
{
	if (Sapphire < 1)
		Sapphire = 0;
	if (Sapphire > 6400)
		Sapphire = 6400;

	FILE* fs = nullptr;
	fopen_s(&fs, "Config.txt", "wt");

	if (fs != nullptr)
	{
		fprintf_s(fs, "# WYD2 DBSRV Configuration File\n");
		fprintf_s(fs, "Sapphire=%d\n", Sapphire);
		fprintf_s(fs, "MaxDeleteLevel=%d\n", MaxDeleteLevel);
		fprintf_s(fs, "CreateAccountAtLogin=%d", CreateAccountAtLogin);
		fclose(fs);
		return true;
	}
	return false;
}
void ReadAdmin()
{
	FILE* fs = 0;
	fopen_s(&fs, "Admin.txt", "rt");
	if (fs == 0)
		return;
	int32_t a = 0, b = 0, c = 0, d = 0;
	int32_t idx = -1;

	char line[256] {0, };

	while (fgets(line, 256, fs))
	{
		for (int i = 0; i < 256; i++)
			if (line[i] == '.')
				line[i] = ' ';
		
		sscanf_s(line, "%d %d %d %d %d", &idx, &a, &b, &c, &d);
		uint32_t ip = (d << 24) + (c << 16) + (b << 8) + a;
		if (idx < 0 || idx >= MAX_ADMIN)
			continue;

		pAdminIP[idx] = ip;
	}
	fclose(fs);
}
BOOL WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
	)
{
	g_pInstance = hInstance;

	if (!InitApplication(hInstance)) return FALSE;

	if (!InitInstance(hInstance, nShowCmd)) return FALSE;

	BASE_InitializeModuleDir();

	if (!BASE_InitializeBasedef())
		return FALSE;
	
	StartLog("DB");
	Log("-system, start log"); 

	memset(&g_pGuildWar, 0, sizeof(g_pGuildWar));
	memset(&g_pGuildAlly, 0, sizeof(g_pGuildAlly));

	ReadConfig(); 

	//ReadGuild(); // !!!

	if (!ListenSocket.WSAInitialize())
	{
		Log("err, wsainitialize fail", "-system", 0);
		return FALSE;
	}
	BASE_InitializeServerlist();

	FILE* fs = 0;
	fopen_s(&fs, "localip.txt", "rt");

	if (fs == 0)
	{
		MessageBox(hWndMain, "Can't find localip.txt", "Booting Error", MB_OK);
		return FALSE;
	}
	char address[64] { 0, };

	fscanf_s(fs, "%s", address, _countof(address));
	fclose(fs);
	fs = 0;

	int a1 = 0, a2 = 0, a3 = 0, a4 = 0;
	sscanf_s(address, "%d.%d.%d.%d", &a1, &a2, &a3, &a4);

	LocalIP[0] = a1;
	LocalIP[1] = a2;
	LocalIP[2] = a3;
	LocalIP[3] = a4;

	ServerGroup = -1;
	int i = 0;
	for (i = 0; i < MAX_SERVERGROUP; i++)
	{
		if (!strcmp(g_pServerlist[i][0], address))
		{
			ServerGroup = i;
			break;
		}
	}

	if (ServerGroup == -1)
	{
		MessageBox(hWndMain, "Can't get Server Group Index LocalIP:", address, MB_OK);
		MessageBox(hWndMain, "Can't get Server Group Index TestServerIP:", g_pServerlist[i][0], MB_OK);
		return TRUE;
	}
	ImportItem(); // OK

	ReadTrandGuildInfo(); // OK

	ListenSocket.StartListen(hWndMain, address, DB_PORT, WSA_ACCEPT);
	AdminSocket.StartListen(hWndMain, address, ADMIN_PORT, WSA_ACCEPTADMIN);

	fopen_s(&fs, "redirect.txt", "rt");

	if (fs != nullptr)
	{
		char sIP[256] = { 0, };
		int port = 0;
		fscanf_s(fs, "%s %d %s %s", sIP, _countof(sIP), &port, adminclientid, _countof(adminclientid),
			adminclientpass, _countof(adminclientpass));
		fclose(fs);
		if (!AdminClient.ConnectServer(hWndMain, sIP, port, WSA_READADMINCLIENT))
		{
			MessageBox(hWndMain, "There's redirect.txt, but can't connect", "Booting Error", MB_OK);
			return FALSE;
		}
		Log("sys, redirection connected", "-system", 0);
		TransperCharacter = 1;
	}

	srand((uint32_t)time(0));
	SetTimer(hWndMain, TIMER_SEC, 1000, 0);
	SetTimer(hWndMain, TIMER_MIN, 1000 * 60, 0);

	MSG msg;

	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_TIMER:
	{
		if (wParam == TIMER_SEC)
		{
			ProcessSecTimer();
			InvalidateRect(hwnd, 0, true);
		}
		else if (wParam == TIMER_MIN)
			ProcessMinTimer();

		break;
	}
	case WSA_ACCEPT:
	{
		if (WSAGETSELECTERROR(lParam))
		{
			Log("err,WSAGETSELECTERROR - accepting new client", "-system", 0);
			break;
		}
		TempUser.AcceptUser(ListenSocket.Sock, WSA_READ);

		in_addr addr; addr.s_addr = TempUser.IP;
		const char* sIP = inet_ntoa(addr);

		int serverIndex = -1;
		for (int i = 0; i < MAX_SERVERGROUP; i++)
		{
			if (pUser[i].IP == TempUser.IP)
			{
				serverIndex = i;
				break;
			}
		}
		if (serverIndex == -1)
		{
			int j = 0;
			for (j = 1; j < MAX_SERVERNUMBER; j++)
			{
				if (!strcmp(g_pServerlist[ServerGroup][j], sIP))
				{
					serverIndex = j;
					break;
				}
			}
			if (j == MAX_SERVERNUMBER)
			{
				Log("err,wsa_accept request from", "-system", TempUser.IP);
				TempUser.cSock.CloseSocket();
				break;
			}
			serverIndex = j - 1;
		}
		if (serverIndex < 0 || serverIndex > MAX_SERVERGROUP)
		{
			Log("err,wsa_accept no match ip", "-system", TempUser.IP);
			TempUser.cSock.CloseSocket();
			break;
		}
		if (pUser[serverIndex].Mode)
		{
			g_pFileDB.SendDBSignalParm3(serverIndex, 0, _MSG_DBSendSapphireCount, -1, Sapphire, serverIndex);

			Log("err,wsa_accept no previous slot", "-system", TempUser.IP);
			TempUser.cSock.CloseSocket();
			break;
		}
		pUser[serverIndex].IP = TempUser.IP;
		pUser[serverIndex].Mode = TempUser.Mode;
		pUser[serverIndex].cSock.Sock = TempUser.cSock.Sock;
		pUser[serverIndex].cSock.nRecvPosition = TempUser.cSock.nRecvPosition;
		pUser[serverIndex].cSock.nProcPosition = TempUser.cSock.nProcPosition;
		pUser[serverIndex].cSock.nSendPosition = TempUser.cSock.nSendPosition;
		pUser[serverIndex].cSock.nSentPosition = TempUser.cSock.nSentPosition;

		g_pFileDB.SendDBSignalParm3(serverIndex, 0, 0x423, ServerGroup, Sapphire, serverIndex);

		if (TransperCharacter)
			g_pFileDB.SendDBSignalParm2(serverIndex, 0, 0x7A9, 0, 0);

		Log("sys, wsa_accept from [%d => %s]", serverIndex, sIP);
		break;
	}
	case WSA_ACCEPTADMIN:
	{
		ReadAdmin();
		if (WSAGETSELECTERROR(lParam))
		{
			Log("err,accept_admin accept fails", "-system", 0);
			break;
		}
		TempUser.AcceptUser(AdminSocket.Sock, WSA_READADMIN);
		char* cIP = (char*)&TempUser.IP;
		int user = -1;
		int i = 0;
		for (i = 0; i < MAX_ADMIN; i++)
		{
			if (TempUser.IP == pAdminIP[i])
			{
				if (pUser[i].Mode == USER_EMPTY)
				{
					user = i;
					break;
				}
			}
		}
		if (user == -1)
		{
			i = 0;
			for (i = 0; i < MAX_ADMIN; i++)
			{
				if (!pAdminIP[i])
				{
					if (pAdmin[i].IP == TempUser.IP)
					{
						user = i;
						break;
					}
				}
			}
			if (i == MAX_ADMIN)
			{
				TempUser.cSock.CloseSocket();
				Log("err, wsa_acceptadmin request accept from %d.%d.%d.%d", cIP[0], cIP[1], cIP[2], cIP[3]);
				break;
			}
		}

		if (user < 0 || user >= MAX_ADMIN)
		{
			Log("err, accept_admin unknown attempt", "-system", TempUser.IP);
			TempUser.cSock.CloseSocket();
			break;
		}

		if (pAdmin[user].Mode != USER_EMPTY)
		{
			Log("err, wsa_acceptadmin not empty previous slot %d", user);
			TempUser.cSock.CloseSocket();
			pAdmin[user].cSock.CloseSocket();
			pAdmin[user].Mode = USER_EMPTY;
			break;
		}
		if (user == -1)
		{
			Log("err, accept_admin no empty", "-system", TempUser.IP);
			TempUser.cSock.CloseSocket();
			break;
		}
		pAdmin[user].Level = -1;
		pAdmin[user].Name[0] = 0;
		pAdmin[user].DisableID = 0;
		pAdmin[user].IP = TempUser.IP;
		pAdmin[user].Mode = TempUser.Mode;
		pAdmin[user].cSock.Sock = TempUser.cSock.Sock;
		pAdmin[user].cSock.nRecvPosition = TempUser.cSock.nRecvPosition;
		pAdmin[user].cSock.nProcPosition = TempUser.cSock.nProcPosition;
		pAdmin[user].cSock.nSendPosition = TempUser.cSock.nSendPosition;
		pAdmin[user].cSock.nSentPosition = TempUser.cSock.nSentPosition;
		Log("sys, wsa_acceptadmin %d.%d.%d.%d <%d>", cIP[0], cIP[1], cIP[2], cIP[3], user);

		pAdmin[user].Encode1 = rand() % 10000;
		pAdmin[user].Encode2 = rand() % 10000;
		MSG_NPIDPASS msg;
		msg.Header.PacketId = _MSG_NPIDPASS;
		msg.Header.Size = sizeof MSG_NPIDPASS;
		msg.Encode1 = pAdmin[user].Encode1;
		msg.Encode2 = pAdmin[user].Encode2;
		pAdmin[user].cSock.SendOneMessage((uint8_t*)&msg, sizeof MSG_NPIDPASS);
		break;
	}
	case WSA_READ:
	{
		int user = GetUserFromSocket(wParam);
		if (user == -1)
		{
			closesocket(wParam);
			Log("err, wsa_read unregistered game server socket", "-system", 0);
			break;
		}
		if (WSAGETSELECTEVENT(lParam) != FD_READ)
		{
			Log("clo server fd [%d]", user + 1);
			pUser[user].cSock.CloseSocket();
			pUser[user].Mode = USER_EMPTY;
			break;
		}
		CurrentTime = timeGetTime();
		if (pUser[user].cSock.Receive() != TRUE)
		{
			Log("clo server receive [%d]", user + 1);
			pUser[user].cSock.CloseSocket();
			pUser[user].Mode = USER_EMPTY;
			break;
		}
		int error = 0, errorCode = 0;
		do
		{
			uint8_t* pMsg = pUser[user].cSock.ReadMessage(&error, &errorCode);
			if (pMsg == 0)
				break;

			MSG_STANDARD* header = (MSG_STANDARD*)pMsg;

			if (!BASE_CheckPacket(header))
				Log("PACKET DEBUG: PacketId: 0x%X Size: %d Server: %d", header->PacketId, header->Size, user);
		
			if (error == 1 || error == 2)
			{
				Log("err, wsa_read (%d -> %d)", error, errorCode);
				break;
			}
			ProcessClientMessage(user, pMsg);
		} while (true);
		break;
	}
	case WSA_READADMINCLIENT:
	{
		if (WSAGETSELECTEVENT(lParam) != FD_READ)
		{
			Log("adminclient close FD_READ", "-system", 0);
			AdminClient.CloseSocket();
			break;
		}
		if (AdminClient.Receive() != TRUE)
		{
			Log("adminclient close receive", "-system", 0);
			break;
		}
		int error = 0, errorCode = 0;
		while (true)
		{
			uint8_t* pMsg = AdminClient.ReadMessage(&error, &errorCode);

			if (pMsg == nullptr)
				break;

			if (error == 1 || error == 2)
			{
				Log("err wsa_readadminclient (%d -> %d)", error, errorCode);
				break;
			}
			ProcessAdminClientMessage(pMsg);
		}
		break;
	}
	case WSA_READADMIN:
	{
		int user = GetAdminFromSocket(wParam);
		if (user == -1)
		{
			closesocket(wParam);
			Log("err, wsa_read unregistered server socket", "-system", 0);
			break;
		}

		if (WSAGETSELECTEVENT(lParam) != FD_READ)
		{
			Log("admin close fd %d", user);
			pAdmin[user].cSock.CloseSocket();
			pAdmin[user].Mode = USER_EMPTY;
			pAdmin[user].Name[0] = 0;
			break;
		}
		CurrentTime = timeGetTime();

		if (pAdmin[user].cSock.Receive() != TRUE)
		{
			Log("admin %d close receive", user);
			break;
		}

		int error = 0, errorCode = 0;

		while (true)
		{
			uint8_t* pMsg = pAdmin[user].cSock.ReadMessage(&error, &errorCode);
			if (pMsg == 0)
				break;

			MSG_STANDARD* header = (MSG_STANDARD*)pMsg;
			if (BASE_CheckPacket(header))
				Log("err, packetID: 0x%X Size: %d user: %d", header->PacketId, header->Size, user);

			if (error == 1 || error == 2)
			{
				Log("err, wsa_readadmin (%d -> %d)", error, errorCode);
				break;
			}
			bool ret = ProcessAdminMessage(user, pMsg);
			if (!ret)
			{
				pAdmin[user].cSock.CloseSocket();
				pAdmin[user].Mode = USER_EMPTY;
				pAdmin[user].Name[0] = 0;
				break;
			}
		};
		break;
	}
	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(hwnd, &ps);
		DrawInfo(ps.hdc);
		EndPaint(hwnd, &ps);
		break;

	case WM_CLOSE:
	{
		int result = MessageBox(hWndMain, "Do you want to close server?", "Server shutdown warning", MB_OKCANCEL);
		if (result == IDOK)
		{
			if (g_pLogFile != nullptr)
				fclose(g_pLogFile);

			WSACleanup();
			PostQuitMessage(0);
			break;
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}
bool ProcessAdminClientMessage(uint8_t* msg)
{
	MSG_STANDARD* header = (MSG_STANDARD*)msg;
	if (!(header->PacketId & FLAG_NP2DB) || header->ClientId < 0 || header->ClientId >= MAX_DBACCOUNT)
	{
		Log("err,adminclient Packet [0x%X ID:%d Size:%d KeyWord:%d", header->PacketId, header->ClientId,
			header->Size, header->Key);
		return false;
	}
	Log("Debug: AdminClientMessage [0x%X ID:%d Size:%d", header->PacketId, header->ClientId, header->Size);
	
	if (header->PacketId == _MSG_NPDeleteCharacter) // 00419C0F
	{
		MSG_NPDeleteCharacter* m = (MSG_NPDeleteCharacter*)msg;
		char * account = m->AccountName;
		int32_t slot = m->SlotId;
		int32_t result = m->Result;
		int32_t IdxName = g_pFileDB.GetIndex(m->AccountName);
		int32_t	svr = IdxName / MAX_USER;
		int32_t	id = IdxName % MAX_USER;
		MSG_ReqTransper sm; 
		memset(&sm, 0, sizeof(sm));
		sm.Header.PacketId = _MSG_ReqTransper;
		sm.Header.Size = sizeof(sm);
		sm.Header.ClientId = id;
		sm.Slot = slot;
		sm.Result = m->Result;

		EnableAccount(-1, account);

		if (m->Result == 0)
		{
			STRUCT_ACCOUNTFILE file;
			memcpy(file.AccountName, m->AccountName, ACCOUNTNAME_LENGTH);
			if (!g_pFileDB.DBReadAccount(&file))
			{
				Log("etc, transper failed!! (read) [%s %s]", m->AccountName, m->CharacterName);
				if (IdxName)
				{
					sm.Result = 4;
					pUser[svr].cSock.SendOneMessage((uint8_t*)&sm, sm.Header.Size);
				}
				return true;
			}
			Log("etc, transper success!! (deleted) [%s %s]", m->AccountName, m->CharacterName);
			memset(&file.Character[slot], 0, sizeof(STRUCT_MOB));
			_strupr_s(file.AccountName);
			if (!g_pFileDB.DBWriteAccount(&file))
			{
				Log("etc, transper failed!! (write) [%s %s]", m->AccountName, m->CharacterName);
				if (IdxName)
				{
					sm.Result = 4;
					pUser[svr].cSock.SendOneMessage((uint8_t*)&sm, sm.Header.Size);
				}
				return true;
			}
			if (IdxName)
			{
				pUser[svr].cSock.SendOneMessage((uint8_t*)&sm, sm.Header.Size);
				memset(&g_pFileDB.pAccountList[IdxName].File.Character[slot], 0, sizeof(STRUCT_MOB));
			}
		}
		else // If result is 1, 2 or 3, just send the message with the result
		{
			if (IdxName)
				pUser[svr].cSock.SendOneMessage((uint8_t*)&sm, sm.Header.Size);
		}
	}
	return true;
}
bool ProcessAdminMessage(int32_t conn, uint8_t* msg)
{
	MSG_STANDARD* header = (MSG_STANDARD*)msg;
	if (!(header->PacketId & FLAG_NP2DB) || header->ClientId < 0 || header->ClientId >= MAX_USER)
	{
		Log("err,Packet [0x%X ID:%d Size:%d KeyWord:%d]", header->PacketId, header->ClientId,
			header->Size, header->Key);
		return false;
	}

	STRUCT_ACCOUNTFILE file;
	memset(&file, 0, sizeof(STRUCT_ACCOUNTFILE));

	switch (header->PacketId)
	{
	case _MSG_NPCreateCharacter: // 0041BB38
	{
		MSG_NPCreateCharacter* m = (MSG_NPCreateCharacter*)msg;

		MSG_NPDeleteCharacter sm; 
		sm.Header.PacketId= _MSG_NPDeleteCharacter;
		sm.Header.Size = sizeof(MSG_NPDeleteCharacter);
		sm.SlotId = m->Slot;
		sm.Header.ClientId = m->Header.ClientId;
		sm.Result = 0;

		strncpy_s(sm.AccountName, m->AccountName, ACCOUNTNAME_LENGTH);
		strncpy_s(sm.CharacterName, m->Mob.Name, MOBNAME_LENGTH);

		char * account = m->AccountName;
		int32_t slot = m->Slot;
		int32_t IdxName = g_pFileDB.GetIndex(m->AccountName);
		if (IdxName != 0)	
		{ 
			sm.Result = 3; 
			pAdmin[conn].cSock.SendOneMessage((uint8_t*)&sm, sm.Header.Size); 
			return true;;
		}

		memcpy(file.AccountName, m->AccountName, ACCOUNTNAME_LENGTH);

		if (!g_pFileDB.DBReadAccount(&file))
		{	
			Log("etc, transper failed!! (read) [%s %s]", m->AccountName, m->Mob.Name);
			sm.Result = 4; 
			pAdmin[conn].cSock.SendOneMessage((uint8_t*)&sm, sm.Header.Size);
			return true;
		}
		int empty = 0;
		for (empty = 0; empty < MOB_PER_ACCOUNT; empty++)
		{
			if (!file.Character[empty].Name[0]) 
				break;
		}
		if (empty == MOB_PER_ACCOUNT)	
		{ 
			sm.Result = 2; 
			pAdmin[conn].cSock.SendOneMessage((uint8_t*)&sm, sm.Header.Size); 
			return true;
		}
		
		if (!g_pFileDB.CreateCharacter(m->AccountName, m->Mob.Name))
		{
			sm.Result = 1; 
			pAdmin[conn].cSock.SendOneMessage((uint8_t*)&sm, sm.Header.Size);
			return true;
		}
		for (int i = 0; i < MAX_INVENTORY + MAX_EQUIPMENT; i++)
		{
			STRUCT_ITEM* item = &m->Mob.Equip[i];
			if (i >= MAX_EQUIPMENT)
				item = &m->Mob.Inventory[i - MAX_EQUIPMENT];

			if (item->Index == 446 || item->Index == 508 || 
				item->Index == 509 || item->Index == 522 ||
				(item->Index >= 526 && item->Index <= 538))
				ConvertGuildNumber(conn, item);
		}
		Log("etc tranper success!! (deleted) [%s %s]", m->AccountName, m->Mob.Name);
		memcpy(&file.Character[slot], &m->Mob, sizeof(STRUCT_MOB));
		_strupr_s(file.AccountName);

		if (!g_pFileDB.DBWriteAccount(&file))
		{
			Log("etc tranper failed!! (write) [%s %s]", m->AccountName, m->Mob.Name);
			sm.Result = 4; 
			pAdmin[conn].cSock.SendOneMessage((uint8_t*)&sm, sm.Header.Size);
			break;
		}
		sm.Result = 0; 
		pAdmin[conn].cSock.SendOneMessage((uint8_t*)&sm, sm.Header.Size);
		return true;
	}
	case _MSG_NPNotice: // 0041BFF1 
	{

		MSG_NPNotice *m = (MSG_NPNotice *)msg;
		m->AccountName[ACCOUNTNAME_LENGTH - 1] = '\0';
		m->AccountName[ACCOUNTNAME_LENGTH - 2] = '\0';
		m->String[MAX_STRING - 1] = 0;
		m->String[MAX_STRING - 2] = 0;
		// Diminui stack size
		MSG_NPNotice sm;
		if (m->AccountName[0] == 0 && m->Parm1 == 1)
		{	
			if (pAdmin[conn].Level <= 2) 
				return true;

			for (int i = 0; i < MAX_SERVERNUMBER; i++)
			{
				if (pUser[i].Mode == USER_EMPTY)
					continue;
				if (pUser[i].cSock.Sock == INVALID_SOCKET)
					continue;

				sm.Header.PacketId = _MSG_NPNotice;
				sm.Header.ClientId = 0;
				sm.Header.Size = sizeof(MSG_NPNotice);
				sm.AccountName[0] = '\0';
				sm.Parm1 = 1;
				sm.Parm2 = 1;
				strncpy_s(sm.String, m->String, MAX_STRING);
				pUser[i].cSock.SendOneMessage((uint8_t*)&sm, sm.Header.Size);
			}
			return true;
		}
		
		if (pAdmin[conn].Level <= 0) 
			return true;

		int32_t IdxName = g_pFileDB.GetIndex(m->AccountName);
		if (IdxName == 0)
		{
			SendAdminMessage(conn, 0, "User not found. Cannot send message.");
			return true;
		}
		int svr = IdxName / MAX_USER;
		int id = IdxName % MAX_USER;
		if (svr < 0 || svr >= MAX_SERVERNUMBER || id <= 0 || id >= MAX_USER)
		{
			SendAdminMessage(conn, 0, "Wrong SVR and ID");
			return true;
		}
		sm.Header.PacketId = _MSG_NPNotice;
		sm.Header.ClientId = id; 
		sm.Parm1 = 0;
		sm.Parm2 = 0;
		sm.Header.Size = sizeof(MSG_NPNotice);
		strncpy_s(sm.String, m->String, MAX_STRING);
		pUser[svr].cSock.SendOneMessage((uint8_t*)&sm, sizeof(sm));
		return true;
	}
	case _MSG_NPIDPASS: // 0041C27C
	{
		MSG_NPIDPASS* m = (MSG_NPIDPASS*)msg;
		_strupr_s(m->Account);

		if (pAdmin[conn].Encode1 != m->Encode1 || pAdmin[conn].Encode2 != m->Encode2) 
			return false;

		if (pAdmin[conn].Level != -1) 
			return false;

		memcpy(file.AccountName, m->Account, ACCOUNTNAME_LENGTH);

		if (!g_pFileDB.DBReadAccount(&file)) 
			return true;

		if (file.AccountPass[0] == '_') 
			return false;
		if (file.AccountPass[0] == '@') 
			return false;

		char *p = (char*)file.AccountPass;

		if (strncmp(m->Pass, p, ACCOUNTNAME_LENGTH)) 
			return false;

		int32_t maxlevel = 0;
		for (int i = 0; i < MOB_PER_ACCOUNT; i++)
		{
			if (file.Character[i].Name[0] != 0 && file.Character[i].BaseScore.Level > maxlevel) 
				maxlevel = file.Character[i].BaseScore.Level;
		}
		if (maxlevel < 1000) 
			return false;

		int32_t admin = maxlevel - 1000;
		pAdmin[conn].Level = admin;
		pAdmin[conn].DisableID = 0;
		strncpy_s(pAdmin[conn].Name, m->Account, ACCOUNTNAME_LENGTH);
		in_addr addr; addr.s_addr = pAdmin[conn].IP;
		char tmp[256] = { 0, };
		sprintf_s(tmp, "sys, Admin Login Success [%s Level: %d IP: %s]", pAdmin[conn].Name, admin, inet_ntoa(addr));
		Log(tmp);
		SendAdminMessage(conn, 0, tmp);
		return true;
	}
	case _MSG_NPReqAccount: // 0041C4E4 
	{
		MSG_NPReqAccount* m = (MSG_NPReqAccount*)msg;
		if (pAdmin[conn].Level <= 0)
			return true;

		_strupr_s(m->Account);
		_strupr_s(m->CharacterName);

		if (*m->CharacterName)
			g_pFileDB.GetAccountByChar(m->Account, m->CharacterName);

		if (!m->Account)
		{
			SendAdminSignal(conn, 0, _MSG_NPNotFound);
			SendAdminMessage(conn, 0, "There's no account with that character name");
			return true;
		}
		memcpy(file.AccountName, m->Account, ACCOUNTNAME_LENGTH);
		if (!g_pFileDB.DBReadAccount(&file))
		{
			SendAdminSignal(conn, 0, _MSG_NPNotFound);
			SendAdminMessage(conn, 0, "There's no account with that account name");
			return true;
		}
		MSG_NPAccountInfo sm;
		sm.Header.PacketId = _MSG_NPAccountInfo;
		sm.Header.Size = sizeof(MSG_NPAccountInfo);
		sm.Account = file;
		sm.Session = g_pFileDB.GetIndex(m->Account);
		sm.Account.AccountName[ACCOUNTNAME_LENGTH - 1] = '\0';
		sm.Account.AccountName[ACCOUNTNAME_LENGTH - 2] = '\0';
		for (int i = 0; i < MOB_PER_ACCOUNT; i++)
		{
			sm.Account.Character[i].Name[MOBNAME_LENGTH - 1] = '\0';
			sm.Account.Character[i].Name[MOBNAME_LENGTH - 2] = '\0';
		}
		char tmp[256] = { 0, };
		sprintf_s(tmp, "%s<%s|%s|%s|%s>", sm.Account.AccountName, sm.Account.Character[0].Name, sm.Account.Character[1].Name,
			sm.Account.Character[2].Name, sm.Account.Character[3].Name);

		tmp[MAX_STRING - 1] = '\0';
		tmp[MAX_STRING - 2] = '\0';
		SendAdminMessage(conn, 0, tmp);

		if (pAdmin[conn].Level <= 0)
			return true;

		int32_t maxlevel = 0;

		for (int i = 0; i < MOB_PER_ACCOUNT; i++)
		{
			if (sm.Account.Character[i].Name[0] != 0 && sm.Account.Character[i].BaseScore.Level > maxlevel)
				maxlevel = sm.Account.Character[i].BaseScore.Level;
		}
		int32_t admin = maxlevel - 1000;
		sm.State = 0;
		if (sm.Account.AccountPass[0] == '@')
			sm.State = 1;
		if (sm.Account.AccountPass[0] == '_')
			sm.State = 2;
		if (sm.Account.AccountPass[0] == '#')
			sm.State = 3;

		sm.Account.AccountPass[0] = (rand() % 24) + 65;
		sm.Account.AccountPass[1] = (rand() % 24) + 65;
		sm.Account.AccountPass[2] = (rand() % 24) + 65;
		sm.Account.AccountName[3] = (rand() % 24) + 65;
		sm.Account.AccountName[4] = '\0';

		pAdmin[conn].cSock.SendOneMessage((uint8_t*)&sm, sizeof(MSG_NPAccountInfo));
		break;
	}
	case _MSG_NPReqSaveAccount: // 0041C883
	{
		if (pAdmin[conn].Level <= 2)
		{
			SendAdminMessage(conn, 0, "Not allowed");
			return true;
		}
		MSG_NPAccountInfo *m = (MSG_NPAccountInfo*)msg;
		int32_t IdxName = g_pFileDB.GetIndex(m->Account.AccountName);
		if (IdxName)
		{
			SendAdminMessage(conn, 0, "For saving account should be disabled.");
			return true;
		}
		int32_t maxlevel = 0;
		for (int i = 0; i < MOB_PER_ACCOUNT; i++)
		{
			if (m->Account.Character[i].Name[0] != 0 && m->Account.Character[i].BaseScore.Level > maxlevel)
				maxlevel = m->Account.Character[i].BaseScore.Level;
		}
		int32_t admin = maxlevel - 1000;
		if (maxlevel >= 1000)
		{
			if (admin > pAdmin[conn].Level)
			{
				SendAdminMessage(conn, 0, "Set admin level error");
				return true;
			}
			if (admin == pAdmin[conn].Level && strcmp(pAdmin[conn].Name, m->Account.AccountName))
			{
				SendAdminMessage(conn, 0, "Set admin level error");
				return true;
			}
		}
		memcpy(file.AccountName, m->Account.AccountName, ACCOUNTNAME_LENGTH);
		if (!g_pFileDB.DBReadAccount(&file))
		{
			SendAdminMessage(conn, 0, "There's no account with that account name.");
			return true;
		}
		memcpy(m->Account.AccountPass, file.AccountPass, ACCOUNTPASS_LENGTH);
		g_pFileDB.DBWriteAccount(&m->Account);
		char tmp[256] = { 0, };
		sprintf_s(tmp, "Saving account [%s] success", m->Account.AccountName);
		SendAdminMessage(conn, 0, tmp);
		Log("sys, [%s] written", m->Account.AccountName);
		break;
	}
	case _MSG_NPDisable: // 0041CB47
	{
		if (pAdmin[conn].Level <= 2)
		{
			SendAdminMessage(conn, 0, "Not allowed");
			return true;
		}
		MSG_NPEnable* m = (MSG_NPEnable*)msg;
		m->AccountName[ACCOUNTNAME_LENGTH - 1] = '\0';
		m->AccountName[ACCOUNTNAME_LENGTH - 2] = '\0';
		_strupr_s(m->AccountName);

		int32_t idxName = g_pFileDB.GetIndex(m->AccountName);
		if (!idxName)
		{
			DisableAccount(conn, m->AccountName);
			return true;
		}
		g_pFileDB.SendDBSavingQuit(conn, 1);
		pAdmin[conn].DisableID = idxName;
		break;
	}
	case _MSG_NPEnable: // 0041CC11
	{
		if (pAdmin[conn].Level <= 2)
		{
			SendAdminMessage(conn, 0, "Not allowed");
			return true;
		}
		MSG_NPEnable* m = (MSG_NPEnable*)msg;
		m->AccountName[ACCOUNTNAME_LENGTH - 1] = '\0';
		m->AccountName[ACCOUNTNAME_LENGTH - 2] = '\0';
		_strupr_s(m->AccountName);

		int32_t idxName = g_pFileDB.GetIndex(m->AccountName);
		if (idxName)
		{	
			SendAdminMessage(conn, 0, "Check session. Already playing.");
			return true;
		}
		EnableAccount(conn, m->AccountName);
		break;
	}
	default:
		Log("err, Unknown AdminMessage [0x%X %d %d]", header->PacketId, header->Size, header->ClientId);
	}
	return true;
}
void ProcessClientMessage(int32_t conn, uint8_t* msg)
{
	MSG_STANDARD* header = (MSG_STANDARD*)msg;
	if ( !(header->PacketId & FLAG_GAME2DB) || header->ClientId < 0 || header->ClientId >= MAX_USER)
	{
		Log("err,Packet [0x%X ID:%d Size:%d KeyWord:%d]", header->PacketId, header->ClientId,
			header->Size, header->Key);
		return;
	}
	g_pFileDB.ProcessMessage(conn, msg);
}
void ImportItem()
{
	char tmp[256] { 0, };
	sprintf_s(tmp, "./Import/Item/%2.2d/*.*", ServerGroup);

	char fileName[256] { 0, };
	char errName[256] { 0, };
	char accountName[16] { 0, };
	STRUCT_ACCOUNTFILE accountFile;

	_finddata_t findData;
	intptr_t handle;
	FILE* fs = nullptr;

	handle = _findfirst(tmp, &findData);

	if (handle == -1)
		return;
	
	int count = 0;

	do
	{	
		if (findData.name[0] == '.')
		{
			if (_findnext(handle, &findData) == -1)
				break;
		}
		else
		{
			sprintf_s(fileName, "./Import/Item/%2.2d/%s", ServerGroup, findData.name);
			sprintf_s(errName, "./Import/Item/Error/%2.2d/%s", ServerGroup, findData.name);
			fopen_s(&fs, fileName, "rt");

			if (fs != nullptr)
			{
				int index = 0, ef1 = 0, ef2 = 0, ef3 = 0, efv1 = 0, efv2 = 0, efv3 = 0;
				if (!fgets(tmp, 256, fs))
				{
					fclose(fs);
					MoveFile(fileName, errName);
					Log("-IMPORT ITEM no contents [%s]", fileName);

					if (_findnext(handle, &findData) == -1)
						break;

					continue;
				}
				else
				{
					sscanf_s(tmp, "%s %d %d %d %d %d %d %d", accountName, _countof(accountName), 
						&index, &ef1, &efv1, &ef2, &efv2, &ef3, &efv3);
					fclose(fs);
					Log("-IMPORT ITEM starting [%s]", fileName);
					_strupr_s(accountName);

					if (index < 0 || index >= MAX_ITEMLIST || ef1 < 0 || ef1 > 255 || ef2 < 0 || ef2 > 255 || ef3 < 0 || ef3 > 255 ||
						efv1 < 0 || efv1 > 255 || efv2 < 0 || efv2 > 255 || efv3 < 0 || efv3 > 255)
					{
						MoveFile(fileName, errName);
						Log("-IMPORT ITEM wrong contents [%s]", fileName);

						if (_findnext(handle, &findData) == -1)
							break;
					}
					else
					{
						bool success = false;
						STRUCT_ITEM item = { index, ef1, efv1, ef2, efv2, ef3, efv3 };
						int idx = g_pFileDB.GetIndex(accountName);
						if (idx > 0 && idx < MAX_DBACCOUNT &&
							g_pFileDB.pAccountList[idx].Slot >= 0 && g_pFileDB.pAccountList[idx].Slot <= 3)
						{
							int srv = idx / MAX_USER;
							int srvNum = idx % MAX_USER;

							MSG_DBImportItem msg;
							msg.Header.Size = 40;
							msg.Header.PacketId = _MSG_DBImportItem;
							msg.ServerId = srvNum;
							strncpy_s(msg.Account, accountName, 16);
							msg.Item = item;
							msg.Header.ClientId = srvNum;
							pUser[srv].cSock.SendOneMessage((uint8_t*)&msg, msg.Header.Size);
							Log("-IMPORT ITEM runtime sent [%s]", fileName);
							success = true;
						}
						memset(&accountFile, 0, sizeof(STRUCT_ACCOUNTFILE));
						memcpy(&accountFile.AccountName[0], &accountName[0], ACCOUNTNAME_LENGTH);

						if (g_pFileDB.DBReadAccount(&accountFile))
						{
							int itemSlot = -1;

							for (int i = 0; i < 126; i++)
							{
								if (BASE_CanCargo(&item, accountFile.Cargo, i % 9, i / 9))
								{
									itemSlot = i;
									break;
								}
							}
							if (itemSlot == -1)
							{
								for (int i = 127; i >= 0; i--)
								{
									if (!accountFile.Cargo[i].Index)
									{
										itemSlot = i;
										break;
									}
								}
							}
							if (itemSlot == -1)
							{
								if (MoveFile(fileName, errName))
									Log("-IMPORT ITEM no space, file moved to Error [%s]", fileName);
								else
									Log("-IMPORT ITEM no space, move failed [%s]", fileName);

								if (_findnext(handle, &findData) == -1)
									break;

							}
							else
							{
								accountFile.Cargo[itemSlot] = item;

								if (g_pFileDB.DBWriteAccount(&accountFile))
								{
									char tmp[128] = { 0, };
									Log("-import item cargo %s(%d)", accountName, itemSlot);
									if (!DeleteFile(fileName))
									{
										MoveFile(fileName, errName);
										Log("-IMPORT ITEM success - file moved (Warning) [%s]", fileName);
									}
									if (_findnext(handle, &findData) == -1)
										break;
								}
								else
								{
									Log("-IMPORT ITEM success, account save fail [%s]", fileName);
									if (!success)
										MoveFile(fileName, errName);

									if (_findnext(handle, &findData) == -1)
										break;
								}
							}
						}
						else
						{
							if (!success)
								MoveFile(fileName, errName);

							Log("-IMPORT ITEM no account [%s]", fileName);

							if (_findnext(handle, &findData) == -1)
								break;
						}
					}
				}
			}
			else
			{
				if (_findnext(handle, &findData) == -1)
					break;
			}
		}
		count++;

	} while (count < 10);

	if (handle != -1)
		_findclose(handle);
	
}
void ImportUser()
{
	char tmp[256] { 0, };
	sprintf_s(tmp, "./Import/User/%2.2d/*.*", ServerGroup);

	char fileName[256] { 0, };
	char errName[256] { 0, };
	char accountName[ACCOUNTNAME_LENGTH] = { 0, }, accountPass[ACCOUNTPASS_LENGTH] = { 0, };
	char numericToken[7] = { 0, };
	int32_t bonus = 0;

	_finddata_t findData;
	intptr_t handle;
	FILE* fs = nullptr;

	handle = _findfirst(tmp, &findData);

	if (handle == -1)
		return;

	int32_t count = 0;
	do
	{
		// Se o arquivo começar com um ponto, ignora
		if (findData.name[0] == '.')
		{
			if (_findnext(handle, &findData) == -1)
				break;

			continue;
		}
		sprintf_s(fileName, "./Import/User/%2.2d/%s", ServerGroup, findData.name);
		sprintf_s(errName, "./Import/User/Error/%2.2d/%s", ServerGroup, findData.name);

		fopen_s(&fs, fileName, "rt");
		// Se o arquivo for válido
		if (fs != 0)
		{
			// Tenta ler 256 caracteres
			auto ret = fgets(tmp, 256, fs);
			// Fecha o arquivo
			fclose(fs);

			// Se a leitura for sucedida
			if (ret)
			{
				// Escaneia por informações
				if (sscanf_s(tmp, "%15s %11s %6s %d", accountName, _countof(accountName), accountPass, _countof(accountPass),
					numericToken, _countof(numericToken), &bonus) == 4)
				{
					// Se as informações forem válidas
					accountName[ACCOUNTNAME_LENGTH - 1] = '\0';
					accountName[ACCOUNTNAME_LENGTH - 2] = '\0';
					accountPass[ACCOUNTPASS_LENGTH - 1] = '\0';
					accountPass[ACCOUNTPASS_LENGTH - 2] = '\0';

					// Tenta adicionar a conta
					if (g_pFileDB.AddAccount(accountName, accountPass, numericToken, bonus == FALSE ? false : true))
					{
						// Se conseguir, deleta o arquivo
						if (DeleteFile(fileName))
							Log("-IMPORT USER success - [%s]", accountName);
						else
						{
							// Se falhar ao deletar o arquivo, move para a pasta de erros.
							MoveFile(fileName, errName);
							Log("-IMPORT USER success - file moved(Warning) [%s]", fileName);
						}
					}
					else
					{
						// Se falhar ao adicionar a conta, move para a pasta de erros
						MoveFile(fileName, errName);
						Log("-IMPORT USER failed - AddAccount - file moved(Warning) [%s]", fileName);
					}
				}
			}
		}
		// Se o arquivo for inválido, não conseguiu ler, não tinha informações válidas
		// se falhou em adicionar a conta, ou se falhou em deletar o arquivo
		// ou ainda se for sucedido, ele procura outro arquivo.
		if (_findnext(handle, &findData) == -1)
			break;
		
		count++;

	} while (count < 10);

	if (handle != -1)
		_findclose(handle);
}
void UpdateUser()
{
	char tmp[256] { 0, };
	sprintf_s(tmp, "./Update/User/%2.2d/*.*", ServerGroup);

	char fileName[256] { 0, };
	char errName[256] { 0, };
	char accountName[ACCOUNTNAME_LENGTH] = { 0, }, accountPass[ACCOUNTPASS_LENGTH] = { 0, };
	char numericToken[7] = { 0, };

	_finddata_t findData;
	intptr_t handle;
	FILE* fs = nullptr;

	handle = _findfirst(tmp, &findData);

	if (handle == -1)
		return;

	int32_t count = 0;
	do
	{
		// Se o arquivo começar com um ponto, ignora
		if (findData.name[0] == '.')
		{
			if (_findnext(handle, &findData) == -1)
				break;

			continue;
		}
		sprintf_s(fileName, "./Update/User/%2.2d/%s", ServerGroup, findData.name);
		sprintf_s(errName, "./Update/User/Error/%2.2d/%s", ServerGroup, findData.name);

		fopen_s(&fs, fileName, "rt");
		// Se o arquivo for válido
		if (fs != 0)
		{
			// Tenta ler 256 caracteres
			auto ret = fgets(tmp, 256, fs);
			// Fecha o arquivo
			fclose(fs);

			// Se a leitura for sucedida
			if (ret)
			{
				if (sscanf_s(tmp, "%15s %11s %6s", accountName, _countof(accountName), accountPass, _countof(accountPass),
					numericToken, _countof(numericToken)) == 4)
				{
					// Se as informações forem válidas
					accountName[ACCOUNTNAME_LENGTH - 1] = '\0';
					accountName[ACCOUNTNAME_LENGTH - 2] = '\0';
					accountPass[ACCOUNTPASS_LENGTH - 1] = '\0';
					accountPass[ACCOUNTPASS_LENGTH - 2] = '\0';

					// Tenta adicionar a conta
					if (g_pFileDB.UpdateAccount(accountName, accountPass, numericToken))
					{
						// Se conseguir, deleta o arquivo
						if (DeleteFile(fileName))
							Log("-UPDATE USER success - [%s]", accountName);
						else
						{
							// Se falhar ao deletar o arquivo, move para a pasta de erros.
							MoveFile(fileName, errName);
							Log("-UPDATE USER success - file moved(Warning) [%s]", fileName);
						}
					}
					else
					{
						// Se falhar ao adicionar a conta, move para a pasta de erros
						MoveFile(fileName, errName);
						Log("-UPDATE USER failed - UpdateAccount - file moved(Warning) [%s]", fileName);
					}
				}
			}
		}
		// Se o arquivo for inválido, não conseguiu ler, não tinha informações válidas
		// se falhou em adicionar a conta, ou se falhou em deletar o arquivo
		// ou ainda se for sucedido, ele procura outro arquivo.
		if (_findnext(handle, &findData) == -1)
			break;

		count++;

	} while (count < 10);

	if (handle == -1)
		_findclose(handle);
}
void ProcessSecTimer()
{
	SecCounter++;

	if (!(SecCounter % 15))
		ImportItem();

	if (!(SecCounter % 30))
		ImportUser();

	if (!(SecCounter % 45))
		UpdateUser();
}
void ProcessMinTimer()
{
	MinCounter++;

	UpdateConnection();

	if (!(MinCounter % 60))
		HourCounter++;

	if (!(MinCounter % 15))
		UpdateConnectionData();
}
int32_t GetUserFromSocket(uint32_t sock)
{
	for (int i = 0; i < MAX_SERVERNUMBER; i++)
	{
		if (pUser[i].Mode != USER_EMPTY)
		{
			if (pUser[i].cSock.Sock == sock)
				return i;
		}
	}
	return -1;
}
int32_t GetAdminFromSocket(uint32_t sock)
{
	for (int i = 0; i < MAX_ADMIN; i++)
	{
		if (pAdmin[i].Mode != USER_EMPTY)
		{
			if (pAdmin[i].cSock.Sock == sock)
				return i;
		}
	}
	return -1;
}
int32_t GetEmptyUser()
{
	for (int i = 0; i < MAX_SERVERNUMBER; i++)
	{
		if (pUser[i].Mode == USER_EMPTY)
			return i;
	}
	return -1;
}
void DisableAccount(int32_t conn, char* account)
{
	account[ACCOUNTNAME_LENGTH - 1] = '\0';
	account[ACCOUNTNAME_LENGTH - 2] = '\0';

	_strupr_s(account, ACCOUNTNAME_LENGTH);

	STRUCT_ACCOUNTFILE file;
	memcpy(file.AccountName, account, ACCOUNTNAME_LENGTH);
	bool ret = g_pFileDB.DBReadAccount(&file);
	if (!ret)
	{
		if (conn >= 0)
			SendAdminSignal(conn, 0, _MSG_NPNotFound);
		if (conn >= 0)
			SendAdminMessage(conn, 0, "There's no account with that name");

		return;
	}
	if (file.AccountPass[0] == '#')
	{
		if (conn >= 0)
			SendAdminState(conn, 0, _MSG_NPState, file.AccountPass);

		return;
	}
	file.AccountPass[ACCOUNTPASS_LENGTH - 1] = '\0';
	file.AccountPass[ACCOUNTPASS_LENGTH - 2] = '\0';

	char tmp[ACCOUNTPASS_LENGTH] = { 0, };
	sprintf_s(tmp, "#%s", file.AccountPass);
	strcpy_s(file.AccountPass, tmp);
	g_pFileDB.DBWriteAccount(&file);

	if (conn >= 0)
	{
		MSG_NPReqAccount msg;
		msg.Header.PacketId = _MSG_NPReqAccount;
		msg.Header.Size = sizeof(MSG_NPReqAccount);
		strncpy_s(msg.Account, account, ACCOUNTNAME_LENGTH);
		ProcessAdminMessage(conn, (uint8_t*)&msg);
		pAdmin[conn].DisableID = 0;
	}
}
void EnableAccount(int32_t conn, char * account)
{
	account[ACCOUNTNAME_LENGTH - 1] = '\0';
	account[ACCOUNTNAME_LENGTH - 2] = '\0';

	_strupr_s(account, ACCOUNTNAME_LENGTH);


	STRUCT_ACCOUNTFILE file;
	memcpy(file.AccountName, account, ACCOUNTNAME_LENGTH);
	bool ret = g_pFileDB.DBReadAccount(&file);
	if (!ret)
	{
		if (conn >= 0)
			SendAdminSignal(conn, 0, _MSG_NPNotFound);
		if (conn >= 0)
			SendAdminMessage(conn, 0, "There's no account with that name");

		return;
	}
	if (file.AccountPass[0] != '#')
	{
		if (conn >= 0)
			SendAdminState(conn, 0, _MSG_NPState, file.AccountPass);

		return;
	}
	file.AccountPass[ACCOUNTPASS_LENGTH - 1] = '\0';
	file.AccountPass[ACCOUNTPASS_LENGTH - 2] = '\0';

	char tmp[ACCOUNTPASS_LENGTH] = { 0, };
	sprintf_s(tmp, "%s", file.AccountPass + 1);
	strcpy_s(file.AccountPass, tmp);
	g_pFileDB.DBWriteAccount(&file);

	if (conn >= 0)
		SendAdminState(conn, 0, _MSG_NPState, file.AccountPass);

}
bool SendAdminSignal(int32_t svr, uint16_t id, uint16_t signal)
{
	MSG_STANDARD sm;
	sm.Size = sizeof(MSG_STANDARD);
	sm.PacketId = signal;
	sm.ClientId = id;
	return pAdmin[svr].cSock.SendOneMessage((uint8_t*)&sm, sizeof(MSG_STANDARD));
}
bool SendAdminMessage(int32_t svr, uint16_t id, char* message)
{
	MSG_NPNotice sm; 
	sm.Header.PacketId = _MSG_NPNotice;
	sm.Header.ClientId = id; 
	sm.Header.Size = sizeof(MSG_NPNotice);
	strcpy_s(sm.String, message);
	return pAdmin[svr].cSock.SendOneMessage((uint8_t*)&sm, sizeof(MSG_NPNotice));
}
bool SendAdminParm(int32_t svr, uint16_t id, uint16_t signal, int32_t parm)
{
	MSG_STANDARDPARM sm; 
	sm.Header.PacketId = signal;
	sm.Header.ClientId = id;         
	sm.Header.Size = sizeof(MSG_STANDARDPARM);
	sm.Parm = parm;
	return pAdmin[svr].cSock.SendOneMessage((uint8_t*)&sm, sizeof(MSG_STANDARDPARM));
}
bool SendAdminState(int32_t svr, uint16_t id, uint16_t signal, char* pass)
{
	MSG_STANDARDPARM sm; 
	sm.Header.PacketId = signal;
	sm.Header.ClientId = id;
	sm.Header.Size = sizeof(MSG_STANDARDPARM);
	sm.Parm = 0;
	if (pass[0] == '@') sm.Parm = 1;
	if (pass[0] == '_') sm.Parm = 2;
	if (pass[0] == '#') sm.Parm = 3;
	return pAdmin[svr].cSock.SendOneMessage((uint8_t*)&sm, sizeof(MSG_STANDARDPARM));
}
void UpdateConnection()
{
	char tmp[256] = { 0, };
	sprintf_s(tmp, "serv%2.2d.htm", ServerGroup);
	FILE* fs = nullptr;
	fopen_s(&fs, tmp, "wt");

	if (fs == nullptr)
		return;

	for (int i = 0; i < MAX_SERVERNUMBER; i++)
	{
		if (pUser[i].Mode == USER_EMPTY)
		{
			fprintf_s(fs, "-1\n");
			continue;
		}
		if (UserConnection[i] < pUser[i].Count)
			UserConnection[i] = pUser[i].Count;

		fprintf_s(fs, "%d\n", pUser[i].Count);
	}
	fclose(fs);
}
void UpdateConnectionData()
{
	char tmp[256];
	sprintf_s(tmp, "data%2.2d.csv", ServerGroup);
	FILE* fp = nullptr;
	fopen_s(&fp, tmp, "a+");

	if (fp == nullptr) 
		return;
	
	tm when;
	time_t now;
	time(&now);
	localtime_s(&when, &now);

	strftime(tmp, 256, "%y_%m_%d_%H_%M_%S,", &when);
	fprintf_s(fp, tmp);
	
	int32_t total = 0, current = 0;
	for (int i = 0; i < MAX_SERVERNUMBER; i++)
	{
		current = UserConnection[i];
		total += current;
		fprintf_s(fp, "[%2.2d]=%4.4d,", i, current);
		UserConnection[i] = 0;
	}
	fprintf_s(fp, "[Total]=%4.4d\n", total);
	fclose(fp);
}