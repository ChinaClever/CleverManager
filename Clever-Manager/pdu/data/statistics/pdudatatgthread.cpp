/*
 * pdudatatgthread.cpp
 * PDU设备统计线程类 提供的接口函数与PduDataTG类 相对应
 *  1、调用一个公共接口，会启动线程进行数据统计
 *  2、调用getData()接口，获取数据
 *
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "pdudatatgthread.h"
#define PDU_TG_TIME (4*1000) // 2秒针统计一次数据


PduDataTgThread::PduDataTgThread(QObject *parent) : QThread(parent)
{
    mStep = -1;
    mPduDataTg = new PduDataTG(this);
    mData = new sPduTGDataStr;
    memset(mData,0, sizeof(sPduTGDataStr));

    isRun = false;
    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(timeoutDone()));
}

PduDataTgThread::~PduDataTgThread()
{
    wait();

//    delete mTimer;
//    delete mPduDataTg;
    delete mData;
}


/**
 * @brief 级联数据统计
 * @param devHash
 */
void PduDataTgThread::tgCasData(PduDevHash *devHash)
{
    mDevHash = devHash;
    mTimer->start(PDU_TG_TIME);
    mStep = 1;
}

/**
 * @brief 某一类数据统计
 * @param hashIP
 */
void PduDataTgThread::tgDevData(PduHashIP *hashIP)
{
    mHashIP = hashIP;
    mTimer->start(PDU_TG_TIME);
    mStep = 2;
}

/**
 * @brief 统计所有数据
 */
void PduDataTgThread::tgData(void)
{
    mTimer->start(PDU_TG_TIME);
    mStep = 3;
}

/**
 * @brief 获取数据
 * @return
 */
sPduTGDataStr *PduDataTgThread::getData(void)
{
    return mData;
}


/**
 * @brief 定时器启动线程
 */
void PduDataTgThread::timeoutDone()
{
    start();
}

/**
 * @brief 数据统计处理函数
 */
void PduDataTgThread::dataTg(void)
{
    sPduTGDataStr *ptr = NULL;

    switch (mStep)
    {
    case 1:
        ptr = mPduDataTg->getCasTgData(mDevHash);
        break;

    case 2:
        ptr = mPduDataTg->getDevTgData(mHashIP);
        break;

    case 3:
        ptr = mPduDataTg->getTgData();
        break;

    default:
        break;
    }

    if(ptr)
    {
        memcpy(mData, ptr, sizeof(sPduTGDataStr));
        delete ptr;
    }
    else
        memset(mData,0, sizeof(sPduTGDataStr));
}



/**
 * @brief 线程入口函数
 */
void PduDataTgThread::run(void)
{
    if(isRun == false)
    {
        isRun = true;
        dataTg();

        isRun = false;
    }
}
