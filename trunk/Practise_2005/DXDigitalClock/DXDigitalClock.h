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
extern LPDIRECT3DVERTEXBUFFER9 g_pVB        ; // Buffer to hold vertices ����Buffer

// A structure for our custom vertex type 
// �Զ��嶥������
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
	/// ��һ������
	VOID DrawDigit(int nNumber, float fStartXOffset = 0.0f, float fStartYOffset = 0.0f);
	VOID DrawTwoDigits(int nNumber, float fStartXOffset = 0.0f, float fStartYOffset = 0.0f);
	/// ��һ��С����
	VOID DrawCell(const D3DXVECTOR4& vectorOffset);
	/// ��ð��
	VOID DrawColon(float fStartXOffset, float fStartYOffset, float fGap);
	/// һ�����ֵĿ��
	float GetNumberWidth() const
	{
		return 4*m_fSharpH+2*m_fOffsetSmall+m_fMid;
	}
	/// С����Ŀ��
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

