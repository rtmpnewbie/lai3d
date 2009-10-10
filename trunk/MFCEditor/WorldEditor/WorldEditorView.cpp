// WorldEditorView.cpp : CWorldEditorView 类的实现
//

#include "stdafx.h"
#include "WorldEditor.h"

#include "WorldEditorDoc.h"
#include "WorldEditorView.h"

#include "Matrices.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWorldEditorView* g_pEditorView;

// CWorldEditorView

IMPLEMENT_DYNCREATE(CWorldEditorView, CView)

BEGIN_MESSAGE_MAP(CWorldEditorView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CWorldEditorView 构造/析构

CWorldEditorView::CWorldEditorView()
{
	// TODO: 在此处添加构造代码
	g_pEditorView = this;
}

CWorldEditorView::~CWorldEditorView()
{
}

BOOL CWorldEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CWorldEditorView 绘制

void CWorldEditorView::OnDraw(CDC* /*pDC*/)
{
	CWorldEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CWorldEditorView 打印

BOOL CWorldEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CWorldEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CWorldEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清除过程
}


// CWorldEditorView 诊断

#ifdef _DEBUG
void CWorldEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CWorldEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWorldEditorDoc* CWorldEditorView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWorldEditorDoc)));
	return (CWorldEditorDoc*)m_pDocument;
}

#endif //_DEBUG

void CWorldEditorView::Draw3D()
{
	Render();
}

// CWorldEditorView 消息处理程序
