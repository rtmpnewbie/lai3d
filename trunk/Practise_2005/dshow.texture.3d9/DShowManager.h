#pragma once

#include <DShow.h>

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
	IGraphBuilder*  g_pGB;          // GraphBuilder
	IMediaControl*  g_pMC;          // Media Control
	IMediaPosition* g_pMP;          // Media Position
	IMediaEvent*    g_pME;          // Media Event
	IBaseFilter*    g_pRenderer;    // our custom renderer
};