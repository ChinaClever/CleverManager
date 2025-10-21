/*
 * dbrtpduenvsensor.cpp
 * 实时环境状态表
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "dbrtpduenvsensor.h"

DbRtPduEnvSensor::DbRtPduEnvSensor()
{
    createTable();
}

DbRtPduEnvSensor *DbRtPduEnvSensor::get()
{
    static DbRtPduEnvSensor* sington = nullptr;
    if(sington == nullptr)
        sington = new DbRtPduEnvSensor();
    return sington;
}


int DbRtPduEnvSensor::getId(int rid, int type)
{
    int id = -1;
    QVector<PduEnvSensorItem> items = selectItems(QString("where rid = %1 and type = %2").arg(rid).arg(type));
    if(items.size() == 1) {
        id = items.first().id;
    }
    return id;
}


void DbRtPduEnvSensor::updateItem(PduEnvSensorItem &item)
{
    QString cmd = "update %1 set "
                  "rid         = :rid,"
                  "type        = :type,"
                  "sensor1     = :sensor1,"
                  "sensor2     = :sensor2,"
                  "sensor3     = :sensor3,"
                  "sensor4     = :sensor4"
                  " where id = :id";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(!ret)
        insertItem(item);
//    else
//        emit itemChanged(item.id,Update);
}
