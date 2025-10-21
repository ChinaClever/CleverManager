// Energylogs.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "clevermanager.h"
#include "Energylogs.h"
#include <afxdb.h>
#include "SnmpObj.h"
#include "CanbinObjIcon.h"
// CEnergylogs �Ի���

IMPLEMENT_DYNAMIC(CEnergylogs, CDialog)

CEnergylogs::CEnergylogs(CWnd* pParent /*=NULL*/)
: CDialog(CEnergylogs::IDD, pParent)
{

}

CEnergylogs::~CEnergylogs()
{
}

void CEnergylogs::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, tm1);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, tm2);
	DDX_Control(pDX, IDC_COMBO1, m_device_ip);
	DDX_Control(pDX, IDC_COMBO2, m_device_num);
	DDX_Control(pDX, IDC_COMBO4, m_type);
	DDX_Control(pDX, IDC_COMBO5, m_data);
}


BEGIN_MESSAGE_MAP(CEnergylogs, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCHITTEST()
	ON_CBN_SELCHANGE(IDC_COMBO4, &CEnergylogs::OnCbnSelchangeCombo4)
END_MESSAGE_MAP()


// CEnergylogs ��Ϣ�������

BOOL CEnergylogs::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	CBrush brush(RGB(233,240,249));
	BkDc.SelectObject(&brush);
	BkDc.Rectangle(&rect);

	CBrush brushtitle(RGB(255,255,255));
	BkDc.SelectObject(&brushtitle);
	BkDc.Rectangle(0,0,rect.Width(),25);
	retange(&BkDc,1,1,rect.Width()-2,23);

	drawFont(&BkDc,1,RGB(64,64,64));
	BkDc.TextOut(20,5,"���ܼ�¼");
	drawImagePNG(&BkDc,getFilePath()+"\\icon\\close.png",rect.right-20,6,10,10);

	drawFont(&BkDc,0,RGB(0,0,0));
	CBrush brushBTN(RGB(240,240,240));
	BkDc.SelectObject(&brushBTN);
	BkDc.Rectangle(rect.right-95,38,rect.right-15,58);
	drawImagePNG(&BkDc,getFilePath()+"\\icon\\search.png",rect.right-85,40,15,15);
	BkDc.TextOut(rect.right-65,42,"����");
	BkDc.Rectangle(rect.right-95,66,rect.right-15,86);
	drawImagePNG(&BkDc,getFilePath()+"\\icon\\pencil.png",rect.right-85,70,15,15);
	BkDc.TextOut(rect.right-65,70,"����");
	BkDc.TextOut(18,70,"�����ĵ�����־����:");
	BkDc.TextOut(290,70,"����:");
	BkDc.TextOut(425,70,"�豸IP:");

	BkDc.TextOut(18,42,"��ʼ:");
	BkDc.TextOut(180,42,"����:");
	//BkDc.TextOut(320,42,"�豸IP:");
	BkDc.TextOut(510,42,"����:");

	////////////////��ʾ���豸///////////////////////
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&BkDc,0,0,SRCCOPY);
	BkDc.DeleteDC();
	return true;

	//return CDialog::OnEraseBkgnd(pDC);
}

void CEnergylogs::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rect;
	GetClientRect(&rect);
	if(point.x>rect.right-25 && point.x<rect.right && point.y> 0 && point.y<28)
	{
		::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
	}
	else if(point.x>rect.right-95 && point.x<rect.right-15 && point.y> 38 && point.y<58)
	{
		::SetCursor(::LoadCursor(NULL,   MAKEINTRESOURCE(IDC_HAND)));
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CEnergylogs::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rect;
	GetClientRect(&rect);
	if(point.x>rect.right-25 && point.x<rect.right && point.y> 0 && point.y<28)
	{
		OnOK();
		return;
	}
	else if(point.x>rect.right-95 && point.x<rect.right-15 && point.y> 38 && point.y<58)
	{
		insertList();
	}
	else if(point.x>rect.right-95 && point.x<rect.right-15 && point.y> 66 && point.y<86)
	{
		put_record_energy();
	}
	CDialog::OnLButtonDown(nFlags, point);
}

