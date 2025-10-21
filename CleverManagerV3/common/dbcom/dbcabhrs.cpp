/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "dbcabhrs.h"

DbCabHrs::DbCabHrs()
{
    createTable();
    tableTile = tr("机柜数据纪录");
    hiddens  << 4;
    headList << tr("机房") << tr("模块") << tr("机柜")  << tr("主备路")
             << tr("电压") << tr("电流") << tr("功率") << tr("功率因数") << tr("电能");
}


void DbCabHrs::createTable()
{
    QString cmd =
            "create table if not exists %1("
            "id             INTEGER primary key autoincrement not null,"
            "date           VCHAR,"
            "time           VCHAR,"
            "room           VCHAR,"
            "modular        VCHAR,"
            "cab            VCHAR,"
            "road           VCHAR,"
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


DbCabHrs *DbCabHrs::bulid()
{
    static DbCabHrs* sington = nullptr;
    if(sington == nullptr)
        sington = new DbCabHrs();
    return sington;
}


bool DbCabHrs::insertItem(sDbCabHrsItem &item)
{
    QString cmd = "insert into %1 (date,time,room,modular,cab,road,vol,cur,pow,pf,ele) "
                  "values(:date,:time,:room,:modular,:cab,:road,:vol,:cur,:pow,:pf,:ele)";
    return modifyItem(item,cmd.arg(tableName()));
}


bool DbCabHrs::modifyItem(const sDbCabHrsItem &item, const QString &cmd)
{
    QSqlQuery query(mDb);
    query.prepare(cmd);
    query.bindValue(":date",item.date);
    query.bindValue(":time",item.time);
    query.bindValue(":room",item.room);
    query.bindValue(":modular",item.modular);
    query.bindValue(":cab",item.cab);
    query.bindValue(":road",item.road);
    query.bindValue(":vol",item.vol);
    query.bindValue(":cur",item.cur);
    query.bindValue(":pow",item.pow);
    query.bindValue(":pf",item.pf);
    query.bindValue(":ele",item.ele);
    bool ret = query.exec();
    if(!ret)  throwError(query.lastError());
    return ret;
}


void DbCabHrs::selectItem(QSqlQuery &query,sDbCabHrsItem &item)
{
    item.id = query.value("id").toInt();
    item.date = query.value("date").toString();
    item.time = query.value("time").toString();
    item.room = query.value("room").toString();
    item.modular = query.value("modular").toString();
    item.cab = query.value("cab").toString();
    item.road = query.value("road").toString();
    item.vol = query.value("vol").toDouble();
    item.cur = query.value("cur").toDouble();
    item.pow = query.value("pow").toDouble();
    item.pow = query.value("pow").toDouble();
    item.pf = query.value("pf").toDouble();
    item.ele = query.value("ele").toDouble();
}
