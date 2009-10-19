// BlpViewerDoc.cpp : CBlpViewerDoc 类的实现
//

#include "stdafx.h"
#include "BlpViewer.h"

#include "BlpViewerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBlpViewerDoc

IMPLEMENT_DYNCREATE(CBlpViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CBlpViewerDoc, CDocument)
END_MESSAGE_MAP()


// CBlpViewerDoc 构造/析构

CBlpViewerDoc::CBlpViewerDoc()
{
	// TODO: 在此添加一次性构造代码

}

CBlpViewerDoc::~CBlpViewerDoc()
{
}

BOOL CBlpViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CBlpViewerDoc 序列化

void CBlpViewerDoc::Serialize(CArchive& ar)
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


// CBlpViewerDoc 诊断

#ifdef _DEBUG
void CBlpViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBlpViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CBlpViewerDoc 命令
