/*
 * log_updatethread.cpp
 * 设备电能线程更新
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "log_updatethread.h"

Log_UpdateThread::Log_UpdateThread(QObject *parent) : QThread(parent)
{
    mTempTable = Log_EleTempTable::get();
    mTgRoomDb = new LOG_TgRoomDb(this);
    mElechange = new Log_Elechange(this);
    mPduDevDb = new LOG_PduDevDb(this);
    mTgCabDb = new LOG_TgCabDb(this);
    connect(this, SIGNAL(updateSig()), this,SLOT(updateSlot()));
}


void Log_UpdateThread::getDbRoomEle(const QString &name)
{
    mTgRoomDb->setRoomName(name);
    mTgRoomDb->setDate(mBarWid->startDate, mBarWid->endDate);
    mTgRoomDb->setTime(mBarWid->startTime, mBarWid->endTime);
    QVector<TgRoomItem> items = mTgRoomDb->getResult();

    EleTempItem eleItem;
    bool ret = mElechange->roomEleChange(items, eleItem);
    if(ret)
        mTempTable->insertItem(eleItem);
}


void Log_UpdateThread::findRoomEle()
{    
    QStringList roomList;
    if(mBarWid->room.isEmpty())
        roomList = mTgRoomDb->getRoomList();
    else
        roomList << mBarWid->room;

    for(int i=0; i<roomList.size(); ++i)
        getDbRoomEle(roomList.at(i));
}

void Log_UpdateThread::getDbDevElc()
{
    mPduDevDb->setDev(mBarWid->ip, mBarWid->slaveNum);
    mPduDevDb->setDate(mBarWid->startDate, mBarWid->endDate);
    mPduDevDb->setTime(mBarWid->startTime, mBarWid->endTime);

    QVector<PduElecParamsItem> startItem, endItem;
    QVector<PduDevIndexItem> indexItem = mPduDevDb->getResult(startItem, endItem);
    if(indexItem.size() > 0)
    {
        QVector<EleTempItem> eleItem;
        bool ret = mElechange->roomDevChange(indexItem, startItem, endItem,eleItem);
        if(ret)
        {
            for(int i=0; i<eleItem.size(); ++i)
                mTempTable->insertItem(eleItem[i]);
        }
    }
}

void Log_UpdateThread::startUpdate(int mode)
{
     this->mode = mode;
     start();
}

void Log_UpdateThread::getDbCabEle()
{
    mTgCabDb->setRoomName(mBarWid->cab);
    mTgCabDb->setDate(mBarWid->startDate, mBarWid->endDate);
    mTgCabDb->setTime(mBarWid->startTime, mBarWid->endTime);

    QVector<TgCabNameCountItem> item = mTgCabDb->getCabNameCount();
    if(!item.isEmpty()){
        for(int i = 0 ; i < item.size() ; i++){
            mTgCabDb->setRoomCabStr(mBarWid->cab,item.at(i).cabName);
            mTgCabDb->setDate(mBarWid->startDate, mBarWid->endDate);
            mTgCabDb->setTime(mBarWid->startTime, mBarWid->endTime);
            QVector<TgCabItem> items = mTgCabDb->getResult();
            EleTempItem eleItemA,eleItemB;
            bool ret = mElechange->cabEleChange(items, eleItemA , eleItemB);
            if(ret){
                mTempTable->insertItem(eleItemA);
                mTempTable->insertItem(eleItemB);
            }
        }
    }
}

void Log_UpdateThread::getDbOneCabEle()
{
    mTgCabDb->setCabName(mBarWid->cab);
    mTgCabDb->setDate(mBarWid->startDate, mBarWid->endDate);
    mTgCabDb->setTime(mBarWid->startTime, mBarWid->endTime);

    QVector<TgCabItem> item = mTgCabDb->getResult();
    if(!item.isEmpty()){
        EleTempItem eleItemA,eleItemB;
        bool ret = mElechange->cabEleChange(item, eleItemA , eleItemB);
        if(ret){
            mTempTable->insertItem(eleItemA);
            mTempTable->insertItem(eleItemB);
        }
    }
}

void Log_UpdateThread::run()
{
    mTempTable->clear();
    if(mode == LOG_EXPORT_ROOM) {
        findRoomEle();
    } else if(mode == LOG_EXPORT_CAB){
        getDbOneCabEle();
    } else if(mode == LOG_EXPORT_ALLBYROOM){
        getDbCabEle();
    }
    else{
        getDbDevElc();
    }

    emit updateSig();
}

void Log_UpdateThread::updateSlot()
{
    mTabView->refreshTable();
}
