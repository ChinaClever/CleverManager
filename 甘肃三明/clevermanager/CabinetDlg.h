#pragma once


// CCabinetDlg �Ի���

class CCabinetDlg : public CDialog
{
	DECLARE_DYNAMIC(CCabinetDlg)

public:
	CCabinetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCabinetDlg();

// �Ի�������
	enum { IDD = IDD_CABINETDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};
