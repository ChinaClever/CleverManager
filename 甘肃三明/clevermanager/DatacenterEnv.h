#pragma once


// CDatacenterEnv �Ի���

class CDatacenterEnv : public CDialog
{
	DECLARE_DYNAMIC(CDatacenterEnv)

public:
	CDatacenterEnv(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDatacenterEnv();

// �Ի�������
	enum { IDD = IDD_DATACENTER_ENV };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	int position[20][20];
	void drawColor(CDC *pDC);
	//void drawdelay(CDC *pDC);
	//bool delay;
	Color CDatacenterEnv::setcolor(int value);
	void drawflags(CDC *pDC);
	virtual BOOL OnInitDialog();
	CString DCname;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
