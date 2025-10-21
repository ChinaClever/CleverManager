#ifndef NETDEVDATA_H
#define NETDEVDATA_H

#include "net_datacom.h"
#include "dp_roompackets.h"

enum PDU_CMD_DATA_TYPE{
    PDU_CMD_VALUE =1,

    PDU_CMD_MIN, //最小值
    PDU_CMD_MAX, //最大值
    PDU_CMD_ALARM, //报警

    PDU_CDM_CRMIN,//临界最小值
    PDU_CMD_CRMAX, //临界最大值
    PDU_CMD_CRALARM, //临界报警
    PDU_CMD_NUM_SIZE, //数量
};


enum PDU_CMD_OBJ{
    PDU_CMD_CUR = 1, //电流
    PDU_CMD_VOL, // 电压
    PDU_CMD_POW, // 功率
    PDU_CMD_ELE, // 电能
    PDU_CMD_PF, // 功率因数
    PDU_CMD_SW, // 开关状态 0 表示未启用
    PDU_CMD_CA, // 排碳量
    PDU_CMD_RATE //电压频率
};


enum PDU_CMD_ENV_ENUM{
    PDU_CMD_TEMP = 1, //温度
    PDU_CMD_HUM, // 湿度
    PDU_CMD_DOOR, //门禁
    PDU_CMD_WATER, //水浸
    PDU_CMD_SMOKE, //烟雾
};

// 设备数据主功能码
enum PDU_CMD_ID_DEV_DATA{
    PDU_CMD_STATUS=0,  //设备状态
    PDU_CMD_LOOP, // 回路数据
    PDU_CMD_LINE, //相电参数
    PDU_CMD_OUTPUT,//输出位
    PDU_CMD_ENV //环境数据
};

class Net_DevData
{
    Net_DevData();
public:
    static Net_DevData *bulid();

    void save(sDevData &dev, Net_sDevData *data);

protected:
    int unitData(sDataUnit &unit, Net_sDevData *data);
    int objData(sObjData &obj, Net_sDevData *data);
    void envData(sEnvData &env,Net_sDevData *data);

private:
    Net_DataCom *mDataCom;
};

#endif // NETDEVDATA_H
