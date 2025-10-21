/*
 * sim900a_call.cpp
 * 打电话操作
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "sim900a_call.h"

SIM900A_Call::SIM900A_Call()
{
    mSim = new SIM900A();
    mutex = &g_sim_mutex;
}

SIM900A_Call::~SIM900A_Call()
{
    delete mSim;
}

/**
  * @brief 打电话
  * @param phone
  * @return
  */
bool SIM900A_Call::callPhone(QString phone)
{
    QMutexLocker locker(mutex);
    return mSim->sim900a_call_Phone(phone);
}

/**
  * @brief 检测到来电立即挂断电话
  */
void SIM900A_Call::timeoutDone(void)
{
    QMutexLocker locker(mutex);

    QString phone;
    bool ret = mSim->sim900a_answer_Phone(phone); //接收到来电
    if(ret)
    {
        if(!phone.isEmpty())
            emit callPhoneSig(phone); // 发出信号
        mSim->sim900a_call_hangUp(); // 挂断电话
    }
}
