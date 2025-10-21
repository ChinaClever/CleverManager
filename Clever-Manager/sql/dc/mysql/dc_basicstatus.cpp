/*
 * dc_basicstatus.cpp
 *
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dc_basicstatus.h"

DC_BasicStatus::DC_BasicStatus()
{

}


bool DC_BasicStatus::insertItem(const DC_StatusItem& item)
{
    QString cmd = "insert into %1 (%2, status) "
                  "values(:rid, :status)";
    bool ret = modifyItem(item,cmd.arg(tableName()).arg(ridName()));
    if(ret)
        emit itemChanged(item.id,Insert);
    return ret;
}

bool DC_BasicStatus::updateItem(const DC_StatusItem& item)
{
    QString cmd = "update %1 set "
                  "%2               = :rid,"
                  "status           = :status,"
                  "update_time      = :update_time"
                  " where id        = :id";
    bool ret = modifyItem(item,cmd.arg(tableName()).arg(ridName()));
    if(ret)
        emit itemChanged(item.id,Update);
    return ret;
}

bool DC_BasicStatus::modifyItem(const DC_StatusItem &item, const QString &cmd)
{
    bool ret = false;
    if(mDb) {
        QSqlQuery query(*mDb);
        query.prepare(cmd);
        query.bindValue(":id",item.id);
        query.bindValue(":rid",item.rid);
        query.bindValue(":status",item.status);
        query.bindValue(":update_time", QTime::currentTime());
        ret = query.exec();
        if(!ret)
            throwError(query.lastError());
    }
    return ret;
}

void DC_BasicStatus::selectItem(QSqlQuery &query, DC_StatusItem &item)
{
    item.id = query.value("id").toInt();
    item.rid = query.value(ridName()).toInt();
    item.status = query.value("status").toInt();
    item.create_time = query.value("create_time").toDateTime();
    item.update_time = query.value("update_time").toDateTime();
}

bool DC_BasicStatus::setStatus(int rid, int status)
{
    return updateColumn("status", status, QString("where %1=%2").arg(ridName()).arg(rid));
}

bool DC_BasicStatus::containRid(int rid)
{
    int ret = count(ridName(), QString("where %1 = %2").arg(ridName()).arg(rid));
    return (ret > 0) ? true:false;
}

bool DC_BasicStatus::updateStatus(int rid, int status)
{
    bool ret = containRid(rid);
    if(ret)
        ret = setStatus(rid, status);
    else {
        DC_StatusItem item;
        item.rid = rid;
        item.status = status;
        ret = insertItem(item);
    }
    return ret;
}
