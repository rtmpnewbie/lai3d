// AsyncSocket.h : AsyncSocket Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CAsyncSocketApp:
// �йش����ʵ�֣������ AsyncSocket.cpp
//

class CAsyncSocketApp : public CWinApp
{
public:
	CAsyncSocketApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CAsyncSocketApp theApp;