/*
 * dc_modules.cpp
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dc_modules.h"

DC_Modules::DC_Modules()
{

}


DC_Modules *DC_Modules::bulid()
{
    static DC_Modules* sington = nullptr;
    if(sington == nullptr)
        sington = new DC_Modules();
    return sington;
}


bool DC_Modules::insertItem(const DC_ModuleItem &item)
{
    QString cmd = "insert into %1 (idcs_id,name) "
                  "values(:idcs_id,:name)";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Insert);
    return ret;
}

bool DC_Modules::updateItem(const DC_ModuleItem &item)
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

bool DC_Modules::modifyItem(const DC_ModuleItem &item, const QString &cmd)
{
    bool ret = false;
    if(mDb) {
        QSqlQuery query(*mDb);
        query.prepare(cmd);
        query.bindValue(":id",item.id);
        query.bindValue(":idcs_id",item.idcs_id);
        query.bindValue(":name",item.name);
        query.bindValue(":update_time", QTime::currentTime());
        ret = query.exec();
        if(!ret)
            throwError(query.lastError());
    }
    return ret;
}

void DC_Modules::selectItem(QSqlQuery &query,DC_ModuleItem &item)
{
    item.id = query.value("id").toInt();
    item.idcs_id = query.value("idcs_id").toInt();
    item.name = query.value("name").toString();
    item.create_time = query.value("create_time").toDateTime();
    item.update_time = query.value("update_time").toDateTime();
}

