// UserTabCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "clevermanager.h"
#include "UserTabCtrl.h"


// CUserTabCtrl

IMPLEMENT_DYNAMIC(CUserTabCtrl, CTabCtrl)

CUserTabCtrl::CUserTabCtrl()
{

}

CUserTabCtrl::~CUserTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CUserTabCtrl, CTabCtrl)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CUserTabCtrl ��Ϣ�������



BOOL CUserTabCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rect;
	GetClientRect(&rect);
	//CDC memdc;
	//memdc.CreateCompatibleDC(pDC);
	////////////////����һ��ͼƬ�Ļ�ͼ����///////////////////////
    CBitmap BkMap;
	BkMap.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CDC BkDc;
	BkDc.CreateCompatibleDC(pDC);
	BkDc.SelectObject(&BkMap);
	/////////////////////////////////
	SetBkMode(BkDc,TRANSPARENT);
    CBrush brush(RGB(255,255,255));
	BkDc.SelectObject(&brush);
	BkDc.SelectStockObject(NULL_PEN);
	rect.bottom += 1;
	rect.right += 1;
	BkDc.Rectangle(&rect);
	////////////////��ʾ���豸///////////////////////
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&BkDc,0,0,SRCCOPY);
	BkDc.DeleteDC();
	//memdc.DeleteDC();
	return true;

	//return CTabCtrl::OnEraseBkgnd(pDC);
}
