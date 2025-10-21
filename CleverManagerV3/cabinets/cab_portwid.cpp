/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "cab_portwid.h"
#include <QHBoxLayout>
#include "common.h"
#include <QDebug>

Cab_PortWid::Cab_PortWid(bool alignLeft,QWidget *parent) :
    QWidget(parent)
{
    m_pushbutton = new QPushButton();
    m_label_index = new QLabel();
    m_label_currentValue = new QLabel();
    m_label_state = new QLabel();

    m_timer.setSingleShot(true);
    m_timer.setInterval(5*1000);
    m_timer.setTimerType(Qt::VeryCoarseTimer);

    m_pushbutton->setCheckable(true);
    m_pushbutton->setStyleSheet("QPushButton{border-image: url(:/topology/topology/kai.png);background-color: transparent;}"
                                "QPushButton::!checked{border-image: url(:/topology/topology/guan.png);}");

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(1);
    if(alignLeft){
        layout->addWidget(m_label_index);
        layout->addWidget(m_label_currentValue);
        layout->addWidget(m_pushbutton);
        layout->addWidget(m_label_state,1);
    } else {
        layout->addWidget(m_label_state,1);
        layout->addWidget(m_pushbutton);
        layout->addWidget(m_label_index);
        layout->addWidget(m_label_currentValue);
    }

    set_switch_flag = false;

    connect(m_pushbutton,SIGNAL(clicked(bool)),SLOT(buttonClicked(bool)));
    connect(&m_timer,SIGNAL(timeout()),SLOT(timeOut()));
}

Cab_PortWid::~Cab_PortWid()
{
}

void Cab_PortWid::setIndex(int id)
{
    if(id < 10)
        m_label_index->setText(QString("0%1:  ").arg(id));
    else
        m_label_index->setText(QString("%1:  ").arg(id));
}

int Cab_PortWid::index()
{
    QString str = m_label_index->text();
    str.truncate(str.size()-1);
    return str.toInt();
}

void Cab_PortWid::setCurrentValue(double cur)
{
    m_label_currentValue->setText(QString::number(cur,'f',1)+"A  ");
}

void Cab_PortWid::setSwitchState(bool opened)
{
    if(m_pushbutton->isEnabled() == false)
    {
        if((m_pushbutton->isChecked() == opened) || (true == set_switch_flag))
        {
            m_pushbutton->setEnabled(true);
            m_label_state->clear();
            m_timer.stop();
            set_switch_flag = false;
        }
    }
    else if (m_pushbutton->isChecked() != opened)
    {
        m_pushbutton->setChecked(opened);
    }
}

void Cab_PortWid::setSwitchDisabled()
{
    m_pushbutton->setChecked(true);
    m_pushbutton->setDisabled(true);
}

void Cab_PortWid::setWarningState(bool b)
{
    if(b)
    {
        m_pushbutton->setStyleSheet("QPushButton{border-image: url(:/topology/topology/kai_.png);background-color: transparent;}"
                                    "QPushButton::!checked{border-image: url(:/topology/topology/guan_.png);}");
    }
    else
    {
        m_pushbutton->setStyleSheet("QPushButton{border-image: url(:/topology/topology/kai.png);background-color: transparent;}"
                                    "QPushButton::!checked{border-image: url(:/topology/topology/guan.png);}");
    }
}

QPushButton *Cab_PortWid::button()
{
    return m_pushbutton;
}

void Cab_PortWid::buttonClicked(bool b)
{
    m_pushbutton->setEnabled(false);
    m_label_state->setStyleSheet("color:black;");
    m_label_state->setText(b?tr("正在启动中"):tr("正在关闭中"));
    m_timer.start();
    emit clicked(b);
}

//超时后，重置状态
void Cab_PortWid::timeOut()
{
    if(!m_label_state->text().isEmpty() && !m_pushbutton->isEnabled())
    {
        m_pushbutton->setChecked(!m_pushbutton->isChecked());
        m_label_state->setStyleSheet("color:red;");
        set_switch_flag = true;
    }
}
