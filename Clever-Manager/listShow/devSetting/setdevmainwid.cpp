/*
 * setdevmainwid.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "setdevmainwid.h"
#include "ui_setdevmainwid.h"
#include "common.h"
#include<qpalette.h>
#include"register/sharpness.h"
#include<qdebug.h>

SetDevMainWid::SetDevMainWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetDevMainWid)
{
    ui->setupUi(this);
    ui->label->resize(1146,50);

    ui->groupBox->setStyleSheet("border:0px");
    initWidget();
    initLayout();

    set_background_color(this, QColor(0,17,55));
    btnBlue_style_sheet(this);
}

SetDevMainWid::~SetDevMainWid()
{
    delete ui;
}

/**
 * @brief 初始化窗口
 */
void SetDevMainWid::initWidget(void)
{
    mDevName = new SetDevNameWid(ui->devBasicWid);
    connect(this,SIGNAL(selectSig(QString&,int)), mDevName,SLOT(updateSlot(QString&,int)));

    mDevUsr = new SetDevUsrWid(ui->devUsrWid);
    connect(this,SIGNAL(selectSig(QString&,int)), mDevUsr,SLOT(updateSlot(QString&,int)));

    mDevNet = new SetDevNetWid(ui->devNetWid);
    connect(this,SIGNAL(selectSig(QString&,int)), mDevNet,SLOT(updateSlot(QString&,int)));

    mDevSnmp = new SetDevSnmpWid(ui->devSnmpWid);
    connect(this,SIGNAL(selectSig(QString&,int)), mDevSnmp,SLOT(updateSlot(QString&,int)));

    mDevSmtp = new SetDevSmtpWid(ui->devSmtpWid);
    connect(this,SIGNAL(selectSig(QString&,int)), mDevSmtp,SLOT(updateSlot(QString&,int)));

    mSysCmd = new SetDevSysCmdWid(ui->devSysWid);
    connect(this,SIGNAL(selectSig(QString&,int)), mSysCmd,SLOT(updateSlot(QString&,int)));
}

/**
 * @brief 布局
 */
void SetDevMainWid::initLayout(void)
{

}


void SetDevMainWid::updateWidget(QString &ip, int num)
{
    mIP = ip;
    emit selectSig(mIP, num); // 发出信号
}
