/*
 * log_historydatawid.cpp
 * 设备纪录数据
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "log_historydatawid.h"
#include "ui_log_historydatawid.h"
#include "log_hexportdlg.h"

Log_HistoryDataWid::Log_HistoryDataWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Log_HistoryDataWid)
{
    ui->setupUi(this);
    mBarWid = new Log_ExportBarWid(ui->widget);
    mBarWid->setDevMode();
    setWindowFlags(Qt::FramelessWindowHint);

    connect(mBarWid, SIGNAL(findSig(int)), this, SLOT(findSLot(int)));
    connect(mBarWid, SIGNAL(exportSig(int)), this, SLOT(exportSLot(int)));
    connect(mBarWid, SIGNAL(clearSig()), this, SLOT(clearTableSlot()));

    DbPduDevIndex::get(); DbPduElecParams::get();
    mPduDevTabView = new Log_PduDevTabView(ui->pduDevWid);
    mPduDevTabView->refreshTable();

    mPduParamsView = new Log_PduParamsVIew(ui->pduParWid);
    mPduParamsView->refreshTable();

    // 为了快速解决问题，临时隐藏 。后期需打开 2017/12/27 lzy
    /////////====
    //    ui->pduParWid->setHidden(true);
    //    ui->pduDevWid->setMaximumHeight(1024);
    /////====

    connect(mPduDevTabView, SIGNAL(selectedSig(int)), mPduParamsView, SLOT(selectedSlot(int)));
}

Log_HistoryDataWid::~Log_HistoryDataWid()
{
    delete ui;
}

void Log_HistoryDataWid::findSLot(int id)
{
    QString cmd;

    switch(id) {
    case LOG_EXPORT_DEV:
        cmd += QString(" ip = \'%1\' and devNum = \'%2\' ").arg(mBarWid->ip).arg(mBarWid->slaveNum);
        break;

    case LOG_EXPORT_ROOM:
         cmd += QString(" roomName=\'%1\'  ").arg(mBarWid->room);
        break;

    case LOG_EXPORT_CAB:
        cmd += QString(" cabinetName=\'%1\' ").arg(mBarWid->cab);
       break;
    }
    mSqlCmd = cmd + getBetweenDateStr(mBarWid->startDate, mBarWid->endDate);
    mPduDevTabView->selected(mSqlCmd);
}

int Log_HistoryDataWid::getRow(const QString &name)
{
    int ret = 0;
    QSqlQuery query;
    QString cmd = QString("select count(id) from %1 where ").arg(name) + mSqlCmd;

    if(query.exec(cmd)){
        if(query.next()) ret = query.value(0).toInt();
    } else
        qDebug() << cmd << "Err";

    return ret;
}

void Log_HistoryDataWid::exportSLot(int)
{
    Log_HExportDlg dlg(this);
    dlg.init(mBarWid);

    int row = getRow("pduDevIndex");
    for(int i=0; i<row; ++i) {
        mPduDevTabView->setSelectRow(i);
        QCoreApplication::processEvents(QEventLoop::AllEvents,35);
        mPduDevTabView->getList(dlg.mExcelThread->strList);
        mPduParamsView->getList(dlg.mExcelThread->strList);
    }
    dlg.exec();
}

void Log_HistoryDataWid::clearTableSlot()
{
    mPduDevTabView->refreshInitTable();
    mPduParamsView->refreshInitTable();

    mPduDevTabView->clearTableSlot();
    mPduParamsView->clearTableSlot();
}
