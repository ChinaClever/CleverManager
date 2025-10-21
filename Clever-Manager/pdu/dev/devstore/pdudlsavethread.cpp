/*
 * pdudlsavethread.cpp
 * 设备链表保存线程
 *  1、每15秒会保存一数设备数据
 *
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdudlsavethread.h"
#include "pdudevdel.h"

PduDLSaveThread::PduDLSaveThread(QObject *parent) : QThread(parent)
{
    isRun = false;

    devAddr = new PduDevAddrThread(this);
    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(timeoutDone()));
    mTimer->start(15*1000);
}


PduDLSaveThread::~PduDLSaveThread()
{
    wait();
}


/**
 * @brief 定时器启动线程
 */
void PduDLSaveThread::timeoutDone()
{
    if(isRun == false)
    {
        isRun = true;
        start();
        // run();
    }
}


/**
 * @brief 线程入口函数
 */
void PduDLSaveThread::run(void)
{
    do{
        msleep(50);
    } while (dl_del_devData()>0);

    QSqlDatabase::database().transaction();
    do {
        msleep(1);
    } while (dl_save_devData()>0);
    QSqlDatabase::database().commit();

    isRun = false;
//    msleep(5);
}
