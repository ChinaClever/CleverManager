/*
 * dbrtpduelecparams.cpp
 * 实时PDU设备索引表
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "dbrtpdudevindex.h"

DbRtPduDevIndex::DbRtPduDevIndex()
{
    createTable();
}

DbRtPduDevIndex *DbRtPduDevIndex::get()
{
    static DbRtPduDevIndex* sington = nullptr;
    if(sington == nullptr)
        sington = new DbRtPduDevIndex();
    return sington;
}

/**
 * @brief 获取ID号
 * @return
 */
int DbRtPduDevIndex::getId(QString &ip, int devNum, int devType)
{
    int id = -1;
    QVector<PduDevIndexItem> items = selectItems(QString("where ip = \"%1\" and devNum = %2 and devType = %3").arg(ip).arg(devNum).arg(devType));
    if(items.size() >= 1) {
        id = items.first().id;
    }
    return id;
}


void DbRtPduDevIndex::initTime(PduDevIndexItem &item)
{
    QDateTime dateTime = QDateTime::currentDateTime();
    item.date = dateTime.toString("yyyy-MM-dd");
    item.time = dateTime.toString("hh:mm:ss");
}

bool DbRtPduDevIndex::updateItem(PduDevIndexItem &item)
{
    initTime(item);
    QString cmd = "update %1 set "
                  "date         = :date,"
                  "time         = :time,"
                  "ip           = :ip,"
                  "devNum       = :devNum,"
                  "devType      = :devType,"
                  "typeStr      = :typeStr,"
                  "roomName     = :roomName,"
                  "cabinetName  = :cabinetName,"
                  "status       = :status"
                  "statusStr    = :statusStr"
                  " where id    = :id";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(!ret) {
        ret = insertItem(item);
    } else {
//        emit itemChanged(item.id,Update);
    }
    return ret;
}
