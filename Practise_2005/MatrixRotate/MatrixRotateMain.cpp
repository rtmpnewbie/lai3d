#include <d3d9.h>
#include <d3dx9math.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 ) 

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D       = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Our rendering device
LPDIRECT3DVERTEXBUFFER9 g_pVB        = NULL; // Buffer to hold vertices 顶点Buffer

// A structure for our custom vertex type 自定义顶点类型
struct CUSTOMVERTEX
{
	FLOAT x, y, z; // The transformed position for the vertex
	DWORD color;        // The vertex color
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

HRESULT InitD3D( HWND hWnd )
{
	// Create the D3D object.
	if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		return E_FAIL;

	// Set up the structure used to create the D3DDevice
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES; // 多重采样,反锯齿 

	// Create the D3DDevice
	if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pd3dDevice ) ) )
	{
		return E_FAIL;
	}

	// Turn off culling, so we see the front and back of the triangle
	// 关闭剔除，以便三角形的前后都能被我们看到
	//g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	// Turn off D3D lighting, since we are providing our own vertex colors
	// 关闭D3D光照，因为我们提供我们自己的顶点颜色
	// 这行是必需的，没有这行看不到，why？
	g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	return S_OK;
}

HRESULT InitVB()
{
	// 初始化要渲染的三角形链的顶点
	CUSTOMVERTEX vertices[] =
	{
		{ -5.0f, 0.0f, 0.0f, 0xff00ffff, },
		{ -4.0f, 1.0f, 0.0f, 0xff00ffff, },
		{ -4.0f,-1.0f, 0.0f, 0xff00ffff, },
		{  4.0f, 1.0f, 0.0f, 0xff00ffff, },
		{  4.0f,-1.0f, 0.0f, 0xff00ffff, },
		{  5.0f, 0.0f, 0.0f, 0xff00ffff, },
	};

	if( FAILED( g_pd3dDevice->CreateVertexBuffer( 6*sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB, NULL ) ) )
	{
		return E_FAIL;
	}

	VOID* pVertices; // 输出参数
	if( FAILED( g_pVB->Lock( 0, sizeof(vertices), (void**)&pVertices, 0 ) ) )
		return E_FAIL;
	memcpy( pVertices, vertices, sizeof(vertices) );
	g_pVB->Unlock();

	return S_OK;
}

VOID Cleanup()
{
	if( g_pVB != NULL )        
		g_pVB->Release();

	if( g_pd3dDevice != NULL ) 
		g_pd3dDevice->Release();

	if( g_pD3D != NULL )       
		g_pD3D->Release();
}

VOID SetupMatrices(float fX, float fY, float fZ, int rotateAngle)
{
	D3DXMATRIXA16 matWorld;
	FLOAT fAngle = rotateAngle * (2.0f * D3DX_PI) / 360.0f;
	D3DXMatrixRotationZ( &matWorld, fAngle );

	D3DXMATRIXA16 matTranslation, matScale;
	D3DXMatrixTranslation(&matTranslation, fX, fY, fZ);
	D3DXMatrixMultiply(&matWorld, &matWorld, &matTranslation);

	D3DXMatrixScaling(&matScale, 0.15f, 0.15f, 0.15f);
	D3DXMatrixMultiply(&matWorld, &matWorld, &matScale);

	g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	D3DXVECTOR3 vEyePt( 0.0f, 0.0f,-5.0f );
	D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
	g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, 1.0f, 1.0f, 100.0f );
	g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
}

VOID Render()
{
	g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );

	if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
	{
		// 三条横着的
		SetupMatrices(0.0f, 11.7f, 0.0f, 0);

		g_pd3dDevice->SetStreamSource( 0, g_pVB, 0, sizeof(CUSTOMVERTEX) );
		g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
		g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 4 );

		SetupMatrices(0.0f, 0.0f, 0.0f, 0);
		g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 4 );

		SetupMatrices(0.0f, -11.7f, 0.0f, 0);
		g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 4 );
		// 四条竖着的
		SetupMatrices(5.6f, 5.7f, 0.0f, 90);
		g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 4 );
		SetupMatrices(-5.6f, 5.7f, 0.0f, 90);
		g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 4 );
		SetupMatrices(5.6f, -5.9f, 0.0f, 90);
		g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 4 );
		SetupMatrices(-5.6f, -5.9f, 0.0f, 90);
		g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 4 );

		g_pd3dDevice->EndScene();
	}
	// Present the backbuffer contents to the display
	g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
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

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	// Register the window class
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		"D3D Tutorial", NULL };
	RegisterClassEx( &wc );

	// Create the application's window
	HWND hWnd = CreateWindow( "D3D Tutorial", "D3D Tutorial 02: Vertices",
		WS_OVERLAPPEDWINDOW, 100, 100, 700, 700,
		NULL, NULL, wc.hInstance, NULL );

	// Initialize Direct3D
	if( SUCCEEDED( InitD3D( hWnd ) ) )
	{
		// Create the vertex buffer
		if( SUCCEEDED( InitVB() ) )
		{
			// Show the window
			ShowWindow( hWnd, SW_SHOWDEFAULT );
			UpdateWindow( hWnd );

			// Enter the message loop
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
					Render();
			}
		}
	}
	UnregisterClass( "D3D Tutorial", wc.hInstance );
	return 0;
}