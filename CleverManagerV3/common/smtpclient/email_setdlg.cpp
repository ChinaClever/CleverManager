/*
 * email_setdlg.cpp
 *  邮件配置界面
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "email_setdlg.h"
#include "ui_email_setdlg.h"
#include "common.h"
#include "email.h"

static email_setInfo *gSet = NULL;
email_setInfo *email_get_setInfo()
{
    return gSet;
}

Email_SetDlg::Email_SetDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Email_SetDlg)
{
    ui->setupUi(this);
    setWindowTitle(tr("SMTP设置"));
    groupBox_background_icon(this);

    gSet = mSet = new email_setInfo;
    initData();
    initWidget();
}

Email_SetDlg::~Email_SetDlg()
{
    delete ui;
}


/**
 * @brief 初始化数据
 */
void Email_SetDlg::initData(void)
{
    bool ret = com_cfg_open();
    if(ret) {
        QString str = com_cfg_readStr("Email_Server");
        if(str.isEmpty()) str = Email_Server;
        mSet->server = str;

        str = com_cfg_readStr("Email_UsrName");
        if(str.isEmpty()) str = Email_UsrName;
        mSet->usr = str;

        str = com_cfg_readStr("Email_Password");
        if(str.isEmpty()) str = Email_Password;
        mSet->pwd = str;

        str = com_cfg_readStr("Email_To");
        if(str.isEmpty()) str = Email_ToUsrName;
        mSet->to = str;

        int port = com_cfg_readInt("Email_Port");
        if(port <= 0) port = Email_Port;
        mSet->port = port;

        int ssl = com_cfg_readInt("Email_ssl");
        if(ssl < 0) mSet->ssl = 0;
        mSet->ssl = ssl;

        com_cfg_close();
    }
}

/**
 * @brief 窗口初始
 */
void Email_SetDlg::initWidget(void)
{
    ui->usrEdit->setText(mSet->usr);
    ui->pwdEdit->setText(mSet->pwd);
    ui->serEdit->setText(mSet->server);
    ui->portEdit->setText(QString::number(mSet->port));
    ui->comboBox->setCurrentIndex(mSet->ssl);
}

/**
 * @brief 数据验证
 * @return
 */
bool Email_SetDlg::dataCheck(void)
{
    QString str = ui->portEdit->text();

    bool ret = cm_isDigitStr(str);
    if(ret == false)
        CriticalMsgBox box(this, "发件箱端口号错误!");

    return ret;
}

/**
 * @brief 保存数据
 */
void Email_SetDlg::saveData(void)
{
    mSet->usr =  ui->usrEdit->text();
    com_cfg_writeParam("Email_UsrName", mSet->usr);

    mSet->pwd = ui->pwdEdit->text();
    com_cfg_writeParam("Email_Password", mSet->pwd);

    mSet->server = ui->serEdit->text();
    com_cfg_writeParam("Email_Server", mSet->server);

    mSet->to = ui->toEdit->text();
    com_cfg_writeParam("Email_To", mSet->to);

    mSet->port = ui->portEdit->text().toInt();
    com_cfg_writeParam("Email_Port", ui->portEdit->text());

    mSet->ssl = ui->comboBox->currentIndex();
    com_cfg_writeParam("Email_ssl", QString::number(mSet->ssl));
}

/**
 * @brief 保存按键
 */
void Email_SetDlg::on_saveBtn_clicked()
{
    bool ret = dataCheck();
    if(ret) {
        this->close();
        saveData();
    }
}


void Email_SetDlg::on_testBtn_clicked()
{
    bool ret = dataCheck();
    if(ret) {
        saveData();

        QString subject = tr("PDU测试邮件");
        QString body = tr("这是一份测试邮件!");

        Email email;
        QStringList recipient;
        recipient << mSet->to;
        email.sentEmail(recipient,subject,body);

        InfoMsgBox box(this,tr("邮件已发送至：%1，请等待!").arg(mSet->to));
    } else {

    }
}
