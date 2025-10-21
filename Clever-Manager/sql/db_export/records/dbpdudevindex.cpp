/*
 * dbpdudevindex.cpp
 * PDU设备索引表
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "dbpdudevindex.h"
#include "pdu/dev/devType/pdudtname.h"
#include "common.h"

DbPduDevIndex::DbPduDevIndex()
{
    mKeyId = -1;
    createTable();
}

void DbPduDevIndex::createTable()
{
    QString cmd =
            "create table if not exists %1("
            "id                INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
            "date              TEXT,"
            "time              TEXT,"
            "ip                TEXT not null,"
            "devNum            INTEGER,"
            "devType           INTEGER,"
            "typeStr           TEXT,"
            "roomName          TEXT,"
            "cabinetName       TEXT,"
            "status            INTEGER,"
            "statusStr         TEXT);";
    QSqlQuery query;
    if(!query.exec(cmd.arg(tableName())))
    {
        throwError(query.lastError());
    }
}

DbPduDevIndex *DbPduDevIndex::get()
{
    static DbPduDevIndex* sington = nullptr;
    if(sington == nullptr)
        sington = new DbPduDevIndex();
    return sington;
}

void DbPduDevIndex::initItem(PduDevIndexItem &item)
{
    if(mKeyId<0)  mKeyId = maxId()+1;
    item.id = mKeyId++;

    QDateTime dateTime = QDateTime::currentDateTime();
    item.date = dateTime.toString("yyyy-MM-dd");
    item.time = dateTime.toString("hh:mm:ss");
}

bool DbPduDevIndex::insertItem(PduDevIndexItem &item)
{
    initItem(item);
    QString cmd = "insert into %1 (id,date,time,ip,devNum,devType,typeStr,roomName,cabinetName,status,statusStr) "
                  "values(:id,:date,:time,:ip,:devNum,:devType,:typeStr,:roomName,:cabinetName,:status,:statusStr)";
    return modifyItem(item,cmd.arg(tableName()));
}

QString DbPduDevIndex::getTypeStr(int type)
{
    QString str;
    PduDTName dt;
    dt.getName(type, str);
    return str;
}

QString DbPduDevIndex::getstatusStr(int status)
{
    QString str = tr("正常");
    if(status > 0)
        str = tr("报警");
    return str;
}

bool DbPduDevIndex::modifyItem(PduDevIndexItem &item, const QString &cmd)
{
    bool ret = true;
    QSqlQuery query;
    query.prepare(cmd);
    query.bindValue(":id",item.id);
    query.bindValue(":date",item.date);
    query.bindValue(":time",item.time);
    query.bindValue(":ip",item.ip);
    query.bindValue(":devNum",item.devNum);
    query.bindValue(":devType",item.devType);

    item.typeStr = getTypeStr(item.devType);
    query.bindValue(":typeStr",item.typeStr);
    query.bindValue(":roomName",item.roomName);
    query.bindValue(":cabinetName",item.cabinetName);
    query.bindValue(":status",item.status);

    item.statusStr = getstatusStr(item.status);
    query.bindValue(":statusStr",item.statusStr);
    if(!query.exec()) {
        throwError(query.lastError());
        ret = false;
    }

    return ret;
}

void DbPduDevIndex::selectItem(QSqlQuery &query,PduDevIndexItem &item)
{
    item.id = query.value("id").toInt();
    item.date = query.value("date").toString();
    item.time = query.value("time").toString();
    item.ip = query.value("ip").toString();
    item.devNum = query.value("devNum").toInt();
    item.devType = query.value("devType").toInt();
    item.typeStr = query.value("typeStr").toString();
    item.roomName = query.value("roomName").toString();
    item.cabinetName = query.value("cabinetName").toString();
    item.status = query.value("status").toInt();
    item.statusStr = query.value("statusStr").toString();
}

QVector<PduDevIndexItem> DbPduDevIndex::selItemsByIp(const QString& ip)
{
    return selectItems(QString("where ip = \"%1\"").arg(ip));
}

void DbPduDevIndex::removeItemsByIp(const QString& ip)
{
    QVector<PduDevIndexItem> items = selItemsByIp(ip);
    foreach(const PduDevIndexItem& item,items)
        removeItem(item);
}

QVector<PduDevIndexItem> DbPduDevIndex::query(const QString& condition)
{
    return selectItems(condition);
}
