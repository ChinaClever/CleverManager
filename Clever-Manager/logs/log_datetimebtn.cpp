/*
 * log_datetimebtn.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "log_datetimebtn.h"

LOG_DateTimeBtn::LOG_DateTimeBtn(QWidget *parent) : QWidget(parent)
{
    initDTWdiget();
    btnyellow_style_sheet(this);
    set_background_color(this, QColor(0,17,55));
}

LOG_DateTimeBtn::~LOG_DateTimeBtn()
{

}


/**
 * @brief 日期、时间部件初始化
 */
void LOG_DateTimeBtn::initDTWdiget(void)
{
    refreshBtn = new QPushButton(tr("刷新"),this);;
    refreshBtn->setFont(QFont("微软雅黑",12));
    refreshBtn->setMaximumSize(100,35);
    connect(refreshBtn, SIGNAL(clicked()),this,SIGNAL(refreshSig()));

    delBtn = new QPushButton(tr("清空"),this);
    delBtn->setFont(QFont("微软雅黑",12));
    delBtn->setMaximumSize(100,35);
    connect(delBtn, SIGNAL(clicked()),this,SIGNAL(delBtnSig()));

    initDate();
    initTime();
    initDTLayout();
}


void LOG_DateTimeBtn::initDTLayout(void)
{
    hLayout = new QHBoxLayout(this);
    hLayout->addStretch();
    hLayout->addWidget(refreshBtn);
    hLayout->addWidget(delBtn);
    hLayout->addStretch();

    hLayout->addWidget(dateLab);
    hLayout->addWidget(dateEdit);
    hLayout->addWidget(dateBtn);
    hLayout->addStretch();

    hLayout->addWidget(timeLab);
    hLayout->addWidget(timeEdit);
    hLayout->addWidget(timeBtn);
    //    hLayout->setSpacing(50);
    //    hLayout->setContentsMargins();

}

/**
  * @brief 初始化日期部件
  */
void LOG_DateTimeBtn::initDate(void)
{
    dateLab = new QLabel(tr("日期:"),this);
    dateLab->setStyleSheet("color:white");
    dateLab->setFont(QFont("微软雅黑",12));
    dateLab->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    dateLab->setMaximumSize(50,30);

    //    dateEdit = new QLineEdit(this);
    dateEdit=new QDateEdit(this);
    dateEdit->setMaximumSize(140,30);
    dateEdit->setDisplayFormat("yyyy-MM-dd");
    dateEdit->setFont(QFont("微软雅黑",12));
    dateEdit->setDate(QDate::currentDate());
    dateEdit->setMinimumWidth(110);

    dateBtn = new QPushButton(tr("查找"),this);
    dateBtn->setFont(QFont("微软雅黑",12));
    dateBtn->setMaximumSize(80,30);
    connect(dateBtn, SIGNAL(clicked()),this,SLOT(dateBtnSlot()));
}

void LOG_DateTimeBtn::dateBtnSlot(void)
{
    QString date = dateEdit->text();
    if(!date.isEmpty())
    {
        QString str = tr("date like '%%1%'").arg(date);
        emit querySig(str);
    }
}

/**
 * @brief 初始化时间部件
 */
void LOG_DateTimeBtn::initTime(void)
{
    timeLab = new QLabel(tr("时间:"),this);
    timeLab->setStyleSheet("color:white");
    timeLab->setFont(QFont("微软雅黑",12));
    timeLab->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    timeLab->setMaximumSize(50,30);

    //    timeEdit = new QLineEdit(this);
    timeEdit=new QTimeEdit(this);
    timeEdit->setFont(QFont("微软雅黑",12));
    timeEdit->setTime(QTime::currentTime());
    timeEdit->setMaximumSize(120,30);
    timeEdit->setMinimumWidth(90);
    //    timeEdit->setDisplayFormat("HH:mm:ss");

    timeBtn = new QPushButton(tr("查找"),this);
    timeBtn->setFont(QFont("微软雅黑",12));
    timeBtn->setMaximumSize(60,30);
    connect(timeBtn, SIGNAL(clicked()),this,SLOT(timeBtnSlot()));
}

void LOG_DateTimeBtn::timeBtnSlot(void)
{
    QString time = timeEdit->text();
    if(!time.isEmpty())
    {
        QString str = tr("time like '%%1%'").arg(time);
        emit querySig(str);
    }
}
