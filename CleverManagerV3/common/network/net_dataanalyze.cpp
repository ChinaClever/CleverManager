/*
 * netdataanalyze.cpp
 * 解析网络数据包
 *   此对象会创建UDP接收套接字，接收UDP数据，并创建心跳包对象，广播心跳包
 *
 * 完成的功能
 *      1、自动启动线程，读取UDP队列数据
 *      2、解析数据包：先还原网络数据，再解析设备数据
 *      3、把解析出来的设备数据保存至Hash表中
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "net_dataanalyze.h"
#include "websocketclient.h"

NetDataAnalyze::NetDataAnalyze(QObject *parent) : QThread(parent)
{
    isRun = true;
    UdpBDSent::bulid(this);
    UdpHeartBeat::bulid(this); //初始化心跳包
    UdpSentSocket::bulid(this);
    WebSocketClient::bulid(this);

    mDataQue = UdpDataQueue::bulid();
    mSocketList = UdpSocketList::bulid(this); //初始化套接字
    mNetAnalytic = NetAnalyticData::bulid();

    mPacket = new Net_sDataPacket;
    mDevData = new Net_sDevData;
    mPduData = new Net_sPacket;
    mDataSave = Net_DataSave::bulid();
    mRecvBuf = (uchar *)malloc(DATA_MSG_SIZE+10);
    QTimer::singleShot(5,this,SLOT(start())); //延时初始化
}

NetDataAnalyze::~NetDataAnalyze()
{
    isRun = false;
    wait();
}


NetDataAnalyze *NetDataAnalyze::bulid(QObject *parent)
{
    static NetDataAnalyze* sington = NULL;
    if(sington == NULL) {
        sington = new NetDataAnalyze(parent);
    }
    return sington;
}

/**
 * @brief 读取接收到的UDP数据、进行解析、保存至Hash中
 * @return
 */
void NetDataAnalyze::recvData(void)
{
    mUdpData = mDataQue->udp_data_get(); // 获取数据
    if(mUdpData)
    {
        int rtn = mUdpData->datagram.size();
        for(int i=0; i<rtn; ++i)
            mRecvBuf[i] = mUdpData->datagram.at(i);

        rtn = mNetAnalytic->net_data_analytic(mRecvBuf, rtn, mPacket); //网络数据包解包
        if(rtn > 0) // 正常解析数据包
        {
            rtn = mPacket->len;
            if(rtn>0) //数据长度大于0
            {
                rtn = mNetAnalytic->dev_data_analytic(mPacket->data,rtn, mDevData); // 解析设备数据
                if(rtn>0) /*获取到完整的设备数据包*/
                {
                    mIP = mUdpData->addr.toString();
                    mIP.remove("::ffff:"); // IPV4删除这个字符串
                    mPduData->ip = mIP;

                    mPduData->code = &mPacket->code;
                    mPduData->data = mDevData;
                    mDataSave->dataSave(mPduData);  // 进行数据的保存
                }
                // else
                //      qDebug() << "dev data analytic err" << rtn;
            }
            else
                qDebug() << "Net Data len err";
        }
        else
            qDebug() << "Net Data Analyze err" << rtn << mUdpData->addr.toString() <<mUdpData->datagram.size();
    }
}

void NetDataAnalyze::run(void)
{
    while (isRun)
    {
        recvData();

        if(mUdpData) {
            delete mUdpData; // 释放空间
            mUdpData = nullptr;
        } else {
            usleep(1);
        }
    }
}
