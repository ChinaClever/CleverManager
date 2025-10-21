/*
 * db_usr.cpp
 * 用户数据库表
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "db_usr.h"
#include "common.h"

DbUsrTable::DbUsrTable()
{
    QString cmd =
            "create table if not exists %1("
            "id             INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
            "dateTime       TEXT,"
            "name           TEXT not null,"
            "pwd            TEXT,"
            "email          TEXT,"
            "telephone      INTEGER,"
            "remarks        INTEGER,"
            "jur            INTEGER);";
    QSqlQuery query;
    if(!query.exec(cmd.arg(tableName())))
    {
        throwError(query.lastError());
    }
}


DbUsrTable *DbUsrTable::get()
{
    static DbUsrTable* sington = nullptr;
    if(sington == nullptr)
        sington = new DbUsrTable();
    return sington;
}

void DbUsrTable::insertItem(UsrItem &item)
{
    item.id = maxId()+1;
    QString cmd = "insert into %1 (id,dateTime,name,pwd,email,telephone,remarks,jur) "
                  "values(:id,:dateTime,:name,:pwd,:email,:telephone,:remarks,:jur)";
    modifyItem(item,cmd.arg(tableName()));
    emit itemChanged(item.id,Insert);
}


void DbUsrTable::updateItem(const UsrItem &item)
{
    QString cmd = "update %1 set "
                  "name        = :name,"
                  "pwd         = :pwd,"
                  "email       = :email,"
                  "telephone   = :telephone,"
                  "remarks     = :remarks,"
                  "jur         = :jur"
                  " where id   = :id";
    modifyItem(item,cmd.arg(tableName()));
    emit itemChanged(item.id,Update);
}

void DbUsrTable::modifyItem(const UsrItem &item, const QString &cmd)
{
    QSqlQuery query;
    query.prepare(cmd);
    query.bindValue(":id",item.id);
    query.bindValue(":dateTime",item.dateTime);
    query.bindValue(":name",item.name);
    query.bindValue(":pwd",item.pwd);
    query.bindValue(":email",item.email);
    query.bindValue(":telephone",item.telephone);
    query.bindValue(":remarks",item.remarks);
    query.bindValue(":jur",item.jur);
    if(!query.exec())
        throwError(query.lastError());
}

void DbUsrTable::selectItem(QSqlQuery &query,UsrItem &item)
{
    item.id = query.value("id").toInt();
    item.dateTime = query.value("dateTime").toString();
    item.name = query.value("name").toString();
    item.pwd = query.value("pwd").toString();
    item.email = query.value("email").toString();
    item.telephone = query.value("telephone").toString();
    item.remarks = query.value("remarks").toString();
    item.jur = query.value("jur").toInt();
}


QVector<UsrItem> DbUsrTable::selItemsByName(const QString& Name)
{
    return selectItems(QString("where name = %1").arg(Name));
}


void DbUsrTable::removeItemsByName(const QString& name)
{
    QVector<UsrItem> items = selItemsByName(name);
    foreach(const UsrItem& item,items)
        removeItem(item);
}
