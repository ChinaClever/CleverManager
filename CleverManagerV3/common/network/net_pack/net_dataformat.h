/*
 * 网络数据格式结构体
 * 按照网络传输数据格式，定义结构体
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#ifndef NETDATAFORMAT_H
#define NETDATAFORMAT_H

#include <QtCore>

#define DATA_MSG_SIZE	512		/*数据缓冲区大小*/
#define DEV_CODE_SIZE	4		/*设备代号位数*/
#define DEV_FN_SIZE     2       /*功能码长度*/
#define DATA_MSG_CODE_SIZE	9  // 结构体大小 9个字节

#define DATA_MSG_HDR	0x7E	/*信息头~*/
#define DATA_MSG_STX	0x5E	/*标识字^*/
#define DATA_MSG_ED	0x23	/*结束符*/

#define DATA_MSG_SERVICE	0x10	/*服务端标志*/
#define DATA_MSG_CLIENT		0x03	/*客户端标志*/

#define DATA_DEV_VERSION		1 /*设备通讯类型*/

/*定义通讯类型*/
enum net_data_traffic_type{
    TRA_TYPR_UDP=1, /*UDP通讯*/
    TRA_TYPR_TCP,
    //===    请在这里增加所需要的通讯类型
};


/*错误码定义*/
enum net_data_return_err{
    DATA_ERR_ABNORMAL=-1, /*数据不完整错误*/
    DATA_ERR_HEAD=-2, /*同步头错误*/
    DATA_ERR_END=-3, /*结束符错误*/
    DATA_ERR_XOR=-4,/*校验码错误*/
    DATA_ERR_LEN=-5,
};


/*代号段结构体*/
struct Net_sDevCode
{
    uchar devCode[DEV_CODE_SIZE]; /*设备代号*/
    uchar type; /*通讯类型*/
    uchar version; /*版本号*/
    uchar trans;	/*服务端发送标志：10H 客户端应答标志：03H */
    ushort reserve; /*预留2字节*/
};


/**
 * 在网络中传输 数据库包
 */
struct Net_sDevData
{
    uchar addr;	/*地址*/
    uchar fn[DEV_FN_SIZE];	/*功能码*/
    ushort len; /*数据长度*/
    uchar *data; /*数据*/
};

/**
 * 数据包结构体，
 */
struct Net_sDataPacket
{
    uchar hdr;	/*信息头 0x7E*/
    uchar stx;	/*标识字0x5E*/
    Net_sDevCode code; /*代号段*/
    ushort len; /*数据长度 */
    uchar *data;	/*数据段*/
    uchar XOR; /*校验码*/
    uchar ED; /*结束符*/
};


struct Net_sPacket
{
    QString ip;	/*地址*/
    Net_sDevCode *code; //设备代号
    Net_sDevData *data; //设备数据
};

#endif // NETDATAFORMAT_H
