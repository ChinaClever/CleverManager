/*
 * log_tgroomdb.cpp
 * 机房电能表查询
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "log_tgroomdb.h"

LOG_TgRoomDb::LOG_TgRoomDb(QObject *parent) : QObject(parent)
{
    mDbTgRoom = DbTgRoom::get();
}


QString LOG_TgRoomDb::getRoomNameStr(const QString &name)
{
    QString cmd = QString("roomName = \'%1\' ").arg(name);
    return cmd;
}

void LOG_TgRoomDb::setRoomName(const QString &name)
{
    mSqlCmd = getRoomNameStr(name);
}

void LOG_TgRoomDb::setDate(const QDate &start, const QDate &end)
{
    mSqlCmd += getBetweenDateStr(start, end);
}

void LOG_TgRoomDb::setTime(const QTime &start, const QTime &end)
{
    mSqlCmd += getBetweenTimeStr(start, end);
}


QVector<TgRoomItem> LOG_TgRoomDb::selItems()
{
    QString str = QString("where %1").arg(mSqlCmd);
    return mDbTgRoom->query(str);
}


QStringList LOG_TgRoomDb::getRoomList()
{
    QStringList list;
    QVector<TgRoomItem> items = mDbTgRoom->allItems();

    QString str;
    for(int i=0; i<items.size(); ++i) {
        str = items.at(i).roomName;
        if(!list.contains(str))
            list.append(str);
    }
    return list;
}


/**
 * @brief 获取最前一个和最后一个结果
 * @return
 */
QVector<TgRoomItem> LOG_TgRoomDb::getResult()
{
    QVector<TgRoomItem> retItems,items = selItems();
    int size = items.size();
    if(size > 0) {
        retItems.append(items.at(0));
        if(size > 1) {
            retItems.append(items.at(size-1));
        }
    }

    return retItems;
}


