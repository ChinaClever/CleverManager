/*
 * log_eleexportdlg.cpp
 * 电能导出窗口
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "log_eleexportdlg.h"
#include "elec_export/log_eletemptable.h"

Log_EleExportDlg::Log_EleExportDlg(QWidget *parent) : LOG_ExportDlg(parent)
{
    mExcelThread = new Log_ExcelThread(this);
}


void Log_EleExportDlg::init(Log_ExportBarWid *bar, int mode)
{
    setDate(bar->startDate, bar->endDate);
    setEle(mode);
}

void Log_EleExportDlg::setEle(int mode)
{
    QString title = tr("设备电能");
    if(LOG_EXPORT_ROOM == mode)
        title = tr("机房电能");
    else if(LOG_EXPORT_CAB == mode)
        title = tr("机柜电能");
    else if(LOG_EXPORT_ALLBYROOM == mode)
        title = tr("机柜电能");///////////////////////////////////////2024-07-25

    QString tb = LOG_TEMP_ELE;
    QStringList headList;
    headList << tr("编号")<< tr("名称") << tr("开始时间") << tr("电能(kWh)")
             << tr("结束时间") << tr("电能(kWh)") << tr("差值(kWh)");
    set(tb, headList, title);
}