BOOL CEnergylogs::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	CRect rect;
	GetClientRect(&rect);
	m_list.ModifyStyle(LVS_LIST|LVS_ICON|LVS_SMALLICON ,   //LVS_ICON 
		LVS_REPORT |LVS_SHOWSELALWAYS|LVS_AUTOARRANGE); //LVS_REPORT|
	m_list.SetExtendedStyle( LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_SUBITEMIMAGES );

	m_list.SetTextColor(RGB(64,64,64));

	m_list.InsertColumn(0,"���",LVCFMT_CENTER,40,-1);
	m_list.InsertColumn(1,"����",LVCFMT_LEFT,rect.Width()*40/100-100,-1);
	m_list.InsertColumn(2,"��ʼʱ��",LVCFMT_CENTER,rect.Width()*20/100,-1);
	m_list.InsertColumn(3,"����ʱ��",LVCFMT_CENTER,rect.Width()*20/100,-1); 
	m_list.InsertColumn(4,"��ֵ",LVCFMT_CENTER,rect.Width()*20/100,-1); 

	CHeaderCtrl * m_head=m_list.GetHeaderCtrl();
	m_list.m_colorheader.SubclassWindow(m_head->GetSafeHwnd());

	POSITION posnow = snmpobj.GetTailPosition ();
	while (posnow != NULL)
	{
		CSnmpObj *p_Objectnow = NULL;
		p_Objectnow = ((CSnmpObj*)(snmpobj.GetPrev (posnow)));
		if(p_Objectnow->ipaddr.GetLength() && p_Objectnow->dtype==0)
		{
			//m_device_ip.AddString(p_Objectnow->ipaddr);
			m_data.AddString(p_Objectnow->ipaddr);
		}
	}
	m_device_num.AddString("0");
	m_device_num.AddString("1");
	m_device_num.AddString("2");
	m_device_num.AddString("3");
	m_device_num.AddString("4");
	m_device_num.AddString("5");
	m_device_num.AddString("6");
	m_device_num.AddString("7");
	m_device_num.AddString("8");
	m_device_num.AddString("9");

	m_type.AddString("����");
	m_type.AddString("����");
	m_type.AddString("��̨NPM");

	
	m_device_num.SetCurSel(0);
	m_type.SetCurSel(0);
	m_data.SetCurSel(0);
	m_device_ip.ShowWindow(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CEnergylogs::SetCabinet(CString type)
{
	m_data.ResetContent();
	if(type=="����")
	{
		for(int i=0;i<10;i++)
		{
			if(DCmember[i].name.GetLength()&&DCmember[i].name)
			{
				m_data.AddString(DCmember[i].name);
			}
		}
	}
	else if(type=="����")
	{
		for(int i=0;i<10;i++)
		{
			if(DCmember[i].name.GetLength()&&DCmember[i].name)
			{
				POSITION pos = DCmember[i].canbin.GetHeadPosition();
				if(pos!=NULL)
				{
					while(pos!=NULL)
					{
						CCanbinObjIcon * p_icon;
						p_icon=(CCanbinObjIcon*)DCmember[i].canbin.GetNext(pos);
						if(p_icon->cabientname.GetLength()&&p_icon->cabientname)
						{
							m_data.AddString(p_icon->cabientname);
						}
					}
				}
				else
				{
					AfxMessageBox("������ӻ���!");
					m_type.SetCurSel(0);
					OnCbnSelchangeCombo4();
					return;
				}
			}
		}
	}
	else if(type=="��̨NPM")
	{
		POSITION posnow = snmpobj.GetTailPosition ();
		while (posnow != NULL)
		{
			CSnmpObj *p_Objectnow = NULL;
			p_Objectnow = ((CSnmpObj*)(snmpobj.GetPrev (posnow)));
			if(p_Objectnow->ipaddr.GetLength() && p_Objectnow->dtype==0)
			{
				m_data.AddString(p_Objectnow->ipaddr);
			}
		}
	}
}

void CEnergylogs::OnCbnSelchangeCombo4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_type.GetCurSel();
	int nCount = m_type.GetCount();
	CString type;
	m_type.GetLBText(nIndex,type);
	SetCabinet(type);
	m_data.SetCurSel(0);
}

