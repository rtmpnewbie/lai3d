// AsyncSocketView.cpp : CAsyncSocketView ���ʵ��
//

#include "stdafx.h"
#include "AsyncSocket.h"

#include "AsyncSocketDoc.h"
#include "AsyncSocketView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAsyncSocketView

IMPLEMENT_DYNCREATE(CAsyncSocketView, CView)

BEGIN_MESSAGE_MAP(CAsyncSocketView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CAsyncSocketView ����/����

CAsyncSocketView::CAsyncSocketView()
{
	// TODO: �ڴ˴���ӹ������

}

CAsyncSocketView::~CAsyncSocketView()
{
}

BOOL CAsyncSocketView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CAsyncSocketView ����

void CAsyncSocketView::OnDraw(CDC* /*pDC*/)
{
	CAsyncSocketDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CAsyncSocketView ��ӡ

BOOL CAsyncSocketView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CAsyncSocketView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CAsyncSocketView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CAsyncSocketView ���

#ifdef _DEBUG
void CAsyncSocketView::AssertValid() const
{
	CView::AssertValid();
}

void CAsyncSocketView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAsyncSocketDoc* CAsyncSocketView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAsyncSocketDoc)));
	return (CAsyncSocketDoc*)m_pDocument;
}
#endif //_DEBUG


// CAsyncSocketView ��Ϣ�������
