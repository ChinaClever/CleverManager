/*
 * up_tcpwidget.cpp
 * TCP升级 窗口显示
 * 1、界面部分：选择升级文件，设置账号和密码
 * 2、设置要好升级的目标ＩＰ
 * 3、升级之前，检查输入信息
 * 4、升级过程中，及时更新进度情况
 * 5、有异常时，显示异常信息
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "up_tcpwidget.h"
#include "ui_up_tcpwidget.h"
#include <QFileDialog>
#include "common.h"

UP_TcpWidget::UP_TcpWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UP_TcpWidget)
{
    ui->setupUi(this);
    mTcpSent = new UP_TcpSentThread(this);
    connect(mTcpSent, SIGNAL(connectSig(int)), this, SLOT(connectSlot(int)));
    connect(mTcpSent, SIGNAL(sentSig(int,QString&)), this, SLOT(sentSlot(int,QString&)));
    connect(mTcpSent, SIGNAL(progressSig(int)), this,SLOT(progressSlot(int)));
    connect(mTcpSent, SIGNAL(sentFinshSig(int,int,int)), this, SLOT(sentFinshSlot(int,int,int)));
}

UP_TcpWidget::~UP_TcpWidget()
{
    delete ui;
}

void UP_TcpWidget::on_openBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(0,tr("文件选择"),"/","",0);
    if (!fileName.isNull()) {
        ui->lineEdit->setText(fileName);
    }
    else{ //点的是取消

    }
}

/**
 * @brief 检查文件输入是否合法
 * @return
 */
bool UP_TcpWidget::checkFile(void)
{
    bool ret = false;
    QString file = ui->lineEdit->text();
    if(!file.isEmpty()) {
        if(file.contains(".tar")) {
            mUpdateStr.file = file;
            ret = true;
        } else {
             CriticalMsgBox box(this, tr("升级文件格式错误!, 请重新选择"));
        }
    } else {
         CriticalMsgBox box(this, tr("请选择升级文件"));
    }
    return ret;
}

/**
 * @brief 检查账号和密码
 * @return
 */
bool UP_TcpWidget::checkAcount(void)
{
    bool ret = false;
    QString usr = ui->usrEdit->text();
    if(!usr.isEmpty()) {
        QString pwd = ui->pwdEdit->text();
        if(!pwd.isEmpty()){
            mUpdateStr.usr = usr;
            mUpdateStr.pwd = pwd;
            ret = true;
        } else {
            CriticalMsgBox box(this, tr("密码不能为空！"));
        }
    } else {
        CriticalMsgBox box(this, tr("账号不能为空！"));
    }
    return ret;
}

/**
 * @brief 升级响应方法
 */
void UP_TcpWidget::on_updateBtn_clicked()
{
    if(checkFile() && checkAcount()){
        mTcpSent->set(mUpdateStr);
        mTcpSent->startSent();

        ui->updateBtn->setEnabled(false);
        ui->quitBtn->setEnabled(false);
    }
}

/**
 * @brief 发送状态改变
 * @param id 状态代码
 * @param ip
 */
void UP_TcpWidget::sentSlot(int id, QString &ip)
{
    QString str;
    switch (id) {
    case UP_ID_START:
        str = tr("等待验证 ");
        break;
    case UP_ID_VERIFY:
        str = tr("正在传输 ");
        break;
    case UP_ID_SENT_OK:
        str = tr("传输完成 ");
        break;

    default:
        break;
    }
    str += ip;
    ui->stateLab->setText(str);
}

/**
 * @brief 进度
 * @param value
 */
void UP_TcpWidget::progressSlot(int value)
{
    ui->progressBar->setValue(value);
}


/**
 * @brief 发送结束时，显示状态信息
 * @param all 升级个数
 * @param num 成功个数
 * @param err 失败个数
 */
void UP_TcpWidget::sentFinshSlot(int all, int num, int err)
{
    QString str = tr("升级个数%1 成功个数%2 失败个数%3").arg(all).arg(num).arg(err);
    ui->numLab->setText(str);

    ui->updateBtn->setEnabled(true);
    ui->quitBtn->setEnabled(true);
//    InfoMsgBox box(this, str);
}

/**
 * @brief 连接异常信号糟
 * @param step
 */
void UP_TcpWidget::connectSlot(int step)
{
    QString str;

    switch (step) {
    case UP_CMD_DIS: // 只有当断开、链接错误时，才发出相应信号出来
    case UP_CMD_ERR:
        str = tr("传输中断！");
        break;

    case UP_CMD_PWDERR:
        str = tr("账户、密码错误！");
        break;

    default:
        break;
    }

    if(!str.isEmpty()) {
        ui->stateLab->setText(str);
//        InfoMsgBox box(this, str);
    }
}



void UP_TcpWidget::on_quitBtn_clicked()
{
    emit quitSig();
}
