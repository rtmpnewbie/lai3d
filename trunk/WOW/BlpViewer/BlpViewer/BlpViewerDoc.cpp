// BlpViewerDoc.cpp : CBlpViewerDoc ���ʵ��
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


// CBlpViewerDoc ����/����

CBlpViewerDoc::CBlpViewerDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CBlpViewerDoc::~CBlpViewerDoc()
{
}

BOOL CBlpViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CBlpViewerDoc ���л�

void CBlpViewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CBlpViewerDoc ���

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


// CBlpViewerDoc ����
