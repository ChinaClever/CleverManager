// clevermanagerDoc.h : CclevermanagerDoc ��Ľӿ�
//


#pragma once


class CclevermanagerDoc : public CDocument
{
protected: // �������л�����
	CclevermanagerDoc();
	DECLARE_DYNCREATE(CclevermanagerDoc)

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
	virtual ~CclevermanagerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


