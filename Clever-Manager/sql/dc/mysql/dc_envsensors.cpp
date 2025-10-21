/*
 * dc_envsensors.cpp
 *
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dc_envsensors.h"

DC_EnvSensors::DC_EnvSensors()
{

}

DC_EnvSensors *DC_EnvSensors::bulid()
{
    static DC_EnvSensors* sington = nullptr;
    if(sington == nullptr)
        sington = new DC_EnvSensors();
    return sington;
}

bool DC_EnvSensors::insertItem(const DC_EnvSensorItem& item)
{
    QString cmd = "insert into %1 (pdu_devices_id, type_index,sensor_num,sensor_value) "
                  "values(:pdu_devices_id, :type_index,:sensor_num,:sensor_value)";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Insert);
    return ret;
}

bool DC_EnvSensors::updateItem(const DC_EnvSensorItem& item)
{
    QString cmd = "update %1 set "
                  "pdu_devices_id        = :pdu_devices_id,"
                  "type_index            = :type_index,"
                  "sensor_num            = :sensor_num,"
                  "sensor_value          = :sensor_value,"
                  "update_time          = :update_time"
                  " where id            = :id";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Update);
    return ret;
}

bool DC_EnvSensors::modifyItem(const DC_EnvSensorItem &item, const QString &cmd)
{
    bool ret = false;
    if(mDb) {
        QSqlQuery query(*mDb);
        query.prepare(cmd);
        query.bindValue(":id",item.id);
        query.bindValue(":pdu_devices_id",item.pdu_devices_id);
        query.bindValue(":type_index",item.type_index);
        query.bindValue(":sensor_num",item.sensor_num);
        query.bindValue(":sensor_value",item.sensor_value);
        query.bindValue(":update_time", QTime::currentTime());
        ret = query.exec();
        if(!ret)
            throwError(query.lastError());
    }
    return ret;
}

void DC_EnvSensors::selectItem(QSqlQuery &query, DC_EnvSensorItem &item)
{
    item.id = query.value("id").toInt();
    item.pdu_devices_id = query.value("pdu_devices_id").toInt();
    item.type_index = query.value("type_index").toInt();
    item.sensor_num = query.value("sensor_num").toFloat();
    item.sensor_value = query.value("sensor_value").toFloat();
    item.create_time = query.value("create_time").toDateTime();
    item.update_time = query.value("update_time").toDateTime();
}


bool DC_EnvSensors::containRid(int rid, int type, int bit)
{
    QString condition = QString("where %1=%2 and type_index=%3 and sensor_num=%4 ")
            .arg(ridName()).arg(rid).arg(type).arg(bit);
    int ret = count(ridName(), condition);
    return (ret > 0) ? true:false;
}

int DC_EnvSensors::getMaxId(int rid, int type, int bit)
{
    int id = -1;
    if(containRid(rid, type, bit)) {
        QString condition = QString("where %1=%2 and type_index=%3 and sensor_num=%4 ")
                .arg(ridName()).arg(rid).arg(type).arg(bit);
        id = maxId("id", condition);
    }
    return id;
}



