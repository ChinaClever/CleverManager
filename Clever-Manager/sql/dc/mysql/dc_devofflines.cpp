/*
 * DC_DevOfflines.cpp
 *
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dc_devofflines.h"

DC_DevOfflines::DC_DevOfflines()
{

}


DC_DevOfflines *DC_DevOfflines::bulid()
{
    static DC_DevOfflines* sington = nullptr;
    if(sington == nullptr)
        sington = new DC_DevOfflines();
    return sington;
}

bool DC_DevOfflines::insertItem(const DC_DevOfflineItem& item)
{
    QString cmd = "insert into %1 (pdu_devices_id,contents) "
                  "values(:pdu_devices_id,:contents)";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Insert);
    return ret;
}

bool DC_DevOfflines::updateItem(const DC_DevOfflineItem& item)
{
    QString cmd = "update %1 set "
                  "pdu_devices_id           = :pdu_devices_id,"
                  "contents                 = :contents,"
                  "update_time          = :update_time"
                  " where id            = :id";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Update);
    return ret;
}

bool DC_DevOfflines::modifyItem(const DC_DevOfflineItem &item, const QString &cmd)
{
    bool ret = false;
    if(mDb) {
        QSqlQuery query(*mDb);
        query.prepare(cmd);
        query.bindValue(":id",item.id);
        query.bindValue(":pdu_devices_id",item.pdu_devices_id);
        query.bindValue(":contents",item.contents);
        query.bindValue(":update_time", QTime::currentTime());
        ret = query.exec();
        if(!ret)
            throwError(query.lastError());
    }
    return ret;
}

void DC_DevOfflines::selectItem(QSqlQuery &query, DC_DevOfflineItem &item)
{
    item.id = query.value("id").toInt();
    item.pdu_devices_id = query.value("pdu_devices_id").toInt();
    item.contents = query.value("contents").toString();

    item.create_time = query.value("create_time").toDateTime();
    item.update_time = query.value("update_time").toDateTime();
}

