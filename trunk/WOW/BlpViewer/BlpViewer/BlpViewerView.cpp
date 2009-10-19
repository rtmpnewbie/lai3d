// BlpViewerView.cpp : CBlpViewerView ���ʵ��
//

#include "stdafx.h"
#include "BlpViewer.h"

#include "BlpViewerDoc.h"
#include "BlpViewerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBlpViewerView

IMPLEMENT_DYNCREATE(CBlpViewerView, CView)

BEGIN_MESSAGE_MAP(CBlpViewerView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CBlpViewerView ����/����

CBlpViewerView::CBlpViewerView()
{
	// TODO: �ڴ˴���ӹ������

}

CBlpViewerView::~CBlpViewerView()
{
}

BOOL CBlpViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CBlpViewerView ����

void CBlpViewerView::OnDraw(CDC* /*pDC*/)
{
	CBlpViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CBlpViewerView ��ӡ

BOOL CBlpViewerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CBlpViewerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CBlpViewerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CBlpViewerView ���

#ifdef _DEBUG
void CBlpViewerView::AssertValid() const
{
	CView::AssertValid();
}

void CBlpViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBlpViewerDoc* CBlpViewerView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBlpViewerDoc)));
	return (CBlpViewerDoc*)m_pDocument;
}
#endif //_DEBUG


// CBlpViewerView ��Ϣ�������
