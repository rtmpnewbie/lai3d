// WorldEditorDoc.cpp : CWorldEditorDoc ���ʵ��
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


// CWorldEditorDoc ����/����

CWorldEditorDoc::CWorldEditorDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CWorldEditorDoc::~CWorldEditorDoc()
{
}

BOOL CWorldEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CWorldEditorDoc ���л�

void CWorldEditorDoc::Serialize(CArchive& ar)
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


// CWorldEditorDoc ���

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


// CWorldEditorDoc ����
