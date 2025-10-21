/*
 * dc_pdutg.cpp
 *
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dc_pdutg.h"

DC_PduTg::DC_PduTg()
{

}


DC_PduTg *DC_PduTg::bulid()
{
    static DC_PduTg* sington = nullptr;
    if(sington == nullptr)
        sington = new DC_PduTg();
    return sington;
}

bool DC_PduTg::insertItem(const DC_PduTgItem& item)
{
    QString cmd = "insert into %1 (pdu_devices_id, vol,cur,pow,ele,pf) "
                  "values(:pdu_devices_id, :vol,:cur,:pow,:ele,:pf)";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Insert);
    return ret;
}

bool DC_PduTg::updateItem(const DC_PduTgItem& item)
{
    QString cmd = "update %1 set "
                  "pdu_devices_id        = :pdu_devices_id,"
                  "vol            = :vol,"
                  "cur            = :cur,"
                  "pow            = :pow,"
                  "ele            = :ele,"
                  "pf             = :pf,"
                  "update_time          = :update_time"
                  " where id            = :id";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Update);
    return ret;
}

bool DC_PduTg::modifyItem(const DC_PduTgItem &item, const QString &cmd)
{
    bool ret = false;
    if(mDb) {
        QSqlQuery query(*mDb);
        query.prepare(cmd);
        query.bindValue(":id",item.id);
        query.bindValue(":pdu_devices_id",item.pdu_devices_id);
        query.bindValue(":vol",item.vol);
        query.bindValue(":cur",item.cur);
        query.bindValue(":pow",item.pow);
        query.bindValue(":ele",item.ele);
        query.bindValue(":pf",item.pf);
        query.bindValue(":update_time", QTime::currentTime());
        ret = query.exec();
        if(!ret)
            throwError(query.lastError());
    }
    return ret;
}

void DC_PduTg::selectItem(QSqlQuery &query, DC_PduTgItem &item)
{
    item.id = query.value("id").toInt();
    item.pdu_devices_id = query.value("pdu_devices_id").toInt();
    item.vol = query.value("vol").toFloat();
    item.cur = query.value("cur").toFloat();
    item.pow = query.value("pow").toFloat();
    item.ele = query.value("ele").toFloat();
    item.pf = query.value("pf").toFloat();
    item.create_time = query.value("create_time").toDateTime();
    item.update_time = query.value("update_time").toDateTime();
}


bool DC_PduTg::containRid(int rid)
{
    int ret = count(ridName(), QString("where %1 = %2").arg(ridName()).arg(rid));
    return (ret > 0) ? true:false;
}

int DC_PduTg::getMaxId(int rid)
{
    int id = -1;
    if(containRid(rid)) {
        QString condition = QString("where %1 = %2").arg(ridName()).arg(rid);
        id = maxId("id", condition);
    }
    return id;
}




