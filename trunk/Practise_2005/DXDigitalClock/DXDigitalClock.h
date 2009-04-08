#pragma once

#include <d3dx9.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 ) 

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
extern LPDIRECT3D9             g_pD3D       ; // Used to create the D3DDevice
extern LPDIRECT3DDEVICE9       g_pd3dDevice ; // Our rendering device
extern LPDIRECT3DVERTEXBUFFER9 g_pVB        ; // Buffer to hold vertices 顶点Buffer

// A structure for our custom vertex type 
// 自定义顶点类型
struct CUSTOMVERTEX
{
	D3DXVECTOR4 position; // The transformed position for the vertex
	DWORD color;          // The vertex color
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

class CDXDigitalClock
{
public:
	CDXDigitalClock();

	~CDXDigitalClock();

	VOID DrawTime(float fStartX, float fStartY);

	VOID Scale(float fScale);
private:
	VOID DrawSegment(const CUSTOMVERTEX* pVerticesTemplate, size_t sizeTemplate, const D3DXVECTOR4& vectorOffset);
	/// 画一个数字
	VOID DrawDigit(int nNumber, float fStartXOffset = 0.0f, float fStartYOffset = 0.0f);
	VOID DrawTwoDigits(int nNumber, float fStartXOffset = 0.0f, float fStartYOffset = 0.0f);
	/// 画一个小方块
	VOID DrawCell(const D3DXVECTOR4& vectorOffset);
	/// 画冒号
	VOID DrawColon(float fStartXOffset, float fStartYOffset, float fGap);
	/// 一个数字的宽度
	float GetNumberWidth() const
	{
		return 4*m_fSharpH+2*m_fOffsetSmall+m_fMid;
	}
	/// 小方块的宽度
	float GetColonWidth() const
	{
		return 2 * m_fHeight;
	}
private:
	float m_fScale;

	float m_fSharpH;     
	float m_fMid;       
	float m_fOffsetSmall;
	float m_fHeight;     
	float m_fGap;        
};