void CEnergylogs::insertList()
{
	CString ip;
	GetDlgItem(IDC_COMBO5)->GetWindowTextA(ip);
	if(!ip.GetLength())
	{
		AfxMessageBox("����д�豸��IP��ַ.");
		return;
	}
	CString slave;
	GetDlgItem(IDC_COMBO2)->GetWindowTextA(slave);
	int slavenum;
	slavenum=atoi(slave);
	if(slavenum<0 || slavenum>9)
	{
		AfxMessageBox("����д��ȷ���豸�������.");
		return;
	}

	CTime et1;
	tm1.GetTime(et1);
	CString start;
	start.Format("%4d%2d%2d",et1.GetYear(),et1.GetMonth(),et1.GetDay());
	start.Replace(" ","0");

	CTime et2;
	tm2.GetTime(et2);
	CString end;
	end.Format("%4d%2d%2d",et2.GetYear(),et2.GetMonth(),et2.GetDay());
	end.Replace(" ","0");
	struct energydata
	{
		int TEnergy[10][3];
		int energy[10][24];
	};
	energydata *startenergy,*endenergy;
	startenergy = new energydata;
	endenergy = new energydata;
	memset(startenergy,0,sizeof(energydata));
	memset(endenergy,0,sizeof(energydata));
	if(makesureFile(getFilePath()+"\\energy\\"+ip+"\\"+start))
	{
		CFile file;
		if(file.Open(getFilePath()+"\\energy\\"+ip+"\\"+start,CFile::modeRead | CFile::modeCreate |CFile::modeNoTruncate))
		{
			file.SeekToBegin();
			file.Read(startenergy,sizeof(energydata));
			file.Close();
		}
	}
	if(makesureFile(getFilePath()+"\\energy\\"+ip+"\\"+end))
	{
		CFile file;
		if(file.Open(getFilePath()+"\\energy\\"+ip+"\\"+end,CFile::modeRead | CFile::modeCreate |CFile::modeNoTruncate))
		{
			file.SeekToBegin();
			file.Read(endenergy,sizeof(energydata));
			file.Close();
		}
	}
	m_list.DeleteAllItems();

	for(int j = 0 ; j < 3 ; j++){
		CString value;
		value.Format("%d",j+1);
		m_list.InsertItem(LVIF_TEXT|LVIF_STATE|LVIF_IMAGE,j,value,0,LVIS_SELECTED,0,0);

		value.Empty();
		value.Format("%s->%d  L%d����",ip,slavenum,j+1);
		m_list.SetItemText(j,1,value);
		if(startenergy->TEnergy[slavenum])
		{
			value.Empty();
			value.Format("��ʼ:%.1f kWh",((float)(startenergy->TEnergy[slavenum][j]))/10);
			m_list.SetItemText(j,2,value);
		}
		else
		{
			m_list.SetItemText(j,2,"�޼�¼");
		}
		if(endenergy->TEnergy[slavenum])
		{
			value.Empty();
			value.Format("����:%.1f kWh",((float)(endenergy->TEnergy[slavenum][j])/10));
			m_list.SetItemText(j,3,value);
		}
		else
		{
			m_list.SetItemText(j,3,"�޼�¼");
		}

		value.Empty();
		value.Format("%.1f kWh",((float)(endenergy->TEnergy[slavenum][j] - startenergy->TEnergy[slavenum][j]))/10);
		m_list.SetItemText(j,4,value);
	}

	int i;
	for(i=0;i<24;i++)
	{
		CString value;
		value.Format("%d",i+4);
		m_list.InsertItem(LVIF_TEXT|LVIF_STATE|LVIF_IMAGE,i+3,value,0,LVIS_SELECTED,0,0);
		value.Empty();
		value.Format("%s->%d  ����%d",ip,slavenum,i+1);
		m_list.SetItemText(i+3,1,value);
		/////////
		if(startenergy->energy[slavenum][i])
		{
			value.Empty();
			value.Format("��ʼ:%.1f kWh",((float)(startenergy->energy[slavenum][i])/10));
			m_list.SetItemText(i+3,2,value);
		}
		else
		{
			m_list.SetItemText(i+3,2,"�޼�¼");
		}
		if(endenergy->energy[slavenum][i])
		{
			value.Empty();
			value.Format("����:%.1f kWh",((float)(endenergy->energy[slavenum][i])/10));
			m_list.SetItemText(i+3,3,value);
		}
		else
		{
			m_list.SetItemText(i+3,3,"�޼�¼");
		}

		value.Empty();
		value.Format("%.1f kWh",((float)(endenergy->energy[slavenum][i] - startenergy->energy[slavenum][i]))/10);
		m_list.SetItemText(i+3,4,value);

	}
	////////////////////////////////
	delete startenergy;
	delete endenergy;
}
LRESULT CEnergylogs::OnNcHitTest(CPoint point)
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
	return CDialog::OnNcHitTest(point);
}

