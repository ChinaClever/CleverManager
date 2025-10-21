/*
 * DC_Cabinets.cpp
 *  机柜表
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dc_cabinets.h"

DC_Cabinets::DC_Cabinets()
{

}


DC_Cabinets *DC_Cabinets::bulid()
{
    static DC_Cabinets* sington = nullptr;
    if(sington == nullptr)
        sington = new DC_Cabinets();
    return sington;
}

bool DC_Cabinets::insertItem(const DC_CabinetItem& item)
{
    QString cmd = "insert into %1 (rooms_id, name,height,row_index,column_index) "
                  "values(:rooms_id, :name,:height,:row_index,:column_index)";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Insert);
    return ret;
}

bool DC_Cabinets::updateItem(const DC_CabinetItem& item)
{
    QString cmd = "update %1 set "
                  "rooms_id            = :rooms_id,"
                  "name                = :name,"
                  "height              = :height,"
                  "row_index           = :row_index,"
                  "column_index        = :column_index,"
                  "update_time         = :update_time"
                  " where id           = :id";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Update);
    return ret;
}


bool DC_Cabinets::modifyItem(const DC_CabinetItem &item, const QString &cmd)
{
    bool ret = false;
    if(mDb) {
        QSqlQuery query(*mDb);
        query.prepare(cmd);
        query.bindValue(":id",item.id);
        query.bindValue(":rooms_id",item.rooms_id);
        query.bindValue(":name",item.name);
        query.bindValue(":height",item.height);
        query.bindValue(":row_index",item.row_index);
        query.bindValue(":column_index",item.column_index);
        query.bindValue(":update_time", QTime::currentTime());
        ret = query.exec();
        if(!ret)
            throwError(query.lastError());
    }
    return ret;
}

void DC_Cabinets::selectItem(QSqlQuery &query, DC_CabinetItem &item)
{
    item.id = query.value("id").toInt();
    item.rooms_id = query.value("rooms_id").toInt();
    item.name = query.value("name").toString();
    item.height = query.value("height").toInt();
    item.row_index = query.value("row_index").toInt();
    item.column_index = query.value("column_index").toInt();
    item.create_time = query.value("create_time").toDateTime();
    item.update_time = query.value("update_time").toDateTime();
}




