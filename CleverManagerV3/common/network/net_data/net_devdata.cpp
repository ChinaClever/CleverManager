/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "net_devdata.h"

Net_DevData::Net_DevData()
{
    mDataCom = Net_DataCom::bulid();
}

Net_DevData *Net_DevData::bulid()
{
    static Net_DevData* sington = NULL;
    if(sington == NULL) {
        sington = new Net_DevData();
    }
    return sington;
}

/**
 * @brief 数据单元处理 主要针对 当前值、最大、小值等数据的处理
 * @param unit
 * @param data
 */
int  Net_DevData::unitData(sDataUnit &unit, Net_sDevData *data)
{
    ushort ret=0, *ptr = NULL;

    int fc = data->fn[1] & 0x0f; // 处理功能码，第二字节的低四位数据
    switch (fc)
    {
    case PDU_CMD_VALUE:
        ptr = unit.value;
        break;

    case PDU_CMD_MIN:
        ptr = unit.min;
        break;

    case PDU_CMD_MAX:
        ptr = unit.max;
        break;

    case PDU_CMD_ALARM:
        // ptr = unit.alarm;
        break;

    case PDU_CDM_CRMIN:
        ptr = unit.crMin;
        break;

    case PDU_CMD_CRMAX:
        ptr = unit.crMax;
        break;

    case PDU_CMD_CRALARM:
        // ptr = unit.crAlarm;
        break;

    case PDU_CMD_NUM_SIZE:
        unit.size = data->data[0];
        return unit.size;

    default:
        qDebug() << "pdu_hashUnitData err" << fc;
        break;
    }

    if(ptr) ret = mDataCom->saveData(ptr, data->data, data->len);
    if(ret > 0) unit.size = ret;

    return ret;
}


/**
 * @brief 设备对象数据的处理 主要电流、电压、功率、电能
 * @param obj
 * @param data
 */
int Net_DevData::objData(sObjData &obj,Net_sDevData *data)
{
    uint *pInt = nullptr;
    uchar *pChart = nullptr;
    ushort *pShort = nullptr;
    int ret = 0;

    int fc = data->fn[1] >> 4; // // 处理功能码，第二字节的高四位
    switch (fc)
    {
    case PDU_CMD_CUR: // 电流
        ret = unitData(obj.cur, data);
        break;

    case PDU_CMD_VOL: // 电压
        unitData(obj.vol, data);
        break;

    case PDU_CMD_POW: // 功率
        pInt = obj.pow;
        break;

    case PDU_CMD_ELE: // 电能
        pInt = obj.ele;
        break;

    case PDU_CMD_PF: // 功率因数
        pShort = obj.pf;
        break;

    case PDU_CMD_SW: // 开关状态
        pChart = obj.sw;
        break;

    case PDU_CMD_RATE: //电压频率
        //ptr = obj->rate;
        break;

    default:
        qDebug() << "pdu_hash_objData err" << fc;
        break;
    }

    if(pInt) mDataCom->saveData(pInt, data->data, data->len);
    if(pChart) ret = mDataCom->saveData(pChart, data->data, data->len);
    if(pShort) mDataCom->saveData(pShort, data->data, data->len);
    if(ret > 0) obj.size = ret;

    return ret;
}


/**
 * @brief 环境数据的处理
 * @param env
 * @param data
 */
void Net_DevData::envData(sEnvData &env,Net_sDevData *data)
{
    uchar ret=0, *ptr = NULL;

    int fc = data->fn[1] >> 4; // // 处理功能码，第二字节的高四位
    switch (fc)
    {
    case PDU_CMD_TEMP: // 温度
        ret = unitData(env.tem, data);
        break;

    case PDU_CMD_HUM: //湿度
        unitData(env.hum, data);
        break;

    case PDU_CMD_DOOR: //门禁
        ptr = env.door;
        break;

    case PDU_CMD_WATER: //水浸
        ptr = env.water;
        break;

    case PDU_CMD_SMOKE: //烟雾
        ptr = env.smoke;
        break;

    default:
        qDebug() << "pdu_hash_envData err" << fc;
        break;
    }

   if(ptr) mDataCom->saveData(ptr, data->data, data->len);
   if(ret > 0) env.size = ret;
}


/**
 * @brief 保存设备数据 主要针对PduDevData类操作
 * @param dev
 * @param data
 */
void Net_DevData::save(sDevData &dev,Net_sDevData *data)
{
    int fn = data->fn[0]; // 处理功能码第一位数据
    switch (fn)
    {
    case PDU_CMD_LOOP: // 回路数据
        objData(dev.loop, data);
        break;

    case PDU_CMD_LINE: //相电气参数
        objData(dev.line, data);
        break;

    case PDU_CMD_OUTPUT: // 输出位电气参数
        objData(dev.output, data);
        break;

    case PDU_CMD_ENV: //环境数据
        envData(dev.env, data);
        break;

    default:
        qDebug() << "pdu_hashDevData_save err" << fn;
        break;
    }
}

