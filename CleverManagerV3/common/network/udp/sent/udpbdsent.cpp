/*
 * tcpclient.cpp
 * UDP广播发送数据
 *      1、增加要发送的数据 udp_queue_append()
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "udpbdsent.h"


UdpBDSent::UdpBDSent(QObject *parent) : QObject(parent)
{
    mSocket = UdpSentSocket::bulid(this);
}


UdpBDSent *UdpBDSent::bulid(QObject *parent)
{
    static UdpBDSent* sington = NULL;
    if(sington == NULL) {
        sington = new UdpBDSent(parent);
    }
    return sington;
}

bool UdpBDSent::sent(uchar *buf, ushort len, quint16 port)
{
    return mSocket->sentBroadcastData(buf, len, port);
}

bool UdpBDSent::sent(QByteArray &msg, quint16 port)
{
     return mSocket->sentBroadcastData(msg, port);
}


