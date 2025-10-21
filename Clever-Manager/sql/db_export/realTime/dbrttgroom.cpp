/*
 * dbrttgroom.cpp
 * 机房实时统计表
 * 继承自DbTgRoom机房统计表
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "dbrttgroom.h"

DbRtTgRoom::DbRtTgRoom()
{
    createTable();
}

DbRtTgRoom *DbRtTgRoom::get()
{
    static DbRtTgRoom* sington = nullptr;
    if(sington == nullptr)
        sington = new DbRtTgRoom();
    return sington;
}

int DbRtTgRoom::getId(const QString &name)
{
    int id = -1;
    QVector<TgRoomItem> items = selItemsByRoomName(name);
    if(items.size() >= 1) {
        id = items.first().id;
    }
    return id;
}


void DbRtTgRoom::initTime(TgRoomItem &item)
{
    QDateTime dateTime = QDateTime::currentDateTime();
    item.date = dateTime.toString("yyyy-MM-dd");
    item.time = dateTime.toString("hh:mm:ss");
}

void DbRtTgRoom::updateItem(TgRoomItem &item)
{
    initTime(item);
    QString cmd = "update %1 set "
                  "date         = :date,"
                  "time         = :time,"
                  "vol          = :vol,"
                  "cur          = :cur,"
                  "pow          = :pow,"
                  "ele          = :ele,"
                  "pf           = :pf,"
                  "tem          = :tem,"
                  "hum          = :hum,"
                  "allNum       = :allNum,"
                  "lineNum      = :lineNum,"
                  "alarmNum     = :alarmNum,"
                  "offLineNum   = :offLineNum"
                  " where id    = :id";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(!ret)
        insertItem(item);
//    else
//        emit itemChanged(item.id,Update);
}
