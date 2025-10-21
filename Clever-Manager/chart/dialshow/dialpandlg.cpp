/*
 * dialpandlg.cpp
 * 显示湿度 大小固定:为170 100
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "dialpandlg.h"

DialpanDlg::DialpanDlg(QWidget *parent) : QWidget(parent)
{
    resize(170,100);
    setMaximumSize(170,100);

    m_dialpan = new dialpan(this);
    m_dialpan->move(0,-30);

    lab = new QLabel(tr("温度:--"),this);
    lab->setAlignment(Qt::AlignCenter);
    lab->resize(100,40);
    lab->move(40,60);

    QFont ft;
    ft.setPointSize(12);
    //ft.setBold(true);
    lab->setFont(ft);
}

DialpanDlg::~DialpanDlg()
{

}


void DialpanDlg::setValue(double value)
{
    QString str = tr("温度:") + QString::number(value)+"℃";
    lab->setText(str);

    m_dialpan->setRateValue(value);
    m_dialpan->repaint();
}
