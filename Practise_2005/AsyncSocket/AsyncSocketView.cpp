// AsyncSocketView.cpp : CAsyncSocketView 类的实现
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CAsyncSocketView 构造/析构

CAsyncSocketView::CAsyncSocketView()
{
	// TODO: 在此处添加构造代码

}

CAsyncSocketView::~CAsyncSocketView()
{
}

BOOL CAsyncSocketView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CAsyncSocketView 绘制

void CAsyncSocketView::OnDraw(CDC* /*pDC*/)
{
	CAsyncSocketDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CAsyncSocketView 打印

BOOL CAsyncSocketView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CAsyncSocketView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CAsyncSocketView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清除过程
}


// CAsyncSocketView 诊断

#ifdef _DEBUG
void CAsyncSocketView::AssertValid() const
{
	CView::AssertValid();
}

void CAsyncSocketView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAsyncSocketDoc* CAsyncSocketView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAsyncSocketDoc)));
	return (CAsyncSocketDoc*)m_pDocument;
}
#endif //_DEBUG


// CAsyncSocketView 消息处理程序
