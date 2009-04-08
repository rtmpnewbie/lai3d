#pragma once

BOOL GetClipFileName(LPTSTR szName);
void Msg(TCHAR *szFormat, ...);

HRESULT AddToROT(IUnknown *pUnkGraph); 
void RemoveFromROT(void);

#define SAFE_RELEASE(p) if(p){ (p)->Release(); (p) = NULL; }