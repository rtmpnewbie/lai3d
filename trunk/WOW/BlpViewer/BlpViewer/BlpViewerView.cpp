// BlpViewerView.cpp : CBlpViewerView 类的实现
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CBlpViewerView 构造/析构

CBlpViewerView::CBlpViewerView()
{
	// TODO: 在此处添加构造代码

}

CBlpViewerView::~CBlpViewerView()
{
}

BOOL CBlpViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CBlpViewerView 绘制

void CBlpViewerView::OnDraw(CDC* /*pDC*/)
{
	CBlpViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CBlpViewerView 打印

BOOL CBlpViewerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CBlpViewerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CBlpViewerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清除过程
}


// CBlpViewerView 诊断

#ifdef _DEBUG
void CBlpViewerView::AssertValid() const
{
	CView::AssertValid();
}

void CBlpViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBlpViewerDoc* CBlpViewerView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBlpViewerDoc)));
	return (CBlpViewerDoc*)m_pDocument;
}
#endif //_DEBUG


// CBlpViewerView 消息处理程序
