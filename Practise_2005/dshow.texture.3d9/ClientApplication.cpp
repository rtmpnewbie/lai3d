//=============================================================================
// Desc: ����Ӱ�����
//=============================================================================

#include "stdafx.h"
#include "ClientApplication.h"

#include "dshow.texture3d9.h"
#include "DShowTextures.h"
#include "DShowManager.h"

CDShowManager*                   g_pApp = NULL;

//-----------------------------------------------------------------------------
// Desc: ��Ϣ����
//-----------------------------------------------------------------------------
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



//-----------------------------------------------------------------------------
// Desc: ��ں���
//-----------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	// Initialize COM
	CoInitialize (NULL);
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

	UnregisterClass(  L"ClassName", wc.hInstance );
	CoUninitialize();
	return 0;
}
