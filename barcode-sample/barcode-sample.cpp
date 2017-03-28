#include "../NTKO_BARCODE_LIB/NTKO_BarCodeLib.h"
#include <windows.h>

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;
HINSTANCE g_hInst = NULL;;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
     static TCHAR szAppName[] = TEXT ("NTKO ¶þÎ¬Âë²âÊÔ³ÌÐò") ;
     HWND         hwnd ;
     MSG          msg ;
     WNDCLASS     wndclass ;
	 g_hInst = hInstance ;
     wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = NULL ;
     wndclass.lpszClassName = szAppName ;


     if (!RegisterClass (&wndclass))
     {
          MessageBox (NULL, TEXT ("This program requires Windows NT!"), szAppName, MB_ICONERROR) ;
          return 0 ;
     }
     hwnd = CreateWindow (szAppName,			// window class name
                          szAppName,			// window caption
                          WS_OVERLAPPED|WS_SYSMENU,	  // window style
                          250,					// initial x position
                          200,					// initial y position
                          700,					// initial x size
                          400,					// initial y size
                          NULL,                 // parent window handle
                          NULL,                 // window menu handle
                          hInstance,            // program instance handle
                          NULL) ;               // creation parameters
     
     ShowWindow (hwnd, iCmdShow) ;
     UpdateWindow (hwnd) ;
     
     while (GetMessage (&msg, NULL, 0, 0))
     {
          TranslateMessage (&msg) ;
          DispatchMessage (&msg) ;
     }
     return msg.wParam ;
}

STDAPI_(LPSTR) NtkoLPWSTRToUTF8(LPWSTR pwszString)
{
	LPSTR psz;
	UINT cblen;
	
	if (NULL==pwszString)
	{
		return NULL;
	}
	
	cblen = WideCharToMultiByte(CP_UTF8, 0, pwszString, -1, NULL, 0, NULL, NULL);
	
	if ((psz = (LPSTR)malloc(cblen + 1)) && 
		(0 == WideCharToMultiByte(CP_UTF8, 0, pwszString, -1, psz, cblen, NULL, NULL)))
	{
		free(psz);
	}
	return psz;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC         hdc;
	PAINTSTRUCT ps ;
	switch (message)
	{
	case WM_CREATE:
		return 0 ;
	case WM_PAINT:
		{
			int left=20, top=20;
			_NTKO_PDF417_::PDF417PARAM pdf417param;
			_NTKO_QRCODE_::QRCODEPARAM qrcodeparam;
			//³õÊ¼»¯pdf417param
			{
				pdf417param.options = PDF417_INVERT_BITMAP|PDF417_FIXED_COLUMNS;
				pdf417param.codeColumns = 1;
				pdf417param.errorLevel = 2;
				pdf417param.outBits = NULL;
				pdf417param.lenBits = 0;
				pdf417param.error = 0;
				pdf417param.lenText = -1;
				pdf417param.text = "hello";
				pdf417param.yHeight = 3;
				pdf417param.aspectRatio = 0.5;
			}

			{
				qrcodeparam.nLevel = QR_LEVEL_M;
				qrcodeparam.nVersion = 3;
				qrcodeparam.bAutoExtent = true;
				qrcodeparam.nMaskingNo = -1;
				qrcodeparam.lpsSource = NtkoLPWSTRToUTF8(L"Ð¤ÆôÃ÷");
			}

			hdc = BeginPaint (hwnd, &ps) ;
// 			_NTKO_PDF417_::PDF417DrawImg(hdc,&pdf417param,left,top,1,3);
 			_NTKO_QRCODE_::QRCODEDrawImg(hdc,&qrcodeparam, left, top, 2);
//			_NTKO_EAN13_::NTKOEan13DrawImg(hdc,"7501031311309", left, top,1,30);
//			_NTKO_CODE128_::NTKOCode128DrawImg(hdc,"7501031311309ABCDEFG",left,top,1,30,_NTKO_CODE128_::CODE128_TYPE_AUTO,false);
//			_NTKO_CODE39_::NTKOCode39DrawImg(hdc,"123456ABCDEFG",left,top,1,30);;
			EndPaint (hwnd, &ps);
		}
		return 0 ;
	case WM_DESTROY:
		PostQuitMessage (0) ;
		return 0 ;
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}