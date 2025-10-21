// MakeSureDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "clevermanager.h"
#include "MakeSureDlg.h"
#include "InitClever.h"
#include "CanbinObjIcon.h"
#include "InitClever.h"
#include "MainFrm.h"
#include "Show.h"
// CMakeSureDlg �Ի���

IMPLEMENT_DYNAMIC(CMakeSureDlg, CDialog)
CString CMakeSureDlg::username = "";
CMakeSureDlg::CMakeSureDlg(CWnd* pParent /*=NULL*/)
: CDialog(CMakeSureDlg::IDD, pParent)
, order(0)
, DCname(_T(""))
, datercenterNUM(-1)
{
	p_icon = NULL;
	p_list=NULL;
	p_device=NULL;
	p_cabin = NULL;
	p_emsats=NULL;
}

CMakeSureDlg::~CMakeSureDlg()
{
	onok=false;
	onok1=false;
}

void CMakeSureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMakeSureDlg, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()

	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CMakeSureDlg ��Ϣ�������

BOOL CMakeSureDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int i;
	CRect rect;
	GetClientRect(&rect);
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
	//BkDc.SelectStockObject(NULL_PEN);
	BkDc.Rectangle(0,24,rect.Width(),rect.Height());

	CBrush brushtitle(RGB(255,255,255));
	BkDc.SelectObject(&brushtitle);
	BkDc.Rectangle(0,0,rect.Width(),25);

	//BkDc.MoveTo(0,25);
	//BkDc.LineTo(rect.Width(),25);
	retange(&BkDc,1,1,rect.Width()-2,23);

	drawFont(&BkDc,1,RGB(64,64,64));
	BkDc.TextOutA(20,5,"��Ϣȷ��");

	drawFont(&BkDc,0,RGB(64,64,64));
	drawImagePNG(&BkDc,getFilePath()+"\\icon\\close.png",rect.Width()-25,6,12,12);
	CBrush brush1(RGB(240,240,240));
	BkDc.SelectObject(&brush1);
	BkDc.Rectangle(275,170,335,190);
	drawImagePNG(&BkDc,getFilePath()+"\\icon\\open.png",280,173,12,12);
	BkDc.TextOutA(295,175,"ȷ��");

	//////////////////////////////////
	if(order==1)
	{
		//GetDlgItem(IDC_EDIT2)->ShowWindow(0);
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->MoveWindow(60,68,110,18);

		BkDc.TextOutA(20,40,"����һ���µĻ�������");
		BkDc.TextOutA(20,70,"����:");
	}
	else if(order==2)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		BkDc.TextOutA(50,70,"ȷ��ɾ��ѡ�еĻ���������?");
	}
	else if(order==3)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		BkDc.TextOutA(40,60,"�Ƿ����û�����ͼ�༭?");
		BkDc.TextOutA(40,80,"�������˺���Ϣȷ��.");
	}
	else if(order==4)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT2)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->MoveWindow(100,50,120,18);//��������
		GetDlgItem(IDC_EDIT5)->MoveWindow(100,85,120,18);//��NPM
		GetDlgItem(IDC_EDIT3)->MoveWindow(265,85,40,18);//����NPM

		GetDlgItem(IDC_EDIT4)->MoveWindow(100,125,120,18);//����1
		GetDlgItem(IDC_EDIT1)->MoveWindow(265,125,40,18);//����2
		BkDc.TextOutA(20,30,"����д��������NPM����ϸ��Ϣ:");
		BkDc.TextOutA(40,55,"��������:");///��������
		BkDc.TextOutA(20,90,"��ҪNPM��IP:");
		BkDc.TextOutA(230,90,"����:");
		BkDc.TextOutA(20,130,"����NPM��IP:");
		BkDc.TextOutA(230,130,"����:");
		BkDc.TextOutA(20,175,"����(0 ����:����,1-9����:1-9�������)");

		////////////////��ʾ���豸///////////////////////
		pDC->BitBlt(0,0,rect.Width(),rect.Height(),&BkDc,0,0,SRCCOPY);
		BkDc.DeleteDC();
		return true;
	}
	else if(order==5)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->MoveWindow(130,35,120,18);//����
		GetDlgItem(IDC_EDIT3)->MoveWindow(100,95,40,18);//�߶�
		GetDlgItem(IDC_EDIT4)->MoveWindow(260,95,40,18);//λ��
		GetDlgItem(IDC_EDIT6)->MoveWindow(120,65,35,18);//��Ҫ�˿�
		//GetDlgItem(IDC_EDIT5)->MoveWindow(300,65,35,18);//���ö˿�
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		BkDc.TextOutA(20,40,"���ӻ����豸����:");
		BkDc.TextOutA(20,70,"��ҪNPM���˿�:");
		//BkDc.TextOutA(200,70,"����NPM���˿�:");
		BkDc.TextOutA(20,100,"����λ��:"); BkDc.TextOutA(145,100,"U");
		BkDc.TextOutA(180,100,"ռ�ø߶�:");BkDc.TextOutA(305,100,"U");

	}
	else if(order==6)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->MoveWindow(120,75,35,18);
		BkDc.TextOutA(20,50,"ɾ�����ܣ�������");
		BkDc.TextOutA(20,80,"����λ��:");
	}
	else if(order==7)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->ShowWindow(0);
		BkDc.TextOutA(20,70,"ȷ��ɾ��ѡ�е��û���?");
	}
	else if(order==8)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->ShowWindow(0);
		bool admin =false;
		for(i=0;i<20;i++)
		{
			if(user[i].grade && strlen(user[i].name))
			{
				admin=true;
			}
		}
		if(!admin)
		{
			GetDlgItem(IDC_EDIT1)->ShowWindow(0);
			GetDlgItem(IDC_EDIT2)->ShowWindow(0);
			BkDc.TextOutA(20,70,"�����û�.");
			pDC->BitBlt(0,0,rect.Width(),rect.Height(),&BkDc,0,0,SRCCOPY);
			BkDc.DeleteDC();
			return true;
		}
		BkDc.TextOutA(20,70,"�����û�,��ȷ��!");
	}
	else if(order==9 || order==22|| order==35|| order==36)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		BkDc.TextOutA(20,50,"�豸IP��ַ:");
		BkDc.TextOutA(20,80,"Get community:");
		GetDlgItem(IDC_EDIT3)->MoveWindow(120,50,120,18);
		GetDlgItem(IDC_EDIT4)->MoveWindow(120,80,120,18);
	}
	else if(order==10)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		BkDc.TextOutA(20,70,"ɾ��ѡ�е��豸���������û���Ϣȷ��!");
	}
	else if(order==11)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		BkDc.TextOutA(20,70,"�޸�SMTP������Ϣ���������û���Ϣȷ��!");
	}
	else if(order==12)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		BkDc.TextOutA(20,70,"�޸�WEB������Ϣ���������û���Ϣȷ��!");
	}
	else if(order==13)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		if(p_cabin)
		{
			CString detail;
			detail.Format("��ҪNPM�˿�:%s   ״̬:%.1fA (%.1f-%.1f)",
				p_cabin->master->outputname[p_cabin->masterindex][p_cabin->checkoutput]
			,p_cabin->master->curout[p_cabin->masterindex][p_cabin->checkoutput]
			,p_cabin->master->curmin[p_cabin->masterindex][p_cabin->checkoutput]
			,p_cabin->master->curmax[p_cabin->masterindex][p_cabin->checkoutput]);
			BkDc.TextOutA(20,40,detail);
			BkDc.TextOutA(20,70,"����(ON/OFF):");
			BkDc.TextOutA(20,100,"SET community:");
			GetDlgItem(IDC_EDIT3)->MoveWindow(150,68,90,18);
			GetDlgItem(IDC_EDIT4)->MoveWindow(150,98,90,18);
		}
	}
	else if(order==14)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		if(p_cabin)
		{
			CString detail;
			detail.Format("����NPM�˿�:%s   ״̬:%.1fA (%.1f-%.1f)",
				p_cabin->salve->outputname[p_cabin->slaveindex][p_cabin->checkoutput]
			,p_cabin->salve->curout[p_cabin->slaveindex][p_cabin->checkoutput]
			,p_cabin->salve->curmin[p_cabin->slaveindex][p_cabin->checkoutput]
			,p_cabin->salve->curmax[p_cabin->slaveindex][p_cabin->checkoutput]);
			BkDc.TextOutA(20,40,detail);
			BkDc.TextOutA(20,70,"����(ON/OFF):");
			BkDc.TextOutA(20,100,"SET community:");
			GetDlgItem(IDC_EDIT3)->MoveWindow(150,68,90,18);
			GetDlgItem(IDC_EDIT4)->MoveWindow(150,98,90,18);
		}
	}
	else if(order==15)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		if(p_cabin)
		{
			CString detail;
			detail.Format("���յ��쳣��Ϣ:%s",p_cabin->master->trapinformation);
			BkDc.TextOutA(20,50,detail);
			BkDc.TextOutA(20,90,"�Ƿ����SNMP�쳣��Ϣ���������û���Ϣȷ��!");
		}
	}
	else if(order==16)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		if(p_cabin)
		{
			CString detail;
			detail.Format("���յ��쳣��Ϣ:%s",p_cabin->salve->trapinformation);
			BkDc.TextOutA(20,50,detail);
			BkDc.TextOutA(20,90,"�Ƿ����SNMP�쳣��Ϣ���������û���Ϣȷ��!");
		}
	}
	else if(order==17)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		if(p_device)
		{
			if(p_device->ipselect && p_device->slaveselect>=0 && p_device->slaveselect<10 &&
				p_device->ipselect->slaveOffline[p_device->slaveselect] ==2)
			{
				CString detail;
				detail.Format("���յ��쳣��Ϣ:%s",p_device->ipselect->trapinformation);
				BkDc.TextOutA(20,50,detail);
				BkDc.TextOutA(20,90,"�Ƿ����SNMP�쳣��Ϣ���������û���Ϣȷ��!");
			}
		}
	}
	else if(order==18)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		BkDc.TextOutA(20,70,"���ѡ���豸�쳣��Ϣ���������û���Ϣȷ��!");
	}
	else if(order==19)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		BkDc.TextOutA(20,50,"EMS IP:");
		BkDc.TextOutA(20,80,"Get community:");
		GetDlgItem(IDC_EDIT3)->MoveWindow(120,50,120,18);
		GetDlgItem(IDC_EDIT4)->MoveWindow(120,80,120,18);
	}
	else if(order==20)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		BkDc.TextOutA(20,50,"ATS IP:");
		BkDc.TextOutA(20,80,"Get community:");
		GetDlgItem(IDC_EDIT3)->MoveWindow(120,50,120,18);
		GetDlgItem(IDC_EDIT4)->MoveWindow(120,80,120,18);
	}
	else if(order==21||order==41)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		BkDc.TextOutA(20,70,"ɾ��ѡ���豸���������û���Ϣȷ��!");
	}
	else if(order==23)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		BkDc.TextOutA(40,60,"�Ƿ�رջ�����ͼ�༭?");
		BkDc.TextOutA(40,80,"�������˺���Ϣȷ��.");
	}

	else if(order==24)
	{
		CString name;
		int mainnum;
		int slavenum;
		int height;
		int start;
		for(int i=0;i<=12;i++)
		{
			if(p_cabin->p_icon->device_master[i].maininputNum==(p_cabin->checkoutput+1))
			{
				name=p_cabin->p_icon->device_master[i].name;
				mainnum=p_cabin->p_icon->device_master[i].maininputNum;
				slavenum=p_cabin->p_icon->device_master[i].slaveinputNum;
				height=p_cabin->p_icon->device_master[i].height;
				start=p_cabin->p_icon->device_master[i].start;
				mainputnum=p_cabin->p_icon->device_master[i].maininputNum;
			}
		}
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->MoveWindow(130,35,120,18);//����
		this->SetDlgItemTextA(IDC_EDIT7,name);
		GetDlgItem(IDC_EDIT3)->MoveWindow(100,95,40,18);//�߶�
		this->SetDlgItemInt(IDC_EDIT3,start);
		GetDlgItem(IDC_EDIT4)->MoveWindow(260,95,40,18);//λ��
		this->SetDlgItemInt(IDC_EDIT4,height);
		GetDlgItem(IDC_EDIT6)->MoveWindow(120,65,35,18);//��Ҫ�˿�
		this->SetDlgItemInt(IDC_EDIT6,mainnum);
		GetDlgItem(IDC_EDIT5)->MoveWindow(300,65,35,18);//���ö˿�
		this->SetDlgItemInt(IDC_EDIT5,slavenum);

		BkDc.TextOutA(20,40,"���ӻ���:");
		BkDc.TextOutA(20,70,"���˿�:");
		BkDc.TextOutA(200,70,"���˿�:");
		BkDc.TextOutA(20,100,"λ��:"); BkDc.TextOutA(145,100,"U");
		BkDc.TextOutA(180,100,"�߶�:");BkDc.TextOutA(305,100,"U");
	}

	else if(order==25)
	{

		CString name;
		int mainnum;
		int slavenum;
		int height;
		int start;
		for(int i=0;i<=12;i++)
		{
			if(p_cabin->p_icon->device_master[i].maininputNum==(p_cabin->checkoutput+1))
			{
				name=p_cabin->p_icon->device_master[i].name;
				mainnum=p_cabin->p_icon->device_master[i].maininputNum;
				slavenum=p_cabin->p_icon->device_master[i].slaveinputNum;
				height=p_cabin->p_icon->device_master[i].height;
				start=p_cabin->p_icon->device_master[i].start;
				mainputnum=p_cabin->p_icon->device_master[i].maininputNum;
			}
		}
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->MoveWindow(130,35,120,18);//����
		this->SetDlgItemTextA(IDC_EDIT7,name);
		GetDlgItem(IDC_EDIT3)->MoveWindow(100,95,40,18);//�߶�
		this->SetDlgItemInt(IDC_EDIT3,start);
		GetDlgItem(IDC_EDIT4)->MoveWindow(260,95,40,18);//λ��
		this->SetDlgItemInt(IDC_EDIT4,height);
		GetDlgItem(IDC_EDIT6)->MoveWindow(120,65,35,18);//��Ҫ�˿�
		this->SetDlgItemInt(IDC_EDIT6,mainnum);

		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		BkDc.TextOutA(20,40,"���ӻ���:");
		BkDc.TextOutA(20,70,"���˿�:");
		BkDc.TextOutA(20,100,"λ��:"); BkDc.TextOutA(145,100,"U");
		BkDc.TextOutA(180,100,"�߶�:");BkDc.TextOutA(305,100,"U");
	}

	else if(order==26)
	{

		CString name;
		int mainnum;
		int slavenum;
		int height;
		int start;
		for(int i=0;i<=12;i++)
		{
			if(p_cabin->p_icon->device_slave[i].slaveinputNum==(p_cabin->checkoutput+1))
			{
				name=p_cabin->p_icon->device_slave[i].name;
				mainnum=p_cabin->p_icon->device_slave[i].maininputNum;
				slavenum=p_cabin->p_icon->device_slave[i].slaveinputNum;
				height=p_cabin->p_icon->device_slave[i].height;
				start=p_cabin->p_icon->device_slave[i].start;
				slaveputnum=p_cabin->p_icon->device_slave[i].slaveinputNum;
			}
		}
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->MoveWindow(130,35,120,18);//����
		this->SetDlgItemTextA(IDC_EDIT7,name);
		GetDlgItem(IDC_EDIT3)->MoveWindow(100,95,40,18);//�߶�
		this->SetDlgItemInt(IDC_EDIT3,start);
		GetDlgItem(IDC_EDIT4)->MoveWindow(260,95,40,18);//λ��
		this->SetDlgItemInt(IDC_EDIT4,height);
		//GetDlgItem(IDC_EDIT6)->MoveWindow(120,65,35,18);//��Ҫ�˿�
		//this->SetDlgItemInt(IDC_EDIT6,mainnum);
		GetDlgItem(IDC_EDIT5)->MoveWindow(120,65,35,18);//���ö˿�
		this->SetDlgItemInt(IDC_EDIT5,slavenum);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);

		BkDc.TextOutA(20,40,"���ӻ���:");
		//BkDc.TextOutA(20,70,"Main PDU port:");
		BkDc.TextOutA(20,70,"���˿�:");
		BkDc.TextOutA(20,100,"λ��:"); BkDc.TextOutA(145,100,"U");
		BkDc.TextOutA(180,100,"�߶�:");BkDc.TextOutA(305,100,"U");

	}


	else if(order==27)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->MoveWindow(130,35,120,18);//����
		GetDlgItem(IDC_EDIT3)->MoveWindow(100,95,40,18);//�߶�
		GetDlgItem(IDC_EDIT4)->MoveWindow(260,95,40,18);//λ��
		GetDlgItem(IDC_EDIT5)->MoveWindow(120,65,35,18);//���ö˿�

		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		BkDc.TextOutA(20,40,"���ӻ���:");
		BkDc.TextOutA(20,70,"�����˿�:");
		BkDc.TextOutA(20,100,"λ��:"); BkDc.TextOutA(145,100,"U");
		BkDc.TextOutA(180,100,"�߶�:");BkDc.TextOutA(305,100,"U");

	}

	else if(order==28)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->MoveWindow(130,35,120,18);//����
		GetDlgItem(IDC_EDIT3)->MoveWindow(100,95,40,18);//�߶�
		GetDlgItem(IDC_EDIT4)->MoveWindow(260,95,40,18);//λ��
		//GetDlgItem(IDC_EDIT6)->MoveWindow(120,65,35,18);//��Ҫ�˿�
		GetDlgItem(IDC_EDIT5)->MoveWindow(120,65,35,18);//���ö˿�

		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		BkDc.TextOutA(20,40,"���ӻ���:");
		//BkDc.TextOutA(20,70,"Main PDU port:");
		BkDc.TextOutA(20,70,"�����˿�:");
		BkDc.TextOutA(20,100,"λ��:"); BkDc.TextOutA(145,100,"U");
		BkDc.TextOutA(180,100,"�߶�:");BkDc.TextOutA(305,100,"U");

	}
	else if(order==29)
	{
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->MoveWindow(120,75,35,18);
		BkDc.TextOutA(20,50,"ɾ�����ܣ�������");
		BkDc.TextOutA(20,80,"����λ��:");
	}


	else if(order==30)
	{
		POSITION pos = snmpobj.GetTailPosition ();
		while (pos != NULL)
		{
			CSnmpObj *p_Object = NULL;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
			if(p_Object->ipaddr==pre_ip)
			{
				GetDlgItem(IDC_EDIT8)->ShowWindow(0);
				GetDlgItem(IDC_EDIT9)->ShowWindow(0);
				GetDlgItem(IDC_EDIT5)->ShowWindow(0);
				GetDlgItem(IDC_EDIT6)->ShowWindow(0);
				GetDlgItem(IDC_EDIT7)->ShowWindow(0);
				BkDc.TextOutA(20,50,"�豸 IP:");
				BkDc.TextOutA(20,80,"Get community:");
				GetDlgItem(IDC_EDIT3)->MoveWindow(120,50,120,18);
				this->SetDlgItemTextA(IDC_EDIT3,p_Object->ipaddr);
				GetDlgItem(IDC_EDIT4)->MoveWindow(120,80,120,18);
				this->SetDlgItemTextA(IDC_EDIT4,p_Object->community);
				break;
			}
		}
	}

	else if(order==31)
	{
		POSITION pos = snmpobj.GetTailPosition ();
		while (pos != NULL)
		{
			CSnmpObj *p_Object = NULL;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
			if(p_Object->ipaddr==pre_ip)
			{
				GetDlgItem(IDC_EDIT8)->ShowWindow(0);
				GetDlgItem(IDC_EDIT9)->ShowWindow(0);
				GetDlgItem(IDC_EDIT5)->ShowWindow(0);
				GetDlgItem(IDC_EDIT6)->ShowWindow(0);
				GetDlgItem(IDC_EDIT7)->ShowWindow(0);
				BkDc.TextOutA(20,50,"�豸 IP:");
				BkDc.TextOutA(20,80,"Get community:");
				GetDlgItem(IDC_EDIT3)->MoveWindow(120,50,120,18);
				this->SetDlgItemTextA(IDC_EDIT3,p_Object->ipaddr);
				GetDlgItem(IDC_EDIT4)->MoveWindow(120,80,120,18);
				this->SetDlgItemTextA(IDC_EDIT4,p_Object->community);
				break;
			}
		}
	}

	else if(order==32)
	{
		POSITION pos = snmpobj.GetTailPosition ();
		while (pos != NULL)
		{
			CSnmpObj *p_Object = NULL;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
			if(p_Object->ipaddr==pre_ip)
			{
				GetDlgItem(IDC_EDIT8)->ShowWindow(0);
				GetDlgItem(IDC_EDIT9)->ShowWindow(0);
				GetDlgItem(IDC_EDIT5)->ShowWindow(0);
				GetDlgItem(IDC_EDIT6)->ShowWindow(0);
				GetDlgItem(IDC_EDIT7)->ShowWindow(0);
				BkDc.TextOutA(20,50,"�豸 IP:");
				BkDc.TextOutA(20,80,"Get community:");
				GetDlgItem(IDC_EDIT3)->MoveWindow(120,50,120,18);
				this->SetDlgItemTextA(IDC_EDIT3,p_Object->ipaddr);
				GetDlgItem(IDC_EDIT4)->MoveWindow(120,80,120,18);
				this->SetDlgItemTextA(IDC_EDIT4,p_Object->community);
				break;
			}
		}
	}

	else if(order==33||order==40)
	{
		POSITION pos = snmpobj.GetTailPosition ();
		while (pos != NULL)
		{
			CSnmpObj *p_Object = NULL;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
			if(p_Object->ipaddr==pre_ip)
			{
				GetDlgItem(IDC_EDIT8)->ShowWindow(0);
				GetDlgItem(IDC_EDIT9)->ShowWindow(0);
				GetDlgItem(IDC_EDIT5)->ShowWindow(0);
				GetDlgItem(IDC_EDIT6)->ShowWindow(0);
				GetDlgItem(IDC_EDIT7)->ShowWindow(0);
				BkDc.TextOutA(20,50,"�豸 IP:");
				BkDc.TextOutA(20,80,"Get community:");
				GetDlgItem(IDC_EDIT3)->MoveWindow(120,50,120,18);
				this->SetDlgItemTextA(IDC_EDIT3,p_Object->ipaddr);
				GetDlgItem(IDC_EDIT4)->MoveWindow(120,80,120,18);
				this->SetDlgItemTextA(IDC_EDIT4,p_Object->community);
				break;
			}
		}
	}

	else if(order==34)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT2)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->MoveWindow(100,50,120,18);//��������
		this->SetDlgItemTextA(IDC_EDIT7,this->cabinet_name);
		GetDlgItem(IDC_EDIT5)->MoveWindow(100,85,120,18);//��NPM
		this->SetDlgItemTextA(IDC_EDIT5,this->ip_master);
		GetDlgItem(IDC_EDIT4)->MoveWindow(265,85,40,18);//����NPM
		this->SetDlgItemTextA(IDC_EDIT3,this->ip_slave);
		GetDlgItem(IDC_EDIT3)->MoveWindow(100,125,120,18);//����1
		this->SetDlgItemInt(IDC_EDIT4,this->slave_masternum);
		GetDlgItem(IDC_EDIT1)->MoveWindow(265,125,40,18);//����2
		this->SetDlgItemInt(IDC_EDIT1,this->slave_slavenum);
		BkDc.TextOutA(20,30,"����д�޸�������NPM����ϸ��Ϣ:");
		BkDc.TextOutA(40,55,"��������:");///��������
		BkDc.TextOutA(20,90,"��ҪNPM��IP:");
		BkDc.TextOutA(230,90,"����:");
		BkDc.TextOutA(20,130,"����NPM��IP:");
		BkDc.TextOutA(230,130,"����:");
		BkDc.TextOutA(20,175,"����(0 ����:����,1-9����:1-9�������)");

		////////////////��ʾ���豸///////////////////////
		pDC->BitBlt(0,0,rect.Width(),rect.Height(),&BkDc,0,0,SRCCOPY);
		BkDc.DeleteDC();
		return true;
	}
	else if(order==37)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		BkDc.TextOutA(20,50,"IP-PDU IP:");
		BkDc.TextOutA(20,80,"Get community:");
		GetDlgItem(IDC_EDIT3)->MoveWindow(120,50,120,18);
		GetDlgItem(IDC_EDIT4)->MoveWindow(120,80,120,18);
	}

	else if(order==38)
	{
		POSITION pos = snmpobj.GetTailPosition ();
		while (pos != NULL)
		{
			CSnmpObj *p_Object = NULL;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
			if(p_Object->ipaddr==pre_ip)
			{
				GetDlgItem(IDC_EDIT8)->ShowWindow(0);
				GetDlgItem(IDC_EDIT9)->ShowWindow(0);
				GetDlgItem(IDC_EDIT5)->ShowWindow(0);
				GetDlgItem(IDC_EDIT6)->ShowWindow(0);
				GetDlgItem(IDC_EDIT7)->ShowWindow(0);
				BkDc.TextOutA(20,50,"�豸 IP:");
				BkDc.TextOutA(20,80,"Get community:");
				GetDlgItem(IDC_EDIT3)->MoveWindow(120,50,120,18);
				this->SetDlgItemTextA(IDC_EDIT3,p_Object->ipaddr);
				GetDlgItem(IDC_EDIT4)->MoveWindow(120,80,120,18);
				this->SetDlgItemTextA(IDC_EDIT4,p_Object->community);
				break;
			}
		}
	}
	else if(order==39)
	{
		POSITION pos = snmpobj.GetTailPosition ();
		while (pos != NULL)
		{
			CSnmpObj *p_Object = NULL;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
			if(p_Object->ipaddr==pre_ip)
			{
				GetDlgItem(IDC_EDIT8)->ShowWindow(0);
				GetDlgItem(IDC_EDIT9)->ShowWindow(0);
				GetDlgItem(IDC_EDIT5)->ShowWindow(0);
				GetDlgItem(IDC_EDIT6)->ShowWindow(0);
				GetDlgItem(IDC_EDIT7)->ShowWindow(0);
				BkDc.TextOutA(20,50,"�豸 IP:");
				BkDc.TextOutA(20,80,"Get community:");
				GetDlgItem(IDC_EDIT3)->MoveWindow(120,50,120,18);
				this->SetDlgItemTextA(IDC_EDIT3,p_Object->ipaddr);
				GetDlgItem(IDC_EDIT4)->MoveWindow(120,80,120,18);
				this->SetDlgItemTextA(IDC_EDIT4,p_Object->community);
				break;
			}
		}
	}
	else if(order==50)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->MoveWindow(130,35,120,18);//����
		GetDlgItem(IDC_EDIT3)->MoveWindow(100,95,40,18);//�߶�
		GetDlgItem(IDC_EDIT4)->MoveWindow(260,95,40,18);//λ��
		GetDlgItem(IDC_EDIT6)->MoveWindow(120,65,35,18);//��Ҫ�˿�
		//GetDlgItem(IDC_EDIT5)->MoveWindow(300,65,35,18);//���ö˿�
		GetDlgItem(IDC_EDIT5)->MoveWindow(300,65,35,18);
		BkDc.TextOutA(20,40,"���ӻ����豸����:");
		BkDc.TextOutA(20,70,"��ҪNPM���˿�:");
		BkDc.TextOutA(180,70,"����NPM���˿�:");
		//BkDc.TextOutA(200,70,"����NPM���˿�:");
		BkDc.TextOutA(20,100,"����λ��:"); BkDc.TextOutA(145,100,"U");
		BkDc.TextOutA(180,100,"ռ�ø߶�:");BkDc.TextOutA(305,100,"U");

	}
	else if(order==51)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->MoveWindow(120,75,35,18);
		BkDc.TextOutA(20,50,"ɾ��˫��������豸");
		BkDc.TextOutA(20,80,"�豸�ڻ���λ��:");
	}
	else if(order==52)
	{
		GetDlgItem(IDC_EDIT7)->MoveWindow(130,35,120,18);//����
		GetDlgItem(IDC_EDIT3)->MoveWindow(100,105,40,18);//�߶�
		GetDlgItem(IDC_EDIT4)->MoveWindow(260,105,40,18);//λ��
		GetDlgItem(IDC_EDIT6)->MoveWindow(120,60,35,18);//��Ҫ�˿�
		//GetDlgItem(IDC_EDIT5)->MoveWindow(300,65,35,18);//���ö˿�
		GetDlgItem(IDC_EDIT5)->MoveWindow(280,60,35,18);
		BkDc.TextOutA(20,40,"���ӻ����豸����:");
		GetDlgItem(IDC_EDIT9)->MoveWindow(120,85,35,18);
		GetDlgItem(IDC_EDIT8)->MoveWindow(280,85,35,18);
		BkDc.TextOutA(20,65,"��ҪNPM���˿�1:");
		BkDc.TextOutA(20,85,"��ҪNPM���˿�2:");
		BkDc.TextOutA(180,65,"����NPM���˿�1:");
		BkDc.TextOutA(180,90,"����NPM���˿�2:");
		//BkDc.TextOutA(200,70,"����NPM���˿�:");
		BkDc.TextOutA(20,110,"����λ��:"); BkDc.TextOutA(145,110,"U");
		BkDc.TextOutA(180,110,"ռ�ø߶�:");BkDc.TextOutA(305,110,"U");

	}
	else if(order==53)
	{
		GetDlgItem(IDC_EDIT8)->ShowWindow(0);
		GetDlgItem(IDC_EDIT9)->ShowWindow(0);
		GetDlgItem(IDC_EDIT4)->ShowWindow(0);
		GetDlgItem(IDC_EDIT5)->ShowWindow(0);
		GetDlgItem(IDC_EDIT6)->ShowWindow(0);
		GetDlgItem(IDC_EDIT7)->ShowWindow(0);
		GetDlgItem(IDC_EDIT3)->MoveWindow(120,75,35,18);
		BkDc.TextOutA(20,50,"ɾ����·��������豸");
		BkDc.TextOutA(20,80,"�豸�ڻ���λ��:");
	}
	BkDc.TextOutA(20,130,"�˺�:");
	BkDc.TextOutA(175,130,"����:");
	////////////////��ʾ���豸///////////////////////
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&BkDc,0,0,SRCCOPY);
	BkDc.DeleteDC();
	return true;

	//return CDialog::OnEraseBkgnd(pDC);
}

