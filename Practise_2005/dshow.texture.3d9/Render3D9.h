#pragma once

#include <d3dx9.h>

//-----------------------------------------------------------------------------
// Desc: 顶点结构
//-----------------------------------------------------------------------------
struct CUSTOMVERTEX
{
	FLOAT x, y, z;    //顶点位置  
	FLOAT u,v ;          //顶点纹理坐标
};
#define D3DFVF_CUSTOMVERTEX   (D3DFVF_XYZ|D3DFVF_TEX1)

extern LPDIRECT3D9             g_pD3D       ;    //Direct3D对象
extern LPDIRECT3DDEVICE9       g_pd3dDevice ;    //Direct3D设备对象
extern LPDIRECT3DVERTEXBUFFER9 g_pVB        ;    //顶点缓冲区对象
extern LPDIRECT3DTEXTURE9      g_pTexture   ;    //纹理对象

extern HWND g_hWnd;


VOID SetupMatrices();
HRESULT InitD3D( HWND hWnd );
HRESULT InitGriphics();
VOID Cleanup();
VOID Render();