int CEnergylogs::get_ip_energy(CString ip,int num,CString time,int load)
{
	int ret_energy=0;
	struct energydata
	{
		int TEnergy[10][3];
		int energy[10][24];
	};
	energydata *read_energy;
	read_energy = new energydata;
	memset(read_energy,0,sizeof(energydata));
	CString slave;
	GetDlgItem(IDC_COMBO2)->GetWindowTextA(slave);
	int slavenum;
	slavenum=atoi(slave);
	if(slavenum<0 || slavenum>9)
	{
		AfxMessageBox("����д��ȷ���豸�������.");
		return ret_energy;
	}

	if(makesureFile(getFilePath()+"\\energy\\"+ip+"\\"+time))
	{
		CFile file;
		if(file.Open(getFilePath()+"\\energy\\"+ip+"\\"+time,CFile::modeRead | CFile::modeCreate |CFile::modeNoTruncate))
		{
			file.SeekToBegin();
			file.Read(read_energy,sizeof(energydata));
			file.Close();
		}
	}
	ret_energy=read_energy->TEnergy[slavenum][load];
	////////////////////////////////
	delete read_energy;
	return ret_energy;
}

int CEnergylogs::get_room_energy(CString name,CString time,int change, int load)
{
	int master_energy,slave_energy;
	master_energy=slave_energy=0;
	for(int i=0;i<10;i++)
	{
		if(DCmember[i].name.GetLength()&&DCmember[i].name==name)
		{
			POSITION pos = DCmember[i].canbin.GetHeadPosition();
			while(pos!=NULL)
			{
				CCanbinObjIcon * p_icon;
				p_icon=(CCanbinObjIcon*)DCmember[i].canbin.GetNext(pos);
				if(p_icon->ipMaster&&p_icon->ipMaster.GetLength()&&change==1)
				{
					master_energy+=get_ip_energy(p_icon->ipMaster,p_icon->MasterNUM,time,load);
				}
				if(p_icon->ipSlave&&p_icon->ipSlave.GetLength()&&change==2)
				{
					slave_energy+=get_ip_energy(p_icon->ipSlave,p_icon->SlaveNUM,time,load);
				}
			}
		}
	}
	if(change==1)
		return master_energy;
	else if(change==2)
		return slave_energy;
	else 
		return 0;
}

int CEnergylogs::get_cabinet_energy(CString name,CString time,int change , int load)
{
	int master_energy,slave_energy;
	master_energy=slave_energy=0;
	for(int i=0;i<10;i++)
	{
		if(DCmember[i].name.GetLength()&&DCmember[i].name)
		{
			POSITION pos = DCmember[i].canbin.GetHeadPosition();
			while(pos!=NULL)
			{
				CCanbinObjIcon * p_icon;
				p_icon=(CCanbinObjIcon*)DCmember[i].canbin.GetNext(pos);
				if(p_icon->cabientname==name)
				{
					if(p_icon->ipMaster&&p_icon->ipMaster.GetLength()&&change==1)
					{
						master_energy+=get_ip_energy(p_icon->ipMaster,p_icon->MasterNUM,time, load);
					}
					if(p_icon->ipSlave&&p_icon->ipSlave.GetLength()&&change==2)
					{
						slave_energy+=get_ip_energy(p_icon->ipSlave,p_icon->SlaveNUM,time, load);
					}
				}
			}
		}
	}
	if(change==1)
		return master_energy;
	else if(change==2)
		return slave_energy;
	else 
		return 0;
}

