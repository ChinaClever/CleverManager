/*
 * 配置文件公共基类
 *
 *  Created on: 2018年1月1日
 *      Author: Lzy
 */
#include "configbase.h"

ConfigBase::ConfigBase()
{
    item = new sConfigItem();
    item->sound = getSound();
    item->logTimes = getLogTime();
    item->logCount = getLogCount();
    item->elePrice = getElePrice();

    item->wsEn = getWsEn();
    item->wsUrl = getWsUrl();
    item->wsPort = getWsPort();
    item->reTime = getReTime();

    for(int i=0; i<PUSH_UDP_SIZE; ++i) {
        item->udpEn[i] = getUdpEn(i);
        item->udpUrl[i] = getUdpUrl(i);
        item->udpPort[i] = getUdpPort(i);
    }

    item->envEn = getEnvEn();
    item->netAddrEn = getNetEn();
    item->pushTime = getPushTime();
    item->thresholdEn = getThresholdEn();
}

ConfigBase *ConfigBase::bulid()
{
    static ConfigBase* sington = nullptr;
    if(sington == nullptr)
        sington = new ConfigBase();
    return sington;
}


int ConfigBase::getPushTime()
{
    QString prefix = getPrefix();
    QString str = QString("%1_push_time").arg(prefix);
    int ret = com_cfg_readInt(str, prefix);
    if(ret <= 0)  ret = 15;
    return ret;
}

/**
 * @brief 设置日志存储时间间隔
 * @param num
 */
void ConfigBase::setPushTime(int s)
{
    QString prefix = getPrefix();
    QString str = QString("%1_push_time").arg(prefix);
    com_cfg_writeParam(str, QString::number(s), prefix);
}



int ConfigBase::getReTime()
{
    QString prefix = getPrefix();
    QString str = QString("%1_ws_retime").arg(prefix);
    int ret = com_cfg_readInt(str, prefix);
    if(ret <= 0)  ret = 10;
    return ret;
}

/**
 * @brief 设置日志存储时间间隔
 * @param num
 */
void ConfigBase::setReTime(int s)
{
    QString prefix = getPrefix();
    QString str = QString("%1_ws_retime").arg(prefix);
    com_cfg_writeParam(str, QString::number(s), prefix);
}


int ConfigBase::getThresholdEn()
{
    QString prefix = getPrefix();
    QString str = QString("%1_Threshold_en").arg(prefix);
    int ret = com_cfg_readInt(str, prefix);
    if(ret <= 0)  ret = 0;
    return ret;
}

/**
 * @brief 设置日志存储时间间隔
 * @param num
 */
void ConfigBase::setThresholdEn(int s)
{
    QString prefix = getPrefix();
    QString str = QString("%1_Threshold_en").arg(prefix);
    com_cfg_writeParam(str, QString::number(s), prefix);
}




int ConfigBase::getNetEn()
{
    QString prefix = getPrefix();
    QString str = QString("%1_Net_en").arg(prefix);
    int ret = com_cfg_readInt(str, prefix);
    if(ret <= 0)  ret = 0;
    return ret;
}

/**
 * @brief 设置日志存储时间间隔
 * @param num
 */
void ConfigBase::setNetEn(int s)
{
    QString prefix = getPrefix();
    QString str = QString("%1_Net_en").arg(prefix);
    com_cfg_writeParam(str, QString::number(s), prefix);
}



int ConfigBase::getEnvEn()
{
    QString prefix = getPrefix();
    QString str = QString("%1_env_en").arg(prefix);
    int ret = com_cfg_readInt(str, prefix);
    if(ret <= 0)  ret = 0;
    return ret;
}

/**
 * @brief 设置日志存储时间间隔
 * @param num
 */
void ConfigBase::setEnvEn(int s)
{
    QString prefix = getPrefix();
    QString str = QString("%1_env_en").arg(prefix);
    com_cfg_writeParam(str, QString::number(s), prefix);
}



int ConfigBase::getUdpPort(int id)
{
    QString prefix = getPrefix();
    QString str = QString("%1_Udp_port_%2").arg(prefix).arg(id);
    int ret = com_cfg_readInt(str, prefix);
    if(ret <= 0)  ret = 8080;
    return ret;
}

/**
 * @brief 设置日志存储时间间隔
 * @param num
 */
void ConfigBase::setUdpPort(int id, int s)
{
    QString prefix = getPrefix();
    QString str = QString("%1_Udp_port_%2").arg(prefix).arg(id);
    com_cfg_writeParam(str, QString::number(s), prefix);
}



QString ConfigBase::getUdpUrl(int id)
{
    QString prefix = getPrefix();
    QString str = QString("%1_Udp_url_%2").arg(prefix).arg(id);
    return  com_cfg_readStr(str, prefix);
}

/**
 * @brief 设置日志存储时间间隔
 * @param num
 */
