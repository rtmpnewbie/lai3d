// MFC_CString_Test.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFC_CString_TestApp:
// �йش����ʵ�֣������ MFC_CString_Test.cpp
//

class CMFC_CString_TestApp : public CWinApp
{
public:
	CMFC_CString_TestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFC_CString_TestApp theApp;