/*
 * dbpduelecparams.cpp
 * 电气参数表
 *  (类型 0:统计数据 1:输入相  2：输出位)
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "dbpduelecparams.h"
#include "common.h"
#include "pdu/dev/devType/pdudtname.h"
extern int gDbDevType;

DbPduElecParams::DbPduElecParams()
{
    createTable();
}

void DbPduElecParams::createTable()
{
    QString cmd =
            "create table if not exists %1("
            "id                INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
            "rid               INTEGER,"
            "type              INTEGER,"
            "typeStr           TEXT,"
            "inde              INTEGER,"
            "vol               DOUBLE,"
            "cur               DOUBLE,"
            "pow               DOUBLE,"
            "ele               DOUBLE,"
            "pf                DOUBLE,"
            "sw                INTEGER,"
            "swStr             TEXT);";
    QSqlQuery query;
    if(!query.exec(cmd.arg(tableName())))
    {
        throwError(query.lastError());
    }
}

DbPduElecParams *DbPduElecParams::get()
{
    static DbPduElecParams* sington = nullptr;
    if(sington == nullptr)
        sington = new DbPduElecParams();
    return sington;
}


bool DbPduElecParams::insertItem(PduElecParamsItem &item)
{
   // item.id = maxId()+1;
    QString cmd = "insert into %1 (id,rid,type,typeStr,inde,vol,cur,pow,ele,pf,sw,swStr) "
                  "values(:id,:rid,:type,:typeStr,:inde,:vol,:cur,:pow,:ele,:pf,:sw,:swStr)";

    return modifyItem(item,cmd.arg(tableName()));
}

QString DbPduElecParams::getTypeStr(int type)
{
    QString str =tr("总参数");
    if(type == 1)
        str = "Line";
    if(type == 2)
        str = tr("回路");
    if(type == 3)
        str = tr("输出位");
    return str;
}

QString DbPduElecParams::getSwStr(int sw)
{
    QString str =tr("开");
    if(sw == 1)
        str = tr("关");
    return str;
}

bool DbPduElecParams::modifyItem(PduElecParamsItem &item, const QString &cmd)
{
    bool ret = true;
    QSqlQuery query;
    query.prepare(cmd);
    //query.bindValue(":id",item.id);
    query.bindValue(":rid",item.rid);
    query.bindValue(":type",item.type);
    item.typeStr = getTypeStr(item.type);
    query.bindValue(":typeStr",item.typeStr);

    query.bindValue(":inde",item.index);    
    query.bindValue(":vol",item.vol);
    query.bindValue(":cur",item.cur);
    query.bindValue(":pow",item.pow);
    query.bindValue(":ele",item.ele);

     if(gDbDevType != PDU_TYPE_IP_PDU) {
         if(gDbDevType == PDU_TYPE_NPM_PDU)
         {
             if(item.type != 3)
                 query.bindValue(":pf",item.pf);
         } else
            query.bindValue(":pf",item.pf);
     }
    query.bindValue(":sw",item.sw);

    if(item.type>0) {
        if(gDbDevType != PDU_TYPE_IP_PDU)
        item.swStr = getSwStr(item.sw);
    }
    query.bindValue(":swStr",item.swStr);
    if(!query.exec()) {
        throwError(query.lastError());
        ret = false;
    }   
    return ret;
}


void DbPduElecParams::selectItem(QSqlQuery &query,PduElecParamsItem &item)
{
    item.id = query.value("id").toInt();
    item.rid = query.value("rid").toInt();
    item.type = query.value("type").toInt();
    item.typeStr = query.value("typeStr").toString();
    item.index = query.value("inde").toInt();
    item.vol = query.value("vol").toDouble();
    item.cur = query.value("cur").toDouble();
    item.pow = query.value("pow").toDouble();
    item.ele = query.value("ele").toDouble();
    item.pf = query.value("pf").toDouble();
    item.sw = query.value("sw").toInt();
    item.swStr = query.value("swStr").toString();
}


QVector<PduElecParamsItem> DbPduElecParams::selItemsByRid(int rid)
{
    return selectItems(QString("where rid = %1").arg(rid));
}

void DbPduElecParams::removeItemsByRid(int rid)
{
    QVector<PduElecParamsItem> items = selItemsByRid(rid);
    foreach(const PduElecParamsItem& item,items)
        removeItem(item);
}

QVector<PduElecParamsItem> DbPduElecParams::query(const QString& condition)
{
    return selectItems(condition);
}
