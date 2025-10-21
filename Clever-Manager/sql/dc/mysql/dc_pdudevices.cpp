/*
 * dc_pdudevices.cpp
 *  PDU设备索引表
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dc_pdudevices.h"

DC_PduDevices::DC_PduDevices()
{

}


DC_PduDevices *DC_PduDevices::bulid()
{
    static DC_PduDevices* sington = nullptr;
    if(sington == nullptr)
        sington = new DC_PduDevices();
    return sington;
}

bool DC_PduDevices::insertItem(const DC_PduDevItem& item)
{
    QString cmd = "insert into %1 (cabinets_id, type_index,name,ip, num) "
                  "values(:cabinets_id, :type_index,:name,:ip,:num)";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Insert);
    return ret;
}

bool DC_PduDevices::updateItem(const DC_PduDevItem& item)
{
    QString cmd = "update %1 set "
                  "cabinets_id           = :cabinets_id,"
                  "type_index            = :type_index,"
                  "name                  = :name,"
                  "ip                    = :ip,"
                  "num                   = :num,"
                  "update_time          = :update_time"
                  " where id            = :id";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Update);
    return ret;
}


bool DC_PduDevices::modifyItem(const DC_PduDevItem &item, const QString &cmd)
{
    bool ret = false;
    if(mDb) {
        QSqlQuery query(*mDb);
        query.prepare(cmd);
        query.bindValue(":id",item.id);
        query.bindValue(":cabinets_id",item.cabinets_id);
        query.bindValue(":type_index",item.type_index);
        query.bindValue(":name",item.name);
        query.bindValue(":ip",item.ip);
        query.bindValue(":num",item.num);
        query.bindValue(":update_time", QTime::currentTime());
        ret = query.exec();
        if(!ret)
            throwError(query.lastError());
    }
    return ret;
}

void DC_PduDevices::selectItem(QSqlQuery &query, DC_PduDevItem &item)
{
    item.id = query.value("id").toInt();
    item.cabinets_id = query.value("cabinets_id").toInt();
    item.type_index = query.value("type_index").toInt();

    item.name = query.value("name").toString();
    item.ip = query.value("ip").toString();
    item.num = query.value("num").toInt();

    item.create_time = query.value("create_time").toDateTime();
    item.update_time = query.value("update_time").toDateTime();
}

/**
 * @brief 机柜中存在几个设备
 * @param cabinets_id
 * @return
 */
int DC_PduDevices::countDev(int cabinets_id)
{
    int ret = count("id", QString("where cabinets_id = %1 ").arg(cabinets_id));
    return ret;
}

/**
 * @brief 设备是否已经存在
 * @param item
 * @return
 */
bool DC_PduDevices::containDev(const DC_PduDevItem& item)
{
    int ret = count("id",QString("where cabinets_id=%1 and type_index=%2 and ip=\"%3\" and num=%4")
                    .arg(item.cabinets_id).arg(item.type_index).arg(item.ip).arg(item.num));
    return (ret > 0) ? true:false;
}

int DC_PduDevices::getId(DC_PduDevItem& item)
{
    int id = -1;
    QString cmd = QString("where cabinets_id=%1 and type_index=%2 and ip=\"%3\" and num=%4")
            .arg(item.cabinets_id).arg(item.type_index).arg(item.ip).arg(item.num);
    QVector<int> items = selectIds(cmd);
    if(items.size() > 0) {
        id = item.id = items.first();
    }
    return id;
}

/**
 * @brief 根据机柜ID查找
 * @param cabinets_id
 * @return
 */
QVector<DC_PduDevItem> DC_PduDevices::selItemsByCabinetId(int cabinets_id)
{
    QString cmd = QString("where cabinets_id = %1 ").arg(cabinets_id);
    return selectItems(cmd);
}

QVector<DC_PduDevItem> DC_PduDevices::selItemsByIp(int cabinets_id, const QString& ip)
{
    QString cmd = QString("where cabinets_id = %1 and ip=\"%2\"").arg(cabinets_id).arg(ip);
    return selectItems(cmd);
}



