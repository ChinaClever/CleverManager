/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "setup_mainwid.h"
#include "ui_setup_mainwid.h"
#include "logmainwid.h"
#include "pdudevices/setup_pdustablewid.h"
#include "sound.h"
#include "configbase.h"
#include "dt_mainwid.h"

Setup_MainWid::Setup_MainWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Setup_MainWid)
{
    ui->setupUi(this);
    QTimer::singleShot(rand()%50,this,SLOT(initFunSLot()));

}

Setup_MainWid::~Setup_MainWid()
{
    delete ui;
}


void Setup_MainWid::initPdusTable()
{
    mUpdlg = new Up_MainDlg(this);
    Setup_PdusTableWid *pdu = new Setup_PdusTableWid(ui->stackedWid);
    pdu->initWid(DbPduDevices::get(), new Setup_PdusQueryDlg(this));
    ui->stackedWid->addWidget(pdu);

    Setup_PdusTableWid *cab = new Setup_PdusTableWid(ui->stackedWid);
    cab->initWid(DbCabinetList::get(), new Setup_CabQueryDlg(this));
    ui->stackedWid->addWidget(cab);

    Setup_PdusTableWid *room = new Setup_PdusTableWid(ui->stackedWid);
    room->initWid(DbRoomList::get(), new Setup_RoomQueryDlg(this));
    ui->stackedWid->addWidget(room);

    new DT_MainWid(ui->groupBox);
    button_style_sheet(ui->upBtn);
}

void Setup_MainWid::initFunSLot()
{
    mUserWid = new UserMainWid(ui->stackedWid);
    ui->stackedWid->addWidget(mUserWid);

    mPush = new Setup_PushWid(ui->stackedWid);
    ui->stackedWid->addWidget(mPush);

    initPdusTable();
    sConfigItem *item = ConfigBase::bulid()->item;
    ui->alarmCmb->setCurrentIndex(item->sound);
    ui->logSpin->setValue(item->logTimes);
    ui->priceSPin->setValue(item->elePrice);
    ui->logCountSpin->setValue(item->logCount/10000);
}

void Setup_MainWid::on_comboBox_currentIndexChanged(int index)
{
    ui->stackedWid->setCurrentIndex(index);
}

void Setup_MainWid::on_upBtn_clicked()
{
    mUpdlg->exec();
}

void Setup_MainWid::on_alarmCmb_currentIndexChanged(int index)
{
    Sound::bulid(this)->setAllow(index);
    ConfigBase *con = ConfigBase::bulid();
    con->item->sound = index;
    con->setSound(index);
}


void Setup_MainWid::on_logSpin_valueChanged(int arg1)
{
    ConfigBase *con = ConfigBase::bulid();
    con->item->logTimes = arg1;
    con->setLogTime(arg1);
}

void Setup_MainWid::on_priceSPin_valueChanged(double arg1)
{
    ConfigBase *con = ConfigBase::bulid();
    con->item->elePrice = arg1;
    con->setElePrice(arg1);
}

void Setup_MainWid::on_logCountSpin_valueChanged(int arg1)
{
    ConfigBase *con = ConfigBase::bulid();
    con->item->logCount = arg1*10000;
    con->setLogCount(arg1*10000);
}
