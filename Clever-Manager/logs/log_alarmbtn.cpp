/*
 * log_alarmbtn.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "log_alarmbtn.h"

LOG_AlarmBtn::LOG_AlarmBtn(QWidget *parent) : LOG_DateTimeBtn(parent)
{
    initAlarmBtn();
}

LOG_AlarmBtn::~LOG_AlarmBtn()
{

}


void LOG_AlarmBtn::initAlarmBtn(void)
{
    initAlarmLayout();
}

void LOG_AlarmBtn::initOffLineBtn(void)
{
    //    alarmTypeLab->setText(tr("内容"));
}

void LOG_AlarmBtn::initAlarmLayout(void)
{
    QLabel *label =new QLabel(tr("请选择查找类型："),this);
    label->setFont(QFont("微软雅黑",12));
    label->setStyleSheet("color:white");

    box = new QComboBox(this);
    box->addItem(tr("设备IP"));
    box->addItem(tr("设备名"));
    box->addItem(tr("类型"));
    box->setFont(QFont("微软雅黑",12));
    box->setMinimumWidth(90);

    Edit = new QLineEdit(this);
    Edit->setFixedSize(120,30);

    alarmTypeBtn = new  QPushButton(tr("查找"),this);
    alarmTypeBtn->setFont(QFont("微软雅黑",12));
    alarmTypeBtn->setMaximumSize(60,30);
    connect(alarmTypeBtn, SIGNAL(clicked()),this,SLOT(comboxIndexChanged()));

    hLayout->addStretch();
    hLayout->addWidget(label);
    hLayout->addWidget(box);
    hLayout->addWidget(Edit);
    hLayout->addWidget(alarmTypeBtn);
    hLayout->addStretch();
}

/**
 * @brief 异常类型
 */
void LOG_AlarmBtn::initAlarmType(void)
{
    alarmTypeLab = new QLabel(tr("类型:"),this);
    alarmTypeLab->setStyleSheet("color:black");
    alarmTypeLab->setFont(QFont("微软雅黑",12));
    alarmTypeLab->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
}

void LOG_AlarmBtn::alarmTypeBtnSlot(void)
{
    QString type = Edit->text();
    if(!type.isEmpty())
    {
        QString str = tr("title like '%%1%'").arg(type);
        emit querySig(str);
    }
}

/**
 * @brief 初始化设备名部件
 */
void LOG_AlarmBtn::initDevName(void)
{
    devNameLab = new QLabel(tr("设备名:"),this);
    devNameLab->setStyleSheet("color:black");
    devNameLab->setFont(QFont("微软雅黑",12));
    devNameLab->setAlignment(Qt::AlignRight| Qt::AlignVCenter);

    devNameBtn = new QPushButton(tr("查找"),this);
    devNameBtn->setFont(QFont("微软雅黑",12));
    devNameBtn->setMaximumSize(60,30);
    connect(devNameBtn, SIGNAL(clicked()),this,SLOT(devNameBtnSlot()));
}

void LOG_AlarmBtn::devNameBtnSlot(void)
{
    QString name = Edit->text();
    if(!name.isEmpty())
    {
        QString str = tr("devName like '%%1%'").arg(name);
        emit querySig(str);
    }
}


/**
 * @brief 初始化设备IP部件
 */
void LOG_AlarmBtn::initDevIP(void)
{
    devIPLab = new QLabel(tr("设备IP:"),this);
    devIPLab->setStyleSheet("color:black");
    devIPLab->setFont(QFont("微软雅黑",12));
    devIPLab->setAlignment(Qt::AlignRight| Qt::AlignVCenter);

    devIPBtn = new QPushButton(tr("查找"),this);
    devIPBtn->setFont(QFont("微软雅黑",12));
    devIPBtn->setMaximumSize(60,30);
    connect(devIPBtn, SIGNAL(clicked()),this,SLOT(devIPBtnSlot()));
}

void LOG_AlarmBtn::devIPBtnSlot(void)
{
    QString ip = Edit->text();
    if(!ip.isEmpty())
    {
        QString str = tr("devIP like '%%1%'").arg(ip);
        emit querySig(str);
    }
}

void LOG_AlarmBtn::comboxIndexChanged()
{
    int index = box->currentIndex();

    switch (index) {
    case 0:
        devIPBtnSlot();
        break;
    case 1:
        devNameBtnSlot();
        break;
    case 2:
        alarmTypeBtnSlot();
        break;
    }

}
