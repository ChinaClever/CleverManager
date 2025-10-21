/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "dbroomhrs.h"


DbRoomHrs::DbRoomHrs()
{
    createTable();
    tableTile = tr("机房数据纪录");
    headList << tr("机房") << tr("电压") << tr("电流") << tr("功率") << tr("功率因数") << tr("电能");
}


void DbRoomHrs::createTable()
{
    QString cmd =
            "create table if not exists %1("
            "id             INTEGER primary key autoincrement not null,"
            "date           VCHAR,"
            "time           VCHAR,"
            "room           VCHAR,"
            "vol            DOUBLE,"
            "cur            DOUBLE,"
            "pow            DOUBLE,"
            "pf             DOUBLE,"
            "ele            DOUBLE);";
    QSqlQuery query(mDb);
    if(!query.exec(cmd.arg(tableName()))) {
        throwError(query.lastError());
    }
}


DbRoomHrs *DbRoomHrs::bulid()
{
    static DbRoomHrs* sington = nullptr;
    if(sington == nullptr)
        sington = new DbRoomHrs();
    return sington;
}


bool DbRoomHrs::insertItem(sDbRoomHrsItem &item)
{
    QString cmd = "insert into %1 (date,time,room,vol,cur,pow,pf,ele) "
                  "values(:date,:time,:room,:vol,:cur,:pow,:pf,:ele)";
    return modifyItem(item,cmd.arg(tableName()));
}


bool DbRoomHrs::modifyItem(const sDbRoomHrsItem &item, const QString &cmd)
{
    QSqlQuery query(mDb);
    query.prepare(cmd);
    query.bindValue(":date",item.date);
    query.bindValue(":time",item.time);
    query.bindValue(":room",item.room);
    query.bindValue(":vol",item.vol);
    query.bindValue(":cur",item.cur);
    query.bindValue(":pow",item.pow);
    query.bindValue(":pf",item.pf);
    query.bindValue(":ele",item.ele);
    bool ret = query.exec();
    if(!ret)  throwError(query.lastError());
    return ret;
}


void DbRoomHrs::selectItem(QSqlQuery &query,sDbRoomHrsItem &item)
{
    item.id = query.value("id").toInt();
    item.date = query.value("date").toString();
    item.time = query.value("time").toString();
    item.room = query.value("room").toString();
    item.vol = query.value("vol").toDouble();
    item.cur = query.value("cur").toDouble();
    item.pow = query.value("pow").toDouble();
    item.pow = query.value("pow").toDouble();
    item.pf = query.value("pf").toDouble();
    item.ele = query.value("ele").toDouble();
}

