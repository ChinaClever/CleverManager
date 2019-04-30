// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "clevermanager.h"
#include "DlgDataCenterMap.h"
#include "MainFrm.h"
#include "SnmpObj.h"
#include "PopDlgInfo.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_MESSAGE(SEND_MSG_WRITELOG,msgWriteLogs)
	//ON_MESSAGE(IDS_DEL_OBJ_MEM,msgDelMem)
	ON_WM_CLOSE()
	//ON_MESSAGE(WM_TRAY, &OnTray)
	ON_WM_CREATE()
	//ON_COMMAND(ID_SHOW_CLEVERManager, &CMainFrame::OnShowClevermanager)
	//ON_COMMAND(ID_CLOSE_CLEVERManager, &CMainFrame::OnCloseClevermanager)
	//ON_WM_DESTROY()
	//ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
}

CMainFrame::~CMainFrame()
{
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;
	cs.hMenu=NULL;

	return TRUE;
}


// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame ��Ϣ�������
/*LONG CMainFrame::msgDelMem(WPARAM wParam,LPARAM lParam)
{
	struct snmpobjmem
	{
		CSnmpObj *p_obj;
		POSITION positon;
	};
	struct snmpobjmem *snmpobj_mem = NULL;
	snmpobj_mem = (struct snmpobjmem *)wParam;

	delete (snmpobj_mem->p_obj);
	snmpobj.RemoveAt(snmpobj_mem->positon);
	delete snmpobj_mem;
	return 0;
}*/

