#pragma once

#include <streams.h>

class CDShowManager
{
public:
	CDShowManager(){}
	~CDShowManager(){}

	//-----------------------------------------------------------------------------
	// InitDShowTextureRenderer : Create DirectShow filter graph and run the graph
	//-----------------------------------------------------------------------------
	HRESULT InitDShowTextureRenderer();

	//-----------------------------------------------------------------------------
	// CheckMovieStatus: If the movie has ended, rewind to beginning
	//-----------------------------------------------------------------------------
	void CheckMovieStatus(void);

	//-----------------------------------------------------------------------------
	// CleanupDShow
	//-----------------------------------------------------------------------------
	void CleanupDShow(void);

private:
	CComPtr<IGraphBuilder>  m_pGB;          // GraphBuilder
	CComPtr<IMediaControl>  m_pMC;          // Media Control
	CComPtr<IMediaPosition> m_pMP;          // Media Position
	CComPtr<IMediaEvent>    m_pME;          // Media Event
	CComPtr<IBaseFilter>    m_pRenderer;    // our custom renderer
};