/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "up_devselectwid.h"
#include "ui_up_devselectwid.h"
#include "msgbox.h"

Up_DevSelectWid::Up_DevSelectWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Up_DevSelectWid)
{
    ui->setupUi(this);
    mData = Up_DataPacket::bulid()->data;
    timer = new QTimer(this);
    timer->start(200);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    on_comboBox_currentIndexChanged(0);
    mCount = 1;
}

Up_DevSelectWid::~Up_DevSelectWid()
{
    delete ui;
}


void Up_DevSelectWid::timeoutDone(void)
{
    this->setDisabled(mData->isRun);
}


bool Up_DevSelectWid::checkInput()
{
    QString str;
    QString user =ui->userEdit->text();
    if(user.isEmpty())
        str = tr("用户名不能为空，请重新输入!!");

    QString pwd = ui->pwdEdit->text();
    if(pwd.isEmpty())
        str = tr("密码不能为空，请重新输入!!");

    bool ret = true;
    if(str.isEmpty()) {
        mData->usr = user;
        mData->pwd = pwd;
    } else {
        ret = false;
        CriticalMsgBox box(this, str);
    }

    return ret;
}

void Up_DevSelectWid::setenabled(bool e)
{
    ui->comboBox->setEnabled(e);
    ui->userEdit->setEnabled(e);
    ui->pwdEdit->setEnabled(e);
}

void Up_DevSelectWid::on_okBtn_clicked()
{
    bool en = false;
    QString str = tr("修改");
    if(mCount++ %2) {
        if(mData->devtype > 1) {
            if(!checkInput()) {
                mCount--; return;
            }
        }
    } else {
        en = true;
        str = tr("确认");
    }

    setenabled(en);
    ui->okBtn->setText(str);
}

void Up_DevSelectWid::on_comboBox_currentIndexChanged(int index)
{
    bool en = true;
    mData->devtype = index;
    if(index < 3) en = false;
    ui->userEdit->setEnabled(en);
    ui->pwdEdit->setEnabled(en);

    QString dev_type = ui->comboBox->currentText();
    Up_DataPacket::bulid()->data->dev_type = dev_type;
}


