/*
 * sim900a_err.cpp
 * SIM900A检测类
 *      1、初始化SIM时会自动打开串口
 *      2、5秒定时器会检测SIM状态，如有异常会发出异常信号
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "sim900a_err.h"

SIM_ModuInfo sim_moduInfo;
SIM_gsmInfo  sim_gsmInfo;

SIM900A_ERR::SIM900A_ERR(QObject *parent) : QThread(parent)
{
    mSim = new SIM900A(this);
    mutex = &g_sim_mutex;
}

SIM900A_ERR::~SIM900A_ERR()
{
    delete mSim;
}

/**
 * @brief 获取相应信息
 * @return
 */
int SIM900A_ERR::getSimInfo(void)
{
    SIM_ModuInfo modu;
    SIM_gsmInfo gsm;
    int rtn = SIM_OK;

    bool ret = mSim->sim900a_modu_info(modu);
    if(!ret)
        rtn = SIM_MODU_INFO_ERR;
    else
    {
        ret = mSim->sim900a_gsm_info(gsm);
        if(!ret)
            rtn = SIM_PIN_INFO_ERR;
    }

    sim_moduInfo = modu;
    sim_gsmInfo = gsm;

    return rtn;
}



/**
 * @brief 检查SIM
 * @return
 */
int SIM900A_ERR::cleckSim(void)
{
    bool ret = mSim->sim900a_AT_cmd();
    if(!ret)
        return SIM_MODU_ERR;

    ret = mSim->simp00a_cpin();
    if(!ret)
        return SIM_PIN_ERR;

    ret = mSim->simp900a_cops();
    if(!ret)
        return SIM_COPS_ERR;

    return SIM_OK;
}


void SIM900A_ERR::timeoutDone(void)
{
    QMutexLocker locker(mutex);

    if(g_sim_isOpen)
    {
        int rtn = cleckSim();   // 检查SIM卡
        if(rtn == SIM_OK)
            rtn = getSimInfo(); // 获取相应信息

        emit simErrSig(rtn);
    }
    else
        emit simErrSig(SIM_COM_ERR);
}
