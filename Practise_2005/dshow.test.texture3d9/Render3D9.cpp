#include "stdafx.h"

#include "Render3D9.h"
#include "ClientApplication.h"
#include "DShowManager.h"

//-----------------------------------------------------------------------------
// Desc: 全局变量
//-----------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D       = NULL;    //Direct3D对象
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL;    //Direct3D设备对象
LPDIRECT3DVERTEXBUFFER9 g_pVB        = NULL;    //顶点缓冲区对象
LPDIRECT3DTEXTURE9      g_pTexture   = NULL;    //纹理对象
HWND                    g_hWnd;
bool                    g_bDeviceLost = false;

UINT nTextureWidth  = 512; // 单位： 像素
UINT nTextureHeight = 512; // 单位： 像素

//-----------------------------------------------------------------------------
// Desc: 设置变换矩阵
//-----------------------------------------------------------------------------
VOID SetupMatrices()
{
	//创建并设置世界矩阵
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity( &matWorld );
	g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	//创建并设置观察矩阵
	D3DXVECTOR3 vEyePt( 0.0f, 0.0f, -10 );
	D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
	g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

	//创建并设置投影矩阵
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, 1.0f, 1.0f, 100.0f );
	g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
}


//-----------------------------------------------------------------------------
// Desc: 初始化Direct3D
//-----------------------------------------------------------------------------
HRESULT InitD3D( HWND hWnd )
{
	//创建Direct3D对象, 该对象用于创建Direct3D设备对象
	if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		return E_FAIL;

	//设置D3DPRESENT_PARAMETERS结构, 准备创建Direct3D设备对象
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	//创建Direct3D设备对象
	if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pd3dDevice ) ) )
	{
		return E_FAIL;
	}

	//禁用照明效果
	g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE ); 

	//设置变换矩阵
	SetupMatrices();

	return S_OK;
}


//-----------------------------------------------------------------------------
// Desc: 创建场景图形
//-----------------------------------------------------------------------------
HRESULT InitGriphics()
{
	//顶点数据
	CUSTOMVERTEX g_Vertices[] =
	{
		{ -3,   -4,  0.0f,  0.0f, 0.0f},   
		{ -3,    2,  0.0f,  0.0f, 1.0f},    
		{  3,   -4,  0.0f,  1.0f, 0.0f},    
		{  3,    2,  0.0f,  1.0f, 1.0f }

	};

	//创建顶点缓冲区
	if( FAILED( g_pd3dDevice->CreateVertexBuffer( 4*sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_MANAGED, &g_pVB,NULL ) ) )
	{
		return E_FAIL;
	}

	//填充顶点缓冲区
	VOID* pVertices;
	if( FAILED( g_pVB->Lock( 0, sizeof(g_Vertices), (void**)&pVertices, 0 ) ) )
		return E_FAIL;
	memcpy( pVertices, g_Vertices, sizeof(g_Vertices) );
	g_pVB->Unlock();

	g_pApp = new CDShowManager();
	// DShow: Set up filter graph with our custom renderer.
	if( FAILED( g_pApp->InitDShowTextureRenderer() ) )
		return E_FAIL;

	return S_OK;
}


//-----------------------------------------------------------------------------
// Desc: 释放创建的对象
//-----------------------------------------------------------------------------
VOID Cleanup()
{
	g_pApp->CleanupDShow();
	//释放纹理对象
	if( g_pTexture != NULL )
		g_pTexture->Release();

	//释放顶点缓冲区对象
	if( g_pVB != NULL )        
		g_pVB->Release();

	//释放Direct3D设备对象
	if( g_pd3dDevice != NULL ) 
		g_pd3dDevice->Release();

	//释放Direct3D对象
	if( g_pD3D != NULL )       
		g_pD3D->Release();
}


//-----------------------------------------------------------------------------
// Desc: 渲染图形 
//-----------------------------------------------------------------------------
VOID Render()
{
	HRESULT hr = S_OK;

	if (!g_pd3dDevice)
		return;
	// Handle the loss of the Direct3D surface (for example, by switching
	// to a full-screen command prompt and back again)
	if( g_bDeviceLost )
	{
		// Test the cooperative level to see if it's okay to render
		if( FAILED( hr = g_pd3dDevice->TestCooperativeLevel() ) )
		{
			// If the device was lost, do not render until we get it back
			if( D3DERR_DEVICELOST == hr ||
				D3DERR_DRIVERINTERNALERROR == hr )
				return;

			// Check if the device needs to be reset.
			if( D3DERR_DEVICENOTRESET == hr )
			{
				// Reset the D3D environment
				Cleanup();
				hr = InitD3D(g_hWnd);
				hr = InitGriphics();
			}

			return;
		}

		// TestCooperativeLevel() succeeded, so we have regained the device
		g_bDeviceLost = false;
	}

	//清空后台缓冲区
	hr = g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(45, 50, 170), 1.0f, 0 );

	//开始在后台缓冲区绘制图形
	if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
	{
		g_pd3dDevice->SetTexture( 0, g_pTexture ); //设置纹理(重剑：在俩三角形上贴了张图)
		g_pd3dDevice->SetStreamSource( 0, g_pVB, 0, sizeof(CUSTOMVERTEX) );
		g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
		g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);

		//结束在后台缓冲区绘制图形    
		g_pd3dDevice->EndScene();                                                                                                                                                                              
	}                                      

	//将在后台缓冲区绘制的图形提交到前台缓冲区显示
	hr = g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
	// If the Present call failed because we lost the Direct3D surface,
	// set a state variable for the next render pass, where we will attempt
	// to recover the lost surface.
	if( D3DERR_DEVICELOST == hr ||
		D3DERR_DRIVERINTERNALERROR == hr )
		g_bDeviceLost = true;

	// Check to see if we need to restart the movie
	g_pApp->CheckMovieStatus();
}