/*
 * ls_devstatusnum.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "ls_devstatusnum.h"

LS_DevStatusNum::LS_DevStatusNum(QWidget *parent) : QWidget(parent)
{
    DL_DevStatusNum *data = dev_num_dl();
    memset(data,0,sizeof(DL_DevStatusNum));

    initWidget();

    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    timer->start(1000);
}

LS_DevStatusNum::~LS_DevStatusNum()
{

}

void LS_DevStatusNum::initWidget(void)
{
    lineIconLab = new QLabel(this);
    lineIconLab->setMinimumHeight(22);
    lineIconLab->setMaximumSize(20,25);
    lineIconLab->setStyleSheet("background-image: url(:/images/state_line.png);");

    lineDevLab = new QLabel(this);    
    lineDevLab->setAlignment(Qt::AlignCenter);
    lineDevLab->setMinimumSize(55,30);

    alarmIconLab = new QLabel(this);
    alarmIconLab->setMinimumHeight(22);
    alarmIconLab->setMaximumSize(20,25);
    alarmIconLab->setStyleSheet("background-image: url(:/images/state_alarm.png);");

    alarmDevLab = new QLabel(this);
    alarmDevLab->setAlignment(Qt::AlignCenter);
    alarmDevLab->setMinimumSize(65,30);

    offLineIconLab = new QLabel(this);
    offLineIconLab->setMinimumHeight(22);
    offLineIconLab->setMaximumSize(20,25);
    offLineIconLab->setStyleSheet("background-image: url(:/images/state_offLine.png);");

    offLineDevLab = new QLabel(this);
    offLineDevLab->setAlignment(Qt::AlignCenter);
    offLineDevLab->setMinimumSize(55,30);

    initLayout();
}

void LS_DevStatusNum::initLayout(void)
{
    QHBoxLayout *hLayout_1 = new QHBoxLayout;
    hLayout_1->addWidget(lineIconLab);
    hLayout_1->addWidget(alarmIconLab);
    hLayout_1->addWidget(offLineIconLab);

    QHBoxLayout *hLayout_2 = new QHBoxLayout;
    hLayout_2->addWidget(lineDevLab);
    hLayout_2->addWidget(alarmDevLab);
    hLayout_2->addWidget(offLineDevLab);

    QGroupBox *groupBox = new QGroupBox(tr("设备工作状态"),this);
    QGridLayout *pLayout = new QGridLayout(groupBox);
    pLayout->addLayout(hLayout_1,0,0);
    pLayout->addLayout(hLayout_2,1,0);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(groupBox);
}

/**
 * @brief 在线设备个数
 */
void LS_DevStatusNum::lineDevNum(void)
{
    int num = dev_num_line();
    QString str = tr("在线：")+QString::number(num);
    lineDevLab->setText(str);
}

/**
 * @brief 报警设备个数
 */
void LS_DevStatusNum::alarmDevNum(void)
{
    int num = dev_num_alarm();
    QString str = tr("异常：")+QString::number(num);
    alarmDevLab->setText(str);
}

/**
 * @brief 离线设备个数
 */
void LS_DevStatusNum::offLineDevNum(void)
{
    int num = dev_num_offLine();
    QString str = tr("离线：")+QString::number(num);
    offLineDevLab->setText(str);
}

void LS_DevStatusNum::timeoutDone(void)
{
    lineDevNum();
    alarmDevNum();
    offLineDevNum();
}
