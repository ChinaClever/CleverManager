// StatusLogs.cpp : implementation file
//

#include "stdafx.h"
#include "clevermanager.h"
#include "StatusLogs.h"
#include <afxdb.h>
#include "SnmpObj.h"
// CStatusLogs dialog

IMPLEMENT_DYNAMIC(CStatusLogs, CDialog)


    
CStatusLogs::CStatusLogs(CWnd* pParent /*=NULL*/)
: CDialog(CStatusLogs::IDD, pParent)
, title(_T(""))
{

}

CStatusLogs::~CStatusLogs()
{
}

void CStatusLogs::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, tm1);
	DDX_Control(pDX, IDC_COMBO1, m_com);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, tm2);
	DDX_Control(pDX, IDC_COMBO2, m_ip);
}


BEGIN_MESSAGE_MAP(CStatusLogs, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CStatusLogs message handlers

BOOL CStatusLogs::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	title = "��ѡ����Ҫ��ʾ������";
	CRect rect;
	GetClientRect(&rect);
	int i;
	for(i=0;i<25;i++)
	{
		x_zhou[i] = rect.bottom-50;
	}
	m_com.ResetContent();
	m_com.AddString("�ܹ���");
	m_com.AddString("�ܵ���");
	m_com.AddString("�ܵ�ѹ");
	m_com.AddString("�¶�1");
	m_com.AddString("�¶�2");
	m_com.AddString("ʪ��1");
	m_com.AddString("ʪ��2");

	m_com.AddString("OUTPUT1 ����");
	m_com.AddString("OUTPUT2 ����");
	m_com.AddString("OUTPUT3 ����");
	m_com.AddString("OUTPUT4 ����");
	m_com.AddString("OUTPUT5 ����");
	m_com.AddString("OUTPUT6 ����");

	m_com.AddString("OUTPUT7 ����");
	m_com.AddString("OUTPUT8 ����");
	m_com.AddString("OUTPUT9 ����");
	m_com.AddString("OUTPUT10 ����");
	m_com.AddString("OUTPUT11 ����");
	m_com.AddString("OUTPUT12 ����");

	m_com.AddString("OUTPUT13 ����");
	m_com.AddString("OUTPUT14 ����");
	m_com.AddString("OUTPUT15 ����");
	m_com.AddString("OUTPUT16 ����");
	m_com.AddString("OUTPUT17 ����");
	m_com.AddString("OUTPUT18 ����");

	m_com.AddString("OUTPUT19 ����");
	m_com.AddString("OUTPUT20 ����");
	m_com.AddString("OUTPUT21 ����");
	m_com.AddString("OUTPUT22 ����");
	m_com.AddString("OUTPUT23 ����");
	m_com.AddString("OUTPUT24 ����");

	m_com.AddString("OUTPUT1 ����");
	m_com.AddString("OUTPUT2 ����");
	m_com.AddString("OUTPUT3 ����");
	m_com.AddString("OUTPUT4 ����");
	m_com.AddString("OUTPUT5 ����");
	m_com.AddString("OUTPUT6 ����");

	m_com.AddString("OUTPUT7 ����");
	m_com.AddString("OUTPUT8 ����");
	m_com.AddString("OUTPUT9 ����");
	m_com.AddString("OUTPUT10 ����");
	m_com.AddString("OUTPUT11 ����");
	m_com.AddString("OUTPUT12 ����");

	m_com.AddString("OUTPUT13 ����");
	m_com.AddString("OUTPUT14 ����");
	m_com.AddString("OUTPUT15 ����");
	m_com.AddString("OUTPUT16 ����");
	m_com.AddString("OUTPUT17 ����");
	m_com.AddString("OUTPUT18 ����");

	m_com.AddString("OUTPUT19 ����");
	m_com.AddString("OUTPUT20 ����");
	m_com.AddString("OUTPUT21 ����");
	m_com.AddString("OUTPUT22 ����");
	m_com.AddString("OUTPUT23 ����");
	m_com.AddString("OUTPUT24 ����");

	POSITION posnow = snmpobj.GetTailPosition ();
	while (posnow != NULL)
	{
		CSnmpObj *p_Objectnow = NULL;
		p_Objectnow = ((CSnmpObj*)(snmpobj.GetPrev (posnow)));
		
		if(p_Objectnow->ipaddr&&p_Objectnow->ipaddr.GetLength()&&p_Objectnow->dtype==0)
		{
			m_ip.AddString(p_Objectnow->ipaddr);
		}
	}
//	m_ip

	data_x=1;
	data_y=1;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CStatusLogs::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
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
	CBrush brush(RGB(245,250,255));
	BkDc.SelectObject(&brush);
	BkDc.Rectangle(&rect);

	CBrush brushtitle(RGB(255,255,255));
	BkDc.SelectObject(&brushtitle);
	BkDc.Rectangle(0,0,rect.Width(),25);
	retange(&BkDc,1,1,rect.Width()-2,23);

	drawFont(&BkDc,1,RGB(64,64,64));
	BkDc.TextOut(20,5,"״̬��¼");
	drawImagePNG(&BkDc,getFilePath()+"\\icon\\close.png",rect.right-20,6,10,10);

	drawFont(&BkDc,0,RGB(0,0,0));
	CBrush brushBTN(RGB(240,240,240));
	BkDc.SelectObject(&brushBTN);
	BkDc.Rectangle(rect.right-95,38,rect.right-15,58);
	drawImagePNG(&BkDc,getFilePath()+"\\icon\\search.png",rect.right-85,40,15,15);
	BkDc.TextOut(rect.right-65,42,"����");

	BkDc.Rectangle(400,68,480,89);
	drawImagePNG(&BkDc,getFilePath()+"\\icon\\pencil.png",405,70,15,15);
	BkDc.TextOut(425,73,"����");

	BkDc.TextOut(25,42,"ʱ��:");
	BkDc.TextOut(25,73,"ʱ��-2:");
	BkDc.TextOut(210,42,"�豸IP:");
	BkDc.TextOut(400,42,"����:");
	BkDc.TextOut(530,42,"����:");
	BkDc.TextOut(200,73,"��־����:");
	drawgraph(&BkDc,0);
	////////////////��ʾ���豸///////////////////////
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&BkDc,0,0,SRCCOPY);
	BkDc.DeleteDC();
	return true;

	//return CDialog::OnEraseBkgnd(pDC);
}

