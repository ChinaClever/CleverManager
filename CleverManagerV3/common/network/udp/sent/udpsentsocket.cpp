/*
 * udpsentsocket.cpp
 * UDP 发送套接字
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "udpsentsocket.h"

UdpSentSocket::UdpSentSocket(QObject *parent) : QObject(parent)
{
    sentLock = new QReadWriteLock();
    mUdpSocket = new QUdpSocket(this);
}


UdpSentSocket::~UdpSentSocket()
{
    delete mUdpSocket;
    delete sentLock;
}


UdpSentSocket *UdpSentSocket::bulid(QObject *parent)
{
    static UdpSentSocket* sington = NULL;
    if(sington == NULL) {
        sington = new UdpSentSocket(parent);
    }
    return sington;
}

bool UdpSentSocket::sentData(const QString &addr, uchar *buf, ushort len, quint16 port)
{
    QByteArray byte;

    for(int i=0; i<len; ++i)
        byte.append(buf[i]);

    return sentData(addr,byte, port);
}

/**
 * @brief 发送点对点数据
 * @param msg 数据内容
 * @return true
 */
bool UdpSentSocket::sentData(const QString &addr, const QByteArray &msg, quint16 port)
{
    bool ret = true;
    QHostAddress host(addr);

    QWriteLocker locker(sentLock);
    int len = mUdpSocket->writeDatagram(msg.data(),msg.size(),host,port); // UDP发送端口18726
    if(len != msg.length())
    {
        ret = false;
        qDebug() << "UdpServer SentData Err" << msg;
    }

    return ret;
}

/**
 * @brief 发送广播数据
 * @param msg 数据内容
 * @return true
 */
bool UdpSentSocket::sentBroadcastData(uchar *buf, ushort len, quint16 port)
{
    QByteArray byte;

    for(int i=0; i<len; ++i)
    {
        byte.append(buf[i]);
    }

    return sentBroadcastData(byte,port);
}



/**
 * @brief 发送广播数据
 * @param msg 数据内容
 * @return true
 */
bool UdpSentSocket::sentBroadcastData(QByteArray &msg, quint16 port)
{
    bool ret = true;

    QWriteLocker locker(sentLock);
    int len = mUdpSocket->writeDatagram(msg.data(),msg.size(),QHostAddress::Broadcast,port);
    if(len != msg.length())
    {
        ret = false;
        qDebug() << "UdpServer SentData Err" << msg;
    }

    return ret;
}
