/*
 * dc_cabinetsmain.cpp
 *
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dc_cabinetsmain.h"

DC_CabinetsMain::DC_CabinetsMain()
{

}

DC_CabinetsMain *DC_CabinetsMain::bulid()
{
    static DC_CabinetsMain* sington = nullptr;
    if(sington == nullptr)
        sington = new DC_CabinetsMain();
    return sington;
}

bool DC_CabinetsMain::insertItem(const DC_CabinetPduItem& item)
{
    QString cmd = "insert into %1 (cabinets_id, dev_type,ip,dev_num) "
                  "values(:cabinets_id, :dev_type,:ip,:dev_num)";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Insert);
    return ret;
}

bool DC_CabinetsMain::updateItem(const DC_CabinetPduItem& item)
{
    QString cmd = "update %1 set "
                  "cabinets_id          = :cabinets_id,"
                  "dev_type             = :dev_type,"
                  "ip                   = :ip,"
                  "dev_num              = :dev_num,"
                  "update_time          = :update_time"
                  " where id            = :id";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Update);
    return ret;
}


bool DC_CabinetsMain::modifyItem(const DC_CabinetPduItem &item, const QString &cmd)
{
    bool ret = false;
    if(mDb) {
        QSqlQuery query(*mDb);
        query.prepare(cmd);
        query.bindValue(":id",item.id);
        query.bindValue(":cabinets_id",item.cabinets_id);
        query.bindValue(":dev_type",item.dev_type);
        query.bindValue(":ip",item.ip);
        query.bindValue(":dev_num",item.dev_num);
        query.bindValue(":update_time", QTime::currentTime());
        ret = query.exec();
        if(!ret)
            throwError(query.lastError());
    }
    return ret;
}

void DC_CabinetsMain::selectItem(QSqlQuery &query, DC_CabinetPduItem &item)
{
    item.id = query.value("id").toInt();
    item.cabinets_id = query.value("cabinets_id").toInt();
    item.dev_type = query.value("dev_type").toInt();
    item.ip = query.value("ip").toString();
    item.dev_num = query.value("dev_num").toInt();
    item.create_time = query.value("create_time").toDateTime();
    item.update_time = query.value("update_time").toDateTime();
}