void ConfigBase::setUdpUrl(int id, QString &url)
{
    QString prefix = getPrefix();
    QString str = QString("%1_Udp_url_%2").arg(prefix).arg(id);
    com_cfg_writeParam(str, url, prefix);
}


/**
 * @brief 获取日志存储时间隔
 * @return
 */
int ConfigBase::getUdpEn(int id)
{
    QString prefix = getPrefix();
    QString str = QString("%1_Udp_en_%2").arg(prefix).arg(id);
    int ret = com_cfg_readInt(str, prefix);
    if(ret <= 0)  ret = 0;
    return ret;
}

/**
 * @brief 设置日志存储时间间隔
 * @param num
 */
void ConfigBase::setUdpEn(int id, int s)
{
    QString prefix = getPrefix();
    QString str = QString("%1_Udp_en_%2").arg(prefix).arg(id);
    com_cfg_writeParam(str, QString::number(s), prefix);
}


int ConfigBase::getWsPort()
{
    QString prefix = getPrefix();
    QString str = QString("%1_ws_port").arg(prefix);
    int ret = com_cfg_readInt(str, prefix);
    if(ret <= 0)  ret = 2346;
    return ret;
}

/**
 * @brief 设置日志存储时间间隔
 * @param num
 */
void ConfigBase::setWsPort(int s)
{
    QString prefix = getPrefix();
    QString str = QString("%1_ws_port").arg(prefix);
    com_cfg_writeParam(str, QString::number(s), prefix);
}



QString ConfigBase::getWsUrl()
{
    QString prefix = getPrefix();
    QString str = QString("%1_ws_url").arg(prefix);
    return  com_cfg_readStr(str, prefix);
}

/**
 * @brief 设置日志存储时间间隔
 * @param num
 */
void ConfigBase::setWsUrl(QString &url)
{
    QString prefix = getPrefix();
    QString str = QString("%1_ws_url").arg(prefix);
    com_cfg_writeParam(str, url, prefix);
}

/**
 * @brief 获取日志存储时间隔
 * @return
 */
int ConfigBase::getLogTime()
{
    QString prefix = getPrefix();
    QString str = QString("%1_log_time").arg(prefix);
    int ret = com_cfg_readInt(str, prefix);
    if(ret <= 0)  ret = 24;
    return ret;
}

/**
 * @brief 设置日志存储时间间隔
 * @param num
 */
void ConfigBase::setLogTime(int num)
{
    QString prefix = getPrefix();
    QString str = QString("%1_log_time").arg(prefix);
    com_cfg_writeParam(str, QString::number(num), prefix);
}


/**
 * @brief 获取日志存储时间隔
 * @return
 */
int ConfigBase::getLogCount()
{
    QString prefix = getPrefix();
    QString str = QString("%1_log_count").arg(prefix);
    int ret = com_cfg_readInt(str, prefix);
    if(ret <= 0)  ret = 10*10000;
    return ret;
}

/**
 * @brief 设置日志存储时间间隔
 * @param num
 */
void ConfigBase::setLogCount(int count)
{
    QString prefix = getPrefix();
    QString str = QString("%1_log_count").arg(prefix);
    com_cfg_writeParam(str, QString::number(count), prefix);
}

/**
 * @brief 获取单价
 * @return
 */
double ConfigBase::getElePrice()
{
    QString prefix = getPrefix();
    QString str = QString("%1_ele_price").arg(prefix);
    double ret = com_cfg_readDouble(str, prefix);
    if(ret < 0)  ret = 1;
    return ret;
}

/**
 * @brief 设置单价
 * @param num
 */
void ConfigBase::setElePrice(double p)
{
    QString prefix = getPrefix();
    QString str = QString("%1_ele_price").arg(prefix);
    com_cfg_writeParam(str, QString::number(p), prefix);
}


/**
 * @brief 获取日志存储时间隔
 * @return
 */
int ConfigBase::getSound()
{
    QString prefix = getPrefix();
    QString str = QString("%1_sound_sw").arg(prefix);
    int ret = com_cfg_readInt(str, prefix);
    if(ret <= 0)  ret = 1;
    return ret;
}

/**
 * @brief 设置日志存储时间间隔
 * @param num
 */
void ConfigBase::setSound(int s)
{
    QString prefix = getPrefix();
    QString str = QString("%1_sound_sw").arg(prefix);
    com_cfg_writeParam(str, QString::number(s), prefix);
}



/**
 * @brief 获取日志存储时间隔
 * @return
 */
int ConfigBase::getWsEn()
{
    QString prefix = getPrefix();
    QString str = QString("%1_ws_en").arg(prefix);
    int ret = com_cfg_readInt(str, prefix);
    if(ret <= 0)  ret = 0;
    return ret;
}

/**
 * @brief 设置日志存储时间间隔
 * @param num
 */
void ConfigBase::setWsEn(int s)
{
    QString prefix = getPrefix();
    QString str = QString("%1_ws_en").arg(prefix);
    com_cfg_writeParam(str, QString::number(s), prefix);
}
