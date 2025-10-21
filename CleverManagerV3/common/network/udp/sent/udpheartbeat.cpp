/*
 * udpheartbeat.cpp
 * UDP 心跳包线程
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "udpheartbeat.h"

#define UDP_HB_PORT     18725 /*UDP心跳端口*/
#define UDP_HEARTBEAT_MSG   "Clever-Manager PDU PC Server OK!" // 心跳包

UdpHeartBeat::UdpHeartBeat(QObject *parent) : QObject(parent)
{
    mSocket = UdpSentSocket::bulid(this);
}



UdpHeartBeat *UdpHeartBeat::bulid(QObject *parent)
{
    static UdpHeartBeat* sington = NULL;
    if(sington == NULL) {
        sington = new UdpHeartBeat(parent);
    }
    return sington;
}


void UdpHeartBeat::sent(const QString &ip)
{
    QByteArray msg = UDP_HEARTBEAT_MSG;
    mSocket->sentData(ip, msg,UDP_HB_PORT);
}


void UdpHeartBeat::sent(void)
{
    QByteArray msg = UDP_HEARTBEAT_MSG;
    mSocket->sentBroadcastData(msg,UDP_HB_PORT);
}



