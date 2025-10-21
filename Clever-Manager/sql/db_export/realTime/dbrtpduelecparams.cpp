/*
 * dbrtpduelecparams.cpp
 * 实时电气参数表
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "dbrtpduelecparams.h"

DbRtPduElecParams::DbRtPduElecParams()
{
    createTable();
}


DbRtPduElecParams *DbRtPduElecParams::get()
{
    static DbRtPduElecParams* sington = nullptr;
    if(sington == nullptr)
        sington = new DbRtPduElecParams();
    return sington;
}


int DbRtPduElecParams::getId(int rid, int type, int index)
{
    int id = -1;
    QVector<PduElecParamsItem> items = selectItems(QString("where rid = %1 and type = %2 and inde = %3").arg(rid).arg(type).arg(index));
    if(items.size() == 1) {
        id = items.first().id;
    }
    return id;
}

bool DbRtPduElecParams::updateItem(PduElecParamsItem &item)
{
    QString cmd = "update %1 set "
                  "rid         = :rid,"
                  "type        = :type,"
                  "inde       = :inde,"
                  "vol         = :vol,"
                  "cur         = :cur,"
                  "pow         = :pow,"
                  "ele         = :ele,"
                  "pf          = :pf,"
                  "sw          = :sw"
                  " where id = :id";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(!ret) {
        insertItem(item);
    } else {
//        emit itemChanged(item.id,Update);
    }

    return ret;
}
