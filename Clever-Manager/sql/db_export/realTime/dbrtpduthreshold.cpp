/*
 * dbrtpduthreshold.cpp
 * 实时PDU设备阈值表
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "dbrtpduthreshold.h"

DbRtPduThreshold::DbRtPduThreshold()
{
    QString cmd =
            "create table if not exists %1("
            "id                INTEGER primary key autoincrement not null,"
            "rid               INTEGER,"
            "type              INTEGER,"
            "inde              INTEGER,"
            "min               DOUBLE,"
            "max               DOUBLE,"
            "alarm             INTEGER,"
            "crMin             DOUBLE,"
            "crMax             DOUBLE,"
            "crAlarm           INTEGER);";
    QSqlQuery query;
    if(!query.exec(cmd.arg(tableName())))
    {
        throwError(query.lastError());
    }
}


DbRtPduThreshold *DbRtPduThreshold::get()
{
    static DbRtPduThreshold* sington = nullptr;
    if(sington == nullptr)
        sington = new DbRtPduThreshold();
    return sington;
}

void DbRtPduThreshold::insertItem(PduThresholdItem &item)
{
    item.id = maxId()+1;
    QString cmd = "insert into %1 (id,rid,type,inde,min,max,alarm,crMin,crMax,crAlarm) "
                  "values(:id,:rid,:type,:inde,:min,:max,:alarm,:crMin,:crMax,:crAlarm)";
    modifyItem(item,cmd.arg(tableName()));
//    emit itemChanged(item.id,Insert);
}


void DbRtPduThreshold::updateItem(PduThresholdItem &item)
{
    QString cmd = "update %1 set "
                  "rid         = :rid,"
                  "type        = :type,"
                  "inde       = :inde,"
                  "min         = :min,"
                  "max         = :max,"
                  "alarm       = :alarm,"
                  "crMin       = :crMin,"
                  "crMax       = :crMax,"
                  "crAlarm     = :crAlarm"
                  " where id = :id";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(!ret) {
        insertItem(item);
    } else {
//        emit itemChanged(item.id,Update);
    }
}



bool DbRtPduThreshold::modifyItem(const PduThresholdItem &item, const QString &cmd)
{
    bool ret = true;
    QSqlQuery query;
    query.prepare(cmd);
    query.bindValue(":id",item.id);
    query.bindValue(":rid",item.rid);
    query.bindValue(":type",item.type);
    query.bindValue(":inde",item.index);
    query.bindValue(":min",item.min);
    query.bindValue(":max",item.max);
    query.bindValue(":alarm",item.alarm);
    query.bindValue(":crMin",item.crMin);
    query.bindValue(":crMax",item.crMax);
    query.bindValue(":crAlarm",item.crAlarm);
    if(!query.exec()) {
        throwError(query.lastError());
        ret = false;
    }

    return ret;
}


void DbRtPduThreshold::selectItem(QSqlQuery &query, PduThresholdItem &item)
{
    item.id = query.value("id").toInt();
    item.rid = query.value("rid").toInt();
    item.type = query.value("type").toInt();
    item.index = query.value("inde").toInt();
    item.min = query.value("min").toDouble();
    item.max = query.value("max").toDouble();
    item.alarm = query.value("alarm").toInt();
    item.crMin = query.value("crMin").toDouble();
    item.crMax = query.value("crMax").toDouble();
    item.crAlarm = query.value("crAlarm").toInt();
}

/**
 * @brief 获取ID号
 * @return
 */
int DbRtPduThreshold::getId(int rid, int type, int index)
{
    int id = -1;
    QVector<PduThresholdItem> items = selectItems(QString("where rid = %1 and type = %2 and inde = %3").arg(rid).arg(type).arg(index));
    if(items.size() == 1) {
        id = items.first().id;
    }
    return id;
}



QVector<PduThresholdItem> DbRtPduThreshold::selItemsByRid(int rid)
{
    return selectItems(QString("where rid = %1").arg(rid));
}

void DbRtPduThreshold::removeItemsByRid(int rid)
{
    QVector<PduThresholdItem> items = selItemsByRid(rid);
    foreach(const PduThresholdItem& item,items)
        removeItem(item);
}