void CEnergylogs::put_record_energy()
{
	int nIndex = m_type.GetCurSel();
	int nCount = m_type.GetCount();
	CString type;
	m_type.GetLBText(nIndex,type);
	//SetCabinet(type);

	CTime et1;
	tm1.GetTime(et1);
	CString start;
	start.Format("%4d%2d%2d",et1.GetYear(),et1.GetMonth(),et1.GetDay());
	start.Replace(" ","0");

	CTime et2;
	tm2.GetTime(et2);
	CString end;
	end.Format("%4d%2d%2d",et2.GetYear(),et2.GetMonth(),et2.GetDay());
	end.Replace(" ","0");

	CString log_name;
	GetDlgItem(IDC_EDIT8)->GetWindowTextA(log_name);

	if(log_name&&log_name.GetLength())
	{
		if(type&&type.GetLength())
		{
			if(type=="����")
			{
				vector<int> start_a,start_b;
				vector<int> end_a,end_b;

				int Index = m_data.GetCurSel();
				int Count = m_data.GetCount();
				CString room_name;
				m_data.GetLBText(Index,room_name);

				for(int j = 0 ; j < 3 ; j++){
				start_a.push_back(get_room_energy(room_name,start,1,j));
				start_b.push_back(get_room_energy(room_name,start,2,j));

				end_a.push_back(get_room_energy(room_name,end,1,j));
				end_b.push_back(get_room_energy(room_name,end,2,j));
				}

				put_record(start,start_a,start_b,end,end_a,end_b,log_name);
			}
			else if(type=="����")
			{
				vector<int> start_a,start_b;
				vector<int> end_a,end_b;

				int Index = m_data.GetCurSel();
				int Count = m_data.GetCount();
				CString cabinet_name;
				m_data.GetLBText(Index,cabinet_name);

				for(int j = 0 ; j < 3 ; j++){
				start_a.push_back(get_cabinet_energy(cabinet_name,start,1,j));
				start_b.push_back(get_cabinet_energy(cabinet_name,start,2,j));

				end_a.push_back(get_cabinet_energy(cabinet_name,end,1,j));
				end_b.push_back(get_cabinet_energy(cabinet_name,end,2,j));
				}
		
				put_record(start,start_a,start_b,end,end_a,end_b,log_name);
			}
			else if(type=="��̨NPM")
			{
				int start_a,start_b,start_c;
				int end_a,end_b,end_c;
				//vector<int> opstartenergy(24,0);
				//vector<int> opendenergy(24,0);

				int Index = m_data.GetCurSel();
				int Count = m_data.GetCount();
				CString device_ip;
				m_data.GetLBText(Index,device_ip);

				start_a=get_ip_energy(device_ip,0,start,0);
				end_a=get_ip_energy(device_ip,0,end,0);

				start_b=get_ip_energy(device_ip,0,start,1);
				end_b=get_ip_energy(device_ip,0,end,1);

				start_c=get_ip_energy(device_ip,0,start,2);
				end_c=get_ip_energy(device_ip,0,end,2);

				//get_output_energy(device_ip,opstartenergy,opendenergy);
				put_record_output(start,start_a,start_b,start_c,end,end_a,end_b,end_c,log_name);
			}
		}
	}
	else
	{
		AfxMessageBox("���������־����!");
	}
}

