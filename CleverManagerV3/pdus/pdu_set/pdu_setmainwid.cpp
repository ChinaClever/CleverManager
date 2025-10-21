/*
 * setdevmainwid.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdu_setmainwid.h"
#include "ui_pdu_setmainwid.h"

Pdu_SetMainWid::Pdu_SetMainWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Pdu_SetMainWid)
{
    ui->setupUi(this);
    //ui->label->resize(1146,50);

    initWidget();
    ui->groupBox->setStyleSheet("border:0px");
    set_background_color(this, QColor(0,17,55));
    btnBlue_style_sheet(this);
}

Pdu_SetMainWid::~Pdu_SetMainWid()
{
    delete ui;
}

/**
 * @brief 初始化窗口
 */
void Pdu_SetMainWid::initWidget()
{
    mDevName = new Pdu_SetNameWid(ui->devBasicWid);
    connect(this,SIGNAL(selectSig(sDataPacket *)), mDevName,SLOT(updateSlot(sDataPacket *)));

    mDevUsr = new Pdu_SetUserWid(ui->devUsrWid);
    connect(this,SIGNAL(selectSig(sDataPacket *)), mDevUsr,SLOT(updateSlot(sDataPacket *)));

    mDevNet = new Pdu_SetNetWid(ui->devNetWid);
    connect(this,SIGNAL(selectSig(sDataPacket *)), mDevNet,SLOT(updateSlot(sDataPacket *)));

    mSysCmd = new Pdu_SetSysCmdWid(ui->devSysWid);
    connect(this,SIGNAL(selectSig(sDataPacket *)), mSysCmd,SLOT(updateSlot(sDataPacket *)));
}