LONG CMainFrame::msgWriteLogs(WPARAM wParam,LPARAM lParam)
{
	userlogs *logs;
	logs = (userlogs *)wParam;
	int logtype;int type;CString name; CString item; CString detail;
	logtype = logs->logtype;
	type = logs->type;
	name = logs->name;
	item = logs->item;
	detail = logs->detail;
	////////////////
	SYSTEMTIME ct;
	GetLocalTime(&ct);
	CString filename;
	filename.Format("%4d%2d%2d",ct.wYear,ct.wMonth,ct.wDay);
	filename.Replace(" ","0");
	if(CreateDirectoryDir((LPSTR)(LPCTSTR)(getFilePath()+"\\logs\\"+filename+"\\"))){
	//    MakeSureDirectoryPathExists(getFilePath()+"\\logs\\"+filename+"\\");
		CFile file;
		//////////////////
		if(logtype==1)
		{
			if(makesureFile(getFilePath()+"\\logs\\"+filename+"\\users"))
			{
				file.Open(getFilePath()+"\\logs\\"+filename+"\\users",CFile::modeReadWrite);
			}
			else
			{
				file.Open(getFilePath()+"\\logs\\"+filename+"\\users",CFile::modeReadWrite|CFile::modeCreate);
			}
			file.SeekToEnd();
			char value[128];
			memset(value,0,sizeof(char)*128);
			value[0] = type;
			file.Write(value,2);

			memset(value,0,sizeof(char)*128);
			sprintf_s(value,sizeof(char)*30,"%d-%d-%d %d:%d",ct.wYear,ct.wMonth,ct.wDay,ct.wHour,ct.wMinute);
			file.Write(value,32);

			memset(value,0,sizeof(char)*128);
			strncpy_s(value,sizeof(char)*32,name,sizeof(char)*30);
			file.Write(value,32);

			memset(value,0,sizeof(char)*128);
			strncpy_s(value,sizeof(char)*32,item,sizeof(char)*30);
			file.Write(value,32);

			memset(value,0,sizeof(char)*128);
			strncpy_s(value,sizeof(char)*128,detail,sizeof(char)*126);
			file.Write(value,128);
		}
		else if(logtype==2)
		{
			if(makesureFile(getFilePath()+"\\logs\\"+filename+"\\alarm"))
			{
				file.Open(getFilePath()+"\\logs\\"+filename+"\\alarm",CFile::modeReadWrite);
			}
			else
			{
				file.Open(getFilePath()+"\\logs\\"+filename+"\\alarm",CFile::modeReadWrite|CFile::modeCreate);
			}
			file.SeekToEnd();
			char value[128];
			memset(value,0,sizeof(char)*128);
			value[0] = type;
			file.Write(value,2);

			memset(value,0,sizeof(char)*128);
			sprintf_s(value,sizeof(char)*30,"%d-%d-%d %d:%d",ct.wYear,ct.wMonth,ct.wDay,ct.wHour,ct.wMinute);
			file.Write(value,32);

			memset(value,0,sizeof(char)*128);
			strncpy_s(value,sizeof(char)*32,name,sizeof(char)*30);
			file.Write(value,32);

			memset(value,0,sizeof(char)*128);
			strncpy_s(value,sizeof(char)*32,item,sizeof(char)*30);
			file.Write(value,32);

			memset(value,0,sizeof(char)*128);
			strncpy_s(value,sizeof(char)*128,detail,sizeof(char)*126);
			file.Write(value,128);
				/*
			CRect   rt; 
			GetWindowRect(&rt);
			CPoint   pt; 
			pt=rt.BottomRight();
			CPopDlgInfo *pDlg=new CPopDlgInfo;
			pDlg->Create(IDD_POP_INFO,this);
			pDlg->MoveWindow(pt.x-315,pt.y-290,300,250,1);
			pDlg->Alarminformation(name,item,detail);
			pDlg->ShowWindow(SW_SHOW);
			CDC *pDC;
			CRect rect;
			pDC=pDlg->GetDC();
			pDlg->GetClientRect(&rect);
			pDC->TextOutA(50,25,"aa");
			pDC->TextOutA(50,25,"bb");
			/*
			////////////////����һ��ͼƬ�Ļ�ͼ����///////////////////////
			CBitmap BkMap;
			BkMap.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
			CDC BkDc;
			BkDc.CreateCompatibleDC(pDC);
			BkDc.SelectObject(&BkMap);
			SetBkMode(BkDc,TRANSPARENT);
			CBrush brush(RGB(255,255,255));
			BkDc.SelectObject(&brush);
			BkDc.Rectangle(0,24,rect.Width(),rect.Height());
			CBrush brushtitle(RGB(255,255,255));
			BkDc.SelectObject(&brushtitle);
			BkDc.Rectangle(0,0,rect.Width(),25);
			drawFont(&BkDc,1,RGB(64,64,64));
			BkDc.TextOutA(20,5,"�澯��Ϣ");
			retange(&BkDc,1,1,rect.Width()-2,23);
			drawImagePNG(&BkDc,getFilePath()+"\\icon\\close.png",rect.Width()-25,6,12,12);
			BkDc.TextOutA(50,25,name);
			BkDc.TextOutA(50,45,item);
			BkDc.TextOutA(50,65,detail);
			pDC->BitBlt(0,0,rect.Width(),rect.Height(),&BkDc,0,0,SRCCOPY);
			BkDc.DeleteDC();
			//////////////////////////////////////////////////////////
			*/

		}
		file.Close();
		//////////////
	}
	delete logs;
	return 0;
}
void CMainFrame::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	/*
	CDlgDataCenterMap aa;
	aa.OnSaveMap();
	if(MessageBox( "ȷ���˳���? ", "", MB_OKCANCEL) != IDOK) 
		return;
		*/
	CFrameWnd::OnClose();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	
	// TODO:  �ڴ������ר�õĴ�������
	//Systnd();
	return 0;
}
LRESULT CMainFrame::OnTray(WPARAM wParam,LPARAM lParam)//���ϵͳͼ��˵���Ӧ����
{
	/*CPoint pt;
	CMenu *pMenu;
	UINT uID,uMouseMsg;
	uID=(UINT)wParam;
	uMouseMsg =(UINT)lParam;
	CMenu menu;
	if(uMouseMsg==WM_RBUTTONDOWN)
	{
		switch(uID)
		{
			case IDR_MAINFRAME:
			GetCursorPos(&pt);
			menu.LoadMenu(IDR_OPEN_CLEVERMANAGER_MENU);
			pMenu=menu.GetSubMenu(0);
			ASSERT(pMenu!=0);
			pMenu->TrackPopupMenu(0,pt.x,pt.y,this); //��ʾһ�������˵���ָ����λ��
			break;
			default:
			break;
		}
	}
	else if(uMouseMsg==WM_LBUTTONDBLCLK)
	{
		
		static bool isMax=false;
		switch(uID)
		{
			case IDR_MAINFRAME:
			if(isMax==false)
			{
				//��ʾ������
				ModifyStyle(WS_POPUP,WS_OVERLAPPEDWINDOW);
				ModifyStyleEx(WS_EX_TOOLWINDOW,WS_EX_TOPMOST);
				ShowWindow(SW_SHOWMAXIMIZED);
				//::ShowWindow(m_hWnd,   SW_RESTORE);
				isMax=true;
			}
			else
			{
//WS_OVERLAPPEDWINDOW=WS_CAPTION(���б��⣩|WS_OVERLAPPED|WS_THICKFRAME(�ɵ��߿�|WS_SYSMENU������ϵͳ�˵���|WS_MINIMIZEBOX��������С����ť��|WS_MAXIMIZEBOX
				ShowWindow(SW_HIDE);
				ModifyStyle(WS_CAPTION|FWS_PREFIXTITLE|WS_SYSMENU|WS_MINIMIZEBOX|WS_MAXIMIZEBOX,WS_POPUP);
				ModifyStyleEx(WS_EX_TOPMOST,WS_EX_TOOLWINDOW);
				//::ShowWindow(m_hWnd,   SW_RESTORE);
				isMax=false;
			}
			break;
			default:
			break;
		}

	}
	*/
	return true;
}
void CMainFrame::OnShowClevermanager()//��ʾϵͳ����ͼ��
{
	// TODO: �ڴ���������������
	/*ModifyStyle(WS_POPUP,WS_OVERLAPPEDWINDOW);
	ModifyStyleEx(WS_EX_TOOLWINDOW,WS_EX_TOPMOST);
	ShowWindow(SW_SHOWMAXIMIZED);
	//::ShowWindow(m_hWnd,   SW_RESTORE);
	*/
}

