
#include "Movie.h"


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D       = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Our rendering device
CMovie*                 g_pMovie     = NULL;

HWND                    g_hwnd       = NULL;




//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT InitD3D( HWND hWnd )
{
	// Create the D3D object, which is needed to create the D3DDevice.
	if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		return E_FAIL;

	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pd3dDevice ) ) )
	{
		return E_FAIL;
	}

	// Device state would normally be set here

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID Cleanup()
{
	if( g_pd3dDevice != NULL) 
		g_pd3dDevice->Release();

	if( g_pD3D != NULL)
		g_pD3D->Release();

	if (g_pMovie != NULL)
	{
		g_pMovie->CloseMovie();
		delete g_pMovie;
		g_pMovie = NULL;
	}
}




//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
VOID Render()
{

	if(g_pMovie->IsPlaying())
	{
		RECT rc;
		::GetClientRect(g_hwnd, &rc);
		g_pMovie->PutMoviePosition(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
		g_pMovie->RepaintVideo(g_hwnd, ::GetDC(g_hwnd));
	}
	else
	{
		if( NULL == g_pd3dDevice )
			return;

		// Clear the backbuffer to a blue color
		g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );

		// Begin the scene
		if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
		{
			// Rendering of scene objects can happen here
			// End the scene
			g_pd3dDevice->EndScene();
		}

		// Present the backbuffer contents to the display
		g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
	}
}




//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
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
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	// Register the window class
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, 
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		L"D3D Tutorial", NULL };
	RegisterClassEx( &wc );

	// Create the application's window
	g_hwnd = CreateWindow( L"D3D Tutorial", L"DShow CMovie Test", 
		WS_OVERLAPPEDWINDOW, 100, 100, 300, 300,
		NULL, NULL, wc.hInstance, NULL );

	g_pMovie = new CMovie(g_hwnd);
	g_pMovie->OpenMovie(L"C:\\Program Files\\Microsoft Platform SDK for Windows Server 2003 R2\\Samples\\Multimedia\\DirectShow\\Media\\Video\\ruby.avi");
	g_pMovie->PlayMovie();

	// Initialize Direct3D
	if( SUCCEEDED( InitD3D( g_hwnd ) ) )
	{ 
		// Show the window
		ShowWindow( g_hwnd, SW_SHOWDEFAULT );
		UpdateWindow( g_hwnd );

		// Enter the message loop
		MSG msg; 
		ZeroMemory(&msg, sizeof(msg));
		while(msg.message != WM_QUIT)
		{
			if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
			else
			{
				Render();
			}
		}
	}

	UnregisterClass( L"D3D Tutorial", wc.hInstance );
	return 0;
}
