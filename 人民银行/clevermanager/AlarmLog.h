#pragma once
#include "afxdtctl.h"


// COutputLog �Ի���

class CAlarmLog : public CDialog
{
	DECLARE_DYNAMIC(CAlarmLog)

public:
	CAlarmLog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAlarmLog();

// �Ի�������
	enum { IDD = IDD_ALARMLOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	int logtime(CString str1 , CString str2 , CString str3);
	CDateTimeCtrl m_time1;
	CDateTimeCtrl m_time2;
	afx_msg LRESULT OnNcHitTest(CPoint point);
};