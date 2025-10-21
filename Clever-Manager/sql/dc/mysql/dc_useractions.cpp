/*
 * dc_useractions.cpp
 *
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dc_useractions.h"

DC_UserActions::DC_UserActions()
{

}

DC_UserActions *DC_UserActions::bulid()
{
    static DC_UserActions* sington = nullptr;
    if(sington == nullptr)
        sington = new DC_UserActions();
    return sington;
}

bool DC_UserActions::insertItem(const DC_UserActionsItem& item)
{
    QString cmd = "insert into %1 (modules_id,name,operation,content) "
                  "values(:modules_id,:name,:operation,:content)";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Insert);
    return ret;
}

bool DC_UserActions::updateItem(const DC_UserActionsItem& item)
{
    QString cmd = "update %1 set "
                  "modules_id           = :modules_id,"
                  "name                 = :name,"
                  "operation            = :operation,"
                  "content              = :content,"
                  "update_time          = :update_time"
                  " where id            = :id";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Update);
    return ret;
}

bool DC_UserActions::modifyItem(const DC_UserActionsItem &item, const QString &cmd)
{
    bool ret = false;
    if(mDb) {
        QSqlQuery query(*mDb);
        query.prepare(cmd);
        query.bindValue(":id",item.id);
        query.bindValue(":modules_id",item.modules_id);
        query.bindValue(":name",item.name);
        query.bindValue(":operation",item.operation);
        query.bindValue(":content",item.content);
        query.bindValue(":update_time", QTime::currentTime());
        ret = query.exec();
        if(!ret)
            throwError(query.lastError());
    }
    return ret;
}

void DC_UserActions::selectItem(QSqlQuery &query, DC_UserActionsItem &item)
{
    item.id = query.value("id").toInt();
    item.modules_id = query.value("modules_id").toInt();
    item.name = query.value("name").toString();
    item.operation = query.value("operation").toString();
    item.content = query.value("content").toString();
    item.create_time = query.value("create_time").toDateTime();
    item.update_time = query.value("update_time").toDateTime();
}
