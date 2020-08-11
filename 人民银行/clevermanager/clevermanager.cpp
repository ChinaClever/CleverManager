// clevermanager.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "clevermanager.h"
#include "MainFrm.h"

#include "clevermanagerDoc.h"
#include "clevermanagerView.h"
#include "InitClever.h"
#include "OverTime.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
typedef int  (*DE)(char *,  char *, int );
typedef int  (*EN)(char * ,int ,char *);

// CclevermanagerApp

BEGIN_MESSAGE_MAP(CclevermanagerApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CclevermanagerApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CclevermanagerApp ����

CclevermanagerApp::CclevermanagerApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CclevermanagerApp ����

CclevermanagerApp theApp;
datacenter DCmember[10];
CObList snmpobj;
CString currentIP1;//��ǰ�鿴IP
CString currentIP2;//��ǰ�鿴IP
userinfo user[20];
bool gShowFlag;//�Զ�ˢ����־��־λfalse//��ˢ�� true //ˢ��

CString changevalue(CString strHex)
{
	CString str;
	CString strSrc;
	CString strTmp;
	long Hex;
	int i =0;
	strHex.Trim();
	strHex.Replace(" ","");
	while(i < strHex.GetLength())
	{  
		strSrc = strHex.Mid(i,2); 
		i += 2;   
		Hex = _tcstoul(strSrc, NULL, 16);
		strTmp += (TCHAR)Hex;
	}
	return strTmp;
}
CString getFilePath(void)
{
	CString sPath;
	GetModuleFileName(NULL,sPath.GetBufferSetLength (MAX_PATH+1),MAX_PATH);
	sPath.ReleaseBuffer();
	int nPos;
	nPos=sPath.ReverseFind (_T('\\'));
	sPath=sPath.Left(nPos);
	return sPath;
}
void drawImagePNG(CDC *pDC,CString src,int px,int py,int w,int h)
{
	Graphics graphics( pDC->GetSafeHdc());
	LPWSTR des =NULL;
	des = new WCHAR[src.GetLength()+2];
	memset(des,0,sizeof(WCHAR)*(src.GetLength()+2));
	
	MultiByteToWideChar(CP_ACP,0,src,src.GetLength(),des,src.GetLength()+2);

	
	Image image(des);
	SelectObject( pDC->m_hDC, &image);
	//if(w && h)
	//{
	//	graphics.DrawImage(&image, px,py,w,h);
	//}
	if(w || h)
	{
		graphics.DrawImage(&image, px,py,w,h);
	}
	else
	{
		graphics.DrawImage(&image, px,py);
	}
	delete[] des;
	graphics.ReleaseHDC(pDC->GetSafeHdc()); 
}
void drawFont(CDC *pDC,int type,COLORREF color)
{
	if(type==0)//����
	{
		CFont font;
		font.CreateFont(
						  12,                        // nHeight
						  0,                         // nWidth
						  0,                         // nEscapement
						  0,                         // nOrientation
						  FW_NORMAL,                 // nWeight
						  0,                     // bItalic
						  FALSE,                     // bUnderline
						  0,                         // cStrikeOut
						  ANSI_CHARSET,              // nCharSet
						  OUT_DEFAULT_PRECIS,        // nOutPrecision
						  CLIP_DEFAULT_PRECIS,       // nClipPrecision
						  DEFAULT_QUALITY,           // nQuality
						  DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
						  "����");                 // lpszFacename
		pDC->SelectObject(&font);
		pDC->SetTextColor(color);
	}
	else if(type==1)//����
	{
		CFont font;
		font.CreateFont(
						  14,                        // nHeight
						  0,                         // nWidth
						  0,                         // nEscapement
						  0,                         // nOrientation
						  FW_MEDIUM,                 // nWeight
						  0,                     // bItalic
						  FALSE,                     // bUnderline
						  0,                         // cStrikeOut
						  ANSI_CHARSET,              // nCharSet
						  OUT_DEFAULT_PRECIS,        // nOutPrecision
						  CLIP_DEFAULT_PRECIS,       // nClipPrecision
						  DEFAULT_QUALITY,           // nQuality
						  DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
						  "����");                 // lpszFacename
		pDC->SelectObject(&font);
		pDC->SetTextColor(color);
	}
	else if(type==2)//����
	{
		CFont font;
		font.CreateFont(
						  18,                        // nHeight
						  0,                         // nWidth
						  0,                         // nEscapement
						  0,                         // nOrientation
						  FW_MEDIUM,                 // nWeight
						  0,                     // bItalic
						  FALSE,                     // bUnderline
						  0,                         // cStrikeOut
						  ANSI_CHARSET,              // nCharSet
						  OUT_DEFAULT_PRECIS,        // nOutPrecision
						  CLIP_DEFAULT_PRECIS,       // nClipPrecision
						  DEFAULT_QUALITY,           // nQuality
						  DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
						  "����");                 // lpszFacename
		pDC->SelectObject(&font);
		pDC->SetTextColor(color);
	}
	else if(type==3)//����
	{
		CFont font;
		font.CreateFont(
						  24,                        // nHeight
						  0,                         // nWidth
						  0,                         // nEscapement
						  0,                         // nOrientation
						  FW_MEDIUM,                 // nWeight
						  0,                     // bItalic
						  FALSE,                     // bUnderline
						  0,                         // cStrikeOut
						  ANSI_CHARSET,              // nCharSet
						  OUT_DEFAULT_PRECIS,        // nOutPrecision
						  CLIP_DEFAULT_PRECIS,       // nClipPrecision
						  DEFAULT_QUALITY,           // nQuality
						  DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
						  "����");                 // lpszFacename
		pDC->SelectObject(&font);
		pDC->SetTextColor(color);
	}
	else if(type==4)//����
	{
		CFont font;
		font.CreateFont(
						  18,                        // nHeight
						  0,                         // nWidth
						  0,                         // nEscapement
						  0,                         // nOrientation
						  FW_SEMIBOLD,                 // nWeight
						  0,                     // bItalic
						  FALSE,                     // bUnderline
						  0,                         // cStrikeOut
						  ANSI_CHARSET,              // nCharSet
						  OUT_DEFAULT_PRECIS,        // nOutPrecision
						  CLIP_DEFAULT_PRECIS,       // nClipPrecision
						  DEFAULT_QUALITY,           // nQuality
						  DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
						  "����");                 // lpszFacename
		pDC->SelectObject(&font);
		pDC->SetTextColor(color);
	}
	else if(type==5)//����
	{
		CFont font;
		font.CreateFont(
						  10,                        // nHeight
						  0,                         // nWidth
						  0,                         // nEscapement
						  0,                         // nOrientation
						  FW_NORMAL,                 // nWeight
						  0,                     // bItalic
						  FALSE,                     // bUnderline
						  0,                         // cStrikeOut
						  ANSI_CHARSET,              // nCharSet
						  OUT_DEFAULT_PRECIS,        // nOutPrecision
						  CLIP_DEFAULT_PRECIS,       // nClipPrecision
						  DEFAULT_QUALITY,           // nQuality
						  DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
						  "����");                 // lpszFacename
		pDC->SelectObject(&font);
		pDC->SetTextColor(color);
	}
}
// CclevermanagerApp ��ʼ��
void makesureDir(CString dir)//�ļ����Ƿ����
{
	CreateDirectoryDir((LPSTR)(LPCTSTR)dir);
	CFileFind   filedir; 
	if(filedir.FindFile(dir))
	{
		filedir.Close();
		return;
	}
	else
	{
		_mkdir(dir);
		filedir.Close();
	}
}

// �ж�Ŀ¼(����������ѯ��Ŀ¼,��������ʧ��.)�Ƿ���ڵĺ���
BOOL IsPathExist(char* lpszPath)
{
	//char* szTemp = W2A(lpszPath);
	char szTemp[MAX_PATH]={0};
	strcpy(szTemp, lpszPath);

	BOOL bExist; // ���������ʾĿ¼�Ƿ����
	if(szTemp[strlen(lpszPath)-1]=='\\')
	{
		szTemp[strlen(lpszPath)-1]='\0';    // ��ɾ�����ġ�\��
	}
	WIN32_FIND_DATAA wfd;                                  // ����
	HANDLE hFind = FindFirstFileA(szTemp, &wfd);                  
	if(hFind == INVALID_HANDLE_VALUE)
	{
		bExist = FALSE;            // û���ҵ��䱸��Ŀ¼�϶�������
	}
	else
	{
		if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // ����ҵ��Ľ���Ƿ�Ŀ¼
			bExist = TRUE;     // ��Ŀ¼,Ŀ¼����
		else
			bExist = FALSE;       // ���ļ�,Ŀ¼������
		FindClose(hFind);
	}
	return bExist;
}

//
// ����: CreateDirectoryEx()
// ����: 
//    ��������·��, ��ʹ�Ƕ༶·��, Ҳ���Դ���. (ע��: ������·�����ܰ���'\'��β, �����Ǹ�Ŀ¼��: C:\ )
// ʾ��: CreateDirectoryEx( "D:\\Path1\\Path2\\..." )
// ����: 
//   lpszCreatePath -- Ҫ������Ŀ¼
// ����ֵ:
//       ����ɹ���������Ŀ¼����TRUE, ���򷵻�FALSE
// ����: SenkoBQB
// ��������: 2009-1-18        
BOOL CreateDirectoryDir(char* lpszCreatePath)
{
	char szCreatePathTemp[MAX_PATH]={0};
	//USES_CONVERSION;
	//char* szCreatePathTemp = W2A(lpszCreatePath);
	strcpy(szCreatePathTemp, lpszCreatePath);
	if(szCreatePathTemp[strlen(szCreatePathTemp)-1]=='\\')
	{
		szCreatePathTemp[strlen(szCreatePathTemp)-1]='\0';    // ��ɾ�����ġ�\��
	}
	char szTemp[MAX_PATH] = ""; // ��ʱ����
	// ����·���ָ�����'\'
	char *p = strchr(szCreatePathTemp, '\\'); // ��һ�εĳ���λ��(��ַ)
	if (p != NULL)
	{
		p = strchr(p+1, '\\');   // �ڶ���λ��, �����NULL ��˵����һ��Ŀ¼, ��: c:\path ���������Ŀ¼�����ڻ�ֱ�Ӵ�����Ŀ¼
	}    
	while (p != NULL)            // ֮���λ��(Ҫ�ܽ����ѭ��, �����Ƕ���Ŀ¼, ��: c:\һ��Ŀ¼\����Ŀ¼\...)
	{
		strncpy(szTemp, szCreatePathTemp, p-szCreatePathTemp);     // ��: e:\1
		if (!IsPathExist(szTemp)) // ���������, ��һ·�����������ڶ���Ŀ¼
		{
			// ����
			if (!CreateDirectoryA(szTemp, NULL)) 
			{
				printf("����Ŀ¼%sʧ��!�������: %d\n", szTemp, GetLastError());
				return FALSE;
			}
			printf("�ѳɹ�����Ŀ¼:%s\n", szTemp); // �����ò�Ŀ¼, ֮��һ·���´��������ж�

			p = strchr(p+1, '\\');   // ��һ����Ŀ¼
			while( p != NULL )
			{
				strncpy(szTemp, szCreatePathTemp, p-szCreatePathTemp); // ��: e:\1\2
				// ����
				if (!CreateDirectoryA(szTemp, NULL)) 
				{
					printf("����Ŀ¼%sʧ��!�������: %d\n", szTemp, GetLastError());
					return FALSE;
				}
				p = strchr(p+1, '\\'); 
			}
		}
		if ( p != NULL ) // ��ֹ�����ڲ��while
		{
			p = strchr(p+1, '\\');   // ��һ����Ŀ¼
		}
	}
	if (!IsPathExist(lpszCreatePath)) // ���Ŀ¼������, �򴴽�Ŀ¼.(Ҳ�Ǵ������һ��Ŀ¼)
	{
		// ����
		if (!CreateDirectoryA(lpszCreatePath, NULL)) 
		{
			printf("����Ŀ¼%sʧ��!�������: %d\n", lpszCreatePath, GetLastError());
			return FALSE;
		}
		printf("�ѳɹ�����Ŀ¼:%s\n", lpszCreatePath); 
	}


	return TRUE;
} 

bool makesureFile(CString name)//�ļ����Ƿ����
{
	CFileFind   filedir; 
	if(filedir.FindFile(name))
	{
		filedir.Close();
		return true;
	}
	else
	{
		filedir.Close();
		return false;
	}
}

void retange(CDC *pDC, int x, int y, int w, int h)
{
	Gdiplus::Graphics graphics(pDC->GetSafeHdc());
	LinearGradientBrush linGrBrush(Point(x,y),Point(x,y+h),Color(128,255,255,255),Color(128,255,255,255));
	//LinearGradientBrush linGrBrush;
	Color colors[] = {
		Color(128, 255,255,255),   // red
		Color(128, 225,225,240),//yellow
		Color(128, 164,164,225),   // blue
		Color(128, 128,128,164)};  // green

		REAL positions[] = {
			0.0f,  
			0.4f,
			0.8f,
			1.0f}; 
			linGrBrush.SetInterpolationColors(colors, positions,4);
			graphics.FillRectangle(&linGrBrush,x,y,w,h);
	graphics.ReleaseHDC(pDC->GetSafeHdc());
}
CString changeToUtf8(CString body)
{
	CString   str; 
	str = body;
	int   nLen   =   MultiByteToWideChar(CP_ACP,   0,   (LPCTSTR)str,   -1,   NULL,0); 
	WCHAR   *wszUTF   =   new   WCHAR[nLen+1]; 
	MultiByteToWideChar(CP_ACP,   0,   (LPCTSTR)str,   -1,   wszUTF,   nLen); 
	nLen   =   WideCharToMultiByte(CP_UTF8,   0,   wszUTF,   -1,   NULL,   0,   NULL,   NULL); 
	TCHAR   *szUTF   =   new   TCHAR[nLen+1]; 
	WideCharToMultiByte(CP_UTF8,   0,   wszUTF,   -1,   szUTF,   nLen,   NULL,NULL); 
	
	CString  wecrec(szUTF);
	delete[]   szUTF; 
	delete[]   wszUTF;
	return wecrec;
}

void ReadString(HKEY key,CString valuename,CString & value)
{
	DWORD type;
	DWORD len =_MAX_PATH;
	BYTE s[_MAX_PATH];
	memset(&s,0,sizeof(BYTE)*_MAX_PATH);
	if (RegQueryValueEx(key,valuename,0,&type,s,&len)==ERROR_SUCCESS)
	{
		value = (TCHAR *) s;
	}
	return;
}

BOOL CclevermanagerApp::InitInstance()
{
	CInitClever cleverinit;
	cleverinit.initData();
	/////////////////////////////////////////////////////////////////
	/*
	HINSTANCE hdelib =NULL;
	hdelib = NULL;
	hdelib = ::LoadLibrary(getFilePath()+"\\dedll.dll");
	DE p_de = NULL;
	EN  p_en = NULL;
	if(NULL == (p_de = (DE)GetProcAddress(hdelib,"?de@@YAXPAD0H@Z"))) {
        ::FreeLibrary(hdelib);
		hdelib = NULL;
		return FALSE;
    }

	if(NULL == (p_en = (EN)GetProcAddress(hdelib,"?en@@YA_NPADH0@Z"))) {
        ::FreeLibrary(hdelib);
		hdelib = NULL;
		return FALSE;
    }

	HKEY key;
	RegOpenKey(HKEY_LOCAL_MACHINE,"Software\\cl", &key);

	CString decode;
	CString encode;
    ReadString(key,"cl1",decode);
	ReadString(key,"cl2",encode);
	if(!decode.GetLength() && !encode.GetLength())
	{
		AfxMessageBox(_T("Software has not been register!"),MB_OK | MB_APPLMODAL | MB_ICONSTOP);
		return FALSE;
	}
	char encodechar[255];
	memset(encodechar,0,sizeof(char)*255);
	char decodechar[255];
	memset(decodechar,0,sizeof(char)*255);
	strcpy_s(decodechar,sizeof(char)*254,decode);
	p_en(decodechar,decode.GetLength(),encodechar);
	CString regcode;
	regcode = encodechar;
	regcode.Replace("=","");
	if(regcode != encode)
	{
		::FreeLibrary(hdelib);
		hdelib = NULL;
		AfxMessageBox(_T("Software has not been register!"),MB_OK | MB_APPLMODAL | MB_ICONSTOP);
		return FALSE;
	}

    ::FreeLibrary(hdelib);
		hdelib = NULL;
	//////////////////////////////////////////////////////////////////////////////////
	HANDLE cleverMutex;
	cleverMutex=INVALID_HANDLE_VALUE;
	cleverMutex=CreateMutex(NULL,true,"clevermutex");
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		 AfxMessageBox(_T("applications may be repeat or applications is ending,please wait!"),MB_OK | MB_APPLMODAL | MB_ICONSTOP);
		 return FALSE;
	}

*/
	///////////////////////////////////////////////////////////////////
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)
	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CclevermanagerDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CclevermanagerView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);



	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand  ֮����
	GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);

	//�޸�����ʱ��
/*	
	COverTime aa;
	SYSTEMTIME ct;
	GetLocalTime(&ct);
	CString str,str1;
	str.Format("%4d%2d%2d",ct.wYear,ct.wMonth,ct.wDay);
	str.Replace(" ","0");
	if(atoi(str.Left(4))==2015)
	{
		str1=str.Mid(4,2);
		if(atoi(str1)>7)
		{
			aa.DoModal();
		}
		else if(atoi(str1)==7)
		{
			str1=str.Right(2);
			if(atoi(str1)>30)
			{
				aa.DoModal();
			}
		}
	}
	else if(atoi(str.Left(4))>2015)
		aa.DoModal();*/	
	
	return TRUE;
}



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CclevermanagerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CclevermanagerApp ��Ϣ�������


int CclevermanagerApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���

	GdiplusShutdown(m_gdiplusToken); 
	CInitClever cleverinit;
	cleverinit.ExitClever();

	return CWinApp::ExitInstance();
}
