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
	IGraphBuilder*  m_pGB;          // GraphBuilder
	IMediaControl * m_pMC;          // Media Control
	IMediaPosition *m_pMP;          // Media Position
	IMediaEvent    *m_pME;          // Media Event
	IBaseFilter    *m_pRenderer;    // our custom renderer
};