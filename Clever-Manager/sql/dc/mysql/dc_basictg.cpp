/*
 * dc_basictg.cpp
 *
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dc_basictg.h"

DC_BasicTg::DC_BasicTg()
{

}


bool DC_BasicTg::insertItem(const DC_TgItem& item)
{
    QString cmd = "insert into %1 (%2, cur,vol,pow,ele,pf,tem,hum,all_num,line_num,alarm_num,offLine_num) "
                  "values(:rid, :cur,:vol,:pow,:ele,:pf,:tem,:hum,:all_num,:line_num,:alarm_num,:offLine_num)";
    bool ret = modifyItem(item,cmd.arg(tableName()).arg(ridName()));
    if(ret)
        emit itemChanged(item.id,Insert);
    return ret;
}

bool DC_BasicTg::updateItem(const DC_TgItem& item)
{
    QString cmd = "update %1 set "
                  "%2               = :rid,"
                  "cur              = :cur,"
                  "vol              = :vol,"
                  "pow              = :pow,"
                  "ele              = :ele,"
                  "pf               = :pf,"
                  "tem              = :tem,"
                  "hum              = :hum,"

                  "all_num          = :all_num,"
                  "line_num         = :line_num,"
                  "alarm_num        = :alarm_num,"
                  "offLine_num      = :offLine_num,"

                  "update_time          = :update_time"
                  " where id            = :id";
    bool ret = modifyItem(item,cmd.arg(tableName()).arg(ridName()));
    if(ret)
        emit itemChanged(item.id,Update);
    return ret;
}

bool DC_BasicTg::modifyItem(const DC_TgItem &item, const QString &cmd)
{
    bool ret = false;
    if(mDb) {
        QSqlQuery query(*mDb);
        query.prepare(cmd);
        query.bindValue(":id",item.id);
        query.bindValue(":rid",item.rid);
        query.bindValue(":cur",item.cur);
        query.bindValue(":vol",item.vol);
        query.bindValue(":pow",item.pow);
        query.bindValue(":ele",item.ele);
        query.bindValue(":pf",item.pf);
        query.bindValue(":tem",item.tem);
        query.bindValue(":hum",item.hum);

        query.bindValue(":all_num",item.all_num);
        query.bindValue(":line_num",item.line_num);
        query.bindValue(":alarm_num",item.alarm_num);
        query.bindValue(":offLine_num",item.offLine_num);

        query.bindValue(":update_time", QTime::currentTime());
        ret = query.exec();
        if(!ret)
            throwError(query.lastError());
    }
    return ret;
}

void DC_BasicTg::selectItem(QSqlQuery &query, DC_TgItem &item)
{
    item.id = query.value("id").toInt();
    item.rid = query.value(ridName()).toInt();

    item.cur = query.value("cur").toFloat();
    item.vol = query.value("vol").toFloat();
    item.pow = query.value("pow").toDouble();
    item.ele = query.value("ele").toDouble();
    item.pf = query.value("pf").toFloat();
    item.tem = query.value("tem").toFloat();
    item.hum = query.value("hum").toFloat();

    item.all_num = query.value("all_num").toInt();
    item.line_num = query.value("line_num").toInt();
    item.alarm_num = query.value("alarm_num").toInt();

    item.create_time = query.value("create_time").toDateTime();
    item.update_time = query.value("update_time").toDateTime();
}

bool DC_BasicTg::containRid(int rid)
{
    int ret = count(ridName(), QString("where %1 = %2").arg(ridName()).arg(rid));
    return (ret > 0) ? true:false;
}

int DC_BasicTg::getMaxId(int rid)
{
    int id = -1;
    if(containRid(rid)) {
        QString condition = QString("where %1 = %2").arg(ridName()).arg(rid);
        id = maxId("id", condition);
    }
    return id;
}
