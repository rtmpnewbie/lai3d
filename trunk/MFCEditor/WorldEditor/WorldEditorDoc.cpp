// WorldEditorDoc.cpp : CWorldEditorDoc 类的实现
//

#include "stdafx.h"
#include "WorldEditor.h"

#include "WorldEditorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWorldEditorDoc

IMPLEMENT_DYNCREATE(CWorldEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CWorldEditorDoc, CDocument)
END_MESSAGE_MAP()


// CWorldEditorDoc 构造/析构

CWorldEditorDoc::CWorldEditorDoc()
{
	// TODO: 在此添加一次性构造代码

}

CWorldEditorDoc::~CWorldEditorDoc()
{
}

BOOL CWorldEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CWorldEditorDoc 序列化

void CWorldEditorDoc::Serialize(CArchive& ar)
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


// CWorldEditorDoc 诊断

#ifdef _DEBUG
void CWorldEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWorldEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CWorldEditorDoc 命令
