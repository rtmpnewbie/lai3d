// Practise_2005.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Practise_2005.h"

#define ID_TIMER    1

#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

// �˴���ģ���а����ĺ�����ǰ������:
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

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PRACTISE_2005, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PRACTISE_2005));

	// ����Ϣѭ��:
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
//    ����ϣ��
//    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
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
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

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
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
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
		// �����˵�ѡ��:
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

		// TODO: �ڴ���������ͼ����...
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

// �����ڡ������Ϣ�������
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
