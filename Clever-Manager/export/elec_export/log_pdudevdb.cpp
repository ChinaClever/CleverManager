/*
 * log_pdudevdb.cpp
 * 设备电能查询
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "log_pdudevdb.h"
#include "ex_common/log_common.h"

LOG_PduDevDb::LOG_PduDevDb(QObject *parent) : QObject(parent)
{
    mDevIndex =  DbPduDevIndex::get();
    mDevParams = DbPduElecParams::get();
}


void LOG_PduDevDb::setDev(const QString &ip, int num)
{
    mSqlCmd = QString("ip = \'%1\' and devNum = \'%2\' ").arg(ip).arg(num);
}

void LOG_PduDevDb::setDate(const QDate &start, const QDate &end)
{
    mSqlCmd += getBetweenDateStr(start, end);
}

void LOG_PduDevDb::setTime(const QTime &start, const QTime &end)
{
    mSqlCmd += getBetweenTimeStr(start, end);
}


QVector<PduDevIndexItem> LOG_PduDevDb::selItems()
{
    QString str = QString("where %1").arg(mSqlCmd);
    return mDevIndex->query(str);
}


/**
 * @brief 获取最前一个和最后一个结果
 * @return
 */
QVector<PduDevIndexItem> LOG_PduDevDb::getIndexResult()
{
    QVector<PduDevIndexItem> retItems,items = selItems();
    int size = items.size();
    if(size > 0) {
        retItems.append(items.at(0));
        if(size > 1) {
            retItems.append(items.at(size-1));
        }
    }

    return retItems;
}


QVector<PduDevIndexItem> LOG_PduDevDb::getResult(QVector<PduElecParamsItem> &start,
                                                 QVector<PduElecParamsItem> &end)
{
    QVector<PduDevIndexItem> indexItem = getIndexResult();
    if(indexItem.size()>0){
        start = mDevParams->selItemsByRid(indexItem.begin()->id);
        end = mDevParams->selItemsByRid(indexItem.last().id);
    }
    return indexItem;
}

