// AsyncSocketDoc.h : CAsyncSocketDoc ��Ľӿ�
//


#pragma once


class CAsyncSocketDoc : public CDocument
{
protected: // �������л�����
	CAsyncSocketDoc();
	DECLARE_DYNCREATE(CAsyncSocketDoc)

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
	virtual ~CAsyncSocketDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


