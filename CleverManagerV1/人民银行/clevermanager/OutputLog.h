#pragma once
#include "afxdtctl.h"


// COutputLog �Ի���

class COutputLog : public CDialog
{
	DECLARE_DYNAMIC(COutputLog)

public:
	COutputLog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COutputLog();

// �Ի�������
	enum { IDD = IDD_OUTPUTLOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int zhang(CString str1 , CString str2 , CString str3);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CDateTimeCtrl m_time1;
	CDateTimeCtrl m_time2;
	afx_msg LRESULT OnNcHitTest(CPoint point);
};
