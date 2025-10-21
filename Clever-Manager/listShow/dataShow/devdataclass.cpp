/*
 * devdataclass.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "devdataclass.h"

DevDataClass::DevDataClass()
{

}

DevDataClass::~DevDataClass()
{

}
/**
 * @brief 根据IP,获取副机个数
 * @param ip
 * @return 副机个数
 */
int DevDataClass::readMasterNum(QString &ip)
{
    QVector<short> vec;

    int num = readSlaveNum(ip,vec);
    if(num==1)
        num = vec.at(0);
    else if(num>1)
        num = 0;
    else
        return -1;

    return num;
}

/**
  * @brief 读取电压
  * @param ip
  * @param num 副机编号
  * @param id
  * @return
  */
int DevDataClass::readVoltage(QString &ip,short num, short id)
{
    int voltage=-1;
    bool ret = true;

    ret = readDevData(ip,num,devdata);
    if(ret)
        voltage = devdata.adeData.voltage[id];

    return voltage;
}


/**
  * @brief 读取电压
  * @param ip
  * @param num 副机编号
  * @param id
  * @return
  */
int DevDataClass::readVoltage(short id)
{
    return devdata.adeData.voltage[id];
}

/**
 * @brief 读取平均电压
 * @param ip
 * @param num
 * @return
 */
int DevDataClass::readVoltage(QString &ip,short num)
{
    int data=0;
    for(int i=0; i<PDU_LINE; ++i)
        data += readVoltage(ip,num,i);
    return data/3;
}

/**
 * @brief 读取平均电压
 * @return
 */
int DevDataClass::readVoltage(void)
{
    int data=0;
    for(int i=0; i<PDU_LINE; ++i)
        data += readVoltage(i);
    return data/3;
}

/**
 * @brief 读取电流
 * @param ip
 * @param num
 * @param id
 * @return
 */
int DevDataClass::readCurrent(QString &ip,short num,short id)
{
    int data=-1;
    bool ret = readDevData(ip,num,devdata);
    if(ret)
        data = devdata.adeData.current[id];

    return data;
}

int DevDataClass::readCurrent(short id)
{
    return devdata.adeData.current[id];
}

/**
 * @brief 总电流
 * @param ip
 * @param num
 * @return
 */
int DevDataClass::readAllCurrent(QString &ip,short num)
{
    int data=0;
    for(int i=0; i<PDU_LINE; ++i)
        data += readCurrent(ip,num,i);
    return data;
}

/**
 * @brief 总电流  高效率函数
 * @return
 */
int DevDataClass::readAllCurrent(void)
{
    int data=0;
    for(int i=0; i<PDU_LINE; ++i)
        data += devdata.adeData.current[i];
    return data;
}
/**
 * @brief 功率
 * @param ip
 * @param num
 * @param id
 * @return
 */
int DevDataClass::readWatt(QString &ip,short num,short id)
{
    int data=-1;
    bool ret = true;

    ret = readDevData(ip,num,devdata);
    if(ret)
        data = devdata.adeData.watt[id];

    return data;
}

int DevDataClass::readWatt(short id)
{
    return devdata.adeData.watt[id];
}

/**
 * @brief 总功率
 * @param ip
 * @param num
 * @return
 */
int DevDataClass::readAllWatt(QString &ip,short num)
{
    int data=0;
    for(int i=0; i<PDU_LINE; ++i)
        data += readWatt(ip,num,i);
    return data;
}

/**
 * @brief 总功率
 * @param ip
 * @param num
 * @return
 */
int DevDataClass::readAllWatt(void)
{
    int data=0;
    for(int i=0; i<PDU_LINE; ++i)
        data += devdata.adeData.watt[i];;
    return data;
}

/**
 * @brief 电能
 * @param ip
 * @param num
 * @param id
 * @return
 */
int DevDataClass::readEnergy(QString &ip,short num,short id)
{
    int data=-1;
    bool ret = true;

    ret = readDevData(ip,num,devdata);
    if(ret)
        data = devdata.adeData.watt_hour[id];

    return data;
}

int DevDataClass::readEnergy(short id)
{
    return devdata.adeData.watt_hour[id];
}

/**
 * @brief 总电能
 * @param ip
 * @param num
 * @return
 */
int DevDataClass::readAllEnergy(QString &ip,short num)
{
    int data=0;
    for(int i=0; i<PDU_LINE; ++i)
        data += readEnergy(ip,num,i);
    return data;
}