void CStatusLogs::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
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

void CStatusLogs::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(&rect);
	if(point.x>rect.right-25 && point.x<rect.right && point.y> 0 && point.y<28)
	{
		OnOK();
		return;
	}
	else if(point.x>rect.right-95 && point.x<rect.right-15 && point.y> 38 && point.y<58)
	{
		insertline();
	}
	else if(point.x>400 && point.x<480 && point.y> 68 && point.y<89)
	{
		exportexl();
	}
	else if(point.x>rect.right-135 && point.x<rect.right-120 && point.y> 150 && point.y<165)
	{
		if(data_x==1)
		{
			data_x=2;
			data_y=1;
		}
		else if(data_x==2)
		{
			data_x=4;
			data_y=1;
		}
		else if(data_x==4)
		{
			data_x=8;
			data_y=1;
		}
		//else if(data_x==8)
		//	data_x=24;
		else
			AfxMessageBox("��ǰ�Ѿ�����С��λʱ�䣡");
		insertline();
	}
	else if(point.x>rect.right-85 && point.x<rect.right-70 && point.y> 150 && point.y<165)
	{
		if(data_x==2)
		{
			data_x=1;
			data_y=1;
		}
		else if(data_x==4)
		{
			data_x=2;
			data_y=1;
		}
		else if(data_x==8)
		{
			data_x=4;
			data_y=1;
		}
		//else if(data_x==24)
		//	data_x=8;
		else
			AfxMessageBox("��ǰ�Ѿ������λʱ�䣡");
		insertline();
	}
	else if(point.x>rect.right-135 && point.x<rect.right-120 && point.y> 180 && point.y<200)
	{
		if(data_x==2)
		{
			if(data_y<13)
				data_y++;
		}
		else if(data_x==4)
		{
			if(data_y<19)
				data_y++;
		}
		else if(data_x==8)
		{
			if(data_y<22)
				data_y++;
		}
		//else if(data_x==24)
		//{
		//	if(data_y<24)
		//		data_y++;
		//}
		insertline();
	}
	else if(point.x>rect.right-85 && point.x<rect.right-70 && point.y> 180 && point.y<200)
	{
		if(data_x==2)
		{
			if(data_y>1)
				data_y--;
		}
		else if(data_x==4)
		{
			if(data_y>1)
				data_y--;
		}
		else if(data_x==8)
		{
			if(data_y>1)
				data_y--;
		}
		//else if(data_x==24)
		//{
		//	if(data_y>1)
		//		data_y--;
		//}
		insertline();
	}

	/*drawImagePNG(pDC,getFilePath()+"\\icon\\add.png",,150,15,15);
	drawImagePNG(pDC,getFilePath()+"\\icon\\remove.png",rect.right-85,150,15,15);*/
	//drawImagePNG(pDC,getFilePath()+"\\icon\\l.jpg",rect.right-135,180,20,20);
	//drawImagePNG(pDC,getFilePath()+"\\icon\\r.jpg",rect.right-85,180,20,20);
	CDialog::OnLButtonDown(nFlags, point);
}

