/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "net_datacom.h"


Net_DataCom *Net_DataCom::bulid()
{
    static Net_DataCom* sington = NULL;
    if(sington == NULL) {
        sington = new Net_DataCom();
    }
    return sington;
}


/**
 * @brief 把uchar型数组中的设备类型号 转化成int数据
 * @param prt
 * @return
 */
int Net_DataCom::devCode(uchar *prt)
{
    int devCode = 0;
    for(int i=0; i<DEV_CODE_SIZE; ++i)
    {
        devCode <<= 8;
        devCode += prt[i];
    }

    return devCode;
}

/**
 * @brief 网络数据包验证
 * @param type  传输类型 必须是UDP模式
 * @param trans 传输方面 必须来源于客户端口
 * @return
 */
bool Net_DataCom::check(uchar type, uchar trans)
{
    bool ret = false;

    if(type == NET_UDP_TYPE) //必须是UDP模式
    {
        if(trans == DATA_MSG_CLIENT) //必须来源于客户端
            ret = true;
        else
            qDebug() << "net udp data not form client";
    }
    else
        qDebug() << "net transfer type not udp";

    return ret;
}


/**
 * @brief 字符符转为整形
 * @param dest 整形缓冲区
 * @param len  数据长度
 * @param buf
 * @param size 几位表示一个整数
 * @return
 */
int Net_DataCom::uchar_to_int(int *dest, ushort len, uchar *buf, int size)
{
    int i=0,j=0,k=0,temp=0;

    if(len%size == 0) //必须是偶数
    {
        for(i=0; i<len/size; ++i)
        {
            temp = 0;
            for(j=0; j<size; ++j)
            {
                temp <<= 8;
                temp += buf[k++];
            }
            dest[i] = temp;
        }
    }
    else
    {
        qDebug() << "char_to_int err" << len;
        len = 0;
    }

    return len/size;
}

/**
 * @brief 把数据以Int保存至对应的数据域
 * @param ptr 数据指针
 * @param len ovr
 * @param data
 * @param sizeBit
 */
int Net_DataCom::saveData(uint *ptr, uchar *data, ushort len)
{
    int *buf = mBuf;
    int rtn = uchar_to_int(buf, len, data, 4); // 数据转化
    if(rtn > 0)
    {
        for(int i=0; i<rtn; ++i)
            ptr[i] = buf[i]; //数据保存
    }
    return rtn;
}

int Net_DataCom::saveData(ushort *ptr, uchar *data, ushort len)
{
    int *buf = mBuf;
    int rtn = uchar_to_int(buf, len, data, 2); // 数据转化
    if(rtn > 0)
    {
        for(int i=0; i<rtn; ++i)
            ptr[i] = buf[i]; //数据保存
    }

    return rtn;
}

int Net_DataCom::saveData(uchar *ptr, uchar *data, ushort len)
{
    for(ushort i=0; i<len; ++i)
        ptr[i] = data[i];
    return len;
}


bool Net_DataCom::toString(QString &str, uchar *data, ushort len)
{
    char *strBuf = mStrBuf;

    memset(strBuf,0, 512);
    for(int i=0; i<len; ++i)
        strBuf[i] = data[i];
    str = strBuf;

    return true;
}



/**
 * @brief 保存字符串数据
 * @param name
 * @param data
 */
bool Net_DataCom::saveStr(QString &str, Net_sDevData *data)
{
    return toString(str, data->data, data->len);
}