void CEnergylogs::put_record(CString start_time,vector<int> start_a_energy,vector<int> start_b_energy,CString end_time,vector<int> end_a_energy,vector<int> end_b_energy,CString logname)
{
	bool end_records=true;
	while(end_records)
	{
		CString strName;

		CDatabase database;  
		CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel��װ����
		CString sExcelFile,sPath; 
		CString sSql;  

		//��ȡ����������·��,����sPath��   
		GetModuleFileName(NULL,sPath.GetBufferSetLength (MAX_PATH+1),MAX_PATH);  
		sPath.ReleaseBuffer ();  
		int nPos;  
		nPos=sPath.ReverseFind ('\\');  
		sPath=sPath.Left (nPos);
		strName.Format("\\%s.xls",logname);
		sPath += strName;
		sExcelFile = sPath;             // Ҫ������Excel�ļ�   
		CFile file;
		try  
		{ 
			// �������д�ȡ���ַ���   
			sSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, sExcelFile); 
			if(database.OpenEx(sSql,CDatabase::noOdbcDialog))  
			{ 
				sSql = "CREATE TABLE Exceldemo (��¼ʱ�� char,A·L1���� int,B·L1���� int,A·L2���� int,B·L2���� int,A·L3���� int,B·L3���� int)";  
				database.ExecuteSQL(sSql);  

				sSql.Format("INSERT INTO Exceldemo (��¼ʱ��,A·L1����,B·L1����,A·L2����,B·L2����,A·L3����,B·L3����)VALUES ('%s','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f')",
					start_time,((float)start_a_energy[0])/10,((float) start_b_energy[0])/10,((float)start_a_energy[1])/10,((float) start_b_energy[1])/10,((float)start_a_energy[2])/10,((float) start_b_energy[2])/10); 
				database.ExecuteSQL(sSql);

				sSql.Format("INSERT INTO Exceldemo (��¼ʱ��,A·L1����,B·L1����,A·L2����,B·L2����,A·L3����,B·L3����)VALUES ('%s','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f')",
					end_time,((float)end_a_energy[0])/10,((float)end_b_energy[0])/10,((float)end_a_energy[1])/10,((float)end_b_energy[1])/10,((float)end_a_energy[2])/10,((float)end_b_energy[2])/10); 
				database.ExecuteSQL(sSql);

				sSql.Format("INSERT INTO Exceldemo (��¼ʱ��,A·L1����,B·L1����,A·L2����,B·L2����,A·L3����,B·L3����)VALUES ('%s','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f')",
					"�ĵ���",((float)(end_a_energy[0]-start_a_energy[0]))/10,((float)(end_b_energy[0]-start_b_energy[0]))/10,
					((float)(end_a_energy[1]-start_a_energy[1]))/10,((float)(end_b_energy[1]-start_b_energy[1]))/10,
					((float)(end_a_energy[2]-start_a_energy[2]))/10,((float)(end_b_energy[2]-start_b_energy[2]))/10); 
				database.ExecuteSQL(sSql);

				end_records=false;
				AfxMessageBox("�����ɹ�!");
			}
		}
		catch(CUserException ex)  
		{  
			ex.ReportError();  
		}
	}
}

void CEnergylogs::put_record_output(CString start_time,int start_a_energy,int start_b_energy,int start_c_energy,CString end_time,int end_a_energy,int end_b_energy,int end_c_energy,CString logname)
{
	bool end_records=true;
	while(end_records)
	{
		CString strName;

		CDatabase database;  
		CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel��װ����
		CString sExcelFile,sPath; 
		CString sSql;  

		//��ȡ����������·��,����sPath��   
		GetModuleFileName(NULL,sPath.GetBufferSetLength (MAX_PATH+1),MAX_PATH);  
		sPath.ReleaseBuffer ();  
		int nPos;  
		nPos=sPath.ReverseFind ('\\');  
		sPath=sPath.Left (nPos);
		strName.Format("\\%s.xls",logname);
		sPath += strName;
		sExcelFile = sPath;             // Ҫ������Excel�ļ�   
		CFile file;
		try  
		{ 
			// �������д�ȡ���ַ���   
			sSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, sExcelFile); 
			if(database.OpenEx(sSql,CDatabase::noOdbcDialog))  
			{ 
				sSql = "CREATE TABLE Exceldemo (��¼���� int,L1����kWh int,L2����kWh int,L3����kWh int)";  
				database.ExecuteSQL(sSql);  

				sSql.Format("INSERT INTO Exceldemo (��¼����,L1����kWh,L2����kWh,L3����kWh)VALUES ('%d','%.1f','%.1f','%.1f')",
					atoi(start_time),((float)start_a_energy)/10,((float)start_b_energy)/10,((float)start_c_energy)/10); 
				database.ExecuteSQL(sSql);

				sSql.Format("INSERT INTO Exceldemo (��¼����,L1����kWh,L2����kWh,L3����kWh)VALUES ('%d','%.1f','%.1f','%.1f')",
					atoi(end_time),((float)end_a_energy)/10,((float)end_b_energy)/10,((float)end_c_energy)/10); 
				database.ExecuteSQL(sSql);

				sSql.Format("INSERT INTO Exceldemo (��¼����,L1����kWh,L2����kWh,L3����kWh)VALUES ('%d','%.1f','%.1f','%.1f')",
					0,((float)(end_a_energy-start_a_energy))/10,((float)(end_b_energy-start_b_energy))/10,((float)(end_c_energy-start_c_energy))/10); 
				database.ExecuteSQL(sSql);

				end_records=false;
				AfxMessageBox("�����ɹ�!");
			}
		}
		catch(CUserException ex)  
		{  
			ex.ReportError();  
		}
	}
}

