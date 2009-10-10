// WorldEditorView.h : CWorldEditorView 类的接口
//


#pragma once


class CWorldEditorView : public CView
{
protected: // 仅从序列化创建
	CWorldEditorView();
	DECLARE_DYNCREATE(CWorldEditorView)

// 属性
public:
	CWorldEditorDoc* GetDocument() const;

// 操作
public:
	void Draw3D();

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CWorldEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // WorldEditorView.cpp 中的调试版本
inline CWorldEditorDoc* CWorldEditorView::GetDocument() const
   { return reinterpret_cast<CWorldEditorDoc*>(m_pDocument); }
#endif

