#ifndef DRAWINFO_H
#define DRAWINFO_H

extern int32_t g_nCurrentTextY;
extern int32_t g_nCurrentTextX;

#define BLACK	RGB(0, 0, 0)
#define RED		RGB(255, 0, 0)
#define BLUE	RGB(0, 0, 255)
#define YELLOW	RGB(255, 255, 0)
#define CYAN	RGB(0, 255, 255)
#define LIGHTGREEN	RGB(0, 255, 0)
#define DARKGREEN	RGB(0, 100, 0)
#define MAGENTA	RGB(255, 0, 255)
#define ORANGE	RGB(255, 69, 0)
#define DARKBLUE	RGB(0, 0, 139)
#define MEDIUMBLUE	RGB(0, 0, 205)
#define CHOCOLATE	RGB(210, 105, 30)

void DrawInfo(HDC hdc);
void PrintText(HDC hdc, COLORREF crText, char *pszFormat, ...);
void LineFeed(int x);
void Tab(int x);

#endif // !DRAWINFO_H
