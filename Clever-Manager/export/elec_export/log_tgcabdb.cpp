/*
 * log_tgroomdb.cpp
 * 机房电能表查询
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "log_tgcabdb.h"

LOG_TgCabDb::LOG_TgCabDb(QObject *parent) : QObject(parent)
{
    mDbTgCab = DbTgCabinet::get();
}


QString LOG_TgCabDb::getRoomNameStr(const QString &name)
{
    QString cmd = QString("roomName = \'%1\' ").arg(name);
    return cmd;
}

void LOG_TgCabDb::setRoomName(const QString &name)
{
    mSqlCmd = getRoomNameStr(name);
}

void LOG_TgCabDb::setDate(const QDate &start, const QDate &end)
{
    mSqlCmd += getBetweenDateStr(start, end);
}

void LOG_TgCabDb::setTime(const QTime &start, const QTime &end)
{
    mSqlCmd += getBetweenTimeStr(start, end);
}

QString LOG_TgCabDb::getCabByRoomCabStr(const QString &room, const QString &cab)
{
    QString cmd = QString("roomName=\'%1\' and  cabName=\'%2\' ").arg(room).arg(cab);
    return cmd;
}

QString LOG_TgCabDb::getCabNameStr(const QString &name)
{
    QString cmd = QString("cabName = \'%1\' ").arg(name);
    return cmd;
}


void LOG_TgCabDb::setCabName(const QString &name)
{
    mSqlCmd = getCabNameStr(name);
}


void LOG_TgCabDb::setRoomCabStr(const QString &roomName , const QString &cabName)
{
    mSqlCmd = getCabByRoomCabStr(roomName , cabName);
}

QVector<TgCabItem> LOG_TgCabDb::selItems()
{
    QString str = QString("where %1").arg(mSqlCmd);
    return mDbTgCab->selItemsByRoomName(str);
}


QStringList LOG_TgCabDb::getCabList()
{
    QStringList list;
    QVector<TgCabItem> items = mDbTgCab->allItems();

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
QVector<TgCabItem> LOG_TgCabDb::getResult()
{
    QVector<TgCabItem> retItems,items = selItems();
    int size = items.size();
//    for(int i = 0 ; i < size ; i++)
//        qDebug()<<items.at(i).cabName <<items.at(i).elea<<items.at(i).eleb;
    if(size > 0) {
        retItems.append(items.at(0));
        if(size > 1) {
            retItems.append(items.at(size-1));
        }
    }

    return retItems;
}

QVector<TgCabNameCountItem> LOG_TgCabDb::getCabNameCount()
{
    QString str = QString("where %1").arg(mSqlCmd);
    QVector<TgCabNameCountItem> items = mDbTgCab->selectItemsGroupByCabName(str);
//    for(int i = 0 ; i < items.size() ; i++)
//    {
//        qDebug() <<"name "<< items.at(i).cabName <<"count "<< items.at(i).count;
//    }
    return items;
}

