/*
 * udpheartbeat.cpp
 * UDP 心跳包线程
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "udpheartbeat.h"
#include "common.h"

#define UDP_HEARTBEAT_MSG   "Clever-Manager PDU PC Server OK!" // 心跳包

UdpHeartBeat::UdpHeartBeat(QObject *parent) : QThread(parent)
{
    isRun=false;
    mSocket = NULL;
}


UdpHeartBeat::~UdpHeartBeat()
{
    isRun=false;
    wait();
}

UdpHeartBeat *UdpHeartBeat::bulid(QObject *parent)
{
    static UdpHeartBeat* sington = nullptr;
    if(sington == nullptr)
        sington = new UdpHeartBeat(parent);
    return sington;
}

/**
 * @brief 启动心跳包线程
 */
void UdpHeartBeat::startSent(void)
{
    mSocket = new UdpSentSocket(this);

    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    timer->start(1500);

    mUdpBDSent = new UdpBDSent(this); // 启动UDP广播数据包
}

void UdpHeartBeat::timeoutDone(void)
{
    //    start(); // 在定时器中启动线程
    run();
}

/**
* @brief 心跳包
*/
void UdpHeartBeat::heartbeatPacket(void)
{
    QByteArray msg = UDP_HEARTBEAT_MSG;

#if (UDP_HB_SENT==0)
    if(mSocket)
        mSocket->sentBroadcastData(msg,UDP_HB_PORT);
#else //发送广播心跳包2018-11-20

#endif
}

void UdpHeartBeat::heartbeatPacket(const QString &ip)
{
#if (UDP_HB_SENT==0)

#else //上海跨网段设备，直接向指定PDU的ip发送心跳包2018-11-20
    if(mSocket) {
        QByteArray msg = UDP_HEARTBEAT_MSG;
        QHostAddress host(ip);
        mSocket->sentData(host, msg,UDP_HB_PORT);
    }
#endif
}

void UdpHeartBeat::run(void)
{
    if(isRun != true) // 在运行就不执行
    {
        isRun = true;
        heartbeatPacket();
        isRun = false;
    }
}

