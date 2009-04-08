#include "stdafx.h"

#include "DShowManager.h"
#include "DShowTextures.h"
#include "dshow.texture3d9.h"

//inline LPWSTR T2W(__in_opt LPTSTR lp) { return lp; }

//-----------------------------------------------------------------------------
// InitDShowTextureRenderer : Create DirectShow filter graph and run the graph
//-----------------------------------------------------------------------------
HRESULT CDShowManager::InitDShowTextureRenderer()
{
	HRESULT hr = S_OK;
	IBaseFilter*    pFSrc;          // Source Filter
	IPin*           pFSrcPinOut;    // Source Filter Output Pin
	CTextureRenderer        *pCTR=0;        // DirectShow Texture renderer

	// Create the filter graph
	//if (FAILED(g_pGB->CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC)))
	//	return E_FAIL;

	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void **)&m_pGB);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

#ifdef REGISTER_FILTERGRAPH
	// Register the graph in the Running Object Table (for debug purposes)
	AddToROT(m_pGB);
#endif

	// Create the Texture Renderer object
	pCTR = new CTextureRenderer(NULL, &hr);
	if (FAILED(hr) || !pCTR)
	{
		delete pCTR;
		Msg(TEXT("Could not create texture renderer object!  hr=0x%x"), hr);
		return E_FAIL;
	}

	// Get a pointer to the IBaseFilter on the TextureRenderer, add it to graph
	m_pRenderer = pCTR;
	if (FAILED(hr = m_pGB->AddFilter(m_pRenderer, L"TEXTURERENDERER")))
	{
		Msg(TEXT("Could not add renderer filter to graph!  hr=0x%x"), hr);
		return hr;
	}

	// Determine the file to load based on windows directory
	// Use the standard win32 API to do this.
	TCHAR strFileName[MAX_PATH] = {0};
	WCHAR wFileName[MAX_PATH];

	if (! GetClipFileName(strFileName))
	{
		DWORD dwDlgErr = CommDlgExtendedError();

		// Don't show output if user cancelled the selection (no dlg error)
		if (dwDlgErr)
		{
			Msg(TEXT("GetClipFileName Failed! Error=0x%x\r\n"), GetLastError());
		}
		return E_FAIL;
	}

	strFileName[MAX_PATH-1] = 0;  // NULL-terminate
	wFileName[MAX_PATH-1] = 0;    // NULL-terminate

	//USES_CONVERSION;
	(void)StringCchCopyW(wFileName, NUMELMS(wFileName), T2W(strFileName));

	// Add the source filter to the graph.
	hr = m_pGB->AddSourceFilter (wFileName, L"SOURCE", &pFSrc);

	// If the media file was not found, inform the user.
	if (hr == VFW_E_NOT_FOUND)
	{
		Msg(TEXT("Could not add source filter to graph!  (hr==VFW_E_NOT_FOUND)\r\n\r\n")
			TEXT("This sample reads a media file from your windows directory.\r\n")
			TEXT("This file is missing from this machine."));
		return hr;
	}
	else if(FAILED(hr))
	{
		Msg(TEXT("Could not add source filter to graph!  hr=0x%x"), hr);
		return hr;
	}

	if (FAILED(hr = pFSrc->FindPin(L"Output", &pFSrcPinOut)))
	{
		Msg(TEXT("Could not find output pin!  hr=0x%x"), hr);
		return hr;
	}

#ifdef NO_AUDIO_RENDERER

	// If no audio component is desired, directly connect the two video pins
	// instead of allowing the Filter Graph Manager to render all pins.

	CComPtr<IPin> pFTRPinIn;      // Texture Renderer Input Pin

	// Find the source's output pin and the renderer's input pin
	if (FAILED(hr = pFTR->FindPin(L"In", &pFTRPinIn)))
	{
		Msg(TEXT("Could not find input pin!  hr=0x%x"), hr);
		return hr;
	}

	// Connect these two filters
	if (FAILED(hr = m_pGB->Connect(pFSrcPinOut, pFTRPinIn)))
	{
		Msg(TEXT("Could not connect pins!  hr=0x%x"), hr);
		return hr;
	}

#else

	// Render the source filter's output pin.  The Filter Graph Manager
	// will connect the video stream to the loaded CTextureRenderer
	// and will load and connect an audio renderer (if needed).

	if (FAILED(hr = m_pGB->Render(pFSrcPinOut)))
	{
		Msg(TEXT("Could not render source output pin!  hr=0x%x"), hr);
		return hr;
	}

#endif

	// Get the graph's media control, event & position interfaces
	m_pGB->QueryInterface(&m_pMC);
	m_pGB->QueryInterface(&m_pMP);
	m_pGB->QueryInterface(&m_pME);

	// Start the graph running;
	if (FAILED(hr = m_pMC->Run()))
	{
		Msg(TEXT("Could not run the DirectShow graph!  hr=0x%x"), hr);
		return hr;
	}


	return S_OK;
}


//-----------------------------------------------------------------------------
// CheckMovieStatus: If the movie has ended, rewind to beginning
//-----------------------------------------------------------------------------
void CDShowManager::CheckMovieStatus(void)
{
	long lEventCode;
	LONG_PTR lParam1, lParam2;
	HRESULT hr;

	if (!m_pME)
		return;

	// Check for completion events
	hr = m_pME->GetEvent(&lEventCode, &lParam1, &lParam2, 0);
	if (SUCCEEDED(hr))
	{
		// If we have reached the end of the media file, reset to beginning
		if (EC_COMPLETE == lEventCode)
		{
			hr = m_pMP->put_CurrentPosition(0);
		}

		// Free any memory associated with this event
		hr = m_pME->FreeEventParams(lEventCode, lParam1, lParam2);
	}
}


//-----------------------------------------------------------------------------
// CleanupDShow
//-----------------------------------------------------------------------------
void CDShowManager::CleanupDShow(void)
{
#ifdef REGISTER_FILTERGRAPH
	// Pull graph from Running Object Table (Debug)
	RemoveFromROT();
#endif

	// Shut down the graph
	//if (m_pMC) 
	//	m_pMC->Stop();
	//m_pGB->RemoveFilter(m_pRenderer);

	//SAFE_RELEASE(m_pMC);
	//SAFE_RELEASE(m_pME);
	//SAFE_RELEASE(m_pMP);
	//SAFE_RELEASE(m_pGB);

	//SAFE_RELEASE(m_pRenderer);
	if (!(!m_pMC)) m_pMC->Stop();

	if (!(!m_pMC)) m_pMC.Release();
	if (!(!m_pME)) m_pME.Release();
	if (!(!m_pMP)) m_pMP.Release();
	if (!(!m_pGB)) m_pGB.Release();
	if (!(!m_pRenderer)) m_pRenderer.Release();
}