/*
 * dbpduenvsensor.cpp
 * 环境状态表
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "dbpduenvsensor.h"

DbPduEnvSensor::DbPduEnvSensor()
{
    createTable();
}

void DbPduEnvSensor::createTable()
{
    QString cmd =
            "create table if not exists %1("
            "id                INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
            "rid               INTEGER,"
            "type              INTEGER,"
            "sensor1           DOUBLE,"
            "sensor2           DOUBLE,"
            "sensor3           DOUBLE,"
            "sensor4           DOUBLE);";
    QSqlQuery query;
    if(!query.exec(cmd.arg(tableName())))
    {
        throwError(query.lastError());
    }
}

DbPduEnvSensor *DbPduEnvSensor::get()
{
    static DbPduEnvSensor* sington = nullptr;
    if(sington == nullptr)
        sington = new DbPduEnvSensor();
    return sington;
}

void DbPduEnvSensor::insertItem(PduEnvSensorItem &item)
{
   // item.id = maxId()+1;
    QString cmd = "insert into %1 (id,rid,type,sensor1,sensor2,sensor3,sensor4) "
                  "values(:id,:rid,:type,:sensor1,:sensor2,:sensor3,:sensor4)";
    modifyItem(item,cmd.arg(tableName()));
//    emit itemChanged(item.id,Insert);
}


bool DbPduEnvSensor::modifyItem(const PduEnvSensorItem &item, const QString &cmd)
{
    bool ret = true;
    QSqlQuery query;
    query.prepare(cmd);
   // query.bindValue(":id",item.id);
    query.bindValue(":rid",item.rid);
    query.bindValue(":type",item.type);
    query.bindValue(":sensor1",item.sensor1);
    query.bindValue(":sensor2",item.sensor2);
    query.bindValue(":sensor3",item.sensor3);
    query.bindValue(":sensor4",item.sensor4);
    if(!query.exec()) {
        throwError(query.lastError());
        ret = false;
    }

    return ret;
}

void DbPduEnvSensor::selectItem(QSqlQuery &query,PduEnvSensorItem &item)
{
    item.id = query.value("id").toInt();
    item.rid = query.value("rid").toInt();
    item.type = query.value("type").toInt();
    item.sensor1 = query.value("sensor1").toDouble();
    item.sensor2 = query.value("sensor2").toDouble();
    item.sensor3 = query.value("sensor3").toDouble();
    item.sensor4 = query.value("sensor4").toDouble();
}

QVector<PduEnvSensorItem> DbPduEnvSensor::selItemsByRid(int rid)
{
    return selectItems(QString("where rid = %1").arg(rid));
}

void DbPduEnvSensor::removeItemsByRid(int rid)
{
    QVector<PduEnvSensorItem> items = selItemsByRid(rid);
    foreach(const PduEnvSensorItem& item,items)
        removeItem(item);
}

