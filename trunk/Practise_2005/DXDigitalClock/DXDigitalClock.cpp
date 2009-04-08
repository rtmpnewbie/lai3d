#include "DXDigitalClock.h"

LPDIRECT3D9             g_pD3D       = NULL;
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVB        = NULL;

const static float c_fSharpH = 4.0f;      // 尖头的高度(注意：尖头是直角的，所以尖头那个三角形的高和底边的一半相等)
const static float c_fMid = 30.0f;        // 中段的宽度
const static float c_fOffsetSmall = 1.0f; // 微调

const static float c_fHeight = 6.0f;      // 小方块对角线/2

const static float c_fGap = 2.0f;         // 数字间或数字与冒号的距离

CDXDigitalClock::CDXDigitalClock()
: m_fSharpH(c_fSharpH)     
, m_fMid(c_fMid)       
, m_fOffsetSmall(c_fOffsetSmall)
, m_fHeight(c_fHeight)    
, m_fGap(c_fGap)
, m_fScale(1.0f)
{

}

CDXDigitalClock::~CDXDigitalClock()
{

}

VOID CDXDigitalClock::DrawSegment( const CUSTOMVERTEX* pVerticesTemplate, size_t sizeTemplate, const D3DXVECTOR4& vectorOffset )
{
	if( g_pVB != NULL )        
		g_pVB->Release();

	CUSTOMVERTEX tmpVitices[6];
	memcpy(tmpVitices, pVerticesTemplate, sizeTemplate);
	for (int i = 0; i < 6; ++i)
	{
		tmpVitices[i].position += vectorOffset;
	}
	g_pd3dDevice->CreateVertexBuffer( 6*sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB, NULL );

	VOID* pVertices; // 输出参数
	g_pVB->Lock( 0, sizeof(tmpVitices), (void**)&pVertices, 0 );
	memcpy( pVertices, tmpVitices, sizeof(tmpVitices) );
	g_pVB->Unlock();
	g_pd3dDevice->SetStreamSource( 0, g_pVB, 0, sizeof(CUSTOMVERTEX) );
	g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
	g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 4 );
}

