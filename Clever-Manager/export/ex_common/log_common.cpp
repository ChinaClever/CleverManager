/*
 * log_common.cpp
 * 日志公共部分
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "log_common.h"

QString getBetweenDateStr(const QDate &start, const QDate &end)
{
    QString startStr = start.toString("yyyy-MM-dd");
    QString endStr = end.toString("yyyy-MM-dd");

    QString cmd = QString(" and date Between \'%1\' and  \'%2\' ").arg(startStr).arg(endStr);
    return cmd;
}

QString getBetweenTimeStr(const QTime &start, const QTime &end)
{
    QString startStr = start.toString("hh:mm:ss");
    QString endStr = end.toString("hh:mm:ss");

    QString cmd = QString(" and time Between \'%1\' and  \'%2\' ").arg(startStr).arg(endStr);
    return cmd;
}

