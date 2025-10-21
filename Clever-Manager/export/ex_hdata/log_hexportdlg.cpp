/*
 * log_hexportdlg.cpp
 * 设备数据导出对话框
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "log_hexportdlg.h"
#include "common.h"

Log_HExportDlg::Log_HExportDlg(QWidget *parent) : LOG_ExportDlg(parent)
{
    mExcelThread = new Log_HExcelThread(this);
}

void Log_HExportDlg::init(Log_ExportBarWid *bar)
{
    setDate(bar->startDate, bar->endDate);
    init();
}


void Log_HExportDlg::init()
{
    QString title = tr("设备记录数据");

    QString tb = "pduElecParams";
    QStringList headList;
    headList << title;

    set(tb, headList, title);
}