VOID CDXDigitalClock::DrawDigit( int nNumber, float fStartXOffset /*= 0.0f*/, float fStartYOffset /*= 0.0f*/ )
{
	// 0x1代表有，0x3代表有并且横着
	const static BYTE  SevenSegment [10][7] = {
		3, 1, 	1, 	0, 	1, 	1, 	3,     		// 0
		0, 0, 	1, 	0, 	0, 	1, 	0,     		// 1
		3, 0, 	1, 	3, 	1, 	0, 	3,     		// 2
		3, 0, 	1, 	3, 	0, 	1, 	3,     		// 3
		0, 1, 	1, 	3, 	0, 	1, 	0,     		// 4
		3, 1, 	0, 	3, 	0, 	1, 	3,     		// 5
		3, 1, 	0, 	3, 	1, 	1, 	3,     		// 6
		3, 0, 	1, 	0, 	0, 	1, 	0,     		// 7
		3, 1, 	1, 	3, 	1, 	1, 	3,     		// 8
		3, 1, 	1, 	3, 	0, 	1, 	3 } ;		// 9
		//  0  1    2   3   4   5   6

		const float c_fStartX = m_fOffsetSmall + m_fSharpH;
		const float c_fStartY = 0.0f;

		// 横着的segment
		CUSTOMVERTEX verticesSegmentHori[] =
		{
			/*0*/{ D3DXVECTOR4( c_fStartX,                      c_fStartY+m_fSharpH,    0.0f, 1.0f), 0xffff0000, }, 
			/*1*/{ D3DXVECTOR4( c_fStartX+m_fSharpH,  	        c_fStartY,              0.0f, 1.0f), 0xffff0000, },
			/*2*/{ D3DXVECTOR4( c_fStartX+m_fSharpH,  	        c_fStartY+2*m_fSharpH,  0.0f, 1.0f), 0xffff0000, },
			/*3*/{ D3DXVECTOR4( c_fStartX+m_fSharpH+m_fMid,  	c_fStartY,              0.0f, 1.0f), 0xffff0000, },
			/*4*/{ D3DXVECTOR4( c_fStartX+m_fSharpH+m_fMid,  	c_fStartY+2*m_fSharpH,  0.0f, 1.0f), 0xffff0000, },
			/*5*/{ D3DXVECTOR4( c_fStartX+m_fSharpH*2+m_fMid,   c_fStartY+m_fSharpH,    0.0f, 1.0f), 0xffff0000, },
		};
		// 竖着的segment
		CUSTOMVERTEX verticesSegmentVert[] =
		{
			/*0*/{ D3DXVECTOR4( c_fStartX+m_fSharpH,     c_fStartY,                    0.0f, 1.0f), 0xffff0000, }, 
			/*1*/{ D3DXVECTOR4( c_fStartX+2*m_fSharpH,   c_fStartY+m_fSharpH,          0.0f, 1.0f), 0xffff0000, },
			/*2*/{ D3DXVECTOR4( c_fStartX,               c_fStartY+m_fSharpH,          0.0f, 1.0f), 0xffff0000, },
			/*3*/{ D3DXVECTOR4( c_fStartX+2*m_fSharpH,   c_fStartY+m_fSharpH+m_fMid,   0.0f, 1.0f), 0xffff0000, },
			/*4*/{ D3DXVECTOR4( c_fStartX,               c_fStartY+m_fSharpH+m_fMid,   0.0f, 1.0f), 0xffff0000, },
			/*5*/{ D3DXVECTOR4( c_fStartX+m_fSharpH,     c_fStartY+2*m_fSharpH+m_fMid, 0.0f, 1.0f), 0xffff0000, },
		};
		// 每个segment的偏移向量
		D3DXVECTOR4 vector4Offset[] = 
		{
			/*0*/D3DXVECTOR4(fStartXOffset
			, fStartYOffset, 0.0f, 0.0f),
			/*1*/D3DXVECTOR4(fStartXOffset-m_fSharpH-m_fOffsetSmall
			, fStartYOffset+m_fSharpH+m_fOffsetSmall, 0.0f, 0.0f),
			/*2*/D3DXVECTOR4(fStartXOffset-m_fSharpH-m_fOffsetSmall+m_fMid+2*m_fSharpH+2*m_fOffsetSmall
			, fStartYOffset+m_fSharpH+m_fOffsetSmall, 0.0f, 0.0f),
			/*3*/D3DXVECTOR4(fStartXOffset
			, fStartYOffset+m_fMid+2*m_fSharpH+2*m_fOffsetSmall, 0.0f, 0.0f),
			/*4*/D3DXVECTOR4(fStartXOffset-m_fSharpH-m_fOffsetSmall
			, fStartYOffset+m_fSharpH+m_fOffsetSmall+(2*m_fSharpH+m_fMid+2*m_fOffsetSmall), 0.0f, 0.0f),
			/*5*/D3DXVECTOR4(fStartXOffset-m_fSharpH-m_fOffsetSmall+m_fMid+2*m_fSharpH+2*m_fOffsetSmall
			, fStartYOffset+m_fSharpH+m_fOffsetSmall+(2*m_fSharpH+m_fMid+2*m_fOffsetSmall), 0.0f, 0.0f),
			/*6*/D3DXVECTOR4(fStartXOffset
			, fStartYOffset+m_fMid*2+4*m_fSharpH+4*m_fOffsetSmall, 0.0f, 0.0f),
		};

		for (int nSeg = 0; nSeg < 7; ++nSeg)
		{
			BYTE bySeg = SevenSegment[nNumber][nSeg];
			if (bySeg & 0x1)
			{
				// 有此段 
				if (bySeg & 0x2) 
				{
					// 横着的
					DrawSegment(verticesSegmentHori, sizeof(verticesSegmentHori), vector4Offset[nSeg]);
				}
				else
				{
					// 竖着的
					DrawSegment(verticesSegmentVert, sizeof(verticesSegmentVert), vector4Offset[nSeg]);
				}
			}
		}
}