int CEnergylogs::year_ret(CString str_year)
{
	if(atoi(str_year)%400==0)
	{
		return 1;
	}
	else if((atoi(str_year)%4)==0&&(atoi(str_year)%100)!=0)
	{
		return 1;
	}
	else 
		return 0;
}

//void CEnergylogs::get_output_energy(CString ip,vector<int>& opstartenergy,vector<int>& opendenergy)
//{
//	struct energydata
//	{
//		int TEnergy[10][3];
//		int energy[10][24];
//	};
//	
//	CTime et1;
//	tm1.GetTime(et1);
//	CString start;
//	start.Format("%4d%2d%2d",et1.GetYear(),et1.GetMonth(),et1.GetDay());
//	start.Replace(" ","0");
//
//	CTime et2;
//	tm2.GetTime(et2);
//	CString end;
//	end.Format("%4d%2d%2d",et2.GetYear(),et2.GetMonth(),et2.GetDay());
//	end.Replace(" ","0");
//
//	CString slave;
//	GetDlgItem(IDC_COMBO2)->GetWindowTextA(slave);
//	int slavenum;
//	slavenum=atoi(slave);
//	if(slavenum<0 || slavenum>9)
//	{
//		AfxMessageBox("����д��ȷ���豸�������.");
//		return ;
//	}
//
//	energydata *startenergy,*endenergy;
//	startenergy = new energydata;
//	endenergy = new energydata;
//	memset(startenergy,0,sizeof(energydata));
//	memset(endenergy,0,sizeof(energydata));
//	if(makesureFile(getFilePath()+"\\energy\\"+ip+"\\"+start))
//	{
//		CFile file;
//		if(file.Open(getFilePath()+"\\energy\\"+ip+"\\"+start,CFile::modeRead | CFile::modeCreate |CFile::modeNoTruncate))
//		{
//			file.SeekToBegin();
//			file.Read(startenergy,sizeof(energydata));
//			file.Close();
//		}
//	}
//	if(makesureFile(getFilePath()+"\\energy\\"+ip+"\\"+end))
//	{
//		CFile file;
//		if(file.Open(getFilePath()+"\\energy\\"+ip+"\\"+end,CFile::modeRead | CFile::modeCreate |CFile::modeNoTruncate))
//		{
//			file.SeekToBegin();
//			file.Read(endenergy,sizeof(energydata));
//			file.Close();
//		}
//	}
//
//	int i;
//	for(i=0;i<24;i++)
//	{
//		
//		/////////
//		if(startenergy->energy[slavenum][i])
//		{
//			opstartenergy[i] = startenergy->energy[slavenum][i];
//		}
//		else
//		{
//			opstartenergy[i] = 0;
//		}
//		if(endenergy->energy[slavenum][i])
//		{
//			opendenergy[i] = endenergy->energy[slavenum][i];
//		}
//		else
//		{
//			opstartenergy[i] = 0;
//		}
//
//	}
//	////////////////////////////////
//	delete startenergy;
//	delete endenergy;
//
//}



