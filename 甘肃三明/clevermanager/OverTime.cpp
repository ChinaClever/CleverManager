// OverTime.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "clevermanager.h"
#include "OverTime.h"
#include "MainFrm.h"

// COverTime �Ի���

IMPLEMENT_DYNAMIC(COverTime, CDialog)

COverTime::COverTime(CWnd* pParent /*=NULL*/)
	: CDialog(COverTime::IDD, pParent)
{

}

COverTime::~COverTime()
{
}

void COverTime::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COverTime, CDialog)
	ON_BN_CLICKED(IDOK, &COverTime::OnBnClickedOk)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// COverTime ��Ϣ�������

void COverTime::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
	AfxGetMainWnd()->SendMessage(WM_CLOSE);
	
}

void COverTime::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(nID==SC_CLOSE)
	{
		OnBnClickedOk();
	}
	CDialog::OnSysCommand(nID, lParam);
}
