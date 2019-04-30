// clevermanagerView.h : CclevermanagerView ��Ľӿ�
//


#pragma once
#include "DlgDataCenterMap.h"
#include "DatacenterInfo.h"
#include "DatacenterEnv.h"
#include "DeviceList.h"
#include "UserTabCtrl.h"
#include "ServerSet.h"
#include "LogsDlg.h"
#include "EmsAtsDlg.h"
#include "Show.h"
#include "Show_title.h"
class CclevermanagerView : public CView
{
protected: // �������л�����
	CclevermanagerView();
	DECLARE_DYNCREATE(CclevermanagerView)

// ����
public:
	CclevermanagerDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CclevermanagerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	CUserTabCtrl m_tab;
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CDlgDataCenterMap tab1;
	CDatacenterInfo tab2;
	CDatacenterEnv tab3;
	CDeviceList tab4;
	CServerSet tab5;
	CLogsDlg tab6;
	CEmsAtsDlg tab7;
	CShow tab8;
	CShow_title tab9;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	int menuwith;
	int page;
	void drawMenu(int index,CDC *pDC);
	
};

#ifndef _DEBUG  // clevermanagerView.cpp �еĵ��԰汾
inline CclevermanagerDoc* CclevermanagerView::GetDocument() const
   { return reinterpret_cast<CclevermanagerDoc*>(m_pDocument); }
#endif

