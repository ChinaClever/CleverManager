/*
 * sim900a_sms.cpp
 * 信息管理类
 *      1、接收到新信息时发出信息
 *      2、读信息时，会把卡片中信息全部删除
 *      3、发送信息
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "sim900a_sms.h"


SIM900A_SMS::SIM900A_SMS(QThread *parent) : QThread(parent)
{
    mSim = new SIM900A(this);
    mutex = &g_sim_mutex;
}

SIM900A_SMS::~SIM900A_SMS()
{
    delete mSim;
}


/**
 * 发送信息
 */
bool SIM900A_SMS::sentSMS(SIM_smsInfo *msg)
{
    QMutexLocker locker(mutex);
    bool ret = mSim->sim900a_sms_send(msg);
    if(!ret)
        emit sentSmsFailed(); //发送失败

    return ret;
}

/**
 * @brief 读信息
 * @param smsInfo
 * @return
 */
SIM_smsInfo * SIM900A_SMS::readSMS(void)
{
    SIM_smsInfo *smsInfo = NULL;
    if(mRecvSmsQue.size() > 0)
        smsInfo = mRecvSmsQue.dequeue();

    return smsInfo;
}

/**
 * @brief 删除所有信息
 * @return
 */
bool SIM900A_SMS::clearSms(void)
{
    return mSim->sim900a_sms_del(1,4);
}

bool SIM900A_SMS::checkNewSms(int msgNum)
{
    for(int i=1; i<=msgNum; i++)
    {
        SIM_smsInfo *msg = new SIM_smsInfo;
        bool ret = mSim->sim900a_sms_read(i, msg); // 读信息
        if(ret)
        {
            if(msg->flag) // 新信息
            {
                mRecvSmsQue.enqueue(msg); // 加入队列
                emit newSMS();
            //    qDebug() << "new SMS:" << i << msg.flag << msg.pohneNum << msg.msg;
            }
           // qDebug()<< i << msg.flag << msg.pohneNum << msg.msg;
        }
       clearSms(); //删除所有信息
    }

    return true;
}

void SIM900A_SMS::timeoutDone(void)
{
    short msgNum, msgMaxNum;
    QMutexLocker locker(mutex);

    bool ret = mSim->sim900a_sms_count(msgNum,msgMaxNum);
    if(ret)
    {
        if(msgNum>0)
            checkNewSms(msgNum); // 读信息
    }
}