void CMakeSureDlg::setOrder(int value)
{
	order = value;
}

void CMakeSureDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(point.x>275 && point.x<335 && point.y>170 && point.y<190)
	{
		::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
	}
	if(point.x>320 && point.x<355 && point.y>0 && point.y<25)
	{
		::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CMakeSureDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(point.x>275 && point.x<335 && point.y>170 && point.y<190)
	{
		::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));//���ȷ������ڵ�
		OnOK();

	}
	if(point.x>320 && point.x<355 && point.y>0 && point.y<25)
	{
		onok1=true;
		OnOK();
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CMakeSureDlg::makesure(void)
{
	int i;
	if(order==1)//��������
	{
		CString dc;
		GetDlgItem(IDC_EDIT7)->GetWindowTextA(dc);
		if(dc.GetLength()!=NULL)
		{
			for(i=0;i<10;i++)
			{
				if(DCmember[i].name.GetLength()==0)
				{
					//CString username;
					if(cheakUser()==0)
					{
						AfxMessageBox("�û��˺ź����벻ƥ��!");
						return;
					}
					else if(cheakUser()==3)
					{
						AfxMessageBox("���û�û��Ȩ��!");
						return;
					}
					else if(cheakUser()==1)
					{
						GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
					}
					else if(cheakUser()==2)
					{
						username="����Ա";
					}


					int j;
					for(j=0;j<10;j++)
					{
						if(DCmember[j].name == dc && dc.GetLength())
						{
							AfxMessageBox("�û����Ѿ������б���!");
							return;
						}
					}
					DCmember[i].name = dc;
					CInitClever writedata;
					writedata.dataCenterWrite();
					writeUserLog(1,1,username,"��������","���ӻ���("+DCmember[i].name+")������������.");
					onok=true;
					CShow show;
					show.AddRoom();
					return;
				}

			}
			AfxMessageBox("������������̫��!");
		}
		else
		{
			AfxMessageBox("����д��������");
		}
	}
	else if(order==2)//ɾ������
	{
		for(i=0;i<10;i++)
		{
			if(DCmember[i].name==DCname && DCmember[i].name.GetLength())
			{
				//CString username;
				CString jifang =  DCmember[i].name;
				if(cheakUser()==0)
				{
					AfxMessageBox("�û��˺ź����벻ƥ��!");
					return;
				}
				else if(cheakUser()==3)
				{
					AfxMessageBox("���û�û��Ȩ��!");
					return;
				}
				else if(cheakUser()==1)
				{
					GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
				}
				else if(cheakUser()==2)
				{
					username="����Ա";
				}

				int num=0;
				while(DCmember[num].name.GetLength())
				{
					num++;
				}

				POSITION pos = DCmember[i].canbin.GetHeadPosition ();
				while (pos != NULL)
					delete DCmember[i].canbin.GetNext (pos);
				DCmember[i].canbin.RemoveAll ();
				DCmember[i].name="";
				DCmember[i].str_draw="";
				DCname="";

				for(;i<(num-1);i++)
				{
					DCmember[i].name=DCmember[i+1].name;
					DCmember[i].str_draw=DCmember[i+1].str_draw;
				}
				DCmember[num-1].name="";
				DCmember[num-1].str_draw="";
				DCname="";

				CInitClever writedata;
				writedata.dataCenterWrite();
				writeUserLog(1,1,username,"ɾ������","�ӻ���������ɾ������("+jifang+")��");
				CShow show;
				show.AddRoom();
				onok=true;
				//OnOK();
				return;
			}
		}
	}
	else if(order==3)//��������
	{
		if(p_map)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("�û��˺ź����벻ƥ��!");
				return;
			}
			else if(cheakUser()==3)
			{
				AfxMessageBox("���û�û��Ȩ��!");
				return;
			}
			else if(cheakUser()==1)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="����Ա";
			}

			if(p_map->mapEdit)
			{
				p_map->mapEdit=false;
				writeUserLog(1,2,username,"�༭��������","�رջ������˱༭����.");
			}
			else
			{
				p_map->mapEdit=true;
				writeUserLog(1,2,username,"�༭��������","�����������˱༭����.");
			}
			onok=true;
			//OnOK();
			return;
		}
	}
	else if(order==4)//���ӻ���
	{
		CString ip1,ip2,num1,num2,cabname;
		GetDlgItem(IDC_EDIT5)->GetWindowTextA(ip1);
		GetDlgItem(IDC_EDIT3)->GetWindowTextA(num1);
		GetDlgItem(IDC_EDIT4)->GetWindowTextA(ip2);
		GetDlgItem(IDC_EDIT1)->GetWindowTextA(num2);
		GetDlgItem(IDC_EDIT7)->GetWindowTextA(cabname);
		bool device1=false;
		bool device2=false;
		///////////////////
		POSITION pos = snmpobj.GetTailPosition ();

		while (pos != NULL)
		{
			CSnmpObj *p_Object = NULL;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
			if(p_Object->ipaddr == ip1 && ip1.GetLength() && p_Object->dtype==0)
			{
				int deviceindex1 = atoi(num1);
				if(deviceindex1>=0 && deviceindex1<=9)
				{
					if(p_Object->slavename[deviceindex1].GetLength())
					{
						device1=true;
					}
				}
			}
			if(p_Object->ipaddr == ip2 && ip2.GetLength() && p_Object->dtype==0)
			{
				int deviceindex2 = atoi(num2);
				if(deviceindex2>=0 && deviceindex2<=9)
				{
					if(p_Object->slavename[deviceindex2].GetLength())
					{
						device2=true;
					}
				}
			}
		}
		if(datercenterNUM<0)
		{
			MessageBox("��ѡ����һ���������˵�ͼ.");
			return;
		}
		if(!device1 && ip1.GetLength()&&num1.GetLength()&&atoi(num1)>=0 && atoi(num1)<=9&& ip2.GetLength())
		{
			if(MessageBox( "��ȷ����������ҪNPM�Ѿ���NPM�豸�б���.", "", MB_OKCANCEL) != IDOK) 
				return;
		}
		if(!device2 && ip2.GetLength()&&num2.GetLength()&&atoi(num2)>=0 && atoi(num2)<=9)
		{
			if(MessageBox( "��ȷ������������NPM�Ѿ���NPM�豸�б���.", "", MB_OKCANCEL) != IDOK) 
				return;
		}
		///////////////////
		CCanbinObjIcon* p_icon=NULL;
		if(ip1.GetLength() && num1.GetLength())
		{
			if(atoi(num1)>=0 && atoi(num1)<=9&&!ip2.GetLength())
			{
				p_icon = new CCanbinObjIcon(1,1,0,ip1,ip2,atoi(num1),atoi(num2),0);
				p_icon->cabientname=cabname;
				//p_icon->DCname=DCmember[datercenterNUM].name;
				CString detail;
				detail.Format("�ڻ���(%s)�У�����һ��NPM,��ҪNPM IP:%s->%s,����NPM IP:%s->%s",DCmember[datercenterNUM].name,ip1,num1,ip2,num2);
				writeUserLog(1,3,username,"���ӻ���",detail);
				
				onok=true;
			}
			else if(atoi(num1)>=0 && atoi(num1)<=9&&ip2.GetLength())
			{
				if(atoi(num2)>=0 && atoi(num2)<=9)
				{
					p_icon = new CCanbinObjIcon(1,1,0,ip1,ip2,atoi(num1),atoi(num2),0);
					p_icon->cabientname=cabname;
					//p_icon->DCname=DCmember[datercenterNUM].name;
					CString detail;
					detail.Format("�ڻ���(%s)�У�����һ��NPM,��ҪNPM IP:%s->%s,����NPM IP:%s->%s",DCmember[datercenterNUM].name,ip1,num1,ip2,num2);
					writeUserLog(1,3,username,"���ӻ���",detail);
					onok=true;
				}
				else
				{
					AfxMessageBox("���õ�NPM����ȡֵ��Χ��0-9��0Ϊ������1-9Ϊ����,����ȷ��д");
				}
			}
			else
			{
				AfxMessageBox("��Ҫ��NPM����ȡֵ��Χ��0-9��0Ϊ������1-9Ϊ����,����ȷ��д");
			}
		}
		else if(ip1.GetLength() && !num1.GetLength())
		{
			AfxMessageBox("����д����������ȡֵ��Χ��0-9��0Ϊ������1-9Ϊ����,����ȷ��д");
		}

		else
		{
			AfxMessageBox("����д�豸IP�͸���������ȡֵ��Χ��0-9��0Ϊ������1-9Ϊ����,����ȷ��д");
		}
		if(p_icon && datercenterNUM>=0 && datercenterNUM<10)
		{
			p_icon->select=true;
			p_icon->move=true;
			DCmember[datercenterNUM].canbin.AddTail(p_icon);
			
		}
		return;
	}
	else if(order==5)//����������
	{
		if(cheakUser()==0)
		{
			AfxMessageBox("�û��˺ź����벻ƥ��!");
			return;
		}
		else if(cheakUser()==3)
		{
			AfxMessageBox("���û�û��Ȩ��!");
			return;
		}
		else if(cheakUser()==2)
		{
			AfxMessageBox("��������һ���û�!");
			return;
		}
		/////////////
		CString devicename;
		CString num1;
		CString num2;
		CString num3;
		CString num4;
		int mNum,Height,Start;
		GetDlgItem(IDC_EDIT7)->GetWindowTextA(devicename);
		GetDlgItem(IDC_EDIT3)->GetWindowTextA(num1);//5
		GetDlgItem(IDC_EDIT4)->GetWindowTextA(num2);//3
		GetDlgItem(IDC_EDIT6)->GetWindowTextA(num3);//2

		Start=atoi(num1);
		Height=atoi(num2);
		mNum=atoi(num3);

		int m=0;
		POSITION pos = snmpobj.GetTailPosition ();
		while (pos != NULL)
		{
			CSnmpObj *p_Object = NULL;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));

			if(p_Object->ipaddr==p_icon->ipMaster)
			{
				while(1)
				{
					if(p_Object->outputname[p_icon->MasterNUM][m].GetLength())
					{
						m++;
					}
					else
						break;
				}
			}
		}
		int ipm=m+1;
		if(p_icon)
		{
			for(i=0;i<24;i++)
			{
				if(!p_icon->device_master[i].name.GetLength())
				{
					if(p_icon->ipMaster.GetLength())
					{
						if( atoi(num3) > 0 && atoi(num3) < ipm)
						{
							p_icon->device_master[i].maininputNum = atoi(num3);
						}
						else
						{
							CString str;
							str.Format("���˿�ȡֵ��1-%d������ȷ��д",m);
							AfxMessageBox(str);
							return;
						}
					}
					if(atoi(num1)>50||atoi(num1)<0)
					{
						CString str;
						str.Format("�豸λ��ȡֵ��1-50������ȷ��д");
						AfxMessageBox(str);
						return;
					}
					p_icon->device_master[i].name = devicename;
					p_icon->device_master[i].start = atoi(num1);
					p_icon->device_master[i].height = atoi(num2);
					//OnOK();
					onok=true;
					return;
				}
			}
		}
		return;
	}
	else if(order==6)//ɾ��������
	{
		if(cheakUser()==0)
		{
			AfxMessageBox("�û��˺ź����벻ƥ��!");
			return;
		}
		else if(cheakUser()==3)
		{
			AfxMessageBox("���û�û��Ȩ��!");
			return;
		}
		else if(cheakUser()==2)
		{
			AfxMessageBox("��������һ���û�!");
			return;
		}

		CString num1;
		GetDlgItem(IDC_EDIT3)->GetWindowTextA(num1);
		if(atoi(num1)>0&&atoi(num1)<51)
		{
			if(p_icon)
			{
				for(i=0;i<24;i++)
				{
					if(p_icon->device_master[i].start == atoi(num1))
					{
						p_icon->device_master[i].name = "";
						p_icon->device_master[i].start = 0;
						p_icon->device_master[i].height = 0;
						p_icon->device_master[i].maininputNum = 0;
						p_icon->device_master[i].slaveinputNum = 0;
					}
				}
			}
			//OnOK();
			onok=true;
		}
		else
		{
			AfxMessageBox("����д��ȷ�Ļ���λ��");
		}
		return;
	}
	else if(order==7)//ɾ���û�
	{
		if(p_list)
		{
			//CString username;
			CString name = p_list->m_list.GetItemText(p_list->m_list.GetSelectionMark(),1);
			if(cheakUser()==0)
			{
				AfxMessageBox("�û��˺ź����벻ƥ��!");
				return;
			}
			else if(cheakUser()==3)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
				if( username != name)
				{
					AfxMessageBox("���û�û��Ȩ��!");
					return;
				}
			}
			else if(cheakUser()==1)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="����Ա";
			}
			if(name.GetLength())
			{
				for(i=0;i<20;i++)
				{
					if(name == user[i].name)
					{
						memset(user[i].name,0,sizeof(char)*32);
						memset(user[i].psd,0,sizeof(char)*32);
						memset(user[i].mail,0,sizeof(char)*64);
						memset(user[i].detail,0,sizeof(char)*128);
						memset(user[i].tel,0,sizeof(char)*20);
						user[i].grade = 0;
					}
				}
				CInitClever init;
				init.userWrite();
				p_list->m_list.DeleteItem(p_list->m_list.GetSelectionMark());
				writeUserLog(1,4,username,"ɾ���û� ",username+"ɾ�����û�"+name+".");
			}
		}
		//OnOK();
		onok=true;
		return;
	}
	else if(order==8)//�����û�
	{
		if(p_list)
		{
			//CString username;
			CString name;
			CString psd;
			CString mail;
			CString tel;
			CString detail;
			CString grade;

			p_list->GetDlgItem(IDC_USER_PSD)->GetWindowTextA(psd);
			p_list->GetDlgItem(IDC_USER_TEL)->GetWindowTextA(tel);
			p_list->GetDlgItem(IDC_USER_MAIL)->GetWindowTextA(mail);
			p_list->m_userEdit.GetWindowTextA(grade);
			p_list->GetDlgItem(IDC_USER_DETAIL)->GetWindowTextA(detail);

			p_list->GetDlgItem(IDC_USER_COUNT)->GetWindowTextA(name);
			if(name.GetLength()<17&&name.GetLength()>0)
			{

				if(psd.GetLength()<17&&psd.GetLength()>0)
				{


					if(mail.GetLength()!=0)
					{
						bool admin =false;
						for(i=0;i<20;i++)
						{
							if(user[i].grade && strlen(user[i].name))
							{
								admin=true;
							}
						}
						if(admin)
						{
							if(cheakUser()==0)
							{
								AfxMessageBox("�û��˺ź����벻ƥ��!");
								return;
							}
							else if(cheakUser()==3)
							{
								GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
								if( name != username)
								{
									AfxMessageBox("���û�û��Ȩ��!");
									return;
								}
							}
							else if(cheakUser()==1)
							{
								GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
							}
							else if(cheakUser()==2)
							{
								username="����Ա";
							}
						}



						int usernum = -1;
						bool exist = false;
						admin =false;
						for(i=0;i<20;i++)
						{
							if(name == user[i].name && name.GetLength())
							{
								exist =true;
								usernum = i;
							}
							if(user[i].grade && strlen(user[i].name))
							{
								admin=true;
							}
						}
						for(i=0;i<20;i++)
						{
							if(strlen(user[i].name) == 0)
							{
								break;
							}
						}
						if(i>=20)
						{
							//OnOK();
							onok=true;
							AfxMessageBox("�����û�����̫��.");
							return;
						}
						if(!exist && name.GetLength() && psd.GetLength())
						{
							strncpy_s(user[i].name,sizeof(char)*32,name,sizeof(char)*30);
							strncpy_s(user[i].psd,sizeof(char)*32,psd,sizeof(char)*30);
							strncpy_s(user[i].mail,sizeof(char)*64,mail,sizeof(char)*62);
							strncpy_s(user[i].tel,sizeof(char)*32,tel,sizeof(char)*30);
							strncpy_s(user[i].detail,sizeof(char)*128,detail,sizeof(char)*126);

							CInitClever init;
							if(!admin)
							{
								p_list->m_userEdit.SetWindowTextA("���Ա༭");
								user[i].grade = 1;

								init.userWrite();

								p_list->m_list.InsertItem(LVIF_TEXT|LVIF_STATE|LVIF_IMAGE,0,"",0,LVIS_SELECTED,0,0);
								p_list->m_list.SetItemText(0,1,user[i].name);
								p_list->m_list.SetItemText(0,2,"Ȩ�� :���Ա༭");
								CString listtel;
								listtel.Format("�绰:%s",user[i].tel);
								p_list->m_list.SetItemText(0,3,listtel);
								p_list->m_list.SetItemText(0,4,user[i].mail);
								writeUserLog(1,4,username,"�����û�",username+"�����û����û���:"+user[i].name+".");
								//OnOK();
								onok=true;
								return;
							}
							else if(grade == "���Ա༭")
							{
								user[i].grade = 1;
							}
							init.userWrite();
							p_list->m_list.InsertItem(LVIF_TEXT|LVIF_STATE|LVIF_IMAGE,0,"",0,LVIS_SELECTED,0,0);
							p_list->m_list.SetItemText(0,1,user[i].name);
							if(user[i].grade == 1)
							{
								p_list->m_list.SetItemText(0,2,"Ȩ��:���Ա༭");
							}
							else
							{
								p_list->m_list.SetItemText(0,2,"Ȩ��:���ɱ༭");
							}
							CString listtel;
							listtel.Format("�绰:%s",user[i].tel);
							p_list->m_list.SetItemText(0,3,listtel);
							p_list->m_list.SetItemText(0,4,user[i].mail);
							writeUserLog(1,4,username,"�����û�",username+"�����û����û���:"+user[i].name+".");
							onok=true;
						}
						else if(exist && username.GetLength())
						{
							memset(user[usernum].name,0,sizeof(char)*32);
							memset(user[usernum].psd,0,sizeof(char)*32);
							memset(user[usernum].mail,0,sizeof(char)*64);
							memset(user[usernum].tel,0,sizeof(char)*32);
							memset(user[usernum].detail,0,sizeof(char)*128);
							strncpy_s(user[usernum].name,sizeof(char)*32,name,sizeof(char)*30);
							strncpy_s(user[usernum].psd,sizeof(char)*32,psd,sizeof(char)*30);
							strncpy_s(user[usernum].mail,sizeof(char)*64,mail,sizeof(char)*62);
							strncpy_s(user[usernum].tel,sizeof(char)*32,tel,sizeof(char)*30);
							strncpy_s(user[usernum].detail,sizeof(char)*128,detail,sizeof(char)*126);
							CInitClever init;
							/*if(cheakUser()==1)
							{
							if(grade == "Edit")
							{
							user[usernum].grade = 1;
							}
							else
							{
							user[usernum].grade = 0;
							admin = false;
							for(i=0;i<20;i++)
							{
							if(user[i].grade && strlen(user[i].name))
							{
							admin=true;
							}
							}
							if(!admin)
							{
							user[usernum].grade = 1;
							}
							}
							}*/
							init.userWrite();
							int len=p_list->m_list.GetItemCount();
							for(i=0;i<len;i++)
							{
								if(p_list->m_list.GetItemText(i,1) == name)
								{
									/*if(cheakUser()==1)
									{
									if(user[usernum].grade == 1)
									{
									p_list->m_list.SetItemText(i,2,"Ȩ��:Edit");
									}
									else
									{
									p_list->m_list.SetItemText(i,2,"Ȩ��:Unable edit");
									}
									}*/
									CString listtel;
									listtel.Format("�绰:%s",user[usernum].tel);
									p_list->m_list.SetItemText(i,3,listtel);
									p_list->m_list.SetItemText(i,4,user[usernum].mail);
								}
							}
							writeUserLog(1,4,username,"�༭�û�",username+"�����û�����Ϣ���޸�.");
							onok=true;
						}
						else
						{
							AfxMessageBox("�û���Ϣ���벻��ȷ.");
						}
					}
					else
					{
						AfxMessageBox("e-mail����Ϊ��");
					}
				}
				else
				{
					AfxMessageBox("����ĳ�����1-16");
				}

			}

			else
			{
				AfxMessageBox("�˺ŵĳ�����1-16");
			}
		}
		//OnOK();
		return;
	}
	else if(order==9 || order==22||order==35||order==36)//npm
	{
		if(p_device)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("�û��˺ź����벻ƥ��!");
				return;
			}
			else if(cheakUser()==3)
			{
				AfxMessageBox("���û�û��Ȩ��!");
				return;
			}
			else if(cheakUser()==1)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="����Ա";
			}
			CString ip;
			CString com;
			GetDlgItem(IDC_EDIT3)->GetWindowTextA(ip);
			GetDlgItem(IDC_EDIT4)->GetWindowTextA(com);
			if(ip.GetLength()!=0)
			{
				if(com=="public")
				{
					if(ip.GetLength()  &&  com.GetLength())
					{
						bool flags = false;
						POSITION pos = snmpobj.GetTailPosition ();
						while (pos != NULL)
						{
							CSnmpObj *p_Object = NULL;
							p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
							if(p_Object->ipaddr==ip)
							{
								flags = true;
								break;
							}
						}
						if(flags)
						{
							//OnOK();
							onok=true;
							return;
						}
						if( order == 9)
						{
							snmpobj.AddTail(new CSnmpObj(ip,com,0));
							p_device->m_tree.InsertItem(ip,1,1,p_device->NPMIII);
							CInitClever sure;
							sure.deviceListWrite();
							writeUserLog(1,5,username,"�����豸",username+" ������һ�����豸,���豸��IP:"+ip);
							onok=true;
						}
						else if(order == 22)
						{
							snmpobj.AddTail(new CSnmpObj(ip,com,0,1));
							p_device->m_tree.InsertItem(ip,1,1,p_device->NPMIV);
							CInitClever sure;
							sure.NPMIVdevicewrite();
							writeUserLog(1,5,username,"�����豸",username+" ������һ�����豸,���豸��IP:"+ip);
							onok=true;
						}
						else if(order == 35)
						{
							snmpobj.AddTail(new CSnmpObj(ip,com,0,2));
							p_device->m_tree.InsertItem(ip,1,1,p_device->NPMV);
							CInitClever sure;
							sure.NPMVdevicewrite();
							writeUserLog(1,5,username,"�����豸",username+" ������һ�����豸,���豸��IP:"+ip);
							onok=true;
						}
						else if(order == 36)
						{
							snmpobj.AddTail(new CSnmpObj(ip,com,0,3));
							p_device->m_tree.InsertItem(ip,1,1,p_device->MSPDU);
							CInitClever sure;
							sure.MSPDUdevicewrite();
							writeUserLog(1,5,username,"�����豸",username+" ������һ�����豸,���豸��IP:"+ip);
							onok=true;
						}
					}
				}
				else
				{
					AfxMessageBox("Get community Ϊpublic������ȷ��д");
				}
			}
			else
			{
				AfxMessageBox("ip����Ϊ��");
			}
		}
		return;
	}
	else if(order==10)
	{
		if(p_device)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("�û��˺ź����벻ƥ��!");
				return;
			}
			else if(cheakUser()==3)
			{
				AfxMessageBox("���û�û��Ȩ��!");
				return;
			}
			else if(cheakUser()==1)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="����Ա";
			}
			CString name = p_device->m_tree.GetItemText(p_device->m_tree.GetSelectedItem());
			if(name.GetLength())
			{
				POSITION pos = snmpobj.GetTailPosition ();
				while (pos != NULL)
				{
					CSnmpObj *p_Object = NULL;
					p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
					if(p_Object->ipaddr == name)
					{
						p_Object->ipaddr.Empty();
						p_device->m_tree.DeleteItem(p_device->m_tree.GetSelectedItem());
						CInitClever sure;
						sure.deviceListWrite();
						sure.NPMIVdevicewrite();
						sure.NPMVdevicewrite();
						sure.MSPDUdevicewrite();
						writeUserLog(1,5,username,"ɾ���豸",username+" ɾ����һ���豸,���豸 IP:"+name);
						onok=true;
					}
				}
			}
			else
			{
				AfxMessageBox("��ѡ��һ���豸.");
			}
		}
		//OnOK();
		return;
	}
	else if(order==11)//smtp
	{
		if(p_list)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("�û��˺ź����벻ƥ��!");
				return;
			}
			else if(cheakUser()==3)
			{
				AfxMessageBox("���û�û��Ȩ��!");
				return;
			}
			else if(cheakUser()==1)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="����Ա";
			}
			char smtpuser[128];
			char psd[128];
			char smtpserver[128];
			char port[128];
			char type[128];
			memset(smtpuser,0,sizeof(char)*128);
			memset(psd,0,sizeof(char)*128);
			memset(smtpserver,0,sizeof(char)*128);
			memset(port,0,sizeof(char)*128);
			memset(type,0,sizeof(char)*128);
			CString value;
			p_list->GetDlgItem(IDC_SMTP_USER)->GetWindowTextA(value);
			strncpy_s(smtpuser,sizeof(char)*128,value,sizeof(char)*126);
			value.Empty();
			p_list->GetDlgItem(IDC_SMTP_PSD)->GetWindowTextA(value);
			strncpy_s(psd,sizeof(char)*128,value,sizeof(char)*126);
			value.Empty();
			p_list->GetDlgItem(IDC_SMTP_SERVER)->GetWindowTextA(value);
			strncpy_s(smtpserver,sizeof(char)*128,value,sizeof(char)*126);
			value.Empty();
			p_list->GetDlgItem(IDC_SMTP_PORT)->GetWindowTextA(value);
			strncpy_s(port,sizeof(char)*128,value,sizeof(char)*126);

			value.Empty();
			p_list->m_mailCom.GetWindowTextA(value);
			strncpy_s(type,sizeof(char)*128,value,sizeof(char)*126);

			CFile file;
			file.Open(getFilePath()+"\\Uinfo\\mail",CFile::modeReadWrite|CFile::modeCreate);
			file.SeekToBegin();

			file.Write(smtpuser,128);
			file.Seek(128,CFile::begin);
			file.Write(psd,128);
			file.Seek(128*2,CFile::begin);
			file.Write(smtpserver,128);
			file.Seek(128*3,CFile::begin);
			file.Write(port,128);
			file.Seek(128*4,CFile::begin);
			file.Write(type,128);

			file.Close();
			writeUserLog(1,6,username,"STMP����༭",username+"�޸���SMTP��������.");

		}
		//OnOK();
		onok=true;
		return;
	}
	else if(order==12)//�޸�WEB����
	{
		if(p_list)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("�û��˺ź����벻ƥ��!");
				return;
			}
			else if(cheakUser()==3)
			{
				AfxMessageBox("���û�û��Ȩ��!");
				return;
			}
			else if(cheakUser()==1)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="����Ա";
			}
			char port[32];
			char timer[32];
			char start[32];
			memset(port,0,sizeof(char)*32);
			memset(timer,0,sizeof(char)*32);
			memset(start,0,sizeof(char)*32);
			CString value;
			p_list->GetDlgItem(IDC_WEB_PORT)->GetWindowTextA(value);
			strncpy_s(port,sizeof(char)*32,value,sizeof(char)*30);

			value.Empty();
			p_list->GetDlgItem(IDC_WEB_TIME)->GetWindowTextA(value);
			strncpy_s(timer,sizeof(char)*32,value,sizeof(char)*30);

			value.Empty();
			p_list->m_webCom.GetWindowTextA(value);
			strncpy_s(start,sizeof(char)*32,value,sizeof(char)*30);

			CFile file;
			file.Open(getFilePath()+"\\Uinfo\\web",CFile::modeReadWrite|CFile::modeCreate);
			file.SeekToBegin();
			file.Write(port,32);
			file.Seek(32,CFile::begin);
			file.Write(timer,32);
			file.Seek(32*2,CFile::begin);
			file.Write(start,32);
			file.Close();
			writeUserLog(1,6,username,"WEB����༭",username+"�޸���WEB��������.");
			MessageBox("��������������޸ĵ��趨��Ч.");
			onok=true;
			

		}
		//OnOK();

		return;
	}
	else if(order==13)//�������λ����
	{
		if(p_cabin)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("�û��˺ź����벻ƥ��!");
				return;
			}
			else if(cheakUser()==3)
			{
				AfxMessageBox("���û�û��Ȩ��!");
				return;
			}
			else if(cheakUser()==1)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="����Ա";
			}
			CInitClever init;
			CString order;
			GetDlgItem(IDC_EDIT3)->GetWindowTextA(order);
			CString setcom;
			GetDlgItem(IDC_EDIT4)->GetWindowTextA(setcom);
			if(order=="ON"||order=="on"||order=="off"||order=="OFF")
			{
				if(setcom=="private")
				{
					if(order.GetLength())
					{
						init.snmpset(p_cabin->master->ipaddr,p_cabin->masterindex,p_cabin->checkoutput,order
							,p_cabin->master->community,setcom);
					}
					CString detail;
					detail.Format("%s������\"%s\" ��������豸(%s->%d)�����λ%d.",username,order ,p_cabin->master->ipaddr
						,p_cabin->masterindex,p_cabin->checkoutput+1);
					writeUserLog(1,6,username,"���ؿ���",detail);
					//OnOK();
					onok=true;
				}
				else
				{
					AfxMessageBox("Set community ��private������ȷ��д");
				}
			}
			else
			{
				AfxMessageBox("����ȷ��д���ؿ���");
			}
		}

		return;
	}
	else if(order==14)
	{
		if(p_cabin)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("�û��˺ź����벻ƥ��!");
				return;
			}
			else if(cheakUser()==3)
			{
				AfxMessageBox("���û�û��Ȩ��!");
				return;
			}
			else if(cheakUser()==1)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="����Ա";
			}
			CInitClever init;
			CString order;
			GetDlgItem(IDC_EDIT3)->GetWindowTextA(order);
			CString setcom;
			GetDlgItem(IDC_EDIT4)->GetWindowTextA(setcom);
			if(order.GetLength())
			{
				init.snmpset(p_cabin->salve->ipaddr,p_cabin->slaveindex,p_cabin->checkoutput,order
					,p_cabin->salve->community,setcom);
			}
			CString detail;
			detail.Format("%s������\"%s\"��������豸(%s->%d) ���λ%d.",username,order,p_cabin->salve->ipaddr
				,p_cabin->slaveindex,p_cabin->checkoutput+1);
			writeUserLog(1,6,username,"���ؿ���",detail);
		}
		onok=true;
		//OnOK();
		return;
	}
	else if(order==15)
	{
		if(p_cabin)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("�û��˺ź����벻ƥ��!");
				return;
			}
			else if(cheakUser()==1 || cheakUser()==3 )
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="����Ա";
			}
			p_cabin->master->slaveOffline[p_cabin->masterindex] = 1;
			CString detail;
			detail.Format("%s����豸(%s->%d)����״̬.",username,p_cabin->master->ipaddr,p_cabin->masterindex);
			writeUserLog(1,7,username,"�������",detail);
		}
		//OnOK();
		onok=true;
		return;
	}
	else if(order==16)
	{
		if(p_cabin)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("�û��˺ź����벻ƥ��!");
				return;
			}
			else if(cheakUser()==1 || cheakUser()==3)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="����Ա";
			}
			p_cabin->salve->slaveOffline[p_cabin->slaveindex] = 1;
			CString detail;
			detail.Format("%s����豸(%s->%d)����״̬.",username,p_cabin->salve->ipaddr,p_cabin->slaveindex);
			writeUserLog(1,7,username,"�������",detail);
		}
		//OnOK();
		onok=true;
		return;
	}
	else if(order==17)
	{
		if(p_device)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("�û��˺ź����벻ƥ��!");
				return;
			}
			else if(cheakUser()==1 || cheakUser()==3)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="����Ա";
			}
			if(p_device->ipselect && p_device->slaveselect>=0 && p_device->slaveselect<10 &&
				p_device->ipselect->slaveOffline[p_device->slaveselect] ==2)
			{
				p_device->ipselect->slaveOffline[p_device->slaveselect] = 1;
				p_device->ipselect->status=1;
				CString detail;
				detail.Format("%s����豸(%s->%d)����״̬.",username,p_device->ipselect->ipaddr,p_device->slaveselect);
				writeUserLog(1,7,username,"�������",detail);
			}
		}
		//OnOK();
		onok=true;
		return;
	}
	else if(order==18)
	{
		if(p_emsats)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("�û��˺ź����벻ƥ��!");
				return;
			}
			else if(cheakUser()==1 || cheakUser()==3)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="����Ա";
			}
			if(p_emsats->p_select)
			{
				if(p_emsats->p_select->dtype==1)
				{
					if(p_emsats->p_select->EMSstatus[p_emsats->index]==0)
						p_emsats->p_select->EMSstatus[p_emsats->index]=0;
					else
						p_emsats->p_select->EMSstatus[p_emsats->index]=1;
				}
				else if(p_emsats->p_select->dtype==3)
				{
					if(p_emsats->p_select->ipStatus==0)
						p_emsats->p_select->ipStatus=0;
					else
						p_emsats->p_select->ipStatus=1;
				}
				else
				{
					if(p_emsats->p_select->ATS_status==0)
						p_emsats->p_select->ATS_status=0;
					else
						p_emsats->p_select->ATS_status=1;
				}
				CString detail;
				detail.Format("%s����豸(%s->%s)����״̬.",username,p_emsats->p_select->ipaddr
					,p_emsats->p_select->EMSname[p_emsats->index]);
				writeUserLog(1,7,username,"�������",detail);
			}
		}
		//OnOK();
		onok=true;
		return;
	}
	else if(order==19)
	{
		if(p_emsats)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("�û��˺ź����벻ƥ��!");
				return;
			}
			else if(cheakUser()==1 || cheakUser()==3)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="����Ա";
			}
			CString emsip;
			CString emscom;
			GetDlgItem(IDC_EDIT3)->GetWindowTextA(emsip);
			GetDlgItem(IDC_EDIT4)->GetWindowTextA(emscom);
			if(emsip.GetLength()!=0)
			{
				if(emscom=="public")
				{
					if(emsip.GetLength()  &&  emscom.GetLength())
					{
						bool flags = false;
						POSITION pos = snmpobj.GetTailPosition ();
						while (pos != NULL)
						{
							CSnmpObj *p_Object = NULL;
							p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
							if(p_Object->ipaddr==emsip)
							{
								flags = true;
								break;
							}
						}
						if(flags)
						{
							//OnOK();
							onok=true;
							return;
						}
						if(p_emsats->ems)
						{
							snmpobj.AddTail(new CSnmpObj(emsip,emscom,1));
							p_emsats->m_tree.InsertItem(emsip,3,3,p_emsats->ems);
							CInitClever sure;
							sure.atsemsListWrite();
							writeUserLog(1,5,username,"�����豸",username+"������һ���µ��豸�����豸IP:"+emsip);
							onok=true;
						}
					}
					//OnOK();
				}
				else
				{
					AfxMessageBox("Get communityΪpublic������ȷ��д");
				}
			}
			else
			{
				AfxMessageBox("�豸IP����Ϊ��");
			}
		}

		return;
	}
	else if(order==20)
	{
		if(p_emsats)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("�û��˺ź����벻ƥ��!");
				return;
			}
			else if(cheakUser()==1 || cheakUser()==3)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="����Ա";
			}
			CString atsip;
			CString atscom;
			GetDlgItem(IDC_EDIT3)->GetWindowTextA(atsip);
			GetDlgItem(IDC_EDIT4)->GetWindowTextA(atscom);
			if(atsip.GetLength()!=0)
			{
				if(atscom=="public")
				{
					if(atsip.GetLength()  &&  atscom.GetLength())
					{
						bool flags = false;
						POSITION pos = snmpobj.GetTailPosition ();
						while (pos != NULL)
						{
							CSnmpObj *p_Object = NULL;
							p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
							if(p_Object->ipaddr==atsip)
							{
								flags = true;
								break;
							}
						}
						if(flags)
						{
							//OnOK();
							onok=true;
							return;
						}
						if(p_emsats->ems)
						{
							snmpobj.AddTail(new CSnmpObj(atsip,atscom,2));
							p_emsats->m_tree.InsertItem(atsip,3,3,p_emsats->ats);
							CInitClever sure;
							sure.atsemsListWrite();
							writeUserLog(1,5,username,"�����豸",username+"������һ���µ��豸�����豸IP:"+atsip);
							onok=true;
						}
					}
					//OnOK();
				}
				else
				{
					AfxMessageBox("Get community ��public������ȷ��д");
				}
			}
			else
			{
				AfxMessageBox("�豸IP����Ϊ��");
			}
		}

		return;
	}
	else if(order==21||order==41)
	{
		if(p_emsats)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("�û��˺ź����벻ƥ��!");
				return;
			}
			else if(cheakUser()==1 || cheakUser()==3)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="����Ա";
			}
			if(p_emsats->m_tree.GetParentItem(p_emsats->m_tree.GetSelectedItem())==p_emsats->ems
				|| p_emsats->m_tree.GetParentItem(p_emsats->m_tree.GetSelectedItem())==p_emsats->ats
				|| p_emsats->m_tree.GetParentItem(p_emsats->m_tree.GetSelectedItem())==p_emsats->ippdu)
			{
				CString name = p_emsats->m_tree.GetItemText(p_emsats->m_tree.GetSelectedItem());
				if(name.GetLength())
				{
					POSITION pos = snmpobj.GetTailPosition ();
					while (pos != NULL)
					{
						CSnmpObj *p_Object = NULL;
						p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
						if(p_Object->ipaddr == name)
						{
							if(order==21)
							{
								p_Object->ipaddr.Empty();
								p_emsats->m_tree.DeleteItem(p_emsats->m_tree.GetSelectedItem());
								CInitClever sure;
								sure.atsemsListWrite();
								writeUserLog(1,5,username,"ɾ���豸",username+"ɾ����һ���豸�����豸IP:"+name);
								onok=true;
							}
							else if(order==41)
							{
								p_Object->ipaddr.Empty();
								p_emsats->m_tree.DeleteItem(p_emsats->m_tree.GetSelectedItem());
								CInitClever sure;
								sure.ippduListWrite();
								writeUserLog(1,5,username,"ɾ���豸",username+"ɾ����һ���豸�����豸IP:"+name);
								onok=true;
							}
						}
					}
				}
				else
				{
					AfxMessageBox("��ѡ��һ���豸.");
				}
			}
		}
		//OnOK();
		return;
	}
	if(order==23)
	{
		if(p_map)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("�û��˺ź����벻ƥ��!");
				return;
			}
			else if(cheakUser()==3)
			{
				AfxMessageBox("���û�û��Ȩ��!");
				return;
			}
			else if(cheakUser()==1)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="����Ա";
			}

			if(p_map->mapEdit)
			{
				p_map->mapEdit=false;
				writeUserLog(1,2,username,"�༭��������","�رջ������˱༭����.");
			}
			else
			{
				p_map->mapEdit=true;
				writeUserLog(1,2,username,"�༭��������","�����������˱༭����.");
			}
			//OnOK();
			onok=true;
			return;
		}

	}

	if(order==24)
	{
		//CString username;
		if(cheakUser()==0)
		{
			AfxMessageBox("�û��˺ź����벻ƥ��!");
			return;
		}
		else if(cheakUser()==3)
		{
			AfxMessageBox("���û�û��Ȩ��!");
			return;
		}
		else if(cheakUser()==1)
		{
			GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
		}
		else if(cheakUser()==2)
		{
			username="����Ա";
		}

		if(p_cabin->p_icon)
		{
			for(i=0;i<=12;i++)
			{
				if(p_cabin->p_icon->device_master[i].maininputNum == mainputnum)
				{
					p_cabin->p_icon->device_master[i].name = "";
					p_cabin->p_icon->device_master[i].start = 0;
					p_cabin->p_icon->device_master[i].height = 0;
					p_cabin->p_icon->device_master[i].maininputNum = 0;
					p_cabin->p_icon->device_master[i].slaveinputNum = 0;
				}
			}
		}
		CString name;
		CString mainnum;
		CString slavenum;
		CString height;
		CString start;
		GetDlgItem(IDC_EDIT7)->GetWindowTextA(name);
		GetDlgItem(IDC_EDIT6)->GetWindowTextA(mainnum);
		GetDlgItem(IDC_EDIT5)->GetWindowTextA(slavenum);
		GetDlgItem(IDC_EDIT3)->GetWindowTextA(start);
		GetDlgItem(IDC_EDIT4)->GetWindowTextA(height);

		if(p_cabin->p_icon)
		{
			for(i=0;i<=12;i++)
			{
				if(!p_cabin->p_icon->device_master[i].name.GetLength())
				{
					p_cabin->p_icon->device_master[i].name = name;
					p_cabin->p_icon->device_master[i].start =atoi(start);
					p_cabin->p_icon->device_master[i].height = atoi(height);
					p_cabin->p_icon->device_master[i].maininputNum = atoi(mainnum);
					p_cabin->p_icon->device_master[i].slaveinputNum = atoi(slavenum);
				}
			}
		}
		CString detail;
		detail.Format("�������� %s �滻�豸IP%s",p_cabin->p_icon->cabientname,p_cabin->p_icon->ipMaster);
		writeUserLog(1,5,username,"�滻�豸",detail);

		onok=true;
	}

	else if(order==25)
	{
		//CString username;
		if(cheakUser()==0)
		{
			AfxMessageBox("�û��˺ź����벻ƥ��!");
			return;
		}
		else if(cheakUser()==3)
		{
			AfxMessageBox("���û�û��Ȩ��!");
			return;
		}
		else if(cheakUser()==1)
		{
			GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
		}
		else if(cheakUser()==2)
		{
			username="����Ա";
		}

		if(p_cabin->p_icon)
		{
			for(i=0;i<=12;i++)
			{
				if(p_cabin->p_icon->device_master[i].maininputNum == mainputnum)
				{
					p_cabin->p_icon->device_master[i].name = "";
					p_cabin->p_icon->device_master[i].start = 0;
					p_cabin->p_icon->device_master[i].height = 0;
					p_cabin->p_icon->device_master[i].maininputNum = 0;
					p_cabin->p_icon->device_master[i].slaveinputNum = 0;
				}
			}
		}
		CString name;
		CString mainnum;
		CString slavenum;
		CString height;
		CString start;
		GetDlgItem(IDC_EDIT7)->GetWindowTextA(name);
		GetDlgItem(IDC_EDIT6)->GetWindowTextA(mainnum);
		GetDlgItem(IDC_EDIT5)->GetWindowTextA(slavenum);
		GetDlgItem(IDC_EDIT3)->GetWindowTextA(start);
		GetDlgItem(IDC_EDIT4)->GetWindowTextA(height);

		if(p_cabin->p_icon)
		{
			for(i=0;i<=12;i++)
			{
				if(!p_cabin->p_icon->device_master[i].name.GetLength())
				{
					p_cabin->p_icon->device_master[i].name = name;
					p_cabin->p_icon->device_master[i].start =atoi(start);
					p_cabin->p_icon->device_master[i].height = atoi(height);
					p_cabin->p_icon->device_master[i].maininputNum = atoi(mainnum);
					p_cabin->p_icon->device_master[i].slaveinputNum = atoi(slavenum);
					break;
				}
			}
		}
		CString detail;
		detail.Format("�������� %s �滻�豸IP%s",p_cabin->p_icon->cabientname,p_cabin->p_icon->ipMaster );
		writeUserLog(1,5,username,"�滻�豸",detail);

		onok=true;
	}

	else if(order==26)
	{

		//CString username;
		if(cheakUser()==0)
		{
			AfxMessageBox("�û��˺ź����벻ƥ��!");
			return;
		}
		else if(cheakUser()==3)
		{
			AfxMessageBox("���û�û��Ȩ��!");
			return;
		}
		else if(cheakUser()==1)
		{
			GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
		}
		else if(cheakUser()==2)
		{
			username="����Ա";
		}

		if(p_cabin->p_icon)
		{
			for(i=0;i<=12;i++)
			{
				if(p_cabin->p_icon->device_slave[i].slaveinputNum == slaveputnum)
				{
					p_cabin->p_icon->device_slave[i].name = "";
					p_cabin->p_icon->device_slave[i].start = 0;
					p_cabin->p_icon->device_slave[i].height = 0;
					p_cabin->p_icon->device_slave[i].maininputNum = 0;
					p_cabin->p_icon->device_slave[i].slaveinputNum = 0;
				}
			}
		}
		CString name;
		CString mainnum;
		CString slavenum;
		CString height;
		CString start;
		GetDlgItem(IDC_EDIT7)->GetWindowTextA(name);
		GetDlgItem(IDC_EDIT5)->GetWindowTextA(slavenum);
		GetDlgItem(IDC_EDIT3)->GetWindowTextA(start);
		GetDlgItem(IDC_EDIT4)->GetWindowTextA(height);

		if(p_cabin->p_icon)
		{
			for(i=0;i<=12;i++)
			{
				if(!p_cabin->p_icon->device_slave[i].name.GetLength())
				{
					p_cabin->p_icon->device_slave[i].name = name;
					p_cabin->p_icon->device_slave[i].start =atoi(start);
					p_cabin->p_icon->device_slave[i].height = atoi(height);
					p_cabin->p_icon->device_slave[i].maininputNum = atoi(mainnum);
					p_cabin->p_icon->device_slave[i].slaveinputNum = atoi(slavenum);
					break;
				}
			}
		}
		CString detail;
		detail.Format("�������� %s �滻�豸IP%s",p_cabin->p_icon->cabientname ,p_cabin->p_icon->ipSlave);
		writeUserLog(1,5,username,"�滻�豸",detail);
		onok=true;
	}

	else if(order==27)
	{
		//CString username;
		if(cheakUser()==0)
		{
			AfxMessageBox("�û��˺ź����벻ƥ��!");
			return;
		}
		else if(cheakUser()==3)
		{
			AfxMessageBox("���û�û��Ȩ��!");
			return;
		}
		else if(cheakUser()==1)
		{
			GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
		}
		else if(cheakUser()==2)
		{
			username="����Ա";
		}
		/////////////
		CString devicename;
		CString num1;
		CString num2;
		CString num3;
		CString num4;
		int sNum,Height,Start;
		GetDlgItem(IDC_EDIT7)->GetWindowTextA(devicename);
		GetDlgItem(IDC_EDIT3)->GetWindowTextA(num1);//5
		GetDlgItem(IDC_EDIT4)->GetWindowTextA(num2);//3

		GetDlgItem(IDC_EDIT5)->GetWindowTextA(num4);//1
		Start=atoi(num1);
		Height=atoi(num2);

		sNum=atoi(num4);
		int s=0;

		POSITION pos1 = snmpobj.GetTailPosition ();
		while (pos1 != NULL)
		{
			CSnmpObj *p_Object = NULL;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos1)));

			if(p_Object->ipaddr==p_icon->ipSlave)
			{
				for(int j=0;j<24;j++)
				{
					if(p_Object->outputname[p_icon->SlaveNUM][s].GetLength())
					{
						s++;
					}
					else
						break;

				}
			}
		}


		int ips=s+1;
		if(p_icon)
		{
			for(i=0;i<24;i++)
			{
				if(!p_icon->device_slave[i].name.GetLength())
				{
					if(p_icon->ipSlave.GetLength())
					{
						if( atoi(num4) > 0 && atoi(num4) < ips)
						{
							p_icon->device_slave[i].slaveinputNum= atoi(num4);
						}
						else
						{
							CString str;
							str.Format("�����˿�ȡֵ 1-%d,����ȷ����",s);
							AfxMessageBox(str);
							return;
						}
					}



					if(atoi(num1)>50||atoi(num1)<0)
					{
						CString str;
						str.Format("�豸�߶�Ϊ 1-50,����ȷ����");
						AfxMessageBox(str);
						return;
					}
					p_icon->device_slave[i].name = devicename;
					p_icon->device_slave[i].start = atoi(num1);
					p_icon->device_slave[i].height = atoi(num2);
					//OnOK();
					onok=true;
					return;
				}
			}
		}
		return;
	}
	else if(order==29)
	{
		//CString username;
		if(cheakUser()==0)
		{
			AfxMessageBox("�û��˺ź����벻ƥ��!");
			return;
		}
		else if(cheakUser()==3)
		{
			AfxMessageBox("���û�û��Ȩ��!");
			return;
		}
		else if(cheakUser()==1)
		{
			GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
		}
		else if(cheakUser()==2)
		{
			username="����Ա";
		}
		/////////////
		CString num1;
		GetDlgItem(IDC_EDIT3)->GetWindowTextA(num1);
		if(atoi(num1)>0&&atoi(num1)<51)
		{
			if(p_icon)
			{
				for(i=0;i<24;i++)
				{
					if(p_icon->device_slave[i].start == atoi(num1))
					{
						p_icon->device_slave[i].name = "";
						p_icon->device_slave[i].start = 0;
						p_icon->device_slave[i].height = 0;
						p_icon->device_slave[i].maininputNum = 0;
						p_icon->device_slave[i].slaveinputNum = 0;
					}
				}
			}
			//OnOK();
			onok=true;
		}
		else
		{
			AfxMessageBox("��������ȷ�Ļ���λ��");
		}
		return;
	}
	else if(order==30)//npm_edit
	{
		if(p_device)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("�û��˺ź����벻ƥ��!");
				return;
			}
			else if(cheakUser()==3)
			{
				AfxMessageBox("���û�û��Ȩ��!");
				return;
			}
			else if(cheakUser()==1)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="����Ա";
			}
			CString ip;
			CString com;
			GetDlgItem(IDC_EDIT3)->GetWindowTextA(ip);
			GetDlgItem(IDC_EDIT4)->GetWindowTextA(com);
			if(ip.GetLength()!=0)
			{
				if(com=="public")
				{
					if(ip.GetLength()  &&  com.GetLength())
					{
						bool flags = false;
						POSITION pos = snmpobj.GetTailPosition ();
						while (pos != NULL)
						{
							CSnmpObj *p_Object = NULL;
							p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
							if(p_Object->ipaddr==ip)
							{
								flags = true;
								break;
							}
						}
						if(flags)
						{
							AfxMessageBox("�豸IP�Ѿ����ڣ���������д��");
							onok=true;
							return;
						}
						////del////
						pos = snmpobj.GetTailPosition ();
						while (pos != NULL)
						{
							CSnmpObj *p_Object = NULL;
							p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
							if(p_Object->ipaddr == pre_ip)
							{
								p_Object->ipaddr.Empty();
								p_device->m_tree.DeleteItem(p_device->m_tree.GetSelectedItem());
								//CInitClever sure;
								//sure.deviceListWrite();
								//sure.NPMIVdevicewrite();

								onok=true;
							}
						}
						/////add///
						snmpobj.AddTail(new CSnmpObj(ip,com,0));
						p_device->m_tree.InsertItem(ip,1,1,p_device->NPMIII);
						CInitClever sure;
						sure.deviceListWrite();
						writeUserLog(1,5,username,"�޸��豸",username+" �޸��豸,�豸��IPΪ:"+ip);
						onok=true;
					}
				}
				else
				{
					AfxMessageBox("Get community  λ public,����ȷ��д");
				}
			}
			else
			{
				AfxMessageBox("�豸IP����Ϊ��");
			}
		}
		return;
	}

	else if(order==31)//npm_edit
	{
		if(p_device)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("�û��˺ź����벻ƥ��!");
				return;
			}
			else if(cheakUser()==3)
			{
				AfxMessageBox("���û�û��Ȩ��!");
				return;
			}
			else if(cheakUser()==1)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="����Ա";
			}
			CString ip;
			CString com;
			GetDlgItem(IDC_EDIT3)->GetWindowTextA(ip);
			GetDlgItem(IDC_EDIT4)->GetWindowTextA(com);
			if(ip.GetLength()!=0)
			{
				if(com=="public")
				{
					if(ip.GetLength()  &&  com.GetLength())
					{
						bool flags = false;
						POSITION pos = snmpobj.GetTailPosition ();
						while (pos != NULL)
						{
							CSnmpObj *p_Object = NULL;
							p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
							if(p_Object->ipaddr==ip)
							{
								flags = true;
								break;
							}
						}
						if(flags)
						{
							AfxMessageBox("�豸IP�Ѿ����ڣ���������д��");
							onok=true;
							return;
						}
						////del////
						pos = snmpobj.GetTailPosition ();
						while (pos != NULL)
						{
							CSnmpObj *p_Object = NULL;
							p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
							if(p_Object->ipaddr == pre_ip)
							{
								p_Object->ipaddr.Empty();
								p_device->m_tree.DeleteItem(p_device->m_tree.GetSelectedItem());
								//CInitClever sure;
								//sure.NPMIVdevicewrite();
							}
						}
						/////add///
						snmpobj.AddTail(new CSnmpObj(ip,com,0,1));
						p_device->m_tree.InsertItem(ip,1,1,p_device->NPMIV);
						CInitClever sure;
						sure.NPMIVdevicewrite();
						writeUserLog(1,5,username,"�޸��豸",username+" �޸��豸,�豸�� IPΪ:"+ip);
						onok=true;
					}
				}
				else
				{
					AfxMessageBox("Get community  Ϊ public,����ȷ��д");
				}
			}
			else
			{
				AfxMessageBox("�豸IP����Ϊ��");
			}
		}
		return;
	}

	else if(order==32)
	{
		if(p_emsats)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("�û��˺ź����벻ƥ��!");
				return;
			}
			else if(cheakUser()==3)
			{
				AfxMessageBox("���û�û��Ȩ��!");
				return;
			}
			else if(cheakUser()==1)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="����Ա";
			}
			CString atsip;
			CString atscom;
			GetDlgItem(IDC_EDIT3)->GetWindowTextA(atsip);
			GetDlgItem(IDC_EDIT4)->GetWindowTextA(atscom);
			if(atsip.GetLength()!=0)
			{
				if(atscom=="public")
				{
					if(atsip.GetLength()  &&  atscom.GetLength())
					{
						bool flags = false;
						POSITION pos = snmpobj.GetTailPosition ();
						while (pos != NULL)
						{
							CSnmpObj *p_Object = NULL;
							p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
							if(p_Object->ipaddr==atsip)
							{
								flags = true;
								break;
							}
						}
						if(flags)
						{
							AfxMessageBox("�豸IP�Ѿ����ڣ���������д��");
							onok=true;
							return;
						}
						////del////
						pos = snmpobj.GetTailPosition ();
						while (pos != NULL)
						{
							CSnmpObj *p_Object = NULL;
							p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
							if(p_Object->ipaddr == pre_ip)
							{
								p_Object->ipaddr.Empty();
								p_emsats->m_tree.DeleteItem(p_emsats->m_tree.GetSelectedItem());
								CInitClever sure;
								sure.atsemsListWrite();

							}
						}
						snmpobj.AddTail(new CSnmpObj(atsip,atscom,2));
						p_emsats->m_tree.InsertItem(atsip,3,3,p_emsats->ats);
						CInitClever sure;
						sure.atsemsListWrite();
						writeUserLog(1,5,username,"�޸��豸",username+"�޸��豸,�豸�� IPΪ:"+atsip);
						onok=true;
					}
				}
				else
				{
					AfxMessageBox("Get community Ϊ public,����ȷ��д");
				}
			}
			else
			{
				AfxMessageBox("���벻��Ϊ��");
			}
		}
		return;
	}
	
	else if(order==33||order==40)
	{
		if(p_emsats)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("�û��˺ź����벻ƥ��!");
				return;
			}
			else if(cheakUser()==3)
			{
				AfxMessageBox("���û�û��Ȩ��!");
				return;
			}
			else if(cheakUser()==1)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="����Ա";
			}
			CString emsip;
			CString emscom;
			GetDlgItem(IDC_EDIT3)->GetWindowTextA(emsip);
			GetDlgItem(IDC_EDIT4)->GetWindowTextA(emscom);
			if(emsip.GetLength()!=0)
			{
				if(emscom=="public")
				{
					if(emsip.GetLength()  &&  emscom.GetLength())
					{
						bool flags = false;
						POSITION pos = snmpobj.GetTailPosition ();
						while (pos != NULL)
						{
							CSnmpObj *p_Object = NULL;
							p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
							if(p_Object->ipaddr==emsip)
							{
								flags = true;
								break;
							}
						}
						if(flags)
						{
							AfxMessageBox("�豸IP�Ѿ����ڣ���������д��");
							onok=true;
							return;
						}
						////del////
						pos = snmpobj.GetTailPosition ();
						while (pos != NULL)
						{
							CSnmpObj *p_Object = NULL;
							p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
							if(p_Object->ipaddr == pre_ip)
							{
								p_Object->ipaddr.Empty();
								p_emsats->m_tree.DeleteItem(p_emsats->m_tree.GetSelectedItem());
								//CInitClever sure;
								//sure.atsemsListWrite();

							}
						}
						if(order==33)
						{
							snmpobj.AddTail(new CSnmpObj(emsip,emscom,1));
							p_emsats->m_tree.InsertItem(emsip,3,3,p_emsats->ems);
							CInitClever sure;
							sure.atsemsListWrite();
							writeUserLog(1,5,username,"�޸��豸",username+"�޸��豸,�豸�� IPΪ:"+emsip);
							onok=true;
						}
						else if(order==40)
						{
							snmpobj.AddTail(new CSnmpObj(emsip,emscom,3));
							p_emsats->m_tree.InsertItem(emsip,3,3,p_emsats->ippdu);
							CInitClever sure;
							sure.ippduListWrite();
							writeUserLog(1,5,username,"�޸��豸",username+"�޸��豸,�豸�� IPΪ:"+emsip);
							onok=true;
						}
					}
				}
				else
				{
					AfxMessageBox("Get community Ϊ public,����ȷ��д");
				}
			}
			else
			{
				AfxMessageBox("���벻��Ϊ��");
			}
		}
	}
	else if(order==34)
	{
		//AfxMessageBox("���벻��Ϊ��");
		CString ip_m;
		CString ip_s;
		CString cabinet_n;
		CString ipmaster_n;
		CString ipslave_n;
		GetDlgItem(IDC_EDIT7)->GetWindowTextA(cabinet_n);
		GetDlgItem(IDC_EDIT5)->GetWindowTextA(ip_m);
		GetDlgItem(IDC_EDIT3)->GetWindowTextA(ip_s);
		GetDlgItem(IDC_EDIT4)->GetWindowTextA(ipmaster_n);
		GetDlgItem(IDC_EDIT1)->GetWindowTextA(ipslave_n);
		if(ip_m.GetLength())
		{
			if(atoi(ipmaster_n)>=0&&atoi(ipmaster_n)<10)
			{
				if(ip_s.GetLength())
				{
					if(atoi(ipslave_n)>=0&&atoi(ipslave_n)<10)
					{

					}
					else 
						AfxMessageBox("������ȡֵΪ0-9������ȷ��д��");
				}
			}
			else
				AfxMessageBox("������ȡֵΪ0-9������ȷ��д��");
		}
		
		int i;
		/////////���ƻ���
		for(i=0;i<24;i++)
		{
			if(p_icon->device_master[i].name.GetLength())
			{
				device_master[i].name=p_icon->device_master[i].name;
				device_master[i].maininputNum=p_icon->device_master[i].maininputNum;
				device_master[i].height=p_icon->device_master[i].height;
				device_master[i].start=p_icon->device_master[i].start;
			}
			if(p_icon->device_slave[i].name.GetLength())
			{
				device_slave[i].name=p_icon->device_slave[i].name;
				device_slave[i].slaveinputNum=p_icon->device_slave[i].slaveinputNum;
				device_slave[i].height=p_icon->device_slave[i].height;
				device_slave[i].start=p_icon->device_slave[i].start;
			}
		}
		//////////
		int datercenterNUM_i;
		for(i=0;i<10;i++)
		{
			if(DCname==DCmember[i].name && DCmember[i].name.GetLength())
			{	
				POSITION pos = DCmember[i].canbin.GetHeadPosition (),pos_old=NULL;
				CCanbinObjIcon * p_icon;
				do
				{
					pos_old = pos;
					p_icon = (CCanbinObjIcon*)DCmember[i].canbin.GetNext(pos);
					if(p_icon->select)
					{
						delete p_icon;
						DCmember[i].canbin.RemoveAt(pos_old);
						datercenterNUM_i=i;
					}
				}
				while(pos != NULL);
			}
		}

		//////////��������
		CCanbinObjIcon* p_icon_2=NULL;
		p_icon_2 = new CCanbinObjIcon(1,1,0,ip_m,ip_s,atoi(ipmaster_n),atoi(ipslave_n),0);
		p_icon_2->cabientname=cabinet_n;
		p_icon_2->select=true;
		p_icon_2->move=true;
		DCmember[datercenterNUM_i].canbin.AddTail(p_icon_2);

		////////���ƻ���
		for(i=0;i<24;i++)
		{
			if(device_master[i].name.GetLength())
			{
				p_icon_2->device_master[i].name =device_master[i].name;
				p_icon_2->device_master[i].maininputNum =device_master[i].maininputNum;
				p_icon_2->device_master[i].height =device_master[i].height;
				p_icon_2->device_master[i].start =device_master[i].start;
			}
			if(device_slave[i].name.GetLength())
			{
				p_icon_2->device_slave[i].name =device_slave[i].name;
				p_icon_2->device_slave[i].slaveinputNum =device_slave[i].slaveinputNum;
				p_icon_2->device_slave[i].height =device_slave[i].height;
				p_icon_2->device_slave[i].start =device_slave[i].start;
			}
		}
		CString detail;
		detail.Format("�ڻ���(%s)�У��޸�һ��NPM,��ҪNPM IP:%s->%s,����NPM IP:%s->%s",DCmember[datercenterNUM_i].name,ip_m,ipmaster_n,ip_s,ipslave_n);
		writeUserLog(1,3,username,"���ӻ���",detail);
		
		onok=true;
	}
	
	else if(order==37)
	{
		if(p_emsats)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("�û��˺ź����벻ƥ��!");
				return;
			}
			else if(cheakUser()==1 || cheakUser()==3)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="����Ա";
			}
			CString emsip;
			CString emscom;
			GetDlgItem(IDC_EDIT3)->GetWindowTextA(emsip);
			GetDlgItem(IDC_EDIT4)->GetWindowTextA(emscom);
			if(emsip.GetLength()!=0)
			{
				if(emscom=="public")
				{
					if(emsip.GetLength()  &&  emscom.GetLength())
					{
						bool flags = false;
						POSITION pos = snmpobj.GetTailPosition ();
						while (pos != NULL)
						{
							CSnmpObj *p_Object = NULL;
							p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
							if(p_Object->ipaddr==emsip)
							{
								flags = true;
								break;
							}
						}
						if(flags)
						{
							onok=true;
							return;
						}
						if(p_emsats->ippdu)
						{
							snmpobj.AddTail(new CSnmpObj(emsip,emscom,3));
							p_emsats->m_tree.InsertItem(emsip,3,3,p_emsats->ippdu);
							CInitClever sure;
							sure.ippduListWrite();
							writeUserLog(1,5,username,"�����豸",username+"������һ���µ��豸�����豸IP:"+emsip);
							onok=true;
						}
					}
				}
				else
				{
					AfxMessageBox("Get communityΪpublic������ȷ��д");
				}
			}
			else
			{
				AfxMessageBox("�豸IP����Ϊ��");
			}
		}
		return;
	}


	else if(order==38)
	{
		if(p_device)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("�û��˺ź����벻ƥ��!");
				return;
			}
			else if(cheakUser()==3)
			{
				AfxMessageBox("���û�û��Ȩ��!");
				return;
			}
			else if(cheakUser()==1)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="����Ա";
			}
			CString ip;
			CString com;
			GetDlgItem(IDC_EDIT3)->GetWindowTextA(ip);
			GetDlgItem(IDC_EDIT4)->GetWindowTextA(com);
			if(ip.GetLength()!=0)
			{
				if(com=="public")
				{
					if(ip.GetLength()  &&  com.GetLength())
					{
						bool flags = false;
						POSITION pos = snmpobj.GetTailPosition ();
						while (pos != NULL)
						{
							CSnmpObj *p_Object = NULL;
							p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
							if(p_Object->ipaddr==ip)
							{
								flags = true;
								break;
							}
						}
						if(flags)
						{
							AfxMessageBox("�豸IP�Ѿ����ڣ���������д��");
							onok=true;
							return;
						}
						////del////
						pos = snmpobj.GetTailPosition ();
						while (pos != NULL)
						{
							CSnmpObj *p_Object = NULL;
							p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
							if(p_Object->ipaddr == pre_ip)
							{
								p_Object->ipaddr.Empty();
								p_device->m_tree.DeleteItem(p_device->m_tree.GetSelectedItem());
								//CInitClever sure;
								//sure.NPMIVdevicewrite();
							}
						}
						/////add///
						snmpobj.AddTail(new CSnmpObj(ip,com,0,3));
						p_device->m_tree.InsertItem(ip,1,1,p_device->MSPDU);
						CInitClever sure;
						sure.MSPDUdevicewrite();
						writeUserLog(1,5,username,"�޸��豸",username+" �޸��豸,�豸�� IPΪ:"+ip);
						onok=true;
					}
				}
				else
				{
					AfxMessageBox("Get community  Ϊ public,����ȷ��д");
				}
			}
			else
			{
				AfxMessageBox("�豸IP����Ϊ��");
			}
		}
		return;
	}
	else if(order==39)//npm_edit
	{
		if(p_device)
		{
			//CString username;
			if(cheakUser()==0)
			{
				AfxMessageBox("�û��˺ź����벻ƥ��!");
				return;
			}
			else if(cheakUser()==3)
			{
				AfxMessageBox("���û�û��Ȩ��!");
				return;
			}
			else if(cheakUser()==1)
			{
				GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
			}
			else if(cheakUser()==2)
			{
				username="����Ա";
			}
			CString ip;
			CString com;
			GetDlgItem(IDC_EDIT3)->GetWindowTextA(ip);
			GetDlgItem(IDC_EDIT4)->GetWindowTextA(com);
			if(ip.GetLength()!=0)
			{
				if(com=="public")
				{
					if(ip.GetLength()  &&  com.GetLength())
					{
						bool flags = false;
						POSITION pos = snmpobj.GetTailPosition ();
						while (pos != NULL)
						{
							CSnmpObj *p_Object = NULL;
							p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
							if(p_Object->ipaddr==ip)
							{
								flags = true;
								break;
							}
						}
						if(flags)
						{
							AfxMessageBox("�豸IP�Ѿ����ڣ���������д��");
							onok=true;
							return;
						}
						////del////
						pos = snmpobj.GetTailPosition ();
						while (pos != NULL)
						{
							CSnmpObj *p_Object = NULL;
							p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));
							if(p_Object->ipaddr == pre_ip)
							{
								p_Object->ipaddr.Empty();
								p_device->m_tree.DeleteItem(p_device->m_tree.GetSelectedItem());
								//CInitClever sure;
								//sure.NPMIVdevicewrite();
							}
						}
						/////add///
						snmpobj.AddTail(new CSnmpObj(ip,com,0,2));
						p_device->m_tree.InsertItem(ip,1,1,p_device->NPMV);
						CInitClever sure;
						sure.NPMVdevicewrite();
						writeUserLog(1,5,username,"�޸��豸",username+" �޸��豸,�豸�� IPΪ:"+ip);
						onok=true;
					}
				}
				else
				{
					AfxMessageBox("Get community  Ϊ public,����ȷ��д");
				}
			}
			else
			{
				AfxMessageBox("�豸IP����Ϊ��");
			}
		}
		return;
	}
	else if(order==50)//����������
	{
		if(cheakUser()==0)
		{
			AfxMessageBox("�û��˺ź����벻ƥ��!");
			return;
		}
		else if(cheakUser()==3)
		{
			AfxMessageBox("���û�û��Ȩ��!");
			return;
		}
		else if(cheakUser()==2)
		{
			AfxMessageBox("��������һ���û�!");
			return;
		}
		/////////////
		CString devicename;
		CString num1;
		CString num2;
		CString num3;
		CString num4;
		CString num5;
		int mNum,sNum,Height,Start;
		GetDlgItem(IDC_EDIT7)->GetWindowTextA(devicename);
		GetDlgItem(IDC_EDIT3)->GetWindowTextA(num1);//5
		GetDlgItem(IDC_EDIT4)->GetWindowTextA(num2);//3
		GetDlgItem(IDC_EDIT6)->GetWindowTextA(num3);//2
		GetDlgItem(IDC_EDIT5)->GetWindowTextA(num5);//2

		Start=atoi(num1);
		Height=atoi(num2);
		mNum=atoi(num3);
		sNum=atoi(num5);

		int m=0,s=0;
		POSITION pos = snmpobj.GetTailPosition ();
		while (pos != NULL)
		{
			CSnmpObj *p_Object = NULL;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));

			if(p_Object->ipaddr==p_icon->ipMaster)
			{
				while(1)
				{
					if(p_Object->outputname[p_icon->MasterNUM][m].GetLength())
					{
						m++;
					}
					else
						break;
				}
			}
		}

		pos = snmpobj.GetTailPosition ();
		while (pos != NULL)
		{
			CSnmpObj *p_Object = NULL;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));

			if(p_Object->ipaddr==p_icon->ipSlave)
			{
				while(1)
				{
					if(p_Object->outputname[p_icon->SlaveNUM][s].GetLength())
					{
						s++;
					}
					else
						break;
				}
			}
		}

		int ipm=m+1;
		if(p_icon)
		{
			for(i=0;i<24;i++)
			{
				if(!p_icon->device_master[i].name.GetLength())
				{
					if(p_icon->ipMaster.GetLength())
					{
						if( atoi(num3) > 0 && atoi(num3) < ipm)
						{
							p_icon->device_master[i].maininputNum = atoi(num3);
						}
						else
						{
							CString str;
							str.Format("���˿�ȡֵ��1-%d������ȷ��д",m);
							AfxMessageBox(str);
							return;
						}
					}
					if(atoi(num1)>50||atoi(num1)<0)
					{
						CString str;
						str.Format("�豸λ��ȡֵ��1-50������ȷ��д");
						AfxMessageBox(str);
						return;
					}

					p_icon->device_master[i].name = devicename;
					p_icon->device_master[i].start = atoi(num1);
					p_icon->device_master[i].height = atoi(num2);
					//OnOK();
					break;
					onok=true;
				}
			}
			for(i=0;i<24;i++)
			{
				if(!p_icon->device_slave[i].name.GetLength())
				{
					if(p_icon->ipSlave.GetLength())
					{
						if( atoi(num5) > 0 && atoi(num5) < s+1)
						{
							p_icon->device_slave[i].slaveinputNum = atoi(num5);
						}
						else
						{
							CString str;
							str.Format("���ö˿�ȡֵ��1-%d������ȷ��д",s);
							AfxMessageBox(str);
							return;
						}
					}
					if(atoi(num1)>50||atoi(num1)<0)
					{
						CString str;
						str.Format("�豸λ��ȡֵ��1-50������ȷ��д");
						AfxMessageBox(str);
						return;
					}


					p_icon->device_slave[i].name = devicename;
					p_icon->device_slave[i].start = atoi(num1);
					p_icon->device_slave[i].height = atoi(num2);
					//OnOK();
					onok=true;
					return;
				}
			}
		}
		return;
	}
	else if(order==51)//ɾ��������
	{
		if(cheakUser()==0)
		{
			AfxMessageBox("�û��˺ź����벻ƥ��!");
			return;
		}
		else if(cheakUser()==3)
		{
			AfxMessageBox("���û�û��Ȩ��!");
			return;
		}
		else if(cheakUser()==2)
		{
			AfxMessageBox("��������һ���û�!");
			return;
		}

		CString num1;
		GetDlgItem(IDC_EDIT3)->GetWindowTextA(num1);
		if(atoi(num1)>0&&atoi(num1)<51)
		{
			if(p_icon)
			{
				for(i=0;i<24;i++)
				{
					if(p_icon->device_master[i].start == atoi(num1))
					{
						p_icon->device_master[i].name = "";
						p_icon->device_master[i].start = 0;
						p_icon->device_master[i].height = 0;
						p_icon->device_master[i].maininputNum = 0;
						p_icon->device_master[i].slaveinputNum = 0;
					}
					if(p_icon->device_slave[i].start == atoi(num1))
					{
						p_icon->device_slave[i].name = "";
						p_icon->device_slave[i].start = 0;
						p_icon->device_slave[i].height = 0;
						p_icon->device_slave[i].maininputNum = 0;
						p_icon->device_slave[i].slaveinputNum = 0;
					}
				}
			}
			//OnOK();
			onok=true;
		}
		else
		{
			AfxMessageBox("����д��ȷ�Ļ���λ��");
		}
		return;
	}
	else if(order==52)//����������
	{
		if(cheakUser()==0)
		{
			AfxMessageBox("�û��˺ź����벻ƥ��!");
			return;
		}
		else if(cheakUser()==3)
		{
			AfxMessageBox("���û�û��Ȩ��!");
			return;
		}
		else if(cheakUser()==2)
		{
			AfxMessageBox("��������һ���û�!");
			return;
		}
		/////////////
		CString devicename;
		CString num1,num2,num3,num4, num5,num6,num7;
		int mNum1,sNum1,mNum2,sNum2,Height,Start;
		GetDlgItem(IDC_EDIT7)->GetWindowTextA(devicename);
		GetDlgItem(IDC_EDIT3)->GetWindowTextA(num1);//5
		GetDlgItem(IDC_EDIT4)->GetWindowTextA(num2);//3
		GetDlgItem(IDC_EDIT6)->GetWindowTextA(num3);//2
		GetDlgItem(IDC_EDIT5)->GetWindowTextA(num5);//2

		GetDlgItem(IDC_EDIT9)->GetWindowTextA(num6);//2
		GetDlgItem(IDC_EDIT8)->GetWindowTextA(num7);//2

		Start=atoi(num1);
		Height=atoi(num2);
		mNum1=atoi(num3);
		sNum1=atoi(num5);

		mNum2=atoi(num6);
		sNum2=atoi(num7);


		int m=0,s=0;
		POSITION pos = snmpobj.GetTailPosition ();
		while (pos != NULL)
		{
			CSnmpObj *p_Object = NULL;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));

			if(p_Object->ipaddr==p_icon->ipMaster)
			{
				while(1)
				{
					if(p_Object->outputname[p_icon->MasterNUM][m].GetLength())
					{
						m++;
					}
					else
						break;
				}
			}
		}

		pos = snmpobj.GetTailPosition ();
		while (pos != NULL)
		{
			CSnmpObj *p_Object = NULL;
			p_Object = ((CSnmpObj*)(snmpobj.GetPrev (pos)));

			if(p_Object->ipaddr==p_icon->ipSlave)
			{
				while(1)
				{
					if(p_Object->outputname[p_icon->SlaveNUM][s].GetLength())
					{
						s++;
					}
					else
						break;
				}
			}
		}

		int ipm=m+1;
		if(p_icon)
		{
			for(i=0;i<24;i++)
			{
				if(!p_icon->device_master[i].name.GetLength())
				{
					if(p_icon->ipMaster.GetLength())
					{
						if( (atoi(num3) > 0 && atoi(num3) < ipm)&&((atoi(num6) > 0 && atoi(num6) < ipm)))
						{
							p_icon->device_master[i].maininputNum = atoi(num3);
						}
						else
						{
							CString str;
							str.Format("���˿�ȡֵ��1-%d������ȷ��д",m);
							AfxMessageBox(str);
							return;
						}
					}
					
					if(atoi(num1)>50||atoi(num1)<0)
					{
						CString str;
						str.Format("�豸λ��ȡֵ��1-50������ȷ��д");
						AfxMessageBox(str);
						return;
					}

					p_icon->device_master[i].name = devicename;
					p_icon->device_master[i].start = atoi(num1);
					p_icon->device_master[i].height = atoi(num2);

					//OnOK();
					onok=true;
					break;
				}
			}
			for(i=0;i<24;i++)
			{
				if(!p_icon->device_slave[i].name.GetLength())
				{
					
					if(p_icon->ipSlave.GetLength())
					{
						if( (atoi(num5) > 0 && atoi(num5) < s+1)&&((atoi(num7) > 0 && atoi(num7) < s+1)))
						{
							p_icon->device_slave[i].slaveinputNum = atoi(num5);
						}
						else
						{
							CString str;
							str.Format("���ö˿�ȡֵ��1-%d������ȷ��д",s);
							AfxMessageBox(str);
							return;
						}
					}
					if(atoi(num1)>50||atoi(num1)<0)
					{
						CString str;
						str.Format("�豸λ��ȡֵ��1-50������ȷ��д");
						AfxMessageBox(str);
						return;
					}

					p_icon->device_slave[i].name = devicename;
					p_icon->device_slave[i].start = atoi(num1);
					p_icon->device_slave[i].height = atoi(num2);

					//OnOK();
					onok=true;
					break;
				}
			}
			for(i=0;i<24;i++)
			{
				if(!p_icon->device_master[i].name.GetLength())
				{
					if(p_icon->ipMaster.GetLength())
					{
						if(((atoi(num6) > 0 && atoi(num6) < ipm)))
						{
							p_icon->device_master[i].maininputNum = atoi(num6);
						}
						else
						{
							CString str;
							str.Format("���˿�ȡֵ��1-%d������ȷ��д",m);
							AfxMessageBox(str);
							return;
						}
					}
					
					if(atoi(num1)>50||atoi(num1)<0)
					{
						CString str;
						str.Format("�豸λ��ȡֵ��1-50������ȷ��д");
						AfxMessageBox(str);
						return;
					}

					p_icon->device_master[i].name = devicename;
					p_icon->device_master[i].start = atoi(num1);
					p_icon->device_master[i].height = atoi(num2);

					//OnOK();
					onok=true;
					break;
				}
			}
			for(i=0;i<24;i++)
			{
				if(!p_icon->device_slave[i].name.GetLength())
				{
					if(p_icon->ipSlave.GetLength())
					{
						if(((atoi(num7) > 0 && atoi(num7) < s+1)))
						{
							p_icon->device_slave[i].slaveinputNum = atoi(num7);
						}
						else
						{
							CString str;
							str.Format("���ö˿�ȡֵ��1-%d������ȷ��д",s);
							AfxMessageBox(str);
							return;
						}
					}
					if(atoi(num1)>50||atoi(num1)<0)
					{
						CString str;
						str.Format("�豸λ��ȡֵ��1-50������ȷ��д");
						AfxMessageBox(str);
						return;
					}

					p_icon->device_slave[i].name = devicename;
					p_icon->device_slave[i].start = atoi(num1);
					p_icon->device_slave[i].height = atoi(num2);

					//OnOK();
					onok=true;
					break;
				}
			}
			return;
		}
		return;
	}
	else if(order==53)//ɾ��������
	{
		if(cheakUser()==0)
		{
			AfxMessageBox("�û��˺ź����벻ƥ��!");
			return;
		}
		else if(cheakUser()==3)
		{
			AfxMessageBox("���û�û��Ȩ��!");
			return;
		}
		else if(cheakUser()==2)
		{
			AfxMessageBox("��������һ���û�!");
			return;
		}

		CString num1;
		GetDlgItem(IDC_EDIT3)->GetWindowTextA(num1);
		if(atoi(num1)>0&&atoi(num1)<51)
		{
			if(p_icon)
			{
				for(i=0;i<24;i++)
				{
					if(p_icon->device_master[i].start == atoi(num1))
					{
						p_icon->device_master[i].name = "";
						p_icon->device_master[i].start = 0;
						p_icon->device_master[i].height = 0;
						p_icon->device_master[i].maininputNum = 0;
						p_icon->device_master[i].slaveinputNum = 0;
					}
					if(p_icon->device_slave[i].start == atoi(num1))
					{
						p_icon->device_slave[i].name = "";
						p_icon->device_slave[i].start = 0;
						p_icon->device_slave[i].height = 0;
						p_icon->device_slave[i].maininputNum = 0;
						p_icon->device_slave[i].slaveinputNum = 0;
					}
				}
			}
			//OnOK();
			onok=true;
		}
		else
		{
			AfxMessageBox("����д��ȷ�Ļ���λ��");
		}
		return;
	}
}


