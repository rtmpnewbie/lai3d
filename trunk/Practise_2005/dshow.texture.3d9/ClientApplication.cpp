//=============================================================================
// Desc: 纹理影射基础
//=============================================================================

#include "stdafx.h"
#include "ClientApplication.h"

#include "dshow.texture3d9.h"
#include "DShowTextures.h"
#include "DShowManager.h"

CDShowManager*                   g_pApp = NULL;

//-----------------------------------------------------------------------------
// Desc: 消息处理
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
// Desc: 入口函数
//-----------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	// Initialize COM
	CoInitialize (NULL);
	//注册窗口类
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		L"ClassName", NULL };
	RegisterClassEx( &wc );

	//创建窗口
	g_hWnd = CreateWindow(  L"ClassName", L"纹理影射基础",
		WS_OVERLAPPEDWINDOW, 200, 100, 600, 500,
		GetDesktopWindow(), NULL, wc.hInstance, NULL );

	//初始化Direct3D
	if( SUCCEEDED( InitD3D( g_hWnd ) ) )
	{
		//创建场景图形
		if( SUCCEEDED( InitGriphics() ) )
		{
			//显示窗口
			ShowWindow( g_hWnd, SW_SHOWDEFAULT );
			UpdateWindow( g_hWnd );

			//进入消息循环
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
					Render();  //渲染图形
				}
			}
		}
	}

	UnregisterClass(  L"ClassName", wc.hInstance );
	CoUninitialize();
	return 0;
}
