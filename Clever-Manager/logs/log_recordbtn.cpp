/*
 * log_recordbtn.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "log_recordbtn.h"

LOG_RecordBtn::LOG_RecordBtn(QWidget *parent) : LOG_DateTimeBtn(parent)
{
    initRecord();
}

LOG_RecordBtn::~LOG_RecordBtn()
{

}

void LOG_RecordBtn::initRecord(void)
{
    initUsr();
    //    initContent();
    //    initInfo();

    initRecordLayout();
}



/**
 * @brief 初始化布局
 */
void LOG_RecordBtn::initRecordLayout(void)
{
    box = new QComboBox(this);
    //box->setFixedSize(60,30);//中文
    box->setFixedSize(90,30);//英文
    box->addItem(tr("用户"));
    box->addItem(tr("操作"));
    box->addItem(tr("内容"));
//    QFont font("微软雅黑,12");
//    box->setFont(font);
    box->setFont(QFont("微软雅黑",10));

    hLayout->addWidget(usrLab);
    hLayout->addWidget(box);
    hLayout->addWidget(usrEdit);
    hLayout->addWidget(usrBtn);
    hLayout->addStretch();
    //    hLayout->setSpacing(0);
    //    hLayout->addWidget(contentLab);
    //    hLayout->addWidget(contentEdit);
    //    hLayout->addWidget(contentBtn);

    //    hLayout->addWidget(infoLab);
    //    hLayout->addWidget(infoEdit);
    //    hLayout->addWidget(infoBtn);
}

void LOG_RecordBtn::initInfo(void)
{
    infoLab = new QLabel(tr("内容："),this);
    infoLab->setStyleSheet("color:black;");
    infoLab->setFont(QFont("微软雅黑",12));
    infoLab->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    infoLab->setMaximumSize(60,30);

    infoEdit = new QLineEdit(this);
    infoEdit->setMaximumSize(140,30);

    infoBtn = new QPushButton(tr("查找"),this);
    infoBtn->setFont(QFont("微软雅黑",12));
    infoBtn->setMaximumSize(60,30);
    //    connect(infoBtn, SIGNAL(clicked()),this,SLOT(infoBtnSlot()));
}


void LOG_RecordBtn::infoBtnSlot(void)
{
    QString msg = usrEdit->text();;
    if(!msg.isEmpty())
    {
        QString str = tr("operation like '%%1%'").arg(msg);
        emit querySig(str);
    }
}


/**
 * @brief 初始化操作控件
 */
void LOG_RecordBtn::initContent(void)
{
    contentLab = new QLabel(tr("操作："),this);
    contentLab->setStyleSheet("color:black;");
    contentLab->setFont(QFont("微软雅黑",12));
    contentLab->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    contentLab->setMaximumSize(60,30);

    contentEdit = new QLineEdit(this);
    contentEdit->setMaximumSize(140,30);

    contentBtn = new QPushButton(tr("查找"),this);
    contentBtn->setFont(QFont("微软雅黑",12));
    contentBtn->setMaximumSize(60,30);
    //    connect(contentBtn, SIGNAL(clicked()),this,SLOT(contentBtnSlot()));
}

void LOG_RecordBtn::contentBtnSlot(void)
{
    QString msg = usrEdit->text();;
    if(!msg.isEmpty())
    {
        QString str = tr("title like '%%1%'").arg(msg);
        emit querySig(str);
    }
}

/**
 * @brief 初始化用户控件
 */
void LOG_RecordBtn::initUsr(void)
{
    usrLab = new QLabel(tr("请选择查找类型："),this);
    usrLab->setStyleSheet("color:white;");
    usrLab->setFont(QFont("微软雅黑",12));
    usrLab->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    usrLab->setFixedSize(280,30);

    usrEdit = new QLineEdit(this);
    usrEdit->setMaximumSize(120,30);

    usrBtn = new QPushButton(tr("查找"),this);
    usrBtn->setFont(QFont("微软雅黑",12));
    usrBtn->setMaximumSize(60,30);
    //    connect(usrBtn, SIGNAL(clicked()),this,SLOT(usrBtnSLot()));
    connect(usrBtn, SIGNAL(clicked()),this,SLOT(button_clicked()));
}

void LOG_RecordBtn::usrBtnSLot(void)
{
    QString usr = usrEdit->text();
    if(!usr.isEmpty())
    {
        QString str = tr("usr like '%%1%'").arg(usr);
        emit querySig(str);
    }
}

void LOG_RecordBtn::button_clicked()
{
    int index =  box->currentIndex();

    switch (index) {
    case 0:
        usrBtnSLot();
        break;
    case 1:
        contentBtnSlot();
        break;
    case 2:
        infoBtnSlot();
        break;
    }

}


