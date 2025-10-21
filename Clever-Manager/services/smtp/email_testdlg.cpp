/*
 * email_testdlg.cpp
 *  邮件界面
 *      1、邮件设置功能
 *      2、发送测试邮件功能
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "email_testdlg.h"
#include "common.h"
#include<QSpacerItem>
static email_setInfo *gSet = NULL;

email_setInfo *email_get_setInfo(void)
{
    return gSet;
}


Email_TestDlg::Email_TestDlg(QWidget *parent) : QWidget(parent)
{
    gSet = new email_setInfo();
    initWidget();
    initLayout();
    initData();
    mEmailThread = new EmailThread(this);
}

Email_TestDlg::~Email_TestDlg()
{
    delete mSet;
}

/**
 * @brief 初始化数据
 */
void Email_TestDlg::initData(void)
{
    mSet = gSet;

    bool ret = Sys_ConfigFile_Open();
    if(ret)
    {
        QString str = Sys_ConfigFile_ReadStr("Email_Server");
        if(str.isEmpty())
            str = Email_Server;
        mSet->server = str;

        str = Sys_ConfigFile_ReadStr("Email_UsrName");
        if(str.isEmpty())
            str = Email_UsrName;
        mSet->usr = str;

        str = Sys_ConfigFile_ReadStr("Email_Password");
        if(str.isEmpty())
            str = Email_Password;
        mSet->pwd = str;

        int port = Sys_ConfigFile_ReadInt("Email_Port");
        if(port <= 0)
            port = Email_Port;
        mSet->port = port;

        int ssl = Sys_ConfigFile_ReadInt("Email_ssl");
        if(ssl <= 0)
            mSet->ssl = false;
        else
            mSet->ssl = true;

        Sys_ConfigFile_Close();
    }
    else
    {
        mSet->server = Email_Server;
        mSet->usr = Email_UsrName;
        mSet->pwd = Email_Password;
        mSet->port = Email_Port;
        mSet->ssl = false;
    }
}

void Email_TestDlg::initWidget(void)
{
    QFont ft;
    ft.setPointSize(12);
    ft.setFamily("微软雅黑");

    title_emil=new QLabel(tr("  邮件测试"),this);
    title_emil->setFixedHeight(30);
    back.setColor(QPalette::WindowText,Qt::white);
    title_emil->setPalette(back);
    title_emil->setStyleSheet("background-color: rgb(12,62,180)");
    title_emil->setFont(ft);

    setLab = new QLabel(tr("邮件设置:"),this);
    setLab->setFont(ft);

    QPalette pal;
    pal.setColor(QPalette::ButtonText,QColor(0,90,171));

    setBtn = new QPushButton(tr("邮件设置"));
//    setBtn->setMinimumHeight(30);
//    setBtn->setMaximumWidth(160);
    setBtn->setFont(ft);
    setBtn->setPalette(pal);
    connect(setBtn,SIGNAL(clicked()),this,SLOT(setClickedBtn()));

    lab = new QLabel(tr("邮件测试:"),this);
    lab->setFont(ft);

    btn = new QPushButton(tr("发送测试邮件"));
    btn->setMinimumHeight(30);
    btn->setMaximumWidth(160);
    btn->setFont(ft);
    btn->setPalette(pal);

    connect(btn,SIGNAL(clicked()),this,SLOT(clickedBtn()));
}

/**
 * @brief 初始化布局
 */
void Email_TestDlg::initLayout(void)
{
    QGroupBox *groupBox = new QGroupBox(this);
    groupBox->setStyleSheet("background-color: rgb(255,255,255)");


    QGridLayout *gLayout = new QGridLayout(groupBox);
    gLayout->addWidget(setLab,1,1);
    gLayout->addWidget(setBtn,1,2);


    gLayout->addWidget(lab,2,1);
    gLayout->addWidget(btn,2,2);

    QGridLayout *layout = new QGridLayout(this);
    layout->setSpacing(0);
    layout->addWidget(title_emil,0,0,1,1);
    layout->addWidget(groupBox,1,0,5,1);
}


void Email_TestDlg::clickedBtn(void)
{
    QStringList recipient;
    QString emilAddr = usr_land_email(); //获取当前登录人的邮件
    if(!emilAddr.isEmpty())
        recipient << emilAddr;

    if(recipient.size() == 0)
    {
        bool ok;
        QString text = QInputDialog::getText(this,tr("邮件测试"),tr("请输入接收地址"),
                                             QLineEdit::Normal,NULL,&ok);
        if(ok && !text.isEmpty())
            recipient << text;
        else
            return ;
    }

    QString subject = tr("PDU测试邮件");
    QString body = tr("这是一份测试邮件!");

    Email email;
    email.sentEmail(recipient,subject,body);
//  email_sent_message(subject,body);

    InfoMsgBox box(this,tr("邮件已发送至：%1，请等待!").arg(recipient.at(0)));
}


void Email_TestDlg::setClickedBtn(void)
{
    Email_SetDlg dlg(this);
    dlg.setData(mSet);
    dlg.exec();
}
