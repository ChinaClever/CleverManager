/*
 * DC_DevTypes.cpp
 *
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dc_devtypes.h"

DC_DevTypes::DC_DevTypes()
{

}


DC_DevTypes *DC_DevTypes::bulid()
{
    static DC_DevTypes* sington = nullptr;
    if(sington == nullptr)
        sington = new DC_DevTypes();
    return sington;
}

bool DC_DevTypes::insertItem(const DC_DevTypeItem& item)
{
    QString cmd = "insert into %1 (type_index,name) "
                  "values(:type_index,:name)";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Insert);
    return ret;
}

bool DC_DevTypes::updateItem(const DC_DevTypeItem& item)
{
    QString cmd = "update %1 set "
                  "type_index           = :type_index,"
                  "name                 = :name,"
                  "update_time          = :update_time"
                  " where id            = :id";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Update);
    return ret;
}

bool DC_DevTypes::modifyItem(const DC_DevTypeItem &item, const QString &cmd)
{
    bool ret = false;
    if(mDb) {
        QSqlQuery query(*mDb);
        query.prepare(cmd);
        query.bindValue(":id",item.id);
        query.bindValue(":type_index",item.type_index);
        query.bindValue(":name",item.name);
        query.bindValue(":update_time", QTime::currentTime());
        ret = query.exec();
        if(!ret)
            throwError(query.lastError());
    }
    return ret;
}

void DC_DevTypes::selectItem(QSqlQuery &query, DC_DevTypeItem &item)
{
    item.id = query.value("id").toInt();
    item.type_index = query.value("type_index").toInt();
    item.name = query.value("name").toString();

    item.create_time = query.value("create_time").toDateTime();
    item.update_time = query.value("update_time").toDateTime();
}


QVector<int> DC_DevTypes::listTypes()
{
    return listColumnToInt("type_index", "");
}

