// BlpViewer.h : BlpViewer Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CBlpViewerApp:
// �йش����ʵ�֣������ BlpViewer.cpp
//

class CBlpViewerApp : public CWinApp
{
public:
	CBlpViewerApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CBlpViewerApp theApp;