void CStatusLogs::drawgraph(CDC *pDC,int y_zhou_show)
{
	CRect rect;
	GetClientRect(&rect);
	drawFont(pDC,0,RGB(0,0,0));
	SetBkMode(pDC->GetSafeHdc(),TRANSPARENT);
	CBrush brush(RGB(255,255,255));
	pDC->SelectObject(&brush);
	pDC->Rectangle(5,90,rect.right-5,rect.bottom-5);

	Graphics graphics(pDC->GetSafeHdc()); 
	graphics.SetSmoothingMode(SmoothingModeHighQuality);
	LinearGradientBrush lgb(Point(5, 70), 
		Point(5,rect.bottom-6), Color::Color(230,235,242), Color::Color(255,255,255));
	graphics.FillRectangle(&lgb, 6,91,rect.right-13,rect.bottom-100);

	pDC->SelectObject(&brush);
	pDC->Rectangle(50,140,rect.right-50,rect.bottom-50);
	LinearGradientBrush lgb1(Point(51, 141), 
		Point(51,rect.bottom-52), Color::Color(255,255,255), Color::Color(240,240,192));
	graphics.FillRectangle(&lgb1, 51,141,rect.right-103,rect.bottom-193);

	CPen  pen(PS_DOT,1,RGB(128,128,128));
	pDC->SelectObject(&pen);
	/////////////////
	drawImagePNG(pDC,getFilePath()+"\\icon\\add.png",rect.right-135,150,20,20);
	drawImagePNG(pDC,getFilePath()+"\\icon\\remove.png",rect.right-85,150,20,20);
	drawImagePNG(pDC,getFilePath()+"\\icon\\l.jpg",rect.right-135,180,20,20);
	drawImagePNG(pDC,getFilePath()+"\\icon\\r.jpg",rect.right-85,180,20,20);
	int i;
	CString type;
	m_com.GetWindowTextA(type);
	if((type.Find("����"))>0)
	{
		for(i=1;i<5;i++)
		{
			pDC->MoveTo(50,(rect.bottom-190)*i/5+140);
			pDC->LineTo(rect.right-50,(rect.bottom-190)*i/5+140);
			if(y_zhou_show>0)
			{
				CString text_out;
				text_out.Format("%d",(5-i)*y_zhou_show/5);
				pDC->TextOutA(52,(rect.bottom-190)*i/5+140,text_out);
			}
		}
		if(y_zhou_show>0)
		{
			CString text_out;
			text_out.Format("%d",y_zhou_show);
			pDC->TextOutA(52,140,text_out);
		}
	}
	else if((type.Find("����"))>0)
	{
		for(i=1;i<5;i++)
		{
			pDC->MoveTo(50,(rect.bottom-190)*i/5+140);
			pDC->LineTo(rect.right-50,(rect.bottom-190)*i/5+140);
			if(y_zhou_show>0)
			{
				CString text_out;
				text_out.Format("%d.%d",y_zhou_show*(5-i)/5,10*y_zhou_show*(5-i)/5%10);
				pDC->TextOutA(52,(rect.bottom-190)*i/5+140,text_out);
			}
		}
		if(y_zhou_show>0)
		{
			CString text_out;
			text_out.Format("%d.%d",y_zhou_show,10*y_zhou_show*(5-i)/5%10);
			pDC->TextOutA(52,140,text_out);
		}
	}
	else 
	{
		for(i=1;i<5;i++)
		{
			pDC->MoveTo(50,(rect.bottom-190)*i/5+140);
			pDC->LineTo(rect.right-50,(rect.bottom-190)*i/5+140);
			if(y_zhou_show>0)
			{
				CString text_out;
				text_out.Format("%d.%d",y_zhou_show*(5-i)/5,10*y_zhou_show*(5-i)/5%10);
				pDC->TextOutA(52,(rect.bottom-190)*i/5+140,text_out);
			}
		}
		if(y_zhou_show>0)
		{
			CString text_out;
			text_out.Format("%d.%d",y_zhou_show,10*y_zhou_show*(5-i)/5%10);
			pDC->TextOutA(52,140,text_out);
		}
	}

	for(i=0;i<6;i++)
	{
		if(i)
		{
			pDC->MoveTo(50+(rect.right-100)*i/6,140);
			pDC->LineTo(50+(rect.right-100)*i/6,rect.bottom-50);
		}
		if(data_x==1)
		{
			CString tm;
			tm.Format("%d:00",i*4);
			pDC->TextOut(50+(rect.right-100)*i/6,rect.bottom-45,tm);
			pDC->TextOut(rect.right-100,rect.bottom-45,"(Сʱ)");
		}
		else if(data_x==2)
		{
			CString tm;
			tm.Format("%d:00",i*2+data_y-1);
			pDC->TextOut(50+(rect.right-100)*i/6,rect.bottom-45,tm);
			pDC->TextOut(rect.right-100,rect.bottom-45,"(Сʱ)");
		}
		else if(data_x==4)
		{
			CString tm;
			tm.Format("%d:00",i+data_y-1);
			pDC->TextOut(50+(rect.right-100)*i/6,rect.bottom-45,tm);
			pDC->TextOut(rect.right-100,rect.bottom-45,"(Сʱ)");
		}
		else if(data_x==8)
		{
			CString tm;
			if(i%2==1)
				tm.Format("%d:30",i/2+data_y-1);
			else
				tm.Format("%d:00",i/2+data_y-1);
			pDC->TextOut(50+(rect.right-100)*i/6,rect.bottom-45,tm);
			pDC->TextOut(rect.right-100,rect.bottom-45,"(Сʱ)");
		}
		else if(data_x==24)
		{
			CString tm;
			tm.Format("%d:%d0",data_y-1,i);
			pDC->TextOut(50+(rect.right-100)*i/6,rect.bottom-45,tm);
			pDC->TextOut(rect.right-100,rect.bottom-45,"(����)");
		}
	}


	drawFont(pDC,2,RGB(0,0,0));
	CSize fontsize= pDC->GetTextExtent(title);
	pDC->TextOut(rect.Width()/2-fontsize.cx/2,100,title);

	/////////////////////////////////

	int data_x_zhou[1440];
	int index_data=0;
	for(i=data_y-1;i<(24/data_x+data_y-1);i++)
	{
		for(int j=0;j<60;j++)
		{
			data_x_zhou[index_data]=x_zhou_60[i][j];
			index_data++;
		}
	}


	Point points[1440];
	Color colors[1440];
	for(i=0;i<1440/data_x;i++)
	{
		//colors[i] =  Color(128,64,255,64);
		points[i]=Point(50+(rect.right-100)*i/(1440/data_x) ,data_x_zhou[i]);
	}
	/*colors[0]=Color(128,64,255,64);
	points[0]=Point(50,540);*/
	//colors[25] =  Color(128,64,255,64);
	//points[25]=Point(rect.right-50 , rect.bottom-50);
	//colors[26] =  Color(128,64,255,64);
	//points[26]=Point(50 , rect.bottom-50);

	CPen  pen1(PS_SOLID,1,RGB(0,0,255));
	pDC->SelectObject(&pen1);

	for(i=0;i<1440/data_x-1;i++)
	{
		pDC->MoveTo(points[i].X,points[i].Y);
		pDC->LineTo(points[i+1].X,points[i+1].Y);
	}
	drawFont(pDC,1,RGB(255,0,0));


	//GraphicsPath path;
	//int count = 1440/data_x;
	//path.AddLines(points,count);
	//PathGradientBrush pthGrBrush(&path);
	//pthGrBrush.SetCenterColor(Color(128,64,255,64));
	//pthGrBrush.SetCenterPoint(Point(300,300));
	//pthGrBrush.SetSurroundColors(colors,&count);
	//graphics.FillPath(&pthGrBrush,&path);

	/////////////////
	//graphics.ReleaseHDC(pDC->GetSafeHdc());
}

