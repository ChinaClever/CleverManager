#pragma once


// COverTime �Ի���

class COverTime : public CDialog
{
	DECLARE_DYNAMIC(COverTime)

public:
	COverTime(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COverTime();

// �Ի�������
	enum { IDD = IDD_OVERTIME};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};