/**
 * @brief 总电能
 * @param ip
 * @param num
 * @return
 */
int DevDataClass::readAllEnergy(void)
{
    int data=0;
    for(int i=0; i<PDU_LINE; ++i)
        data += devdata.adeData.watt_hour[i];
    return data;
}

/**
 * @brief 温度
 * @param ip
 * @param num
 * @param id
 * @return
 */
float DevDataClass::readTemp(QString &ip,short num,short id)
{
    float data=0;
    bool ret = true;

    ret = readDevData(ip,num,devdata);
    if(ret)
        data = devdata.dhtData.Temperature[id];

    return data;
}


/**
 * @brief 温度
 * @param ip
 * @param num
 * @param id
 * @return
 */
float DevDataClass::readTemp(short id)
{
    return devdata.dhtData.Temperature[id];
}

/**
 * @brief 湿度
 * @param ip
 * @param num
 * @param id
 * @return
 */
float DevDataClass::readHum(QString &ip,short num,short id)
{
    float data=0;
    bool ret = true;

    ret = readDevData(ip,num,devdata);
    if(ret)
        data = devdata.dhtData.Humidity[id];

    return data;
}


/**
 * @brief 湿度
 * @param ip
 * @param num
 * @param id
 * @return
 */
float DevDataClass::readHum(short id)
{
    return devdata.dhtData.Humidity[id];
}

/**
 * @brief 电压最小值
 * @param ip
 * @param num
 * @param id
 * @return
 */
int DevDataClass::limitVoltageMin(QString &ip,short num,short id)
{
    int data=-1;
    bool ret = true;

    ret = readDevData(ip,num,devdata);
    if(ret)
        data = devdata.alarmLimit.voltageMin[id];
    return data;
}


/**
 * @brief 电压最小值
 * @param ip
 * @param num
 * @param id
 * @return
 */
int DevDataClass::limitVoltageMin(short id)
{
    return devdata.alarmLimit.voltageMin[id];
}

/**
 * @brief 电压最大值
 * @param ip
 * @param num
 * @param id
 * @return
 */
int DevDataClass::limitVoltageMax(QString &ip,short num,short id)
{
    int data=-1;
    bool ret = true;

    ret = readDevData(ip,num,devdata);
    if(ret)
        data = devdata.alarmLimit.voltageMax[id];

    return data;
}


/**
 * @brief 电压最大值
 * @param ip
 * @param num
 * @param id
 * @return
 */
int DevDataClass::limitVoltageMax(short id)
{
    return devdata.alarmLimit.voltageMax[id];
}

/**
 * @brief 电流最小值
 * @param ip
 * @param num
 * @param id
 * @return
 */
int DevDataClass::limitCurrentMin(QString &ip,short num,short id)
{
    int data=-1;
    bool ret = true;

    ret = readDevData(ip,num,devdata);
    if(ret)
        data = devdata.alarmLimit.currentMin[id];

    return data;
}

/**
 * @brief 电流最大值
 * @param ip
 * @param num
 * @param id
 * @return
 */
int DevDataClass::limitCurrentMax(QString &ip,short num,short id)
{
    int data=-1;
    bool ret = true;

    ret = readDevData(ip,num,devdata);
    if(ret)
        data = devdata.alarmLimit.currentMax[id];

    return data;
}


/**
 * @brief 电流最大值
 * @param ip
 * @param num
 * @param id
 * @return
 */
int DevDataClass::limitCurrentMax(short id)
{
    return devdata.alarmLimit.currentMax[id];
}

/**
 * @brief 温度最小值
 * @param ip
 * @param num
 * @param id
 * @return
 */
int DevDataClass::limitTempMin(QString &ip,short num,short id)
{
    int data=-1;
    bool ret = true;

    ret = readDevData(ip,num,devdata);
    if(ret)
        data = devdata.alarmLimit.TempMin[id];

    return data;
}


/**
 * @brief 温度最小值
 * @param ip
 * @param num
 * @param id
 * @return
 */
int DevDataClass::limitTempMin(short id)
{
    return devdata.alarmLimit.TempMin[id];
}

/**
 * @brief 温度最大值
 * @param ip
 * @param num
 * @param id
 * @return
 */
int DevDataClass::limitTempMax(QString &ip,short num,short id)
{
    int data=-1;
    bool ret = true;

    ret = readDevData(ip,num,devdata);
    if(ret)
        data = devdata.alarmLimit.TempMax[id];

    return data;
}


