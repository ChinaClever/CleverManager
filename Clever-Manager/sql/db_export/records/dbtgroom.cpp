/*
 * dbtgroom.cpp
 * 机房统计表
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "dbtgroom.h"
#include "common.h"
#include "sql/dc/storage/clouddevrecord.h"

DbTgRoom::DbTgRoom()
{
    createTable();
}

void DbTgRoom::createTable()
{
    QString cmd =
            "create table if not exists %1("
            "id                INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
            "date              TEXT not null,"
            "time              TEXT,"
            "roomName          TEXT,"
            "vol               DOUBLE,"
            "cur               DOUBLE,"
            "pow               DOUBLE,"
            "ele               DOUBLE,"
            "pf                DOUBLE,"
            "tem               DOUBLE,"
            "hum               DOUBLE,"
            "allNum            INTEGER,"
            "lineNum           INTEGER,"
            "alarmNum          INTEGER,"
            "offLineNum        INTEGER);";
    QSqlQuery query;
    if(!query.exec(cmd.arg(tableName())))
    {
        throwError(query.lastError());
    }
}

DbTgRoom *DbTgRoom::get()
{
    static DbTgRoom* sington = nullptr;
    if(sington == nullptr)
        sington = new DbTgRoom();
    return sington;
}

void DbTgRoom::initItem(TgRoomItem &item)
{
   // item.id = maxId()+1;
    QDateTime dateTime = QDateTime::currentDateTime();
    item.date = dateTime.toString("yyyy-MM-dd");
    item.time = dateTime.toString("hh:mm:ss");
}

void DbTgRoom::insertItem(TgRoomItem &item)
{
    //initItem(item);
    QString cmd = "insert into %1 (id,date,time,roomName,vol,cur,pow,ele,pf,tem,hum,allNum,lineNum,alarmNum,offLineNum) "
                  "values(:id,:date,:time,:roomName,:vol,:cur,:pow,:ele,:pf,:tem,:hum,:allNum,:lineNum,:alarmNum,:offLineNum)";
    modifyItem(item,cmd.arg(tableName()));
//    emit itemChanged(item.id,Insert);
}



bool DbTgRoom::modifyItem(const TgRoomItem &item, const QString &cmd)
{
    bool ret = true;
    QSqlQuery query;
    query.prepare(cmd);
    // query.bindValue(":id",item.id);
    query.bindValue(":date",item.date);
    query.bindValue(":time",item.time);
    query.bindValue(":roomName",item.roomName);
    query.bindValue(":vol",item.vol);
    query.bindValue(":cur",item.cur);
    query.bindValue(":pow",item.pow);
    query.bindValue(":ele",item.ele);
    query.bindValue(":pf",item.pf);
    query.bindValue(":tem",item.tem);
    query.bindValue(":hum",item.hum);
    query.bindValue(":allNum",item.allNum);
    query.bindValue(":lineNum",item.lineNum);
    query.bindValue(":alarmNum",item.alarmNum);
    query.bindValue(":offLineNum",item.offLineNum);
    if(!query.exec()) {
        throwError(query.lastError());
        ret = false;
    }

    return ret;
}

void DbTgRoom::selectItem(QSqlQuery &query,TgRoomItem &item)
{
    item.id = query.value("id").toInt();
    item.date = query.value("date").toString();
    item.time = query.value("time").toString();
    item.roomName = query.value("roomName").toString();
    item.vol = query.value("vol").toDouble();
    item.cur = query.value("cur").toDouble();
    item.pow = query.value("pow").toDouble();
    item.ele = query.value("ele").toDouble();
    item.pf = query.value("pf").toDouble();
    item.tem = query.value("tem").toDouble();
    item.hum = query.value("hum").toDouble();
    item.allNum = query.value("allNum").toInt();
    item.lineNum = query.value("lineNum").toInt();
    item.alarmNum = query.value("alarmNum").toInt();
    item.offLineNum = query.value("offLineNum").toInt();
}

/**
 * @brief 根据机房名来选择
 * @param roomName 机房名
 * @return
 */
QVector<TgRoomItem> DbTgRoom::selItemsByRoomName(const QString& roomName)
{
    return selectItems(QString("where roomName = \"%1\"").arg(roomName));
}

/**
 * @brief 删除某一个机房所有的数据
 * @param roomName
 */
void DbTgRoom::removeItemsByRoomName(const QString& roomName)
{
    QVector<TgRoomItem> items = selItemsByRoomName(roomName);
    foreach(const TgRoomItem& item,items)
        removeItem(item);
}

QVector<TgRoomItem> DbTgRoom::query(const QString& condition)
{
    return selectItems(condition);
}
