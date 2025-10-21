/*
 * dc_idcs.cpp
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dc_idcs.h"

DC_Idcs::DC_Idcs()
{

}


DC_Idcs *DC_Idcs::bulid()
{
    static DC_Idcs* sington = nullptr;
    if(sington == nullptr)
        sington = new DC_Idcs();
    return sington;
}


bool DC_Idcs::insertItem(const DC_IdcItem &item)
{
    QString cmd = "insert into %1 (companys_id,name) "
                  "values(:companys_id,:name)";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Insert);
    return ret;
}

bool DC_Idcs::updateItem(const DC_IdcItem &item)
{
    QString cmd = "update %1 set "
                  "name         = :name,"
                  "update_time  = :update_time"
                  " where id    = :id";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Update);
    return ret;
}

bool DC_Idcs::modifyItem(const DC_IdcItem &item, const QString &cmd)
{
    bool ret = false;
    if(mDb) {
        QSqlQuery query(*mDb);
        query.prepare(cmd);
        query.bindValue(":id",item.id);
        query.bindValue(":companys_id",item.companys_id);
        query.bindValue(":name",item.name);
        query.bindValue(":update_time", QTime::currentTime());
        ret = query.exec();
        if(!ret)
            throwError(query.lastError());
    }
    return ret;
}

void DC_Idcs::selectItem(QSqlQuery &query,DC_IdcItem &item)
{
    item.id = query.value("id").toInt();
    item.companys_id = query.value("companys_id").toInt();
    item.name = query.value("name").toString();
    item.create_time = query.value("create_time").toDateTime();
    item.update_time = query.value("update_time").toDateTime();
}


