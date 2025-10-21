// ColorHeaderCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "clevermanager.h"
#include "ColorHeaderCtrl.h"


// CColorHeaderCtrl

IMPLEMENT_DYNAMIC(CColorHeaderCtrl, CHeaderCtrl)

CColorHeaderCtrl::CColorHeaderCtrl()
{

}

CColorHeaderCtrl::~CColorHeaderCtrl()
{
}


BEGIN_MESSAGE_MAP(CColorHeaderCtrl, CHeaderCtrl)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CColorHeaderCtrl ��Ϣ�������



void CColorHeaderCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
 
 // TODO: Add your message handler code here
 
 // Do not call CHeaderCtrl::OnPaint() for painting messages
    CRect rect;
	GetClientRect(rect);
	SetBkMode(dc.GetSafeHdc(),TRANSPARENT);
	dc.FillSolidRect(rect,RGB(255,255,255));   //�ػ��������ɫ
	retange(&dc,rect.left,rect.top,rect.Width(),rect.Height());
	int nItems = GetItemCount();
	CRect rectItem;
	CPen m_pen(PS_SOLID,1,RGB(211,211,211));      //�ָ�����ɫ
	CPen * pOldPen=dc.SelectObject(&m_pen);
	CFont m_font;
	m_font.CreatePointFont(90,"����");        //����
	CFont * pOldFont=dc.SelectObject(&m_font);
	dc.SetTextColor(RGB(64,64,64));     //������ɫ
	for(int i = 0; i <nItems; i++)                    //�Ա����ÿ���н����ػ�
	{  
		GetItemRect(i, &rectItem);
		rectItem.top+=2;
		rectItem.bottom+=2; 
		dc.MoveTo(rectItem.right,rect.top);                //�ػ�ָ���
		dc.LineTo(rectItem.right,rectItem.bottom);
		TCHAR buf[256];
		HD_ITEM hditem;

		hditem.mask = HDI_TEXT | HDI_FORMAT | HDI_ORDER;
		hditem.pszText = buf;
		hditem.cchTextMax = 255;
		GetItem( i, &hditem );                                       //��ȡ��Ȼ�е�����
		UINT uFormat = DT_SINGLELINE | DT_NOPREFIX | DT_TOP |DT_CENTER | DT_END_ELLIPSIS ;
		dc.DrawText(buf, &rectItem, uFormat);           //�ػ������������
	}
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldFont);
}
