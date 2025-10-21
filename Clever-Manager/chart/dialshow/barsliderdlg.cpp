/*
 * barsliderdlg.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "barsliderdlg.h"

BarSliderDlg::BarSliderDlg(QWidget *parent) : QWidget(parent)
{
    resize(190,100);
    setMaximumSize(190,100);

    m_barSlider = new BarSlider(this);

    lab = new QLabel(tr("湿度\n---"),this);
    lab->setAlignment(Qt::AlignCenter);
    lab->setMaximumSize(30,100);

    QFont ft;
    ft.setPointSize(12);  
    lab->setFont(ft);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(lab);
    layout->addWidget(m_barSlider);
}

BarSliderDlg::~BarSliderDlg()
{

}


void BarSliderDlg::setValue(double value)
{
    QString str = tr("湿度\n %1%").arg(value);
    lab->setText(str);

    m_barSlider->setValue(value);
    m_barSlider->repaint();
}
