// WorldEditorDoc.h : CWorldEditorDoc ��Ľӿ�
//


#pragma once


class CWorldEditorDoc : public CDocument
{
protected: // �������л�����
	CWorldEditorDoc();
	DECLARE_DYNCREATE(CWorldEditorDoc)

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
	virtual ~CWorldEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