void CStatusLogs::insertline(void)
{
	int i;
	CString ip;
	m_ip.GetWindowTextA(ip);
	//if(!ip.GetLength())
	//{
	//	AfxMessageBox("����д�豸��IP��ַ.");
	//	return;
	//}
	CString slave;
	GetDlgItem(IDC_EDIT2)->GetWindowTextA(slave);
	int slavenum;
	slavenum=atoi(slave);
	if(slavenum<0 || slavenum>9)
	{
		AfxMessageBox("����д��ȷ�ĸ������.");
		return;
	}
	CTime et1;
	tm1.GetTime(et1);
	CString filename;
	filename.Format("%4d%2d%2d",et1.GetYear(),et1.GetMonth(),et1.GetDay());
	filename.Replace(" ","0");

	struct recorddata
	{
		float cur[24][60][10][24];
		float Tcur[24][60][10][3];
		int Tvol[24][60][10][3];
		int th[24][60][10][6];
	};
	CFile file;
	recorddata *p_record;
	p_record = new recorddata;

	//if(file.Open(getFilePath()+"\\status\\"+ip+"\\"+filename,CFile::modeReadWrite | CFile::modeCreate))
	//{
	//	memset(p_record,0,sizeof(recorddata));
	//	p_record->th[1][0][0] = 35;
	//	file.Write(p_record,sizeof(recorddata));
	//	file.Close();
	//}

	if(file.Open(getFilePath()+"\\status\\"+ip+"\\"+filename,CFile::modeRead))
	{
		memset(p_record,0,sizeof(recorddata));
		file.Read(p_record,sizeof(recorddata));
		file.Close();
	}
	else
	{
		CDC *pDC;
		pDC = GetDC();
		CRect rect;
		GetClientRect(&rect);
		int i;
		for(i=0;i<25;i++)
		{
			x_zhou[i] = rect.bottom-50;
		}
		title = "��ѡ����Ҫ��ʾ������";
		drawgraph(pDC,0);
		ReleaseDC(pDC);
		delete p_record;
		return;
	}
	/////////////////////////////////////////////////////////
	CString type;
	m_com.GetWindowTextA(type);
	if(type == "�ܹ���")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->Tcur[i][j][slavenum][0]*p_record->Tvol[i][j][slavenum][0];
			}
		}

		title = "�ܹ���";
		fillcurrent(cur);
	}
	else if(type == "�ܵ���")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->Tcur[i][j][slavenum][0];
			}
		}

		title = "�ܵ���";
		fillcurrent(cur);
	}
	else if(type == "�ܵ�ѹ")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->Tvol[i][j][slavenum][0];
			}
		}

		title = "�ܵ�ѹ";
		fillcurrent(cur);
	}
	else if(type == "�¶�1")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->th[i][j][slavenum][0];
			}
		}

		title = "�¶�1";
		fillcurrent(cur);
	}
	else if(type == "�¶�2")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->th[i][j][slavenum][1];
			}
		}
		title = "�¶�2";
		fillcurrent(cur);
	}
	else if(type == "ʪ��1")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->th[i][j][slavenum][3];
			}
		}
		title = "ʪ��1";
		fillcurrent(cur);
	}
	else if(type == "ʪ��2")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->th[i][j][slavenum][4];
			}
		}
		title = "ʪ��2";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT1 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][0]*p_record->Tvol[i][j][slavenum][0];
			}
		}

		title = "OUTPUT1 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT2 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][1]*p_record->Tvol[i][j][slavenum][0];
			}
		}

		title = "OUTPUT2 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT3 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][2]*p_record->Tvol[i][j][slavenum][0];
			}
		}

		title = "OUTPUT3 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT4 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][3]*p_record->Tvol[i][j][slavenum][0];
			}
		}

		title = "OUTPUT4 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT5 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][4]*p_record->Tvol[i][j][slavenum][0];
			}
		}

		title = "OUTPUT5 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT6 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][5]*p_record->Tvol[i][j][slavenum][0];
			}
		}

		title = "OUTPUT6 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT7 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][6]*p_record->Tvol[i][j][slavenum][0];
			}
		}

		title = "OUTPUT7 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT8 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][7]*p_record->Tvol[i][j][slavenum][0];
			}
		}

		title = "OUTPUT8 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT9 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][8]*p_record->Tvol[i][j][slavenum][0];
			}
		}

		title = "OUTPUT9 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT10 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][9]*p_record->Tvol[i][j][slavenum][0];
			}
		}

		title = "OUTPUT10 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT11 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][10]*p_record->Tvol[i][j][slavenum][0];
			}
		}

		title = "OUTPUT11 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT12 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][11]*p_record->Tvol[i][j][slavenum][0];
			}
		}

		title = "OUTPUT12 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT13 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][12]*p_record->Tvol[i][j][slavenum][0];
			}
		}

		title = "OUTPUT13 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT14 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][13]*p_record->Tvol[i][j][slavenum][0];
			}
		}

		title = "OUTPUT14 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT15 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][14]*p_record->Tvol[i][j][slavenum][0];
			}
		}

		title = "OUTPUT15 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT16 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][15]*p_record->Tvol[i][j][slavenum][0];
			}
		}

		title = "OUTPUT16 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT17 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][16]*p_record->Tvol[i][j][slavenum][0];
			}
		}

		title = "OUTPUT17 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT18 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][17]*p_record->Tvol[i][j][slavenum][0];
			}
		}

		title = "OUTPUT18 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT19 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][18]*p_record->Tvol[i][j][slavenum][0];
			}
		}

		title = "OUTPUT19 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT20 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][19]*p_record->Tvol[i][j][slavenum][0];
			}
		}

		title = "OUTPUT20 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT21 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][20]*p_record->Tvol[i][j][slavenum][0];
			}
		}

		title = "OUTPUT21 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT22 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][21]*p_record->Tvol[i][j][slavenum][0];
			}
		}

		title = "OUTPUT22 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT23 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][22]*p_record->Tvol[i][j][slavenum][0];
			}
		}

		title = "OUTPUT23 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT24 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][23]*p_record->Tvol[i][j][slavenum][0];
			}
		}

		title = "OUTPUT24 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT1 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][0];
			}
		}

		title = "OUTPUT1 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT2 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][1];
			}
		}

		title = "OUTPUT2 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT3 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][2];
			}
		}

		title = "OUTPUT3 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT4 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][3];
			}
		}

		title = "OUTPUT4 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT5 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][4];
			}
		}

		title = "OUTPUT5 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT6 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][5];
			}
		}

		title = "OUTPUT6 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT7 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][6];
			}
		}

		title = "OUTPUT7 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT8 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][7];
			}
		}

		title = "OUTPUT8 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT9 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][8];
			}
		}

		title = "OUTPUT9 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT10 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][9];
			}
		}

		title = "OUTPUT10 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT11 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][10];
			}
		}

		title = "OUTPUT11 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT12 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][11];
			}
		}

		title = "OUTPUT12 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT13 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][12];
			}
		}

		title = "OUTPUT13 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT14 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][13];
			}
		}

		title = "OUTPUT14 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT15 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][14];
			}
		}

		title = "OUTPUT15 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT16 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][15];
			}
		}

		title = "OUTPUT16 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT17 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][16];
			}
		}

		title = "OUTPUT17 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT18 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][17];
			}
		}

		title = "OUTPUT18 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT19 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][18];
			}
		}

		title = "OUTPUT19 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT20 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][19];
			}
		}

		title = "OUTPUT20 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT21 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][20];
			}
		}

		title = "OUTPUT21 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT22 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][21];
			}
		}

		title = "OUTPUT22 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT23 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][22];
			}
		}

		title = "OUTPUT23 ����";
		fillcurrent(cur);
	}
	else if(type == "OUTPUT24 ����")
	{
		float cur[24][60];
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				cur[i][j] = p_record->cur[i][j][slavenum][23];
			}
		}

		title = "OUTPUT24 ����";
		fillcurrent(cur);
	}
	else
	{
		CDC *pDC;
		pDC = GetDC();
		CRect rect;
		GetClientRect(&rect);
		int i;
		for(i=0;i<25;i++)
		{
			x_zhou[i] = rect.bottom-50;
		}
		title = "��ѡ����Ҫ��ʾ������";
		drawgraph(pDC,0);
		ReleaseDC(pDC);
		delete p_record;
		return;
	}
	///////////////////////////////
	delete p_record;
}

