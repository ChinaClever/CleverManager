#pragma once


// CShow_title �Ի���

class CShow_title : public CDialog
{
	DECLARE_DYNAMIC(CShow_title)

public:
	CShow_title(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShow_title();

// �Ի�������
	enum { IDD = IDD_SHOW_TITLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
