/*
 * dc_rooms.cpp
 *  数据机房表：数据楼层索引ID，机房名，机房地址
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dc_rooms.h"

DC_Rooms::DC_Rooms()
{

}

DC_Rooms *DC_Rooms::bulid()
{
    static DC_Rooms* sington = nullptr;
    if(sington == nullptr)
        sington = new DC_Rooms();
    return sington;
}

bool DC_Rooms::insertItem(const DC_RoomItem& item)
{
    QString cmd = "insert into %1 (modules_id, name) "
                  "values(:modules_id, :name)";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Insert);
    return ret;
}

bool DC_Rooms::updateItem(const DC_RoomItem& item)
{
    QString cmd = "update %1 set "
                  "modules_id            = :modules_id,"
                  "name            = :name,"
                  "update_time          = :update_time"
                  " where id            = :id";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Update);
    return ret;
}

bool DC_Rooms::modifyItem(const DC_RoomItem &item, const QString &cmd)
{
    bool ret = false;
    if(mDb) {
        QSqlQuery query(*mDb);
        query.prepare(cmd);
        query.bindValue(":id",item.id);
        query.bindValue(":modules_id",item.modules_id);
        query.bindValue(":name",item.name);
        query.bindValue(":update_time", QTime::currentTime());
        ret = query.exec();
        if(!ret)
            throwError(query.lastError());
    }
    return ret;
}

void DC_Rooms::selectItem(QSqlQuery &query, DC_RoomItem &item)
{
    item.id = query.value("id").toInt();
    item.modules_id = query.value("modules_id").toInt();
    item.name = query.value("name").toString();

    item.create_time = query.value("create_time").toDateTime();
    item.update_time = query.value("update_time").toDateTime();
}


