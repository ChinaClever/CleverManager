/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "logmainwid.h"
#include "ui_logmainwid.h"


LogMainWid::LogMainWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogMainWid)
{
    ui->setupUi(this);
    QTimer::singleShot(rand()%50,this,SLOT(initFunSLot()));
}

LogMainWid::~LogMainWid()
{
    delete ui;
}


void LogMainWid::initFunSLot()
{
    mLandLog = new LogComWid(ui->stackedWid);
    mLandLog->initWid(DbUserLog::bulid(), new Log_UserQueryDlg(this));
    ui->stackedWid->addWidget(mLandLog);

    mPduALarm = new LogComWid(ui->stackedWid);
    mPduALarm->initWid(DbPduAlarm::bulid(), new Log_PduAlarmQueryDlg(this));
    ui->stackedWid->addWidget(mPduALarm);

    mCabAlarm = new LogComWid(ui->stackedWid);
    mCabAlarm->initWid(DbCabAlarm::bulid(), new Log_CabAlarmQueryDlg(this));
    ui->stackedWid->addWidget(mCabAlarm);

    mPduHrs = new LogComWid(ui->stackedWid);
    mPduHrs->initWid(DbPduHrs::bulid(), new Log_PduAlarmQueryDlg(this));
    ui->stackedWid->addWidget(mPduHrs);

    mCabHrs = new LogComWid(ui->stackedWid);
    mCabHrs->initWid(DbCabHrs::bulid(), new Log_CabHrsQueryDlg(this));
    ui->stackedWid->addWidget(mCabHrs);

    mRoomHrs = new LogComWid(ui->stackedWid);
    mRoomHrs->initWid(DbRoomHrs::bulid(), new Log_RoomHrsQueryDlg(this));
    ui->stackedWid->addWidget(mRoomHrs);

    mPduElec = new ElecComWid(ui->stackedWid);
    mPduElec->initWid(DbPduElec::bulid(), new Log_PduElecQueryDlg(this));
    ui->stackedWid->addWidget(mPduElec);

    mCabElec = new ElecComWid(ui->stackedWid);
    mCabElec->initWid(DbCabElec::bulid(), new Log_CabElecQueryDlg(this));
    ui->stackedWid->addWidget(mCabElec);

    mRoomElec = new ElecComWid(ui->stackedWid);
    mRoomElec->initWid(DbRoomElec::bulid(), new Log_RoomElecQueryDlg(this));
    ui->stackedWid->addWidget(mRoomElec);
}

void LogMainWid::on_comboBox_currentIndexChanged(int index)
{
    ui->stackedWid->setCurrentIndex(index);
}
