/*
 * dbcabinetlist.cpp
 *  机柜表
 *
 *  Created on: 2016年10月11日
 *      Author:
 */
#include "dbcabinetlist.h"
#include "dbroomlist.h"

DbCabinetList::DbCabinetList()
{
    QString cmd = "create table if not exists %1("
                  "id                INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                  "room_id           INTEGER references %2(%3),"
                  "name              TEXT,"
                  "height            INTEGER,"
                  "main_ip           TEXT,"
                  "main_num          INTEGER,"
                  "main_type         INTEGER,"
                  "spare_ip          TEXT,"
                  "spare_num         INTEGER,"
                  "spare_type        INTEGER,"
                  "row               INTEGER,"
                  "column            INTEGER"
                  ")";
    QSqlQuery query;
    if(!query.exec(cmd.arg(tableName()).arg(DbRoomList::get()->tableName()).arg("id")))
    {
        throwError(query.lastError());
    }
    connect(DbRoomList::get(),SIGNAL(itemChanged(int,int)),SLOT(roomItemChange(int,int)));
}

DbCabinetList *DbCabinetList::get()
{
    static DbCabinetList* sington = nullptr;
    if(sington == nullptr)
        sington = new DbCabinetList();
    return sington;
}

void DbCabinetList::insertItem(const CabinetItem &item)
{
    QString cmd = "insert into %1 (id,room_id,name,height,main_ip,main_num,main_type,spare_ip,spare_num,spare_type,row,column) "
                  "values(:id,:room_id,:name,:height,:main_ip,:main_num,:main_type,:spare_ip,:spare_num,:spare_type,:row,:column)";
    modifyItem(item,cmd.arg(tableName()));
    emit itemChanged(item.id,Insert);
}

void DbCabinetList::updateItem(const CabinetItem &item)
{
    QString name = findItemById(item.id).first().name;
    QString cmd = "update %1 set "
//                  "id                = :id,"
                  "room_id           = :room_id,"
                  "name              = :name,"
                  "height            = :height,"
                  "main_ip       = :main_ip,"
                  "main_num     = :main_num,"
                  "main_type     = :main_type,"
                  "spare_ip      = :spare_ip,"
                  "spare_num    = :spare_num,"
                  "spare_type     = :spare_type,"
                  "row               = :row,"
                  "column            = :column"
                  " where id = :id";
    modifyItem(item,cmd.arg(tableName()));
    emit itemChanged(item.id,Update);
    emit updateSig(item.id,name);
}

void DbCabinetList::removeItemsByRoomId(int roomId)
{
    QVector<CabinetItem> items = selItemsByRoomId(roomId);
    foreach(const CabinetItem& item,items)
        removeItem(item);
}

QVector<CabinetItem> DbCabinetList::selItemsByRoomId(int room_id)
{
    return selectItems(QString("where room_id = %1").arg(room_id));
}

QVector<int> DbCabinetList::selRoomIdsByAddress(const QString &addr, int num)
{
    QSqlQuery query;
    QString sql =
            QString("SELECT DISTINCT room_id from %1 where %2")
            .arg(tableName())
            .arg(QString("(main_ip = \"%1\" and main_num = %2) or (spare_ip = \"%1\" and spare_num = %2)").arg(addr).arg(num));
    query.prepare(sql);
    QVector<int> items;
    if(query.exec()){
        while(query.next())
            items.append(query.value("room_id").toInt());
    } else {
        qDebug()<<" "<<query.lastError().databaseText();
        qDebug()<<" "<<query.lastError().driverText();
        qDebug()<<sql;
        throwError(query.lastError());
    }
    return items;
}

void DbCabinetList::roomItemChange(int roomId, int type)
{
    if(type == Remove)
        this->removeItemsByRoomId(roomId);
}

void DbCabinetList::modifyItem(const CabinetItem &item, const QString &cmd)
{
    QSqlQuery query;
    query.prepare(cmd);
    query.bindValue(":id",item.id);
    query.bindValue(":name",item.name);
    query.bindValue(":height",item.height);
    query.bindValue(":main_ip",item.main_ip);
    query.bindValue(":spare_ip",item.spare_ip);
    query.bindValue(":room_id",item.room_id);
    query.bindValue(":main_num",item.main_num);
    query.bindValue(":spare_num",item.spare_num);
    query.bindValue(":main_type",item.main_type);
    query.bindValue(":spare_type",item.spare_type);
    query.bindValue(":row",item.row);
    query.bindValue(":column",item.column);
    if(!query.exec())
        throwError(query.lastError());
}

void DbCabinetList::selectItem(QSqlQuery &query, CabinetItem &item)
{
    item.id = query.value("id").toInt();
    item.name = query.value("name").toString();
    item.main_ip = query.value("main_ip").toString();
    item.spare_ip = query.value("spare_ip").toString();
    item.height = query.value("height").toInt();
    item.room_id = query.value("room_id").toInt();
    item.main_num = query.value("main_num").toInt();
    item.spare_num = query.value("spare_num").toInt();
    item.main_type = query.value("main_type").toInt();
    item.spare_type = query.value("spare_type").toInt();
    item.row = query.value("row").toInt();
    item.column = query.value("column").toInt();
}
