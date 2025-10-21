/*
 * dc_idcsremarks.cpp
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dc_idcsremarks.h"

DC_IdcsRemarks::DC_IdcsRemarks()
{

}


DC_IdcsRemarks *DC_IdcsRemarks::bulid()
{
    static DC_IdcsRemarks* sington = nullptr;
    if(sington == nullptr)
        sington = new DC_IdcsRemarks();
    return sington;
}


bool DC_IdcsRemarks::insertItem(const DC_IdcRemarkItem &item)
{
    QString cmd = "insert into %1 (idcs_id,country,area) "
                  "values(:idcs_id,:country,:area)";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Insert);
    return ret;
}

bool DC_IdcsRemarks::updateItem(const DC_IdcRemarkItem &item)
{
    QString cmd = "update %1 set "
                  "idcs_id         = :idcs_id,"
                  "country         = :country,"
                  "area         = :area,"
                  "update_time  = :update_time"
                  " where id    = :id";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Update);
    return ret;
}

bool DC_IdcsRemarks::modifyItem(const DC_IdcRemarkItem &item, const QString &cmd)
{
    bool ret = false;
    if(mDb) {
        QSqlQuery query(*mDb);
        query.prepare(cmd);
        query.bindValue(":id",item.id);
        query.bindValue(":idcs_id",item.idcs_id);
        query.bindValue(":country",item.country);
        query.bindValue(":area",item.area);
        query.bindValue(":update_time", QTime::currentTime());
        ret = query.exec();
        if(!ret)
            throwError(query.lastError());
    }
    return ret;
}

void DC_IdcsRemarks::selectItem(QSqlQuery &query,DC_IdcRemarkItem &item)
{
    item.id = query.value("id").toInt();
    item.idcs_id = query.value("idcs_id").toInt();
    item.country = query.value("country").toString();
    item.area = query.value("area").toString();
    item.create_time = query.value("create_time").toDateTime();
    item.update_time = query.value("update_time").toDateTime();
}


