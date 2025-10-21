/*
 * log_eletemptable.cpp
 * 电能表定义
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "log_eletemptable.h"

Log_EleTempTable::Log_EleTempTable(QObject *parent)
{
    createTable();
}


void Log_EleTempTable::createTable()
{
    QString cmd =
            "create table if not exists %1("
            "id                INTEGER primary key autoincrement not null,"
            "name              TEXT,"
            "startTime         TEXT not null,"
            "startEle          DOUBLE,"
            "endTime           TEXT,"
            "endEle            DOUBLE,"
            "ele        DOUBLE);";
    QSqlQuery query;
    if(!query.exec(cmd.arg(tableName())))
    {
        throwError(query.lastError());
    }
}

Log_EleTempTable *Log_EleTempTable::get()
{
    static Log_EleTempTable* sington = nullptr;
    if(sington == nullptr)
        sington = new Log_EleTempTable();
    return sington;
}


void Log_EleTempTable::insertItem(EleTempItem &item)
{
    item.id = maxId()+1;
    QString cmd = "insert into %1 (id,name,startTime,startEle,endTime,endEle,ele) "
                  "values(:id,:name,:startTime,:startEle,:endTime,:endEle,:ele)";
    modifyItem(item,cmd.arg(tableName()));
//    emit itemChanged(item.id,Insert);
}


bool Log_EleTempTable::modifyItem(const EleTempItem &item, const QString &cmd)
{
    bool ret = true;
    QSqlQuery query;
    query.prepare(cmd);
    query.bindValue(":id",item.id);
    query.bindValue(":name",item.name);
    query.bindValue(":startTime",item.startTime);
    query.bindValue(":startEle",item.startEle);
    query.bindValue(":endTime",item.endTime);
    query.bindValue(":endEle",item.endEle);
    query.bindValue(":ele",item.ele);
    if(!query.exec()) {
        throwError(query.lastError());
        ret = false;
    }

    return ret;
}


void Log_EleTempTable::selectItem(QSqlQuery &query,EleTempItem &item)
{
    item.id = query.value("id").toInt();
    item.name = query.value("name").toString();
    item.startTime = query.value("startTime").toString();
    item.startEle = query.value("startEle").toDouble();

    item.endTime = query.value("endTime").toString();
    item.endEle = query.value("endEle").toDouble();
    item.ele = query.value("ele").toDouble();
}

void Log_EleTempTable::clear()
{
    QSqlQuery query;
    if(!query.exec(QString("DELETE FROM %1 ").arg(tableName())))
        throwError(query.lastError());
}
