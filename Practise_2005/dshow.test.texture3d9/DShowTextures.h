//-----------------------------------------------------------------------------
// File: DShowTextures.h
//
// Desc: DirectShow sample code - adds support for DirectShow videos playing 
//       on a DirectX 8.0 texture surface. Turns the D3D texture tutorial into 
//       a recreation of the VideoTex sample from previous versions of DirectX.
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#include <streams.h>
#include <d3d9.h>

//-----------------------------------------------------------------------------
// Define GUID for Texture Renderer
// {71771540-2017-11cf-AE26-0020AFD79767}
//-----------------------------------------------------------------------------
struct __declspec(uuid("{71771540-2017-11cf-ae26-0020afd79767}")) CLSID_TextureRenderer;

//-----------------------------------------------------------------------------
// CTextureRenderer Class Declarations
//-----------------------------------------------------------------------------
class CTextureRenderer : public CBaseVideoRenderer
{
public:
	CTextureRenderer(LPUNKNOWN pUnk,HRESULT *phr);
	~CTextureRenderer();

public:
	virtual HRESULT CheckMediaType(const CMediaType *pmt );     // Format acceptable?
	virtual HRESULT SetMediaType(const CMediaType *pmt );       // Video format notification
	virtual HRESULT DoRenderSample(IMediaSample *pMediaSample); // New video sample

private:
	BOOL m_bUseDynamicTextures;
	LONG m_lVidWidth;   // Video width
	LONG m_lVidHeight;  // Video Height
	LONG m_lVidPitch;   // Video Pitch

	D3DFORMAT               mTextureFormat; // Texture format
};