void CStatusLogs::fillcurrent(float cur[24][60])
{
	CRect rect;
	GetClientRect(&rect);
	int i;
	int search_max_wHour,search_max_wMinute;
	int search_min_wHour,search_min_wMinute;
	search_max_wHour=search_max_wMinute=search_min_wHour=search_min_wMinute=0;
	float search_max,search_min;
	search_max=search_min=cur[0][0];
	for(i=data_y-1;i<(24/data_x+data_y-1);i++)
	{
		for(int j=0;j<60;j++)
		{
			if(search_max<cur[i][j])
			{
				search_max=cur[i][j];
				search_max_wHour=i;
				search_max_wMinute=j;
			}
			if(search_min>cur[i][j])
			{
				search_min=cur[i][j];
				search_min_wHour=i;
				search_min_wMinute=j;
			}
		}
	}
	int show_unit=0;
	CString type;
	m_com.GetWindowTextA(type);
	if((type.Find("����"))>0)
	{
		if(search_max<7040&&search_max>3520)//32A
			show_unit=7040;
		else if(search_max<3520&&search_max>2200)//16A
			show_unit=3520;
		else if(search_max<2200&&search_max>1100)//10A
			show_unit=2200;
		else if(search_max<1100&&search_max>660)//5A
			show_unit=1100;
		else if(search_max<660&&search_max>220)//3A
			show_unit=660;
		else if(search_max<220&&search_max>0)//1A
			show_unit=220;

		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				x_zhou_60[i][j] = (rect.bottom-50)-(int)( (rect.bottom-190)*(cur[i][j])/show_unit);
			}
		}
	}
	else if((type.Find("����"))>0)
	{
		if(search_max<32&&search_max>16)//32A
			show_unit=32;
		else if(search_max<16&&search_max>10)//16A
			show_unit=16;
		else if(search_max<10&&search_max>5)//10A
			show_unit=10;
		else if(search_max<5&&search_max>3)//5A
			show_unit=5;
		else if(search_max<3&&search_max>1)//3A
			show_unit=3;
		else if(search_max<1&&search_max>0)//1A
			show_unit=1;

		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				x_zhou_60[i][j] = (rect.bottom-50)-(int)( (rect.bottom-190)*(cur[i][j])/show_unit);
			}
		}
	}
	else if((type.Find("��ѹ"))>0)
	{
		show_unit=250;

		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				x_zhou_60[i][j] = (rect.bottom-50)-(int)( (rect.bottom-190)*(cur[i][j])/show_unit);
			}
		}
	}
//	else if((type.Find("�¶�"))>0)
   else if(type=="�¶�1")
	{
		show_unit=50;
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				x_zhou_60[i][j] = (rect.bottom-50)-(int)( (rect.bottom-190)*(cur[i][j])/show_unit);
			}
		}
	}

//	else if((type.Find("ʪ��"))>0)
   else if(type=="ʪ��1")
	{
		show_unit=100;    ////���ֵ
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				x_zhou_60[i][j] = (rect.bottom-50)-(int)( (rect.bottom-190)*(cur[i][j])/show_unit);
			}
		}
	}
   else if(type=="�¶�2")
	{
		show_unit=50;
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				x_zhou_60[i][j] = (rect.bottom-50)-(int)( (rect.bottom-190)*(cur[i][j])/show_unit);
			}
		}
	}

