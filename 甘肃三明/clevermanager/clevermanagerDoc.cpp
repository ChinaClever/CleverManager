// clevermanagerDoc.cpp : CclevermanagerDoc ���ʵ��
//

#include "stdafx.h"
#include "clevermanager.h"

#include "clevermanagerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CclevermanagerDoc

IMPLEMENT_DYNCREATE(CclevermanagerDoc, CDocument)

BEGIN_MESSAGE_MAP(CclevermanagerDoc, CDocument)
END_MESSAGE_MAP()


// CclevermanagerDoc ����/����

CclevermanagerDoc::CclevermanagerDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CclevermanagerDoc::~CclevermanagerDoc()
{
}

BOOL CclevermanagerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CclevermanagerDoc ���л�

void CclevermanagerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CclevermanagerDoc ���

#ifdef _DEBUG
void CclevermanagerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CclevermanagerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CclevermanagerDoc ����
