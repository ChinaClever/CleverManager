// OtherDevice.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "clevermanager.h"
#include "OtherDevice.h"


// COtherDevice �Ի���

IMPLEMENT_DYNAMIC(COtherDevice, CDialog)

COtherDevice::COtherDevice(CWnd* pParent /*=NULL*/)
	: CDialog(COtherDevice::IDD, pParent)
	, type(0)
{

}

COtherDevice::~COtherDevice()
{
}

void COtherDevice::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COtherDevice, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// COtherDevice ��Ϣ�������

BOOL COtherDevice::OnEraseBkgnd(CDC* pDC)
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
	BkDc.Rectangle(&rect);

	if(type == 1)
	{
		drawImagePNG(&BkDc,getFilePath()+"\\icon\\cooler.bmp",5,20,rect.Width()-10,rect.Height()-25);
	}
	else if(type == 2)
	{
		drawImagePNG(&BkDc,getFilePath()+"\\icon\\ups.bmp",5,20,rect.Width()-10,rect.Height()-25);
	}
	drawImagePNG(&BkDc,getFilePath()+"\\icon\\close.png",rect.right-20,5,12,12);

	////////////////��ʾ���豸///////////////////////
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&BkDc,0,0,SRCCOPY);
	BkDc.DeleteDC();
	//memdc.DeleteDC();
	return true;

	//return CDialog::OnEraseBkgnd(pDC);
}

void COtherDevice::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rect;
	GetClientRect(&rect);
	if(point.x>rect.right-20 && point.x<rect.right && point.y>0 && point.y<20)
	{
		::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
	}

	CDialog::OnMouseMove(nFlags, point);
}

void COtherDevice::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rect;
	GetClientRect(&rect);
	if(point.x>rect.right-20 && point.x<rect.right && point.y>0 && point.y<20)
	{
		OnOK();
	}

	CDialog::OnLButtonDown(nFlags, point);
}
