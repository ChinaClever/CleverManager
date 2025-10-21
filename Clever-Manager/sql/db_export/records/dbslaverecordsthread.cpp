/*
 * dbslaverecordsthread.cpp
 * PDU设备数据保存设计思路
 *  1、启动线程读取设备链表
 *  2、读取完设备链表，启动定时器
 *  3、每次存储一个设备
 *  4、存储完成，停止定时器
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "dbslaverecordsthread.h"
#include "dbpdudevrecord.h"

#define DC_SLAVE_TIME 20
extern bool gSlaveing;
extern QSqlDatabase *get_database();

DbSlaveRecordsThread::DbSlaveRecordsThread(QObject *parent) : QThread(parent)
{
    int time = rand()%10+45;
    QTimer::singleShot(time*1000,this,SLOT(dbTimeoutDone()));
}

DbSlaveRecordsThread::~DbSlaveRecordsThread()
{
    isRun = false;
    wait();
}

DbSlaveRecordsThread *DbSlaveRecordsThread::get(QObject *parent)
{
    static DbSlaveRecordsThread* sington = nullptr;
    if(sington == nullptr)
        sington = new DbSlaveRecordsThread(parent);
    return sington;
}


void DbSlaveRecordsThread::dbTimeoutDone()
{
    mTimeCount=0;
    isRun = false;

    timer = new QTimer(this);
    timer->start(1000);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
}

void DbSlaveRecordsThread::saveSleep(int t)
{
    for(int i=0; i<t; ++i){
        if(isRun) sleep(1);
        else break;
    }
}


void DbSlaveRecordsThread::slaveCabData()
{
    int ret = mCabItems.size();
    if(ret) {
        for(int i=0; i<mCabItems.size(); ++i) {
            DbTgCabinet::get()->insertItem(mCabItems.front());
            mCabItems.pop_front();
        }
    }
}

void DbSlaveRecordsThread::slaveRoomData()
{
    QSqlDatabase::database().transaction();
    int ret = mRoomItems.size();
    if(ret) {
        for(int i=0; i<mRoomItems.size(); ++i) {
            DbTgRoom::get()->insertItem(mRoomItems.front());
            mRoomItems.pop_front();
        }
    }
    slaveCabData();
    QSqlDatabase::database().commit();
    saveSleep(15);
}


/**
 * @brief 保存数据
 */
int DbSlaveRecordsThread::slaveData()
{
    int ret = 0;
    bool isSlave = true; //getSlaveTime();
#ifndef CLOUD_MODULE
    if(isSlave)
#endif
    {
        QList<sDbPduDevList *> devList;
        ret = getDevList(devList);
        QSqlDatabase::database().transaction();
        while(devList.size() > 0)
        {
            sDbPduDevList *devNode = devList.first();
            DbPduDevRecord::get(this)->slave(devNode->devType, devNode->ip, devNode->devNum, isSlave);
            devList.removeFirst();
            //msleep(1);
            delete devNode;
        }
        QSqlDatabase::database().commit();
    }
    return ret;
}

int DbSlaveRecordsThread::getDevList(QList<sDbPduDevList *> &devs)
{
    QList<int> list;
    pdu_hashData_list(list); // 获取设备类型接口

    for(int i=0; i<list.size(); ++i)
    {
        PduHashIP *hashIP =  pdu_hashData_getHash(list.at(i)); // 获取某种设备类型的对象
        if(hashIP == nullptr) continue;
        QStringList ipList;
        hashIP->list(ipList); // 根据设备类型，获取设备IP列表

        for(int j=0; j<ipList.size(); ++j)
        {
            PduDevHash *devHash = hashIP->getDev(ipList.at(j));// 获取设备对象
            if(devHash == nullptr) continue;
            QList<int> devList;
            devHash->list(devList); // 根据设备IP，获取设备地址列表 级联

            for(int k=0; k<devList.size(); ++k)
            {
                PduDataPacket *data = devHash->getPacket(devList.at(k)); // 获取设备数据
                if(data == nullptr) continue;
                if(data->offLine > 0)  // 必须在线才进行检查
                {
                    sDbPduDevList *devNode = new sDbPduDevList;
                    if(devNode == nullptr) continue;
                    devNode->devType = data->devType;
                    devNode->ip = data->ip->get();
                    devNode->devNum = data->devNum;
                    devs.append(devNode);
                    msleep(DC_SLAVE_TIME); // 延时1us 让其它线程优先执行
                }
            }
        }
    }
    return devs.size();
}

void DbSlaveRecordsThread::run()
{
    static QReadWriteLock lock;
    QWriteLocker locker(&lock);
    while(gSlaveing) gSlaveing = true;
    //static int count = 0;

    slaveRoomData();
    QSqlDatabase::database().transaction();
    //if(count++ == 9) {
    int ret = slaveData();
    //if(ret>10)saveSleep(5*60);
    //     count = 0;
    //}

    QSqlDatabase::database().commit();
    gSlaveing = false;
    //    saveSleep(35);

    isRun = false;
}


bool DbSlaveRecordsThread::getSlaveTime()
{
    bool ret = false;
    int count  = getTime()*60;
    if(mTimeCount >= count)  //更新数据
    {
        mTimeCount = 0;
        ret = true;
    }
    return ret;
}


void DbSlaveRecordsThread::timeoutDone()
{
    mTimeCount++;
    if(!isRun) {
        if(getSlaveTime()) {
            isRun = true;
            start();
        }
    }
}

