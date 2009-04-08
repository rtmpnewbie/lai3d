#pragma once

#include <d3dx9.h>

//-----------------------------------------------------------------------------
// Desc: ����ṹ
//-----------------------------------------------------------------------------
struct CUSTOMVERTEX
{
	FLOAT x, y, z;    //����λ��  
	FLOAT u,v ;          //������������
};
#define D3DFVF_CUSTOMVERTEX   (D3DFVF_XYZ|D3DFVF_TEX1)

extern LPDIRECT3D9             g_pD3D       ;    //Direct3D����
extern LPDIRECT3DDEVICE9       g_pd3dDevice ;    //Direct3D�豸����
extern LPDIRECT3DVERTEXBUFFER9 g_pVB        ;    //���㻺��������
extern LPDIRECT3DTEXTURE9      g_pTexture   ;    //�������

extern HWND g_hWnd;


VOID SetupMatrices();
HRESULT InitD3D( HWND hWnd );
HRESULT InitGriphics();
VOID Cleanup();
VOID Render();