// AsyncSocketDoc.cpp : CAsyncSocketDoc 类的实现
//

#include "stdafx.h"
#include "AsyncSocket.h"

#include "AsyncSocketDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAsyncSocketDoc

IMPLEMENT_DYNCREATE(CAsyncSocketDoc, CDocument)

BEGIN_MESSAGE_MAP(CAsyncSocketDoc, CDocument)
END_MESSAGE_MAP()


// CAsyncSocketDoc 构造/析构

CAsyncSocketDoc::CAsyncSocketDoc()
{
	// TODO: 在此添加一次性构造代码

}

CAsyncSocketDoc::~CAsyncSocketDoc()
{
}

BOOL CAsyncSocketDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CAsyncSocketDoc 序列化

void CAsyncSocketDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CAsyncSocketDoc 诊断

#ifdef _DEBUG
void CAsyncSocketDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAsyncSocketDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CAsyncSocketDoc 命令
