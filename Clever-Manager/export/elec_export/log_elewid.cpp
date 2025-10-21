/*
 * log_elewid.cpp
 * 电能导出
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "log_elewid.h"
#include "ui_log_elewid.h"
#include "ex_common/log_eleexportdlg.h"
#include"common.h"

Log_EleWid::Log_EleWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Log_EleWid)
{
    ui->setupUi(this);
    initWidget();
}

Log_EleWid::~Log_EleWid()
{
    delete ui;
}

void Log_EleWid::initWidget()
{
    mBarWid = new Log_ExportBarWid(ui->widget);

    mBarWid->setDevMode(1);
    set_background_color(ui->widget,QColor(204,232,207));
    connect(mBarWid, SIGNAL(findSig(int)), this, SLOT(findSLot(int)));
    connect(mBarWid, SIGNAL(exportSig(int)), this, SLOT(exportSLot(int)));

    mTabView = new Log_EleTabView(ui->tabWidget);

    mUpdateThread = new Log_UpdateThread(this);
    mUpdateThread->setBar(mBarWid);
    mUpdateThread->setView(mTabView);
}

void Log_EleWid::findSLot(int mode)
{
    mUpdateThread->startUpdate(mode);
}

void Log_EleWid::exportSLot(int mode)
{
    Log_EleExportDlg dlg(this);
    dlg.init(mBarWid,mode);
    dlg.exec();
}
