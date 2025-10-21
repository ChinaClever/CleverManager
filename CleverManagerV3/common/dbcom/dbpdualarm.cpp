/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "dbpdualarm.h"

DbPduAlarm::DbPduAlarm()
{
    createTable();
    tableTile = tr("PDU报警日志");
    hiddens  << 4;
    headList << tr("机房") << tr("模块") << tr("机柜")  << tr("主备路") << tr("设备类型")
             << tr("设备IP") << tr("级联编号") << tr("报警项目") << tr("报警描述");
}


void DbPduAlarm::createTable()
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
            "dev_type       VCHAR,"
            "ip             VCHAR,"
            "dev_num        VCHAR,"
            "item           VCHAR,"
            "msg        VCHAR);";
    QSqlQuery query(mDb);
    if(!query.exec(cmd.arg(tableName()))) {
        throwError(query.lastError());
    }
}


DbPduAlarm *DbPduAlarm::bulid()
{
    static DbPduAlarm* sington = nullptr;
    if(sington == nullptr)
        sington = new DbPduAlarm();
    return sington;
}


bool DbPduAlarm::insertItem(sDbAlarmItem &item)
{
    QString cmd = "insert into %1 (date,time,room,modular,cab,road,dev_type,ip,dev_num,item,msg) "
                  "values(:date,:time,:room,:modular,:cab,:road,:dev_type,:ip,:dev_num,:item,:msg)";
    return modifyItem(item,cmd.arg(tableName()));
}


bool DbPduAlarm::modifyItem(const sDbAlarmItem &item, const QString &cmd)
{
    QSqlQuery query(mDb);
    query.prepare(cmd);
    query.bindValue(":date",item.date);
    query.bindValue(":time",item.time);
    query.bindValue(":room",item.room);
    query.bindValue(":modular",item.modular);
    query.bindValue(":cab",item.cab);
    query.bindValue(":road",item.road);
    query.bindValue(":dev_type",item.dev_type);
    query.bindValue(":ip",item.ip);
    query.bindValue(":dev_num",item.dev_num);
    query.bindValue(":item",item.item);
    query.bindValue(":msg",item.msg);
    bool ret = query.exec();
    if(!ret)  throwError(query.lastError());
    return ret;
}