/**
 * @brief 温度最大值
 * @param ip
 * @param num
 * @param id
 * @return
 */
int DevDataClass::limitTempMax(short id)
{
    return devdata.alarmLimit.TempMax[id];
}

/**
 * @brief 湿度最小值
 * @param ip
 * @param num
 * @param id
 * @return
 */
int DevDataClass::limitHumMin(QString &ip,short num,short id)
{
    int data=-1;
    bool ret = true;

    ret = readDevData(ip,num,devdata);
    if(ret)
        data = devdata.alarmLimit.HumMin[id];

    return data;
}


/**
 * @brief 湿度最小值
 * @param ip
 * @param num
 * @param id
 * @return
 */
int DevDataClass::limitHumMin(short id)
{
    return devdata.alarmLimit.HumMin[id];
}

/**
 * @brief 温度最大值
 * @param ip
 * @param num
 * @param id
 * @return
 */
int DevDataClass::limitHumMax(QString &ip,short num,short id)
{

    int data=-1;
    bool ret = true;

    ret = readDevData(ip,num,devdata);
    if(ret)
        data = devdata.alarmLimit.HumMax[id];

    return data;
}


/**
 * @brief 温度最大值
 * @param ip
 * @param num
 * @param id
 * @return
 */
int DevDataClass::limitHumMax(short id)
{
    return devdata.alarmLimit.HumMax[id];
}

/**
 * @brief 是否报警
 * @param ip
 * @param num
 * @return
 */
int DevDataClass::isAlarm(QString &ip,short num)
{
    int data=-1;
    bool ret = true;

    ret = readDevData(ip,num,devdata);
    if(ret)
        data = devdata.alarm.alarm;
    return data;
}

/**
 * @brief 报警的设备号
 * @param ip
 * @return
 */
int DevDataClass::alarmDevNum(QString &ip)
{
    QVector<short> vec;

    int num = readSlaveNum(ip,vec);
    for(int i=0; i<num; ++i)
    {
        int data = isAlarm(ip,vec[i]);
        if(data>0)
            return vec[i];
    }

    return  -1;
}


/**
 * @brief 报警相数
 * @return
 */
int DevDataClass::alarmLine(void)
{
    int ret= 0;
    Data_alarmType *ptr = &devdata.alarm;

    for(int i=0; i<PDU_LINE; ++i)
    {
        if((ptr->c_Lalarm & (1 << i)) || (ptr->c_Halarm & (1 << i)) ||
                (ptr->v_Lalarm & (1 << i)) || (ptr->v_Halarm & (1 << i)))
        {
            ret |= (1<<i);
        }
    }

    return ret;
}


/**
 * @brief 报警相数
 * @param alarm 报警标志
 * @return
 */
int DevDataClass::alarmLine(short &alarm)
{
    Data_alarmType *ptr = &devdata.alarm;

    for(int i=0; i<PDU_LINE; ++i)
    {
        if(ptr->c_Lalarm & (1 << i))  // 电流过小
            alarm |= 1;
    }

    for(int i=0; i<PDU_LINE; ++i)
    {
        if(ptr->c_Halarm & (1 << i))  // 电流过大
            alarm |= 1;
    }

    for(int i=0; i<PDU_LINE; ++i)
    {
        if(ptr->v_Lalarm & (1 << i))  // 电压过小
            alarm |= 2;
    }

    for(int i=0; i<PDU_LINE; ++i)
    {
        if(ptr->v_Halarm & (1 << i)) // 电压过大
            alarm |= 2;
    }

    if(ptr->T_Lalarm) // 温度过低
        alarm |= 4;

    if(ptr->T_Halarm) // 温度过高
        alarm |= 4;

    if(ptr->H_Lalarm) // 湿度过低
        alarm |= 8;

    if(ptr->H_Halarm) // 湿度过高
        alarm |= 8;

    int ret = alarmLine();
    for(int i=0; i<PDU_LINE; ++i)
    {
        if(ret & (1<<i))
            return i;
    }

    return 0;
}



/**
 * @brief 是否有报警的设备
 * @param ip
 * @return
 */
bool DevDataClass::isAlarm(QString &ip)
{
    QVector<short> vec;

    int num = readSlaveNum(ip,vec);
    for(int i=0; i<num; ++i)
    {
        int data = isAlarm(ip,vec[i]);
        if(data>0)
            return true;
    }
    return  false;
}
