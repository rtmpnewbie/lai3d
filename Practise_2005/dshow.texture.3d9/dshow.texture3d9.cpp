#include "stdafx.h"

#include "dshow.texture3d9.h"

// Define this if you want to render only the video component with no audio
//
//   #define NO_AUDIO_RENDERER

// An application can advertise the existence of its filter graph
// by registering the graph with a global Running Object Table (ROT).
// The GraphEdit application can detect and remotely view the running
// filter graph, allowing you to 'spy' on the graph with GraphEdit.
//
// To enable registration in this sample, define REGISTER_FILTERGRAPH.
//
#define REGISTER_FILTERGRAPH

// File filter for OpenFile dialog
#define FILE_FILTER_TEXT \
	TEXT("Video Files (*.avi; *.qt; *.mov; *.mpg; *.mpeg; *.m1v)\0*.avi; *.qt; *.mov; *.mpg; *.mpeg; *.m1v\0")\
	TEXT("Audio files (*.wav; *.mpa; *.mp2; *.mp3; *.au; *.aif; *.aiff; *.snd)\0*.wav; *.mpa; *.mp2; *.mp3; *.au; *.aif; *.aiff; *.snd\0")\
	TEXT("MIDI Files (*.mid, *.midi, *.rmi)\0*.mid; *.midi; *.rmi\0") \
	TEXT("Image Files (*.jpg, *.bmp, *.gif, *.tga)\0*.jpg; *.bmp; *.gif; *.tga\0") \
	TEXT("All Files (*.*)\0*.*;\0\0")

//extern HRESULT UpgradeGeometry( LONG lActualW, LONG lTextureW,
//							   LONG lActualH, LONG lTextureH );


#ifdef REGISTER_FILTERGRAPH

//-----------------------------------------------------------------------------
// Running Object Table functions: Used to debug. By registering the graph
// in the running object table, GraphEdit is able to connect to the running
// graph. This code should be removed before the application is shipped in
// order to avoid third parties from spying on your graph.
//-----------------------------------------------------------------------------
DWORD dwROTReg = 0xfedcba98;

HRESULT AddToROT(IUnknown *pUnkGraph)
{
	IMoniker * pmk;
	IRunningObjectTable *pROT;
	if (FAILED(GetRunningObjectTable(0, &pROT))) {
		return E_FAIL;
	}

	WCHAR wsz[256];
	(void)StringCchPrintfW(wsz, NUMELMS(wsz),L"FilterGraph %08x  pid %08x\0", (DWORD_PTR) 0, GetCurrentProcessId());

	HRESULT hr = CreateItemMoniker(L"!", wsz, &pmk);
	if (SUCCEEDED(hr))
	{
		// Use the ROTFLAGS_REGISTRATIONKEEPSALIVE to ensure a strong reference
		// to the object.  Using this flag will cause the object to remain
		// registered until it is explicitly revoked with the Revoke() method.
		//
		// Not using this flag means that if GraphEdit remotely connects
		// to this graph and then GraphEdit exits, this object registration
		// will be deleted, causing future attempts by GraphEdit to fail until
		// this application is restarted or until the graph is registered again.
		hr = pROT->Register(ROTFLAGS_REGISTRATIONKEEPSALIVE, pUnkGraph,
			pmk, &dwROTReg);
		pmk->Release();
	}

	pROT->Release();
	return hr;
}


void RemoveFromROT(void)
{
	IRunningObjectTable *pirot=0;

	if (SUCCEEDED(GetRunningObjectTable(0, &pirot)))
	{
		pirot->Revoke(dwROTReg);
		pirot->Release();
	}
}

#endif


//-----------------------------------------------------------------------------
// Msg: Display an error message box if needed
//-----------------------------------------------------------------------------
void Msg(TCHAR *szFormat, ...)
{
	TCHAR szBuffer[1024];  // Large buffer for long filenames or URLs
	const size_t NUMCHARS = sizeof(szBuffer) / sizeof(szBuffer[0]);
	const int LASTCHAR = NUMCHARS - 1;

	// Format the input string
	va_list pArgs;
	va_start(pArgs, szFormat);

	// Use a bounded buffer size to prevent buffer overruns.  Limit count to
	// character size minus one to allow for a NULL terminating character.
	(void)StringCchVPrintf(szBuffer, NUMCHARS - 1, szFormat, pArgs);
	va_end(pArgs);

	// Ensure that the formatted string is NULL-terminated
	szBuffer[LASTCHAR] = TEXT('\0');

	MessageBox(NULL, szBuffer, TEXT("DirectShow Texture3D9 Sample"),
		MB_OK | MB_ICONERROR);
}




BOOL GetClipFileName(LPTSTR szName)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));


	// Reset filename
	*szName = 0;

	// Fill in standard structure fields
	ofn.lStructSize       = sizeof(OPENFILENAME);
	ofn.hwndOwner         = g_hWnd;
	ofn.lpstrFilter       = FILE_FILTER_TEXT;
	ofn.lpstrCustomFilter = NULL;
	ofn.nFilterIndex      = 1;
	ofn.lpstrFile         = szName;
	ofn.nMaxFile          = MAX_PATH;
	ofn.lpstrTitle        = TEXT("Open Media File...\0");
	ofn.lpstrFileTitle    = NULL;
	ofn.lpstrDefExt       = TEXT("*\0");
	ofn.Flags             = OFN_FILEMUSTEXIST | OFN_READONLY | OFN_PATHMUSTEXIST;


	// Create the standard file open dialog and return its result
	return GetOpenFileName((LPOPENFILENAME)&ofn);
}