//	else if((type.Find("ʪ��"))>0)
   else if(type=="ʪ��2")
	{
		show_unit=100;    ////���ֵ
		for(i=0;i<24;i++)
		{
			for(int j=0;j<60;j++)
			{
				x_zhou_60[i][j] = (rect.bottom-50)-(int)( (rect.bottom-190)*(cur[i][j])/show_unit);
			}
		}
	}



	//x_zhou[24] = (rect.bottom-50)-(int)( (rect.bottom-190)*(cur[0]*10)/350);
	CDC *pDC;
	pDC = GetDC();
	drawgraph(pDC,show_unit);
	/////////////////////
	drawFont(pDC,0,RGB(0,128,0));
	CString textout;
	if((type.Find("����"))>0)
	{
		textout.Format("��λ: W(%d)  ��ʱ������ֵΪ��%.1f(%d-%d),��СֵΪ��%.1f��%d-%d�� ʱ�䣨Сʱ-���ӣ�",show_unit,search_max,search_max_wHour,search_max_wMinute,search_min,search_min_wHour,search_min_wMinute);
	}
	else if((type.Find("����"))>0)
	{
		textout.Format("��λ: A(%d)  ��ʱ������ֵΪ��%.1f(%d-%d),��СֵΪ��%.1f��%d-%d�� ʱ�䣨Сʱ-���ӣ�",show_unit,search_max,search_max_wHour,search_max_wMinute,search_min,search_min_wHour,search_min_wMinute);
	}
	else if((type.Find("��ѹ"))>0)
	{
		textout.Format("��λ: V(%d)  ��ʱ������ֵΪ��%.1f(%d-%d),��СֵΪ��%.1f��%d-%d�� ʱ�䣨Сʱ-���ӣ�",show_unit,search_max,search_max_wHour,search_max_wMinute,search_min,search_min_wHour,search_min_wMinute);
	}
	else if((type.Find("�¶�"))>0)
	{
		textout.Format("��λ: ��(%d)  ��ʱ������ֵΪ��%.1f(%d-%d),��СֵΪ��%.1f��%d-%d�� ʱ�䣨Сʱ-���ӣ�",show_unit,search_max,search_max_wHour,search_max_wMinute,search_min,search_min_wHour,search_min_wMinute);
	}
	else if((type.Find("ʪ��"))>0)
	{
		textout.Format("��λ: %(%d)  ��ʱ������ֵΪ��%.1f(%d-%d),��СֵΪ��%.1f��%d-%d�� ʱ�䣨Сʱ-���ӣ�",show_unit,search_max,search_max_wHour,search_max_wMinute,search_min,search_min_wHour,search_min_wMinute);
	}
	pDC->TextOut(50,120,textout);
	/*drawFont(pDC,5,RGB(128,0,0));
	for(i=0;i<24;i++)
	{
	CString value;
	value.Format("%.1f",cur[i]);
	pDC->TextOut(50+(rect.right-100)*i/24,(rect.bottom-65)-(int)( (rect.bottom-190)*(cur[i]*10)/350 ),value);
	}*/

	ReleaseDC(pDC);
}

void CStatusLogs::fillvoltage(int cur[24][60])
{
	//CRect rect;
	//GetClientRect(&rect);
	//int i;
	//for(i=0;i<24;i++)
	//{
	//	x_zhou[i] = (rect.bottom-50)-(int)( (rect.bottom-190)*(cur[i])/300 );
	//}
	//x_zhou[24] = (rect.bottom-50)-(int)( (rect.bottom-190)*(cur[0])/300 );
	//CDC *pDC;
	//pDC = GetDC();
	//drawgraph(pDC);
	///////////////////////
	//drawFont(pDC,0,RGB(0,128,0));
	//pDC->TextOut(50,120,"��λ: V");
	//drawFont(pDC,5,RGB(128,0,0));
	//for(i=0;i<24;i++)
	//{
	//	CString value;
	//	value.Format("%d",cur[i]);
	//	pDC->TextOut(50+(rect.right-100)*i/24,(rect.bottom-65)-(int)( (rect.bottom-190)*(cur[i])/300 ),value);
	//}

	//ReleaseDC(pDC);
}

void CStatusLogs::fillth(int cur[24][60])
{
	//CRect rect;
	//GetClientRect(&rect);
	//int i;
	//for(i=0;i<24;i++)
	//{
	//	x_zhou[i] = (rect.bottom-50)-(int)( (rect.bottom-190)*(cur[i])/120 );
	//}
	//x_zhou[24] = (rect.bottom-50)-(int)( (rect.bottom-190)*(cur[0])/120 );
	//CDC *pDC;
	//pDC = GetDC();
	//drawgraph(pDC);
	///////////////////////
	//drawFont(pDC,0,RGB(0,128,0));
	//pDC->TextOut(50,120,"�¶�:�� ʪ��:%");
	//drawFont(pDC,5,RGB(128,0,0));
	//for(i=0;i<24;i++)
	//{
	//	CString value;
	//	value.Format("%d",cur[i]);
	//	pDC->TextOut(50+(rect.right-100)*i/24,(rect.bottom-65)-(int)( (rect.bottom-190)*(cur[i])/120 ),value);
	//}

	//ReleaseDC(pDC);
}

LRESULT CStatusLogs::OnNcHitTest(CPoint point)
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

void CStatusLogs::putdata_float(CString text,float data[60])
{
	sSql.Format("INSERT INTO Exceldemo (��ϸ,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60) VALUES ('%s','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f','%.1f')"
		,text
		,data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7],data[8],data[9],data[10],data[11],data[12]
	,data[13],data[14],data[15],data[16],data[17],data[18],data[19],data[20],data[21],data[22],data[23],data[24]
	,data[25],data[26],data[27],data[28],data[29],data[30],data[31],data[32],data[33],data[34],data[35],data[36]
	,data[37],data[38],data[39],data[40],data[41],data[42],data[43],data[44],data[45],data[46],data[47],data[48]
	,data[49],data[50],data[51],data[52],data[53],data[54],data[55],data[56],data[57],data[58],data[59]); 

}

void CStatusLogs::putdata_int(CString text,int data[60])
{
	sSql.Format("INSERT INTO Exceldemo (��ϸ,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60) VALUES ('%s','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d')"
		,text
		,data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7],data[8],data[9],data[10],data[11],data[12]
	,data[13],data[14],data[15],data[16],data[17],data[18],data[19],data[20],data[21],data[22],data[23],data[24]
	,data[25],data[26],data[27],data[28],data[29],data[30],data[31],data[32],data[33],data[34],data[35],data[36]
	,data[37],data[38],data[39],data[40],data[41],data[42],data[43],data[44],data[45],data[46],data[47],data[48]
	,data[49],data[50],data[51],data[52],data[53],data[54],data[55],data[56],data[57],data[58],data[59]); 

}

