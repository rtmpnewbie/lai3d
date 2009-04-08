// AsyncSocketView.h : CAsyncSocketView ��Ľӿ�
//


#pragma once


class CAsyncSocketView : public CView
{
protected: // �������л�����
	CAsyncSocketView();
	DECLARE_DYNCREATE(CAsyncSocketView)

// ����
public:
	CAsyncSocketDoc* GetDocument() const;

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
	virtual ~CAsyncSocketView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // AsyncSocketView.cpp �еĵ��԰汾
inline CAsyncSocketDoc* CAsyncSocketView::GetDocument() const
   { return reinterpret_cast<CAsyncSocketDoc*>(m_pDocument); }
#endif

