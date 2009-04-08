#pragma once

//#include <MMSystem.h>
#include <d3dx9.h>


// A structure for our custom vertex type
struct CUSTOMVERTEX
{
	FLOAT x, y, z;      // The untransformed, 3D position for the vertex
	DWORD color;        // The vertex color
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)




class RenderSystem
{
public:
	RenderSystem();
	~RenderSystem();

	//-----------------------------------------------------------------------------
	// Name: InitD3D()
	// Desc: Initializes Direct3D
	//-----------------------------------------------------------------------------
	HRESULT InitD3D( HWND hWnd );

	//-----------------------------------------------------------------------------
	// Name: InitGeometry()
	// Desc: Creates the scene geometry
	//-----------------------------------------------------------------------------
	HRESULT InitGeometry();

	VOID SetupMatrices();

	VOID Render();
private:
	LPDIRECT3D9             g_pD3D       ; // Used to create the D3DDevice
	LPDIRECT3DDEVICE9       g_pd3dDevice ; // Our rendering device
	LPDIRECT3DVERTEXBUFFER9 g_pVB        ; // Buffer to hold vertices
};