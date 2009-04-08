// FormatV_Test.cpp : 定义控制台应用程序的入口点。
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


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;
int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// 初始化 MFC 并在失败时显示错误
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: MFC 初始化失败\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: 在此处为应用程序的行为编写代码。
		WriteString( _T("%d error(s) found in %d line(s)"), 10, 1351 );

	}

	return nRetCode;
}
