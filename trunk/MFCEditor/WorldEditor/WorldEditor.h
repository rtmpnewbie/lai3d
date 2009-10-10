// WorldEditor.h : WorldEditor 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号

class CWorldEditorView;

// CWorldEditorApp:
// 有关此类的实现，请参阅 WorldEditor.cpp
//

class CWorldEditorApp : public CWinApp
{
public:
	CWorldEditorApp();


// 重写
public:
	virtual BOOL InitInstance();

	// lyl begin
	virtual int ExitInstance(); // return app exit code
	virtual int Run();
    // lyl end 

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CWorldEditorApp theApp;

inline CWorldEditorView* GetMainDrawWnd()
{
	extern CWorldEditorView* g_pEditorView;
	return g_pEditorView;
}