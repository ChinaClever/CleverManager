/*
 * log_excelthread.cpp
 * 电能导出线程
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "log_excelthread.h"
#include <QSqlQuery>
#include <QSqlRecord>

Log_ExcelThread::Log_ExcelThread(QObject *parent) : Excel_ExportLogThread(parent)
{

}

void Log_ExcelThread::readSql(QList<QStringList> &list)
{
    QSqlQuery query;
    QString cmd = QString("select * from %1 ").arg(mEx.db);
    bool ret = query.exec(cmd);
    if(ret) {
        while(query.next())
        {
            QStringList strList;
            int columnNum = query.record().count(); //获取每条记录中属性（即列）的个数
            for(int i=0; i<columnNum; ++i)
                strList << query.value(i).toString();
            list.append(strList);
        }
    } else
        qDebug() << cmd << "Err readSql";
}
