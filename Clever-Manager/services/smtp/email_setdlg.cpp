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

Email_SetDlg::Email_SetDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Email_SetDlg)
{
    ui->setupUi(this);
    setWindowTitle(tr("SMTP设置"));
    groupBox_background_icon(this);
    mSet = NULL;

    //ui->checkBox->setHidden(true); // 隐藏加密邮件功能
}

Email_SetDlg::~Email_SetDlg()
{
    delete ui;
}

/**
 * @brief 设置初值
 * @param email
 */
void Email_SetDlg::setData(email_setInfo *email)
{
    mSet = email;
    initWidget();
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

    if(mSet->ssl)
        ui->checkBox->setChecked(true);
}

/**
 * @brief 数据验证
 * @return
 */
bool Email_SetDlg::dataCheck(void)
{
    QString str = ui->portEdit->text();

    bool ret = isDigitStr(str);
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
    Sys_ConfigFile_WriteParam("Email_UsrName", mSet->usr);

    mSet->pwd = ui->pwdEdit->text();
    Sys_ConfigFile_WriteParam("Email_Password", mSet->pwd);

    mSet->server = ui->serEdit->text();
    Sys_ConfigFile_WriteParam("Email_Server", mSet->server);

    mSet->port = ui->portEdit->text().toInt();
    Sys_ConfigFile_WriteParam("Email_Port", ui->portEdit->text());

    if(ui->checkBox->isChecked()) {
        mSet->ssl = true;
        Sys_ConfigFile_WriteParam("Email_ssl", QString::number(1));
    } else {
        mSet->ssl = false;
        Sys_ConfigFile_WriteParam("Email_ssl", QString::number(0));
    }
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

void Email_SetDlg::on_checkBox_clicked(bool checked)
{
    int port = 25;
    if(checked)
        port = 465;
    ui->portEdit->setText(QString::number(port));
}
