// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once

class CMainFrame : public CFrameWnd
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:

// ����
public:
	afx_msg LONG msgWriteLogs(WPARAM wParam,LPARAM lParam);
	//afx_msg LONG msgDelMem(WPARAM wParam,LPARAM lParam);

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	//NOTIFYICONDATA tnd;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnTray(WPARAM wParam,LPARAM lParam);
	afx_msg void OnShowClevermanager();
	afx_msg void OnCloseClevermanager();
	afx_msg void OnDestroy();
	void Systnd();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};


