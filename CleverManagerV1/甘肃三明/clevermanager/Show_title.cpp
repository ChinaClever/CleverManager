// Show_title.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "clevermanager.h"
#include "Show_title.h"


// CShow_title �Ի���

IMPLEMENT_DYNAMIC(CShow_title, CDialog)

CShow_title::CShow_title(CWnd* pParent /*=NULL*/)
: CDialog(CShow_title::IDD, pParent)
{

}

CShow_title::~CShow_title()
{
}

void CShow_title::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CShow_title, CDialog)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CShow_title ��Ϣ�������

BOOL CShow_title::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rect;
	GetClientRect(rect);

	CBitmap BkMap;
	BkMap.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CDC BkDc;
	BkDc.CreateCompatibleDC(pDC);
	BkDc.SelectObject(&BkMap);
	SetBkMode(BkDc,TRANSPARENT);
	CBrush brushtitle(RGB(255,255,255));
	BkDc.SelectObject(&brushtitle);
	BkDc.Rectangle(0,0,rect.Width(),rect.Height());
	drawFont(&BkDc,1,RGB(0,0,0));
	BkDc.TextOutA(20,5,"�����б�");
	//retange(&BkDc,1,1,rect.Width()-1,rect.Height());
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&BkDc,0,0,SRCCOPY);
	BkDc.DeleteDC();
	return true;
}
