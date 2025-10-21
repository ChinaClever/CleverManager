/*
 * UP_TcpClient.cpp
 * TCP升级客户端套接字部分
 * 1、连接服务端接口，当连接成功时，会发送相应的信号，当出现连接异常时，也会发出相应的信号
 * 2、发送信息，所有要发送的信息先保存至链表中，然后通过定时器发送出去。为什么要使用定时器发送，这是秘密
 * 3、接收信息，当有信息接收时，接收到的数据保存至成员变量中，发出接收信息信号，
 * 4、不要即刻关闭连接，有可能数据还没发送出
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "up_tcpclient.h"

UP_TcpClient::UP_TcpClient(QObject *parent) : QThread(parent)
{
    isConnect = false;
    mServerIP = new QHostAddress();

    mTcpSocket = new QTcpSocket(this);
    connect(mTcpSocket, SIGNAL(connected()), this,SLOT(connectedSlot()));
    connect(mTcpSocket, SIGNAL(disconnected()), this,SLOT(disconnectedSlot()));
    connect(mTcpSocket,SIGNAL(readyRead()),this,SLOT(readMessageSlot()));
    connect(mTcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(displayError(QAbstractSocket::SocketError)));

    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    timer->start(20);
}


UP_TcpClient::~UP_TcpClient()
{
    delete mServerIP;

    if(mTcpSocket)
        closeConnect();
}


/**
 * @brief 关闭连接
 */
void UP_TcpClient::closeConnect(void)
{
    if(isConnect)
    {
        isConnect = false;
        mTcpSocket->abort(); //取消已有的连接
    }
}

/**
 * @brief 建立新的连接
 * @param host 目标主机
 * @return true
 */
bool UP_TcpClient::newConnect(const QString &host, int port)
{
    bool ret = mServerIP->setAddress(host);
    if(ret)
    {
        if(isConnect)
            closeConnect(); //先关闭
        mTcpSocket->connectToHost(*mServerIP,port);//连接到主机，机地址和端口号
    }
    else
        qDebug() << "udp newConnect set ip err" << host;

    return ret;
}


bool UP_TcpClient::sentMessage(char *buf,  int len)
{
    QByteArray data;
    data.append(buf, len);
    return sentMessage(data);
}

bool UP_TcpClient::sentMessage(QString &str)
{
    QByteArray data = str.toLatin1();
    return sentMessage(data);
}


bool UP_TcpClient::sentMessage(QByteArray &data)
{
    mListData.append(data);
    return isConnect;
}

void UP_TcpClient::timeoutDone()
{
    if(mListData.size() > 0) {
        QByteArray data = mListData.first();
        writeMessage(data);
        mListData.removeFirst();
    }
}

/**
 * @brief 发送数据
 * @param data
 * @return -1 出错
 */
int UP_TcpClient::writeMessage(QByteArray &data)
{
    int rtn = -1;
    if(isConnect){
        if(mTcpSocket->isWritable())
        {
            rtn = mTcpSocket->write(data);
            if(rtn != data.size()){
                rtn = -1;
            }
            mTcpSocket->waitForBytesWritten();
        }
    }

    return rtn;
}


/**
 * @brief 已连接
 */
void UP_TcpClient::connectedSlot()
{
    isConnect = true;
    emit connectSig(UP_CMD_CONNECT); // 连接成功
}


/**
 * @brief 连接断开
 */
void UP_TcpClient::disconnectedSlot()
{
    closeConnect();
    emit connectSig(UP_CMD_DIS);  // 断开连接
}

/**
 * @brief 接收到数据
 */
void UP_TcpClient::readMessageSlot(void)
{
    if(isConnect)
    {
        while(mTcpSocket->bytesAvailable() > 0)
        {
            QByteArray datagram;
            datagram.resize(mTcpSocket->bytesAvailable());
            mTcpSocket->read(datagram.data(), datagram.size());
            mRecvData = datagram;
            emit connectSig(UP_CMD_READ);
        }
    }
}

/**
 * @brief 出现异常
 */
void UP_TcpClient::displayError(QAbstractSocket::SocketError)
{
    closeConnect();
    emit connectSig(UP_CMD_ERR); // 连接错误
}


int UP_TcpClient::getData(QByteArray &data)
{
    data = mRecvData;
    return mRecvData.size();
}