VOID CDXDigitalClock::DrawTwoDigits( int nNumber, float fStartXOffset /*= 0.0f*/, float fStartYOffset /*= 0.0f*/ )
{
	DrawDigit(nNumber/10, fStartXOffset, fStartYOffset);
	fStartXOffset += (GetNumberWidth() + m_fGap);
	DrawDigit(nNumber%10, fStartXOffset, fStartYOffset);
}

VOID CDXDigitalClock::DrawCell( const D3DXVECTOR4& vectorOffset )
{
	DWORD dwColor = 0xff00ff00;

	CUSTOMVERTEX vertices[] =
	{
		/*0*/{ D3DXVECTOR4( 0.0f,        m_fHeight,    0.0f, 1.0f), dwColor, }, 
		/*1*/{ D3DXVECTOR4( m_fHeight,   0.0f,         0.0f, 1.0f), dwColor, },
		/*2*/{ D3DXVECTOR4( m_fHeight,   m_fHeight*2,  0.0f, 1.0f), dwColor, },
		/*3*/{ D3DXVECTOR4( m_fHeight*2, m_fHeight,    0.0f, 1.0f), dwColor, },
	};
	if( g_pVB != NULL )        
		g_pVB->Release();

	CUSTOMVERTEX tmpVitices[4];
	memcpy(tmpVitices, vertices, sizeof(vertices));
	for (int i = 0; i < 4; ++i)
	{
		tmpVitices[i].position += vectorOffset;
	}
	g_pd3dDevice->CreateVertexBuffer( 4*sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB, NULL );

	VOID* pVertices; // 输出参数
	g_pVB->Lock( 0, sizeof(tmpVitices), (void**)&pVertices, 0 );
	memcpy( pVertices, tmpVitices, sizeof(tmpVitices) );
	g_pVB->Unlock();
	g_pd3dDevice->SetStreamSource( 0, g_pVB, 0, sizeof(CUSTOMVERTEX) );
	g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
	g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
}

VOID CDXDigitalClock::DrawTime(float fStartX, float fStartY)
{
	float fColonOffsetY = m_fMid/2+3*m_fSharpH-m_fHeight;

	SYSTEMTIME st ;
	GetLocalTime(&st);

	DrawTwoDigits(st.wHour, fStartX, fStartY);

	fStartX += 2*(GetNumberWidth()+m_fGap);
	DrawColon(fStartX, fStartY+fColonOffsetY, m_fMid+2*m_fOffsetSmall);

	fStartX += (GetColonWidth()+m_fGap);
	DrawTwoDigits(st.wMinute, fStartX, fStartY);

	fStartX += 2*(GetNumberWidth()+m_fGap);
	DrawColon(fStartX, fStartY+fColonOffsetY, m_fMid+2*m_fOffsetSmall);

	fStartX += (GetColonWidth()+m_fGap);
	DrawTwoDigits(st.wSecond, fStartX, fStartY);
}

VOID CDXDigitalClock::DrawColon( float fStartXOffset /*= 0.0f*/, float fStartYOffset /*= 0.0f*/, float fGap /*= 50.0f*/ )
{
	DrawCell(D3DXVECTOR4(fStartXOffset, fStartYOffset, 0.0f, 0.0f));
	DrawCell(D3DXVECTOR4(fStartXOffset, fStartYOffset+fGap, 0.0f, 0.0f));
}

VOID CDXDigitalClock::Scale( float fScale )
{
	m_fScale = fScale;

	m_fSharpH = c_fSharpH * m_fScale;     
	m_fMid = c_fMid * m_fScale;       
	m_fOffsetSmall = c_fOffsetSmall * m_fScale;
	m_fHeight = c_fHeight * m_fScale;    
	m_fGap = c_fGap * m_fScale;
}
