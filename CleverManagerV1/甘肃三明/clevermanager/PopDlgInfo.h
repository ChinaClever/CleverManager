#pragma once


// CPopDlgInfo �Ի���

class CPopDlgInfo : public CDialog
{
	DECLARE_DYNAMIC(CPopDlgInfo)

public:
	CPopDlgInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPopDlgInfo();

// �Ի�������
	enum { IDD = IDD_POP_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
	virtual void OnCancel();
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	bool m_bRed;
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void Alarminformation(CString ip, CString position, CString information);
};