void CMakeSureDlg::writeUserLog(int logtype,int type,CString name, CString item, CString detail)
{
	////////////////////////////////////
	name.Replace("\"","");
	item.Replace("\"","");
	detail.Replace("\"","");
	name.Replace("'","");
	item.Replace("'","");
	detail.Replace("'","");
	name.Replace("{","");
	item.Replace("{","");
	detail.Replace("{","");
	name.Replace("}","");
	item.Replace("}","");
	detail.Replace("}","");
	name.Replace("[","");
	item.Replace("[","");
	detail.Replace("[","");
	name.Replace("]","");
	item.Replace("]","");
	detail.Replace("]","");
	userlogs *logs;
	logs = new userlogs;
	logs->logtype = logtype;
	logs->type = type;
	memset(logs->name,0,sizeof(char)*32);
	strncpy_s(logs->name,sizeof(char)*32,name,sizeof(char)*30);
	memset(logs->item,0,sizeof(char)*32);
	strncpy_s(logs->item,sizeof(char)*32,item,sizeof(char)*30);
	memset(logs->detail,0,sizeof(char)*128);
	strncpy_s(logs->detail,sizeof(char)*128,detail,sizeof(char)*126);
	CMainFrame* p_maimframe = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	::PostMessage(p_maimframe->GetSafeHwnd(),SEND_MSG_WRITELOG,(WPARAM)logs,(LPARAM)(sizeof(userlogs)));
}

