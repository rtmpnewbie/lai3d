// BlpViewerDoc.h : CBlpViewerDoc ��Ľӿ�
//


#pragma once


class CBlpViewerDoc : public CDocument
{
protected: // �������л�����
	CBlpViewerDoc();
	DECLARE_DYNCREATE(CBlpViewerDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CBlpViewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


