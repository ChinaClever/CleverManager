/*
 * log_hexcelthread.cpp
 * 设备数据导出线程
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "log_hexcelthread.h"

Log_HExcelThread::Log_HExcelThread(QObject *parent) : Excel_ExportLogThread(parent)
{

}

void Log_HExcelThread::readSql(QList<QStringList> &list)
{
    list = strList;
}
