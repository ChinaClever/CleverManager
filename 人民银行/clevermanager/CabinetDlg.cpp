// CabinetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "clevermanager.h"
#include "CabinetDlg.h"


// CCabinetDlg �Ի���

IMPLEMENT_DYNAMIC(CCabinetDlg, CDialog)

CCabinetDlg::CCabinetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCabinetDlg::IDD, pParent)
{

}

CCabinetDlg::~CCabinetDlg()
{
}

void CCabinetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCabinetDlg, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// CCabinetDlg ��Ϣ�������

BOOL CCabinetDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return CDialog::OnEraseBkgnd(pDC);
}

BOOL CCabinetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CCabinetDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	CMenu Popmenu1;
	CMenu* pMenu1;
	CPoint pt;
	::GetCursorPos(&pt);
	Popmenu1.LoadMenu(IDR_CABINET_MENU);
	pMenu1 = Popmenu1.GetSubMenu(0);
	pMenu1->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTALIGN,pt.x,pt.y,this);
	CDialog::OnRButtonDown(nFlags, point);
}