int CMakeSureDlg::cheakUser(void)
{
	//CString username;
	CString userpsd;
	GetDlgItem(IDC_EDIT1)->GetWindowTextA(username);
	GetDlgItem(IDC_EDIT2)->GetWindowTextA(userpsd);
	int i;
	for(i=0;i<20;i++)
	{
		if(strcmp(user[i].name,username)==0 && strcmp(user[i].psd,userpsd)==0 &&
			username.GetLength() && userpsd.GetLength() && user[i].grade==1)
		{
			return 1;
		}
		else if(strcmp(user[i].name,username)==0 && strcmp(user[i].psd,userpsd)==0 &&
			username.GetLength() && userpsd.GetLength() && user[i].grade==0)
		{
			return 3;
		}
	}
	for(i=0;i<20;i++)
	{
		if(strlen(user[i].name))
		{
			return 0;
		}
	}
	return 2;
}

void CMakeSureDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	if (onok1==true)
	{
		CDialog::OnOK();
		onok1=false;
	}
	else
	{
		makesure();
		if(onok==true)
		{
			CDialog::OnOK();
			onok=false;
		}
	}
}


LRESULT CMakeSureDlg::OnNcHitTest(CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CRect rect;
	GetClientRect(&rect);
	CRect rc;
	rc.bottom=23;
	rc.top=1;
	rc.left=1;
	rc.right=rect.right-25;
	ClientToScreen(&rc);
	return rc.PtInRect(point) ? HTCAPTION : CDialog::OnNcHitTest(point);
}