/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "net_datasave.h"


#define STRING_SPLIT "; " //字符串分割符

enum PDU_CMD_DEV{
    PDU_CMD_DEV_NAME = 1, // 设备名称
    PDU_CMD_DEVINFO = 5, //设备信息 主功能码为5
    PDU_CMD_DEVUSR = 6,
    PDU_CMD_DEVNET = 7,
    PDU_CMD_OUTPUT_NAME = 10
};

// IP地址结构体
enum PDU_CMD_DEV_NET_IP{
    PDU_CMD_NET_IP=1,
    PDU_CMD_NET_GW,
    PDU_CMD_NET_MASK,
    PDU_CMD_NET_DNS,
    PDU_CMD_NET_DNS2,
    PDU_CMD_NET_MODE,
};



Net_DataSave::Net_DataSave()
{
    mDevData = Net_DevData::bulid();
    mDataCom = Net_DataCom::bulid();
//    mPduPackets = Dp_PduPackets::bulid();
}

Net_DataSave *Net_DataSave::bulid()
{
    static Net_DataSave* sington = NULL;
    if(sington == NULL) {
        sington = new Net_DataSave();
    }
    return sington;
}

/**
 * @brief 设置输出位的名称
 * @param name
 * @param data
 */
void Net_DataSave::outputName(QString *name, Net_sDevData *data)
{
    int output = data->fn[1];
    if((output >= 0) && (output < 64))    // 输出位最大32位
    {
        QString str;
        mDataCom->saveStr(str,data);
        name[output] = str; // 设置输出位的名称
    }
    else
        qDebug() << "pdu_output_name err";

}


/**
 * @brief 设备用户名、密码
 * @param usrHash
 * @param data
 */
void Net_DataSave::user(QString &user, QString &pwd, Net_sDevData *data)
{
    QString str;
    mDataCom->saveStr(str,data);
    QStringList strlist = str.split(STRING_SPLIT,QString::SkipEmptyParts);
    if(strlist.size() == 2) {
        user = strlist.at(0);
        pwd = strlist.at(1);
    } else
        qDebug() << "pdu_usrName_save err" << str;
}



/**
 * @brief 设备网络地址保存
 * @param ip
 * @param data
 */
void Net_DataSave::iPAddr(sIpAddr &ip,Net_sDevData *data)
{
    int fc = data->fn[1] & 0x0f; // 获取高四位
    switch (fc) {
    case PDU_CMD_NET_IP:  mDataCom->saveStr(ip.ip,data);  break;
    case PDU_CMD_NET_GW: mDataCom->saveStr(ip.gw,data); break;
    case PDU_CMD_NET_MASK: mDataCom->saveStr(ip.mask,data);  break;
    case PDU_CMD_NET_DNS: mDataCom->saveStr(ip.dns,data);  break;
    case PDU_CMD_NET_DNS2: mDataCom->saveStr(ip.dns2, data); break;
    case PDU_CMD_NET_MODE:  ip.mode = data->data[0]; break;
    }
}




/**
 * @brief 设备信息保存
 * @param info
 * @param data
 */
void Net_DataSave::devName(QString &name,Net_sDevData *data)
{
    int fc = data->fn[1] & 0x0f;
    switch (fc)
    {
    case PDU_CMD_DEV_NAME: // 设备工作名称
        mDataCom->saveStr(name,data);
        break;
    }
}



/**
 * @brief pdu_hashData_function
 * @param dev
 * @param data
 */
void Net_DataSave::dataFun(sDataPacket *dev,Net_sDevData *data)
{
    int fc = data->fn[0]; //根据功能码，进行分支处理

    switch (fc)
    {
    case PDU_CMD_STATUS: //设备工作状态
        dev->devSpec = data->fn[1]; // 设备类型
        break;

    case PDU_CMD_LOOP:
    case PDU_CMD_LINE: // 设备相参数
    case PDU_CMD_OUTPUT: // 设备输出位
    case PDU_CMD_ENV: //环境数据
        mDevData->save(dev->data, data);
        break;

    case PDU_CMD_DEVINFO: // 设备信息
        devName(dev->name, data);
        break;

    case PDU_CMD_DEVUSR: // 设备用户信息
        user(dev->user, dev->pwd, data);
        break;

    case PDU_CMD_DEVNET: // 设备网络信息
        iPAddr(dev->net,data);
        break;

    case PDU_CMD_OUTPUT_NAME: // 输出位名称
        outputName(dev->data.output.name, data);
        break;

    default:
        //qDebug() << "pdu_hashData_function err" << fc;
        break;
    }
}


/**
 * @brief Hash数据保存的入口函数
 *  主要是针对代号段的处理，pdu_dev_code
 *      对数据进行网络传输类型、传输方向、及版本的验证;
 *      根据IP、代号段中的设备类型、设备号来查找对应的设备数据节点
 * @param packet
 */
void Net_DataSave::dataSave(Net_sPacket *packet)
{
    static Dp_PduPackets *ps = Dp_PduPackets::bulid();
    bool ret = mDataCom->check(packet->code->type, packet->code->trans);   /*网络传输类型、传输方向验证*/
    if(ret)
    {
        int devType = mDataCom->devCode(packet->code->devCode); // 获取设备类型码
        if(devType > 0)
        {
            uchar addr = packet->data->addr;
            sDataPacket *dev = ps->get(packet->ip, addr);
            if(packet->code->version == NET_DATA_VERSION) //版本号的验证
            {
                dev->devType = devType; //设备型号
                dev->id = packet->data->addr; // 设备地址
                dev->net.ip = packet->ip; //设备IP
                dev->txType = NET_UDP_TYPE;
                dev->offLine = PDU_OFF_LINE_TIME; // + get_pdu_lineNum(); //设备在线标识
                if(dev->offLine > 10*60) dev->offLine = 600;

                dataFun(dev, packet->data); //功能预处理

            } else qDebug() << "NET DATA VERSION err";
        } else qDebug() << "get pdu dev Code err";
    } else  qDebug() << "pdu netData check err";
}
