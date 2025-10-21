/*
 * dbroomlist.cpp
 *  机房表
 *
 *  Created on: 2016年10月11日
 *      Author:
 */
#include "dbroomlist.h"


DbRoomList::DbRoomList()
{
    createTable();
    tableTile = tr("机房列表");
    hiddens << 2;
    headList.clear();
    headList << tr("编号") << tr("机房") << tr("描述");
}

DbRoomList *DbRoomList::get()
{
    static DbRoomList* sington = nullptr;
    if(sington == nullptr)
        sington = new DbRoomList();
    return sington;
}

void DbRoomList::createTable()
{
    QString cmd =
            "create table if not exists %1("
            "id                INTEGER primary key autoincrement not null,"
            "room              TEXT not null,"
            "address           TEXT);";
    QSqlQuery query(mDb);
    if(!query.exec(cmd.arg(tableName())))
    {
        throwError(query.lastError());
    }
}

void DbRoomList::insertItem(RoomItem &item)
{
    item.id = maxId()+1;
    QString cmd = "insert into %1 (id,room,address) values(:id,:room,:address)";
    modifyItem(item,cmd.arg(tableName()));
    emit itemChanged(item.id,Insert);
}

void DbRoomList::updateItem(const RoomItem &item)
{
    QString roomName = findById(item.id).room;
    emit updateSig(roomName, item.room);

    QString cmd = "update %1 set room = :room,address = :address where id = :id";
    modifyItem(item,cmd.arg(tableName()));
    emit itemChanged(item.id,Update);
}

void DbRoomList::modifyItem(const RoomItem &item, const QString &cmd)
{
    QSqlQuery query(mDb);
    query.prepare(cmd);
    query.bindValue(":id",item.id);
    query.bindValue(":room",item.room);
    query.bindValue(":address",item.address);
    if(!query.exec())
        throwError(query.lastError());
}

void DbRoomList::selectItem(QSqlQuery &query,RoomItem &item)
{
    item.id = query.value("id").toInt();
    item.room = query.value("room").toString();
    item.address = query.value("address").toString();
}

QStringList DbRoomList::list()
{
    return listColumn("room", "");
}

int DbRoomList::contains(const QString &room)
{
    QString condition = QString("where room=\'%1\'").arg(room);
    return count("id", condition);
}

RoomItem DbRoomList::find(const QString& room)
{
    QVector<RoomItem> items = selectItems(QString("where room = \'%1\'").arg(room));
    return items.first();
}

int DbRoomList::findId(const QString& room)
{
     RoomItem item = find(room);
     return item.id;
}

void DbRoomList::removeRoom(const QString& room)
{
    RoomItem item = find(room);
    bool ret = remove(item.id);
    if(ret) {
        emit delSig(item.room);
    }
}
