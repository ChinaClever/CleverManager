#ifndef NET_DATACOM_H
#define NET_DATACOM_H

#include "net_pack/net_packdata.h"

#define NET_DATA_VERSION	1 /*IP设备通讯类型*/
#define PDU_OFF_LINE_TIME   10

typedef enum {
    PDU_CMD_1=1,
    PDU_CMD_2,
    PDU_CMD_3,
    PDU_CMD_4,
    PDU_CMD_5,
    PDU_CMD_6,
    PDU_CMD_7,
    PDU_CMD_8,
    PDU_CMD_9,
    PDU_CMD_10,
    PDU_CMD_11,
    PDU_CMD_12,
    PDU_CMD_13,
    PDU_CMD_14,
    PDU_CMD_15,
    PDU_CMD_16,
}PDU_CMD_NUM;

/*网络通讯类型*/
typedef enum {
    NET_UDP_TYPE=1, //UDP通讯类型
    NET_TCP_TYPE, //TCP通讯类型
}NET_TRA_TYPE;


class Net_DataCom
{
public:
    static Net_DataCom *bulid();
    int devCode(uchar *prt);
    bool check(uchar type, uchar trans);

    int saveData(uint *ptr, uchar *data, ushort len);
    int saveData(uchar *ptr, uchar *data, ushort len);
    int saveData(ushort *ptr, uchar *data, ushort len);

    int uchar_to_int(int *dest, ushort len, uchar *buf, int size);
    bool toString(QString &str, uchar *data, ushort len);
    bool saveStr(QString &str, Net_sDevData *data);

private:
    int mBuf[512];
    char mStrBuf[512];
};



#endif // NET_DATACOM_H
