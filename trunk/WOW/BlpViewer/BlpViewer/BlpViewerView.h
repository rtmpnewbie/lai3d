// BlpViewerView.h : CBlpViewerView ��Ľӿ�
//


#pragma once


class CBlpViewerView : public CView
{
protected: // �������л�����
	CBlpViewerView();
	DECLARE_DYNCREATE(CBlpViewerView)

// ����
public:
	CBlpViewerDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CBlpViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // BlpViewerView.cpp �еĵ��԰汾
inline CBlpViewerDoc* CBlpViewerView::GetDocument() const
   { return reinterpret_cast<CBlpViewerDoc*>(m_pDocument); }
#endif

