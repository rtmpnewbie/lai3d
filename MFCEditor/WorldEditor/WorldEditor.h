// WorldEditor.h : WorldEditor Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

class CWorldEditorView;

// CWorldEditorApp:
// �йش����ʵ�֣������ WorldEditor.cpp
//

class CWorldEditorApp : public CWinApp
{
public:
	CWorldEditorApp();


// ��д
public:
	virtual BOOL InitInstance();

	// lyl begin
	virtual int ExitInstance(); // return app exit code
	virtual int Run();
    // lyl end 

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CWorldEditorApp theApp;

inline CWorldEditorView* GetMainDrawWnd()
{
	extern CWorldEditorView* g_pEditorView;
	return g_pEditorView;
}