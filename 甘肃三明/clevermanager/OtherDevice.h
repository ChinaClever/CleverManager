#pragma once


// COtherDevice �Ի���

class COtherDevice : public CDialog
{
	DECLARE_DYNAMIC(COtherDevice)

public:
	COtherDevice(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COtherDevice();

// �Ի�������
	enum { IDD = IDD_OTHER_DEV };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int type;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
