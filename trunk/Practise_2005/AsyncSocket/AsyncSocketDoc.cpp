// AsyncSocketDoc.cpp : CAsyncSocketDoc ���ʵ��
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


// CAsyncSocketDoc ����/����

CAsyncSocketDoc::CAsyncSocketDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CAsyncSocketDoc::~CAsyncSocketDoc()
{
}

BOOL CAsyncSocketDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CAsyncSocketDoc ���л�

void CAsyncSocketDoc::Serialize(CArchive& ar)
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


// CAsyncSocketDoc ���

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


// CAsyncSocketDoc ����
