// Xvid.class.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include "Render3D9.h"
#include "FileReader.h"

LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	//ע�ᴰ����
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		L"ClassName", NULL };
	RegisterClassEx( &wc );

	//��������
	g_hWnd = CreateWindow(  L"ClassName", L"����Ӱ�����",
		WS_OVERLAPPEDWINDOW, 200, 100, 600, 500,
		GetDesktopWindow(), NULL, wc.hInstance, NULL );

	//��ʼ��Direct3D
	if( SUCCEEDED( InitD3D( g_hWnd ) ) )
	{
		//��������ͼ��
		if( SUCCEEDED( InitGriphics() ) )
		{
			if ( GetFileReader()->ReadVideoFile("E:\\TDDOWNLOAD\\Code\\XVID\\xvidcore-1.2.1\\xvidcore\\build\\win32\\bin\\test.mp4u") )
			{
				//��ʾ����
				ShowWindow( g_hWnd, SW_SHOWDEFAULT );
				UpdateWindow( g_hWnd );

				//������Ϣѭ��
				MSG msg;
				ZeroMemory( &msg, sizeof(msg) );
				while( msg.message!=WM_QUIT )
				{
					if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
					{
						TranslateMessage( &msg );
						DispatchMessage( &msg );
					}
					else
					{
						Render();  //��Ⱦͼ��
					}
				}
			}
		}
	}

	UnregisterClass(  L"ClassName", wc.hInstance );

	return 0;
}

LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_DESTROY:
		Cleanup();
		PostQuitMessage( 0 );
		return 0;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}
