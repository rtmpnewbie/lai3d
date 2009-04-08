#include <dshow.h>
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	IGraphBuilder *pGraph = NULL;
	IMediaControl *pControl = NULL;
	IMediaEvent   *pEvent = NULL;

	// Initialize the COM library.
	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr))
		{
 			printf("ERROR - Could not initialize COM library");
 			return -1;
 		}

	// Create the filter graph manager and query for interfaces.
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, 
			IID_IGraphBuilder, (void **)&pGraph);
	if (FAILED(hr))
	{
		printf("ERROR - Could not create the Filter Graph Manager.");
		return -1;
	}

	hr = pGraph->QueryInterface(IID_IMediaControl, (void **)&pControl);
	hr = pGraph->QueryInterface(IID_IMediaEvent, (void **)&pEvent);

	// Build the graph. IMPORTANT: Change this string to a file on your system.
	// F:\\Program Files\\NVIDIA Corporation\\SDK 9.5\\MEDIA\\textures\\video\\nvidia2.wmv
	// C:\\Program Files\\Microsoft Platform SDK for Windows Server 2003 R2\\Samples\\Multimedia\\DirectShow\\Media\\Video\\ruby.avi
	hr = pGraph->RenderFile(L"F:\\Program Files\\NVIDIA Corporation\\SDK 9.5\\MEDIA\\textures\\video\\nvidia2.wmv", NULL);
	if (SUCCEEDED(hr))
	{
 		// Run the graph.
 		hr = pControl->Run();
 		if (SUCCEEDED(hr))
		{
 			// Wait for completion.
 			long evCode;
 			pEvent->WaitForCompletion(INFINITE, &evCode);
 
 			// Note: Do not use INFINITE in a real application, because it
 			// can block indefinitely.
 		}
	}
	pControl->Release();
	pEvent->Release();
	pGraph->Release();
	CoUninitialize();

	return 0;
}
