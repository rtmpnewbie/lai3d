// FormatV_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "FormatV_Test.h"

#include <string.h>
#include <atlsimpstr.h>
#include <atlstr.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



void WriteString(LPCWSTR pstrFormat, ...)
{
	CString str;

	// format and write the data you were given
	va_list args;
	va_start(args, pstrFormat);

	str.FormatV(pstrFormat, args);
	va_end(args);

	::wprintf_s(str.GetString());
	return;
}


// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;
int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
		WriteString( _T("%d error(s) found in %d line(s)"), 10, 1351 );

	}

	return nRetCode;
}
