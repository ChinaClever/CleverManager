/*
 * dc_pduelecparams.cpp
 *
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dc_pduelecparams.h"

DC_PduElecParams::DC_PduElecParams()
{

}


DC_PduElecParams *DC_PduElecParams::bulid()
{
    static DC_PduElecParams* sington = nullptr;
    if(sington == nullptr)
        sington = new DC_PduElecParams();
    return sington;
}

bool DC_PduElecParams::insertItem(const DC_PduElecParamsItem& item)
{
    QString cmd = "insert into %1 (pdu_devices_id, type_index,bit_index,vol,cur,pow,ele,pf,sw) "
                  "values(:pdu_devices_id, :type_index,:bit_index,:vol,:cur,:pow,:ele,:pf,:sw)";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Insert);
    return ret;
}

bool DC_PduElecParams::updateItem(const DC_PduElecParamsItem& item)
{
    QString cmd = "update %1 set "
                  "pdu_devices_id        = :pdu_devices_id,"
                  "type_index            = :type_index,"
                  "bit_index             = :bit_index,"

                  "vol            = :vol,"
                  "cur            = :cur,"
                  "pow            = :pow,"
                  "ele            = :ele,"
                  "pf             = :pf,"
                  "sw             = :sw,"

                  "update_time          = :update_time"
                  " where id            = :id";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Update);
    return ret;
}

bool DC_PduElecParams::modifyItem(const DC_PduElecParamsItem &item, const QString &cmd)
{
    bool ret = false;
    if(mDb) {
        QSqlQuery query(*mDb);
        query.prepare(cmd);
        query.bindValue(":id",item.id);
        query.bindValue(":pdu_devices_id",item.pdu_devices_id);
        query.bindValue(":type_index",item.type_index);
        query.bindValue(":bit_index",item.bit_index);

        query.bindValue(":vol",item.vol);
        query.bindValue(":cur",item.cur);
        query.bindValue(":pow",item.pow);
        query.bindValue(":ele",item.ele);
        query.bindValue(":pf",item.pf);
        query.bindValue(":sw",item.sw);

        query.bindValue(":update_time", QTime::currentTime());
        ret = query.exec();
        if(!ret)
            throwError(query.lastError());
    }
    return ret;
}

void DC_PduElecParams::selectItem(QSqlQuery &query, DC_PduElecParamsItem &item)
{
    item.id = query.value("id").toInt();
    item.pdu_devices_id = query.value("pdu_devices_id").toInt();
    item.type_index = query.value("type_index").toInt();
    item.bit_index = query.value("bit_index").toInt();

    item.vol = query.value("vol").toFloat();
    item.cur = query.value("cur").toFloat();
    item.pow = query.value("pow").toFloat();
    item.ele = query.value("ele").toFloat();
    item.pf = query.value("pf").toFloat();
    item.sw = query.value("sw").toInt();

    item.create_time = query.value("create_time").toDateTime();
    item.update_time = query.value("update_time").toDateTime();
}

bool DC_PduElecParams::containRid(int rid, int type, int bit)
{
    QString condition = QString("where %1=%2 and type_index=%3 and bit_index=%4 ")
            .arg(ridName()).arg(rid).arg(type).arg(bit);
    int ret = count(ridName(), condition);
    return (ret > 0) ? true:false;
}

int DC_PduElecParams::getMaxId(int rid, int type, int bit)
{
    int id = -1;
    if(containRid(rid, type, bit)) {
        QString condition = QString("where %1=%2 and type_index=%3 and bit_index=%4 ")
                .arg(ridName()).arg(rid).arg(type).arg(bit);
        id = maxId("id", condition);
    }
    return id;
}


