#include "stdafx.h"
#include "DrawInfo.h"

int32_t g_nCurrentTextY = 0;
int32_t g_nCurrentTextX = 10;

void DrawInfo(HDC hdc)
{
	if (hdc == 0)
		return;

	g_nCurrentTextY = 10;
	g_nCurrentTextX = 10;

	PrintText(hdc, DARKGREEN, "Nix Project DBSRV");
	LineFeed(5);
	PrintText(hdc, MEDIUMBLUE, "-------------------------------------------------------------------------------------------");
	LineFeed(5);
	PrintText(hdc, MEDIUMBLUE, "Configuration: ");
	LineFeed(4);
	PrintText(hdc, MEDIUMBLUE, "ServerGroup: %d", ServerGroup);
	LineFeed(4);
	PrintText(hdc, MEDIUMBLUE, "Sapphire: %d  TransperCharacter: %d", Sapphire, TransperCharacter);
	LineFeed(4);
	PrintText(hdc, MEDIUMBLUE, "CreateAccountAtLogin: %d", CreateAccountAtLogin);
	LineFeed(4);
	PrintText(hdc, MEDIUMBLUE, "MaxDeleteLevel: %d", MaxDeleteLevel);
	LineFeed(5);
	PrintText(hdc, MEDIUMBLUE, "-------------------------------------------------------------------------------------------");
	LineFeed(5);

	for (int i = 0; i < MAX_SERVERNUMBER; i++)
	{
		in_addr addr; addr.s_addr = pUser[i].IP;

		DWORD color = BLACK;
		if (pUser[i].Mode == USER_ACCEPT)
			color = CHOCOLATE;

		PrintText(hdc, color, "User [%2.2d]:   Mode: %s   IP: %s   UserCount: %d",
			i, pUser[i].Mode == USER_EMPTY ? "Empty" : "Accepted",
			pUser[i].Mode == USER_EMPTY ? "0.0.0.0" : inet_ntoa(addr),
			pUser[i].Count);

		LineFeed(5);
	}
}

void PrintText(HDC hdc, COLORREF crText, char *pszFormat, ...)
{
	static char szText[512] = { 0, };

	va_list vl;
	va_start(vl, pszFormat);
	vsprintf_s(szText, 512, pszFormat, vl);
	va_end(vl);

	SetTextColor(hdc, crText);
	TextOut(hdc, g_nCurrentTextX, g_nCurrentTextY, szText, (int)strlen(szText));

	g_nCurrentTextY += 15;
}
void LineFeed(int x)
{
	g_nCurrentTextY += x;
}
void Tab(int x)
{
	g_nCurrentTextX += x;
}