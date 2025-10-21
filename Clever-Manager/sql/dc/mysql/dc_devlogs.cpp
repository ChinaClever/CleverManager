/*
 * dc_devlogs.cpp
 *
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dc_devlogs.h"

DC_DevLogs::DC_DevLogs()
{

}

DC_DevLogs *DC_DevLogs::bulid()
{
    static DC_DevLogs* sington = nullptr;
    if(sington == nullptr)
        sington = new DC_DevLogs();
    return sington;
}

bool DC_DevLogs::insertItem(const DC_DevLogItem& item)
{
    QString cmd = "insert into %1 (pdu_devices_id, item_type, contents) "
                  "values(:pdu_devices_id, :item_type, :contents)";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Insert);
    return ret;
}

bool DC_DevLogs::updateItem(const DC_DevLogItem &item)
{
    QString cmd = "update %1 set "
                  "pdu_devices_id       = :pdu_devices_id,"
                  "item_type            = :item_type,"
                  "contents             = :contents,"
                  "update_time          = :update_time"
                  " where id            = :id";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Update);
    return ret;
}

bool DC_DevLogs::modifyItem(const DC_DevLogItem &item, const QString &cmd)
{
    bool ret = false;
    if(mDb) {
        QSqlQuery query(*mDb);
        query.prepare(cmd);
        query.bindValue(":id",item.id);
        query.bindValue(":pdu_devices_id",item.pdu_devices_id);
        query.bindValue(":item_type",item.item_type);
        query.bindValue(":contents",item.contents);
        query.bindValue(":update_time", QTime::currentTime());
        ret = query.exec();
        if(!ret)
            throwError(query.lastError());
    }
    return ret;
}

void DC_DevLogs::selectItem(QSqlQuery &query, DC_DevLogItem &item)
{
    item.id = query.value("id").toInt();
    item.pdu_devices_id = query.value("pdu_devices_id").toInt();
    item.item_type = query.value("item_type").toInt();
    item.contents = query.value("contents").toString();
    item.create_time = query.value("create_time").toDateTime();
    item.update_time = query.value("update_time").toDateTime();
}


