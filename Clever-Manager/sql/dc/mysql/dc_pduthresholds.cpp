/*
 * dc_pduthresholds.cpp
 *
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dc_pduthresholds.h"

DC_PduThresholds::DC_PduThresholds()
{

}

DC_PduThresholds *DC_PduThresholds::bulid()
{
    static DC_PduThresholds* sington = nullptr;
    if(sington == nullptr)
        sington = new DC_PduThresholds();
    return sington;
}

bool DC_PduThresholds::insertItem(const DC_PduThresholdItem& item)
{
    QString cmd = "insert into %1 (pdu_devices_id, type_index,bit_index,min,max,alarm,cr_min,cr_max,cr_alarm) "
                  "values(:pdu_devices_id, :type_index,:bit_index,:min,:max,:alarm,:cr_min,:cr_max,:cr_alarm)";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Insert);
    return ret;
}

bool DC_PduThresholds::updateItem(const DC_PduThresholdItem& item)
{
    QString cmd = "update %1 set "
                  "pdu_devices_id        = :pdu_devices_id,"
                  "type_index            = :type_index,"
                  "bit_index            = :bit_index,"
                  "min                  = :min,"
                  "max                  = :max,"
                  "alarm                = :alarm,"
                  "cr_min               = :cr_min,"
                  "cr_max               = :cr_max,"
                  "cr_alarm             = :cr_alarm,"
                  "update_time          = :update_time"
                  " where id            = :id";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Update);
    return ret;
}

bool DC_PduThresholds::modifyItem(const DC_PduThresholdItem &item, const QString &cmd)
{
    bool ret = false;
    if(mDb) {
        QSqlQuery query(*mDb);
        query.prepare(cmd);
        query.bindValue(":id",item.id);
        query.bindValue(":pdu_devices_id",item.pdu_devices_id);
        query.bindValue(":type_index",item.type_index);
        query.bindValue(":bit_index",item.bit_index);

        query.bindValue(":min",item.min);
        query.bindValue(":max",item.max);
        query.bindValue(":alarm",item.alarm);
        query.bindValue(":cr_min",item.cr_min);
        query.bindValue(":cr_max",item.cr_max);
        query.bindValue(":cr_alarm",item.cr_alarm);
        query.bindValue(":update_time", QTime::currentTime());
        ret = query.exec();
        if(!ret)
            throwError(query.lastError());
    }
    return ret;
}

void DC_PduThresholds::selectItem(QSqlQuery &query, DC_PduThresholdItem &item)
{
    item.id = query.value("id").toInt();
    item.pdu_devices_id = query.value("pdu_devices_id").toInt();
    item.type_index = query.value("type_index").toInt();
    item.bit_index = query.value("bit_index").toInt();

    item.min = query.value("min").toFloat();
    item.max = query.value("max").toFloat();
    item.alarm = query.value("alarm").toInt();
    item.cr_min = query.value("cr_min").toFloat();
    item.cr_max = query.value("cr_max").toFloat();
    item.cr_alarm = query.value("cr_alarm").toInt();

    item.create_time = query.value("create_time").toDateTime();
    item.update_time = query.value("update_time").toDateTime();
}

int DC_PduThresholds::getId(DC_PduThresholdItem& item)
{
    int id = -1;
    QString cmd = QString("where pdu_devices_id=%1 and type_index=%2 and bit_index=%3")
            .arg(item.pdu_devices_id).arg(item.type_index).arg(item.bit_index);
    QVector<int> items = selectIds(cmd);
    if(items.size() > 0) {
        id = item.id = items.first();
    }
    return id;
}


bool DC_PduThresholds::containRid(int rid, int type, int bit)
{
    QString condition = QString("where %1=%2 and type_index=%3 and bit_index=%4 ")
            .arg(ridName()).arg(rid).arg(type).arg(bit);
    int ret = count(ridName(), condition);
    return (ret > 0) ? true:false;
}

int DC_PduThresholds::getMaxId(int rid, int type, int bit)
{
    int id = -1;
    if(containRid(rid, type, bit)) {
        QString condition = QString("where %1=%2 and type_index=%3 and bit_index=%4 ")
                .arg(ridName()).arg(rid).arg(type).arg(bit);
        id = maxId("id", condition);
    }
    return id;
}

