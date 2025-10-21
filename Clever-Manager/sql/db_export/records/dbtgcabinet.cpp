#include "dbtgcabinet.h"

DbTgCabinet::DbTgCabinet()
{
    createTable();
}

void DbTgCabinet::createTable()
{
    QString cmd =
            "create table if not exists %1("
            "id                INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
            "date              TEXT not null,"
            "time              TEXT,"
            "roomName          TEXT,"
            "cabName           TEXT,"
            "cura              DOUBLE,"
            "curb              DOUBLE,"
            "cur               DOUBLE,"
            "vola              DOUBLE,"
            "volb              DOUBLE,"
            "elea              DOUBLE,"
            "eleb              DOUBLE,"
            "pow               DOUBLE);";
    QSqlQuery query;
    if(!query.exec(cmd.arg(tableName())))
    {
        throwError(query.lastError());
    }
}

DbTgCabinet *DbTgCabinet::get()
{
    static DbTgCabinet* sington = nullptr;
    if(sington == nullptr)
        sington = new DbTgCabinet();
    return sington;
}

void DbTgCabinet::initItem(TgCabItem &item)
{
    // item.id = maxId()+1;
    QDateTime dateTime = QDateTime::currentDateTime();
    item.date = dateTime.toString("yyyy-MM-dd");
    item.time = dateTime.toString("hh:mm:ss");
}

void DbTgCabinet::insertItem(TgCabItem &item)
{
    //initItem(item);
    QString cmd = "insert into %1 (id,date,time,roomName,cabName,cura,curb,cur,vola,volb,elea,eleb,pow) "
                  "values(:id,:date,:time,:roomName,:cabName,:cura,:curb,:cur,:vola,:volb,:elea,:eleb,:pow)";
    modifyItem(item,cmd.arg(tableName()));
    //    emit itemChanged(item.id,Insert);
}



bool DbTgCabinet::modifyItem(const TgCabItem &item, const QString &cmd)
{
    bool ret = true;
    QSqlQuery query;
    query.prepare(cmd);
    // query.bindValue(":id",item.id);
    query.bindValue(":date",item.date);
    query.bindValue(":time",item.time);
    query.bindValue(":roomName",item.roomName);
    query.bindValue(":cabName",item.cabName);
    query.bindValue(":cura",item.cura);
    query.bindValue(":curb",item.curb);
    query.bindValue(":cur",item.cur);
    query.bindValue(":vola",item.vola);
    query.bindValue(":volb",item.volb);
    query.bindValue(":elea",item.elea);
    query.bindValue(":eleb",item.eleb);
    query.bindValue(":pow",item.pow);
    if(!query.exec()) {
        throwError(query.lastError());
        ret = false;
    }

    return ret;
}

void DbTgCabinet::selectItem(QSqlQuery &query,TgCabItem &item)
{
    item.id = query.value("id").toInt();
    item.date = query.value("date").toString();
    item.time = query.value("time").toString();
    item.roomName = query.value("roomName").toString();
    item.cabName = query.value("cabName").toString();
    item.vola = query.value("vola").toDouble();
    item.volb = query.value("volb").toDouble();
    item.cura = query.value("cura").toDouble();
    item.curb = query.value("curb").toDouble();
    item.cur = query.value("cur").toDouble();
    item.elea = query.value("elea").toDouble();
    item.eleb = query.value("eleb").toDouble();
    item.pow = query.value("pow").toDouble();
}

void DbTgCabinet::selectTgCabNameCountItem(QSqlQuery &query,TgCabNameCountItem &item)
{
    item.count = query.value("count(*)").toInt();
    item.cabName = query.value("cabName").toString();
}

/**
 * @brief 根据机房名来选择
 * @param roomName 机房名
 * @return
 */
QVector<TgCabItem> DbTgCabinet::selItemsByRoomName(const QString& condition)
{
    return selectItems(condition);
}

QVector<TgCabNameCountItem>  DbTgCabinet::selectItemsGroupByCabName(const QString &condition)
{
    QSqlQuery query;
    QString sql = QString("SELECT cabName,count(*) from %1 %2 group by cabName").arg(tableName()).arg(condition);
    query.prepare(sql);
    QVector<TgCabNameCountItem> items;
    if(query.exec()){
        TgCabNameCountItem item;
        while(query.next()){
            selectTgCabNameCountItem(query,item);
            items.append(item);
        }
    } else {
        qDebug()<<" "<<query.lastError().databaseText();
        qDebug()<<" "<<query.lastError().driverText();
        qDebug()<<sql;
        throwError(query.lastError());
    }
    return items;
}
