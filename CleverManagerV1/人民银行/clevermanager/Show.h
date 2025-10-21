#pragma once
#include "afxwin.h"


// CShow �Ի���

class CShow : public CDialog
{
	DECLARE_DYNAMIC(CShow)

public:
	CShow(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShow();

// �Ի�������
	enum { IDD = IDD_SHOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void Drawroom(CDC* pDC, int roomnum);
	void AddRoom(void);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void Draw_room(int change);
	CScrollBar m_bar;
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};

