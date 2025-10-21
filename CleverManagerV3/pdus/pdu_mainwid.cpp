/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "pdu_mainwid.h"
#include "ui_pdu_mainwid.h"

Pdu_MainWid::Pdu_MainWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Pdu_MainWid)
{
    ui->setupUi(this);
    QTimer::singleShot(500,this,SLOT(initWidSlot()));
}

Pdu_MainWid::~Pdu_MainWid()
{
    delete ui;
}

void Pdu_MainWid::initWidSlot()
{
    mList = new Pdu_ListWid(ui->listWid);

    mLineWid = new Pdu_LineWid(ui->stackedWid);
    ui->stackedWid->addWidget(mLineWid);
    connect(mList, SIGNAL(selectedSig(sDataPacket*)), mLineWid, SLOT(packetSlot(sDataPacket*)));

    mOutputWid = new Pdu_OutputWid(ui->stackedWid);
    ui->stackedWid->addWidget(mOutputWid);
    connect(mList, SIGNAL(selectedSig(sDataPacket*)), mOutputWid, SLOT(packetSlot(sDataPacket*)));

    mEnvWid = new Pdu_EnvWid(ui->stackedWid);
    ui->stackedWid->addWidget(mEnvWid);
    connect(mList, SIGNAL(selectedSig(sDataPacket*)), mEnvWid, SLOT(packetSlot(sDataPacket*)));

    mSetWid = new Pdu_SetMainWid(ui->stackedWid);
    ui->stackedWid->addWidget(mSetWid);
    connect(mList, SIGNAL(selectedSig(sDataPacket*)), mSetWid, SIGNAL(selectSig(sDataPacket*)));
}

void Pdu_MainWid::on_comboBox_currentIndexChanged(int index)
{
    ui->stackedWid->setCurrentIndex(index);
}
