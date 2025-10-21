/*
 * dc_serverdevices.cpp
 *
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dc_serverdevices.h"

DC_ServerDevices::DC_ServerDevices()
{

}

DC_ServerDevices *DC_ServerDevices::bulid()
{
    static DC_ServerDevices* sington = nullptr;
    if(sington == nullptr)
        sington = new DC_ServerDevices();
    return sington;
}

bool DC_ServerDevices::insertItem(const DC_SerDevItem& item)
{
    QString cmd = "insert into %1 (cabinets_id,name,main_pdu_port,spare_pdu_port,address,height) "
                  "values(:cabinets_id,:name,:main_pdu_port,:spare_pdu_port,:address,:height)";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Insert);
    return ret;
}

bool DC_ServerDevices::updateItem(const DC_SerDevItem& item)
{
    QString cmd = "update %1 set "
                  "cabinets_id          = :cabinets_id,"
                  "name                 = :name,"
                  "main_pdu_port        = :main_pdu_port,"
                  "spare_pdu_port       = :spare_pdu_port,"
                  "address              = :address,"
                  "height               = :height,"
                  "update_time          = :update_time"
                  " where id            = :id";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Update);
    return ret;
}

bool DC_ServerDevices::modifyItem(const DC_SerDevItem &item, const QString &cmd)
{
    bool ret = false;
    if(mDb) {
        QSqlQuery query(*mDb);
        query.prepare(cmd);
        query.bindValue(":id",item.id);
        query.bindValue(":cabinets_id",item.cabinets_id);
        query.bindValue(":name",item.name);
        query.bindValue(":main_pdu_port",item.main_pdu_port);
        query.bindValue(":spare_pdu_port",item.spare_pdu_port);
        query.bindValue(":address",item.address);
        query.bindValue(":height",item.height);
        query.bindValue(":update_time", QTime::currentTime());
        ret = query.exec();
        if(!ret)
            throwError(query.lastError());
    }
    return ret;
}

void DC_ServerDevices::selectItem(QSqlQuery &query, DC_SerDevItem &item)
{
    item.id = query.value("id").toInt();
    item.cabinets_id = query.value("cabinets_id").toInt();
    item.name = query.value("name").toString();

    item.main_pdu_port = query.value("main_pdu_port").toInt();
    item.spare_pdu_port = query.value("spare_pdu_port").toInt();
    item.address = query.value("address").toInt();
    item.height = query.value("height").toInt();

    item.create_time = query.value("create_time").toDateTime();
    item.update_time = query.value("update_time").toDateTime();
}



