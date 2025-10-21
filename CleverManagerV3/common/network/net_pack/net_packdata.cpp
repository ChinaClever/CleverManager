/*
 * netpackdata.cpp
 * 把结构体数据打包成网络数据包
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "net_packdata.h"


NetPackData *NetPackData::bulid()
{
    static NetPackData* sington = NULL;
    if(sington == NULL) {
        sington = new NetPackData();
    }
    return sington;
}


/**
 * 功  能：数据加入包头、结束符
 * 入口参数：pkt->包结构休
 * 返回值：
 */
void NetPackData::data_packet_head(Net_sDataPacket *pkt)
{
    pkt->hdr = DATA_MSG_HDR;	/*信息头*/
    pkt->stx = DATA_MSG_STX; /*标识字*/

    pkt->ED = DATA_MSG_ED; /*结束符*/
}

/**
 * 功能：填入代号段数据
 * 入口参数：msg->包结构休
 * 出口参数：buf->数据包首地址
 * 返回值：数据包长度
 */
int NetPackData::data_packet_code(Net_sDevCode *msg, uchar *buf)
{
    ushort i,rtn=DATA_MSG_CODE_SIZE; /*代码段字节数为9*/
    uchar *ptr = buf;

    /*填入代号段*/
    for(i=0; i<DEV_CODE_SIZE; ++i)
        *(ptr++) = msg->devCode[i];	/*设备号*/

    *(ptr++) = msg->type;	/*通讯类型*/
    *(ptr++) = msg->version; /*版本号*/
    *(ptr++) = msg->trans; /*传输方向*/

    msg->reserve = 0;
    memcpy(ptr, &(msg->reserve),sizeof(ushort)); /*预留号*/

    return rtn;
}

/**
 * 功能：填入数据域
 * 入口参数：msg->包结构休
 * 出口参数：buf->数据包首地址
 * 返回值：数据包长度
 */
int NetPackData::data_packet_domain(Net_sDataPacket *msg, uchar *buf)
{
    ushort i,rtn=0;
    uchar *ptr = buf;

    /*填入数据长度*/
    *(ptr++) = ((msg->len) >> 8); /*长度高8位*/
    *(ptr++) = (0xff)&(msg->len); /*低8位*/

    for(i=0; i<msg->len; ++i)
        *(ptr++) = msg->data[i];

    msg->XOR = NetAnalyticData::bulid()->net_data_xor(msg->data, msg->len);	/*生成校验码*/
    *(ptr++) = msg->XOR;

    rtn = 2+ msg->len + 1;
    return rtn;
}


/**
 * 功能：数据打包
 * 入口参数：pkt->包结构休
 * 出口参数：buf->数据包首地址
 * 返回值：数据包长度
 * 说明：当要发送数据时就会调用此函数把数据打包
 */
int NetPackData::data_msg_packet(Net_sDataPacket *pkt, uchar *buf)
{
    ushort rtn=0;
    uchar *ptr = buf;

    data_packet_head(pkt); /*填写包头、包尾*/
    *(ptr++) = pkt->hdr;  /*信息头*/
    *(ptr++) = pkt->stx; /*标识字*/

    ptr += data_packet_code(&(pkt->code),ptr); /*填入代号段数据*/
    ptr += data_packet_domain(pkt, ptr); /*填入数据*/

    *(ptr++) = pkt->ED; /*结束符*/
    rtn = 2 + DATA_MSG_CODE_SIZE + 2 + pkt->len + 1 + 1;

    return rtn;
}



/**
 * 功能：设备数据打包
 * 入口参数：pkt->包结构休
 * 出口参数：buf->数据包首地址
 * 返回值：数据包长度
 * 说明：当要发送数据时就会调用此函数把数据打包
 */
int NetPackData::dev_data_pack(Net_sDevData *pkt, uchar *buf)
{
    ushort i,rtn=0;
    uchar *ptr = buf;

    *(ptr++) = pkt->addr; 	/*设备号*/

    for(int i=0; i<DEV_FN_SIZE; ++i) /*功能码*/
        *(ptr++) = pkt->fn[i];

    /*数据长度*/
    *(ptr++) = (pkt->len >> 8); /*高八位*/
    *(ptr++) = (pkt->len) & 0xFF; /*低八位*/

    if(pkt->len < DATA_MSG_SIZE)
    {
        for(i=0; i<pkt->len; ++i)
            *(ptr++) = pkt->data[i];
        rtn = 1 + DEV_FN_SIZE + 2 + pkt->len;
    } else
        rtn = 0;

    return rtn; /*数据包长度*/
}



/**
 * @brief 整形数据转化为字符符数据
 * @param form 整形数据
 * @param to 字符缓冲区
 * @return
 */
int NetPackData::int_to_uchar(int form, uchar *to)
{
    int offset=0;

    to[offset++] = form >> 24; // 高位
    to[offset++] = (form >> 16) & 0xFF;
    to[offset++] = (form >> 8) & 0xFF;
    to[offset++] = form & 0xFF; // 低8位

    return offset;
}


/**
 * @brief 获取设备代号段
 * @param num 设备类型
 * @param type 通讯类型
 * @param code 设备代号段指针
 */
Net_sDevCode *NetPackData::get_dev_code(int num, uchar type)
{
    static Net_sDevCode *code = NULL;
    if(code == NULL)
        code = (Net_sDevCode*)malloc(sizeof(Net_sDevCode));
    memset(code, 0, sizeof(Net_sDevCode));

    int_to_uchar(num, code->devCode);
    code->type = type;
    code->version = DATA_DEV_VERSION;
    code->trans = DATA_MSG_SERVICE; // 服务端标志
    code->reserve = 0; // 预留位

    return code;
}


/**
 * @brief  获取网络数据包指针
 * @param code 代号段
 * @param buf 缓冲区
 * @param len 数据长度
 * @return 网络数据包指针
 */
Net_sDataPacket *NetPackData::get_data_packet(Net_sDevCode *code, uchar *buf, ushort len)
{
    static Net_sDataPacket *msg = NULL;
    if(msg == NULL)
        msg = (Net_sDataPacket*)malloc(sizeof(Net_sDataPacket));

    memset(msg, 0, sizeof(Net_sDataPacket));
    msg->code = *code; /*代号段*/
    msg->len = len; /*数据长度 */
    msg->data = buf;	/*数据段*/

    return msg;
}


/**
 * @brief 网络数据打包
 * @param num 设备代号
 * @param type 通讯类型
 * @param pkt 设备结构体
 * @param buf 接收缓冲区
 * @return  数据长度
 */
int NetPackData::net_data_packets(int num, Net_sDevData *pkt, uchar *buf,uchar type)
{
    uchar *sentBuf = mSentBuf;
    QWriteLocker locker(&lock); //上锁

    //PDU_TYPE_M_PDU  0x01020101 type: UDP:1 TCP:2   MPDU UDP全局设置时 地址为0x00 2019/07/24 by lzy
    if((num == 0x01020101) && (type == 1)) {
        if(pkt->addr == 0xff)   pkt->addr = 0x00;
    }

    memset(sentBuf,0,DATA_MSG_SIZE);
    ushort len = dev_data_pack(pkt, sentBuf);

    Net_sDevCode *code = get_dev_code(num, type);
    Net_sDataPacket *msg = get_data_packet(code, sentBuf, len);

    return data_msg_packet(msg, buf);
}