void CMainFrame::OnCloseClevermanager()//ɾ��ϵͳ����ͼ��
{
	// TODO: �ڴ���������������
	//Shell_NotifyIcon(NIM_DELETE,&tnd);
	//AfxPostQuitMessage(0);
}

void CMainFrame::OnDestroy()//�˳�ʱɾ������ͼ��
{
	// TODO: �ڴ˴������Ϣ����������
	CFrameWnd::OnDestroy();
	//Shell_NotifyIcon(NIM_DELETE,&tnd);
	
}

void CMainFrame::Systnd()//ϵͳ����
{
/*	
	tnd.cbSize=sizeof(NOTIFYICONDATA);
	tnd.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	tnd.hWnd=this->m_hWnd;
	strcpy_s(tnd.szTip,sizeof(char)*126,"CLEVER Manager");
	tnd.uCallbackMessage=WM_TRAY;
	tnd.uFlags=NIF_MESSAGE | NIF_ICON | NIF_TIP;
	tnd.uID=IDR_MAINFRAME;
	Shell_NotifyIcon(NIM_ADD,&tnd);
	ShowWindow(SW_HIDE);
*/
}

void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)//��С����������
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//if(nID==SC_MINIMIZE)
	//{
		//Systnd();
	//}
	//else

	//{
		CFrameWnd::OnSysCommand(nID, lParam);
	//}
	
}
