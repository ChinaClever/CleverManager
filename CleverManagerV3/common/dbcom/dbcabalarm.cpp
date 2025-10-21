/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "dbcabalarm.h"

DbCabAlarm::DbCabAlarm()
{
    createTable();
    tableTile = tr("机柜报警纪录");
    hiddens  << 4;
    headList << tr("机房") << tr("模块") << tr("机柜") << tr("报警项目") << tr("报警描述");
}


void DbCabAlarm::createTable()
{
    QString cmd =
            "create table if not exists %1("
            "id             INTEGER primary key autoincrement not null,"
            "date           VCHAR,"
            "time           VCHAR,"
            "room           VCHAR,"
            "modular        VCHAR,"
            "cab            VCHAR,"
            "item           VCHAR,"
            "msg        VCHAR);";
    QSqlQuery query(mDb);
    if(!query.exec(cmd.arg(tableName()))) {
        throwError(query.lastError());
    }
}


DbCabAlarm *DbCabAlarm::bulid()
{
    static DbCabAlarm* sington = nullptr;
    if(sington == nullptr)
        sington = new DbCabAlarm();
    return sington;
}


bool DbCabAlarm::insertItem(sDbCabAlarmItem &item)
{
    QString cmd = "insert into %1 (date,time,room,modular,cab,item,msg) "
                  "values(:date,:time,:room,:modular,:cab,:item,:msg)";
    return modifyItem(item,cmd.arg(tableName()));
}


bool DbCabAlarm::modifyItem(const sDbCabAlarmItem &item, const QString &cmd)
{
    QSqlQuery query(mDb);
    query.prepare(cmd);
    query.bindValue(":date",item.date);
    query.bindValue(":time",item.time);
    query.bindValue(":room",item.room);
    query.bindValue(":modular",item.modular);
    query.bindValue(":cab",item.cab);
    query.bindValue(":item",item.item);
    query.bindValue(":msg",item.msg);
    bool ret = query.exec();
    if(!ret)  throwError(query.lastError());
    return ret;
}
