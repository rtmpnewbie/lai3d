// Practise_2005.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Practise_2005.h"

#define ID_TIMER    1

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PRACTISE_2005, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PRACTISE_2005));

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PRACTISE_2005));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PRACTISE_2005);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void DisplayDigit (HDC hdc, int iNumber)
{
	static BOOL  fSevenSegment [10][7] = {
		1, 1, 	1, 	0, 	1, 	1, 	1,     		// 0
		0, 0, 	1, 	0, 	0, 	1, 	0,     		// 1
		1, 0, 	1, 	1, 	1, 	0, 	1,     		// 2
		1, 0, 	1, 	1, 	0, 	1, 	1,     		// 3
		0, 1, 	1, 	1, 	0, 	1, 	0,     		// 4
		1, 1, 	0, 	1, 	0, 	1, 	1,     		// 5
		1, 1, 	0, 	1, 	1, 	1, 	1,     		// 6
		1, 0, 	1, 	0, 	0, 	1, 	0,     		// 7
		1, 1, 	1, 	1, 	1, 	1, 	1,     		// 8
		1, 1, 	1, 	1, 	0, 	1, 	1 } ;		// 9

	static POINT ptSegment [7][6] = {
		7,  6,  11,	2,  31,  2, 35,  6,  31, 10, 11, 10,
		6,  7,  10, 11, 10, 31, 6,   35, 2,  31, 2,  11,
		36, 7,  40, 11, 40, 31, 36,  35, 32, 31, 32, 11,
		7 , 36, 11, 32, 31, 32, 35,  36, 31, 40, 11, 40,
		6 , 37, 10, 41, 10, 61, 6,   65, 2,  61, 2,  41,
		36, 37, 40, 41, 40, 61, 36,  65, 32, 61, 32, 41,
		7 , 66, 11, 62, 31, 62, 35,  66, 31, 70, 11, 70 } ;

	int		iSeg ;
	for (iSeg = 0 ; iSeg < 7 ; iSeg++)
	{
		if (fSevenSegment [iNumber][iSeg])
		{
			Polygon (hdc, ptSegment [iSeg], 6) ;
		}
	}
}

void DisplayTwoDigits (HDC hdc, int iNumber, BOOL fSuppress)
{
	if (!fSuppress || (iNumber / 10 != 0))
		DisplayDigit (hdc, iNumber / 10) ;
	OffsetWindowOrgEx (hdc, -42, 0, NULL) ;
	DisplayDigit (hdc, iNumber % 10) ;
	OffsetWindowOrgEx (hdc, -42, 0, NULL) ;
}

void DisplayColon (HDC hdc)
{
	POINT ptColon [2][4] = {	2,	21,	6,	17,	10,	21,	6,	25,
		2,	51,	6,	47,	10,	51,	6, 	55 } ;

	Polygon (hdc, ptColon [0], 4) ;
	Polygon (hdc, ptColon [1], 4) ;

	OffsetWindowOrgEx (hdc, -12, 0, NULL) ;
}

void DisplayBlank(HDC hdc)
{
	OffsetWindowOrgEx (hdc, -12, 0, NULL) ;
}

void DisplayTime (HDC hdc, BOOL f24Hour, BOOL fSuppress)
{
	SYSTEMTIME st ;
	GetLocalTime (&st) ;
	if (f24Hour)
		DisplayTwoDigits (hdc, st.wHour, fSuppress) ;
	else
		DisplayTwoDigits (hdc, (st.wHour %= 12) ? st.wHour : 12, fSuppress) ;
	if (st.wSecond % 2 == 0)
	{
		DisplayColon (hdc) ;
	}
	else
	{
		DisplayBlank(hdc);
	}
	DisplayTwoDigits (hdc, st.wMinute, FALSE) ;
	if (st.wSecond % 2 != 0)
	{
		DisplayColon (hdc) ;
	}
	else
	{
		DisplayBlank(hdc);
	}
	DisplayTwoDigits (hdc, st.wSecond, FALSE) ;
}


//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	static BOOL   		f24Hour, fSuppress ;
	static HBRUSH 		hBrushRed ;
	static int    			cxClient, cyClient ;
	TCHAR					szBuffer [2] ;


	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case 	WM_CREATE:
		hBrushRed = CreateSolidBrush (RGB (255, 0, 0)) ;
		SetTimer (hWnd, ID_TIMER, 1000, NULL) ;// fall through

	case 	WM_SETTINGCHANGE:
		GetLocaleInfo (LOCALE_USER_DEFAULT, LOCALE_ITIME, szBuffer, 2) ;
		f24Hour = (szBuffer[0] == '1') ;

		GetLocaleInfo (LOCALE_USER_DEFAULT, LOCALE_ITLZERO, szBuffer, 2) ;
		fSuppress = (szBuffer[0] == '0') ;

		InvalidateRect (hWnd, NULL, TRUE) ;
		return 0 ;

	case 	WM_SIZE:
		cxClient = LOWORD (lParam) ;
		cyClient = HIWORD (lParam) ;
		return 0 ;

	case 	WM_TIMER:
		InvalidateRect (hWnd, NULL, TRUE) ;
		return 0 ;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		// TODO: 在此添加任意绘图代码...
		SetMapMode (hdc, MM_ISOTROPIC) ;
		SetWindowExtEx (hdc, 276, 72, NULL) ;
		SetViewportExtEx (hdc, cxClient, cyClient, NULL) ;

		SetWindowOrgEx (hdc, 138, 36, NULL) ;
		SetViewportOrgEx (hdc, cxClient / 2, cyClient / 2, NULL) ;
		SelectObject (hdc, GetStockObject (NULL_PEN)) ;
		SelectObject (hdc, hBrushRed) ;
		DisplayTime (hdc, f24Hour, fSuppress) ;

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		KillTimer (hWnd, ID_TIMER) ;
		DeleteObject (hBrushRed) ;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