void CStatusLogs::exportexl()
{
	//CString ip;
	//GetDlgItem(IDC_EDIT1)->GetWindowTextA(ip);
	//if(!ip.GetLength())
	//{
	//	AfxMessageBox("�������豸IP��ַ");
	//	return;
	//}
	CString log_name;
	GetDlgItem(IDC_EDIT8)->GetWindowTextA(log_name);
	if(!log_name.GetLength())
	{
		AfxMessageBox("��־���ֲ���Ϊ��");
		return;
	}
	CString slave;
	GetDlgItem(IDC_EDIT2)->GetWindowTextA(slave);
	int slavenum;
	slavenum=atoi(slave);
	if(slavenum<0 || slavenum>9)
	{
		AfxMessageBox("�������豸����������0��������1-9Ϊ������");
		return;
	}
	CTime et1;
	tm1.GetTime(et1);
	CString filename;

	CString filename1;
	filename1.Format("%4d%2d%2d",et1.GetYear(),et1.GetMonth(),et1.GetDay());
	filename1.Replace(" ","0");

	CTime et2;
	tm2.GetTime(et2);
	CString filename2;
	filename2.Format("%4d%2d%2d",et2.GetYear(),et2.GetMonth(),et2.GetDay());
	filename2.Replace(" ","0");
	struct recorddata
	{
		  float cur[24][60][10][24];
		  float Tcur[24][60][10][3];
	      int Tvol[24][60][10][3];
		  int th[24][60][10][6];
		  recorddata()
		  {

			   memset(cur,0,sizeof(cur));
			   memset(Tcur,0,sizeof(Tcur));
			   memset(Tvol,0,sizeof(Tvol));
			   memset(th,0,sizeof(th));
		 
		  }

	};


    
	CFile file;
	recorddata *p_record;
	p_record = new recorddata;

	int m=0;
	CString ip;
	GetDlgItem(IDC_COMBO2)->GetWindowTextA(ip);
	POSITION posnow = snmpobj.GetTailPosition ();
	//while (posnow != NULL)
	//{
		CSnmpObj *p_Objectnow = NULL;
		p_Objectnow = ((CSnmpObj*)(snmpobj.GetPrev (posnow)));
//		if(p_Objectnow->ipaddr.GetLength()&&p_Objectnow->ipaddr)
        if(ip)
		{
//			ip.Format("%s",p_Objectnow->ipaddr);;

			filename.Format("%s",filename1);
			CDatabase database;  
			CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel��װ����
			CString sExcelFile,sPath; 
			CString strName;  

			//��ȡ����������·��,����sPath��   
			GetModuleFileName(NULL,sPath.GetBufferSetLength (MAX_PATH+1),MAX_PATH);  
			sPath.ReleaseBuffer ();  
			int nPos;  
			nPos=sPath.ReverseFind ('\\');  
			sPath=sPath.Left (nPos);
			strName.Format("\\%s-%s.xls",log_name,ip);
			sPath += strName;
			sExcelFile = sPath;             // Ҫ������Excel�ļ�

			try  
			{ 
				sSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",sDriver, sExcelFile, sExcelFile); 
				if(database.OpenEx(sSql,CDatabase::noOdbcDialog))  
				{ 
					sSql = "CREATE TABLE Exceldemo (��ϸ char,1 char,2 char,3 char,4 char,5 char,6 char,7 char,8 char,9 char,10 char,11 char,12 char,13 char,14 char,15 char,16 char,17 char,18 char,19 char,20 char,21 char,22 char,23 char,24 char,25 char,26 char,27 char,28 char,29 char,30 char,31 char,32 char,33 char,34 char,35 char,36 char,37 char,38 char,39 char,40 char,41 char,42 char,43 char,44 char,45 char,46 char,47 char,48 char,49 char,50 char,51 char,52 char,53 char,54 char,55 char,56 char,57 char,58 char,59 char,60 char)";  
					//////////////////////////////////////////////////////////////////////////////////////////��ʼ��ȡÿ��ļ�¼
					bool day_bool=true;
					while(day_bool)
					{
						if(filename==filename2)
						{
							database.ExecuteSQL(sSql);  
							//output current 2
							sSql.Format("INSERT INTO Exceldemo  (��ϸ,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60) VALUES ('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",filename,"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""); 
							database.ExecuteSQL(sSql); 
							if(file.Open(getFilePath()+"\\status\\"+ip+"\\"+filename,CFile::modeRead))
							{
								memset(p_record,0,sizeof(recorddata));
								file.Read(p_record,sizeof(recorddata));
								file.Close();
							}

							for(int i=0;i<24;i++)
							{
								CString time;
								time.Format("ʱ��-%d",i);
								sSql.Format("INSERT INTO Exceldemo (��ϸ,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60) VALUES ('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",time,"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""); 
								database.ExecuteSQL(sSql); 

								float data_float[60];
								int data_int[60];
								for(int j=0;j<60;j++)
								{
									data_float[j]=p_record->Tcur[i][j][slavenum][0];//cur
								}
								putdata_float("�ܵ���",data_float);
								database.ExecuteSQL(sSql);

								for(int j=0;j<60;j++)
								{
									data_int[j]=p_record->Tvol[i][j][slavenum][0];//cur
								}
								putdata_int("�ܵ�ѹ",data_int);
								database.ExecuteSQL(sSql);

								for(int j=0;j<60;j++)
								{
									data_float[j]=p_record->Tvol[i][j][slavenum][0]*p_record->Tcur[i][j][slavenum][0];//cur
								}
								putdata_float("�ܹ���",data_float);
								database.ExecuteSQL(sSql);

								for(int j=0;j<60;j++)
								{
									data_int[j]=p_record->th[i][j][slavenum][0];//cur
								}
								putdata_int("�¶�",data_int);
								database.ExecuteSQL(sSql);

								for(int j=0;j<60;j++)
								{
									data_int[j]=p_record->th[i][j][slavenum][3];//cur
								}
								putdata_int("ʪ��",data_int);
								database.ExecuteSQL(sSql);

								for(int mm=0;mm<24;mm++)
								{
									for(int j=0;j<60;j++)
									{
										data_float[j]=p_record->cur[i][j][slavenum][mm];//cur
									}
									CString text;
									text.Format("%s-%d","�ֵ���",mm+1);
									putdata_float(text,data_float);
									database.ExecuteSQL(sSql);
								}

								for(int mm=0;mm<24;mm++)
								{
									for(int j=0;j<60;j++)
									{
										data_float[j]=p_record->cur[i][j][slavenum][mm]*p_record->Tvol[i][j][slavenum][0];//cur
									}
									CString text;
									text.Format("%s-%d","��Ԫ����",mm+1);
									putdata_float(text,data_float);
									database.ExecuteSQL(sSql);
								}
							}

							day_bool=false;
							
						}
						else
						{
							database.ExecuteSQL(sSql);  
							//output current 2
							sSql.Format("INSERT INTO Exceldemo  (��ϸ,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60) VALUES ('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",filename,"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""); 
							database.ExecuteSQL(sSql); 
							if(file.Open(getFilePath()+"\\status\\"+ip+"\\"+filename,CFile::modeRead))
							{
								memset(p_record,0,sizeof(recorddata));
								file.Read(p_record,sizeof(recorddata));
								file.Close();
							}

							for(int i=0;i<24;i++)
							{
								CString time;
								time.Format("ʱ��-%d",i);
								sSql.Format("INSERT INTO Exceldemo (��ϸ,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60) VALUES ('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",time,"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""); 
								database.ExecuteSQL(sSql); 

								float data_float[60];
								int data_int[60];
								for(int j=0;j<60;j++)
								{
									data_float[j]=p_record->Tcur[i][j][slavenum][0];//cur
								}
								putdata_float("�ܵ���",data_float);
								database.ExecuteSQL(sSql);

								for(int j=0;j<60;j++)
								{
									data_int[j]=p_record->Tvol[i][j][slavenum][0];//cur
								}
								putdata_int("�ܵ�ѹ",data_int);
								database.ExecuteSQL(sSql);

								for(int j=0;j<60;j++)
								{
									data_float[j]=p_record->Tvol[i][j][slavenum][0]*p_record->Tcur[i][j][slavenum][0];//cur
								}
								putdata_float("�ܹ���",data_float);
								database.ExecuteSQL(sSql);

								for(int j=0;j<60;j++)
								{
									data_int[j]=p_record->th[i][j][slavenum][0];//cur
								}
								putdata_int("�¶�",data_int);
								database.ExecuteSQL(sSql);

								for(int j=0;j<60;j++)
								{
									data_int[j]=p_record->th[i][j][slavenum][3];//cur
								}
								putdata_int("ʪ��",data_int);
								database.ExecuteSQL(sSql);

								for(int mm=0;mm<24;mm++)
								{
									for(int j=0;j<60;j++)
									{
										data_float[j]=p_record->cur[i][j][slavenum][mm];//cur
									}
									CString text;
									text.Format("%s-%d","�ֵ���",mm+1);
									putdata_float(text,data_float);
									database.ExecuteSQL(sSql);
								}

								for(int mm=0;mm<24;mm++)
								{
									for(int j=0;j<60;j++)
									{
										data_float[j]=p_record->cur[i][j][slavenum][mm]*p_record->Tvol[i][j][slavenum][0];//cur
									}
									CString text;
									text.Format("%s-%d","��Ԫ����",mm+1);
									putdata_float(text,data_float);
									database.ExecuteSQL(sSql);
								}
							}

							int year,month,day,year2,month2,day2;

							year=atoi(filename.Left(4));
							month=atoi(filename.Mid(4,2));
							day=atoi(filename.Right(2));

							year2=atoi(filename2.Left(4));
							month2=atoi(filename2.Mid(4,2));
							day2=atoi(filename2.Right(2));

							if(year==year2)
							{
								if(month==month2)
								{
									if(month==1||month==3||month==5||month==7||month==8||month==10||month==12)
									{
										if(day<=31)
											day++;
										if(day>31)
										{
											year+=1;
											month=1;
											day=1;
										}
									}
									else if(month==4||month==6||month==9||month==11)
									{
										if(day<30)
											day++;
										if(day==30)
										{
											month++;
											day=1;
										}
									}
									else
									{
										if((year/4==0&&year/100!=0)||(year/400==0))
										{
											if(day<29)
												day++;
											if(day==29)
											{
												month++;
												day=1;
											}
										}
										else
										{
											if(day<28)
												day++;
											if(day==28)
											{
												month++;
												day=1;
											}
										}
									}
								}
								else if(month<month2)
								{
									if(month==1||month==3||month==5||month==7||month==8||month==10||month==12)
									{
										if(day<=31)
											day++;
										if(day>31)
										{
											year+=1;
											month=1;
											day=1;
										}
									}
									else if(month==4||month==6||month==9||month==11)
									{
										if(day<30)
											day++;
										if(day==30)
										{
											month++;
											day=1;
										}
									}
									else
									{
										if((year/4==0&&year/100!=0)||(year/400==0))
										{
											if(day<29)
												day++;
											if(day==29)
											{
												month++;
												day=1;
											}
										}
										else
										{
											if(day<28)
												day++;
											if(day==28)
											{
												month++;
												day=1;
											}
										}
									}
								}
							}
							else if(year<year2)
							{
								if(month==1||month==3||month==5||month==7||month==8||month==10||month==12)
								{
									if(day<=31)
										day++;
									if(day>31)
									{
										year+=1;
										month=1;
										day=1;
									}
								}
								else if(month==4||month==6||month==9||month==11)
								{
									if(day<30)
										day++;
									if(day==30)
									{
										month++;
										day=1;
									}
								}
								else
								{
									if((year/4==0&&year/100!=0)||(year/400==0))
									{
										if(day<29)
											day++;
										if(day==29)
										{
											month++;
											day=1;
										}
									}
									else
									{
										if(day<28)
											day++;
										if(day==28)
										{
											month++;
											day=1;
										}
									}
								}
							}
							else
							{
								day_bool=false;
								AfxMessageBox("ʱ����д����");
							}
							filename.Format("%4d%2d%2d",year,month,day);
							filename.Replace(" ","0");
						}
					}
				}
			}
			catch(CUserException ex)  
			{  
				ex.ReportError();  
			}
		//}
	}
	AfxMessageBox("�ɹ��������ݣ�");
	delete p_record;
}
int CStatusLogs::year_ret(CString str_year)
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