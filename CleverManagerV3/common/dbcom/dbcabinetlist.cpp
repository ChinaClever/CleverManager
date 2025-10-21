/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
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
    createTable();
    tableTile = tr("机柜列表");
    hiddens << 1;
    headList.clear();
    headList << tr("编号") << tr("机房ID") << tr("机房") << tr("机柜") << tr("高度") << tr("功率")
             << tr("主用IP")  << tr("主用级联") << tr("主用类型")
             << tr("备用IP")  << tr("备用级联") << tr("备用类型")
             << tr("横坐标") << tr("纵坐标") ;

    connect(DbRoomList::get(),SIGNAL(itemChanged(int,int)),SLOT(roomItemChange(int,int)));
}

DbCabinetList *DbCabinetList::get()
{
    static DbCabinetList* sington = nullptr;
    if(sington == nullptr)
        sington = new DbCabinetList();
    return sington;
}

void DbCabinetList::createTable()
{
    QString cmd = "create table if not exists %1("
                  "id                INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                  "room_id           INTEGER references %2(%3),"
                  "room              VCHAR,"
                  "cab               VCHAR,"
                  "height            INTEGER,"
                  "pow               INTEGER,"
                  "main_ip           VCHAR,"
                  "main_num          VCHAR,"
                  "main_type         VCHAR,"
                  "spare_ip          VCHAR,"
                  "spare_num         VCHAR,"
                  "spare_type        VCHAR,"
                  "row               INTEGER,"
                  "column            INTEGER"
                  ")";
    QSqlQuery query(mDb);
    if(!query.exec(cmd.arg(tableName()).arg(DbRoomList::get()->tableName()).arg("id")))
    {
        throwError(query.lastError());
    }
}

void DbCabinetList::insertItem(CabinetItem &item)
{
    item.id = maxId()+1;
    item.room_id = DbRoomList::get()->findId(item.room);

    QString cmd = "insert into %1 (id,room_id,room,cab,height,pow,main_ip,main_num,main_type,spare_ip,spare_num,spare_type,row,column) "
                  "values(:id,:room_id,:room,:cab,:height,:pow,:main_ip,:main_num,:main_type,:spare_ip,:spare_num,:spare_type,:row,:column)";
    modifyItem(item,cmd.arg(tableName()));
    emit itemChanged(item.id,Insert);
}

bool DbCabinetList::updateItem(const CabinetItem &item)
{
    QString cab = findById(item.id).cab;
    QString cmd = "update %1 set "
            //                  "id                = :id,"
            "room_id           = :room_id,"
            "room           = :room,"
            "cab              = :cab,"
            "height            = :height,"
            "pow            = :pow,"
            "main_ip       = :main_ip,"
            "main_num     = :main_num,"
            "main_type     = :main_type,"
            "spare_ip      = :spare_ip,"
            "spare_num    = :spare_num,"
            "spare_type     = :spare_type,"
            "row               = :row,"
            "column            = :column"
            " where id = :id";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret) {
        emit itemChanged(item.id,Update);
        emit updateSig(item.id,cab);
    }

    return ret;
}

void DbCabinetList::removeItemsByRoomId(int roomId)
{
   roomItemChange(roomId, Remove);
}

QVector<CabinetItem> DbCabinetList::selItemsByRoomId(int room_id)
{
    return selectItems(QString("where room_id = %1").arg(room_id));
}

QVector<int> DbCabinetList::selRoomIdsByAddress(const QString &addr, int num)
{
    QSqlQuery query(mDb);
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
    QString cmd = QString("room_id=%1").arg(roomId);
    switch (type) {
    case Remove: remove(cmd); break;
    case Update: updateColumn("room", DbRoomList::get()->findById(roomId).room, cmd);  break;
    }
}


QStringList DbCabinetList::list(const QString &room)
{
    QString cmd = QString("where room = \'%1\'").arg(room);
    return listColumn("cab", cmd);
}

int DbCabinetList::contains(const QString &room,  const QString &cab)
{
    QString condition = QString("where room=\'%1\' and cab=\'%2\'").arg(room).arg(cab);
    return count("id", condition);
}


CabinetItem DbCabinetList::find(const QString& room, const QString &cab)
{
    QVector<CabinetItem> items = selectItems(QString("where room=\'%1\' and cab=\'%2\'").arg(room).arg(cab));
    return items.first();
}

int DbCabinetList::findId(const QString& room, const QString &cab)
{
     CabinetItem item = find(room, cab);
     return item.id;
}

bool DbCabinetList::removeCab(const QString &room, const QString &cab)
{
    int id =  findId(room, cab);
    return remove(id);
}

bool DbCabinetList::modifyItem(const CabinetItem &item, const QString &cmd)
{
    QSqlQuery query(mDb);
    query.prepare(cmd);
    query.bindValue(":id",item.id);
    query.bindValue(":cab",item.cab);
    query.bindValue(":height",item.height);
    query.bindValue(":pow",item.pow);
    query.bindValue(":main_ip",item.main_ip);
    query.bindValue(":spare_ip",item.spare_ip);
    query.bindValue(":room_id",item.room_id);
    query.bindValue(":room",item.room);
    query.bindValue(":main_num",item.main_num);
    query.bindValue(":spare_num",item.spare_num);
    query.bindValue(":main_type",item.main_type);
    query.bindValue(":spare_type",item.spare_type);
    query.bindValue(":row",item.row);
    query.bindValue(":column",item.column);
    bool ret = query.exec();
    if(!ret)
        throwError(query.lastError());
    return ret;
}

void DbCabinetList::selectItem(QSqlQuery &query, CabinetItem &item)
{
    item.id = query.value("id").toInt();
    item.cab = query.value("cab").toString();
    item.room = query.value("room").toString();
    item.height = query.value("height").toInt();
    item.pow = query.value("pow").toInt();
    item.room_id = query.value("room_id").toInt();
    item.main_ip = query.value("main_ip").toString();
    item.spare_ip = query.value("spare_ip").toString();
    item.main_num = query.value("main_num").toString();
    item.spare_num = query.value("spare_num").toString();
    item.main_type = query.value("main_type").toString();
    item.spare_type = query.value("spare_type").toString();

    item.row = query.value("row").toInt();
    item.column = query.value("column").toInt();
}
