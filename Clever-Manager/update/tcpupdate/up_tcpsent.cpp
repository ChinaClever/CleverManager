/*
 * up_tcpsent.cpp
 * TCP升级发送部分  升级某一台设备
 * 1、首先设置发送结构体
 * 2、连接目标IP，连接成功就回收到连接成功的信息，开始发送升级文件
 * 3、升级文件发送：首先发送文件长度，然后发送账号和密码，最后以1024字节的长度发送数据
 * 4、发送完成，升级文件发送成功，就会发出成功信号
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "up_tcpsent.h"

UP_TcpSent::UP_TcpSent(QObject *parent) : QThread(parent)
{
    mTcpClient = new UP_TcpClient(this);
    connect(mTcpClient,SIGNAL(connectSig(int)), this,SLOT(connectSlot(int)));
}

UP_TcpSent::~UP_TcpSent()
{
}


/**
 * @brief 设置发送信息
 * @param tcpStr
 */
void UP_TcpSent::set(sTcpUpdateStr &tcpStr)
{
    mTcpUpdateStr = tcpStr;
}

/**
 * @brief 读文件所有内容
 * @param file
 */
bool UP_TcpSent::readFile(const QString &fn)
{
    QFile file(fn);
    bool ret = file.open(QIODevice::ReadOnly);
    if(ret){
        mByFile = file.readAll();
        file.close();
    }
    else
        qDebug() << " UP_TcpSent readFile err";

    return ret;
}

/**
 * @brief 发送文件长度
 * @return
 */
bool UP_TcpSent::sentLen(void)
{
    bool ret =  readFile(mTcpUpdateStr.file);
    if(ret) {
        char buf[32] = {0};
        sprintf(buf,"%d",mByFile.length());
        ret = mTcpClient->sentMessage(buf,strlen(buf)); // 发送文件长度
    }
    return ret;
}

/**
 * @brief 开始发送
 * @return
 */
bool UP_TcpSent::startSent(void)
{
    bool ret = sentLen(); // 发送文件长度
    if(ret) {
        ret =  mTcpClient->sentMessage(mTcpUpdateStr.usr);
        ret =  mTcpClient->sentMessage(mTcpUpdateStr.pwd); // 发送用户名信息
        QTimer::singleShot(1000,this,SLOT(timeoutDone()));
    }
    return ret;
}

/**
 * @brief 接收验证信息
 * @return
 */
bool UP_TcpSent::recvVerify(void)
{
    bool ret = false;
    QByteArray data;
    int rtn = mTcpClient->getData(data); // 接收回应信息
    if(rtn > 0) {
        QString str;
        str.append(data);
        if(str == "err"){ // 验证错误
            isStart = isRun = false;
            emit connectSig(UP_CMD_PWDERR); // 账号错误
        }  else {
            ret = true;
             emit connectSig(UP_CMD_VERIFY); // 验证成功
        }
    }

    return ret;
}

/**
 * @brief 发送数据
 * @return
 */
bool UP_TcpSent::sentData(void)
{
    bool ret = true;
    while(mByFile.size() > 0)
    {
        int len = mByFile.size();
        if(len > 1024) // 一次只发送1024个数据
            len = 1024;

        QByteArray data;
        for(int i=0; i<len; ++i)
            data.append(mByFile.at(i));

        mByFile.remove(0, len);
        len = mTcpClient->sentMessage(data);
        if(len <= 0) {
            ret = false;
            break;
        }
    }
    isStart = false; // 发送完成，标志为复位

    return ret;
}

bool UP_TcpSent::connectServer(const QString &ip)
{
    if(!ip.isEmpty()) {
        isStart = isRun = true; // 开始运行
       return mTcpClient->newConnect(ip);
    }

    return false;
}

/**
 * @brief 连接响应函数
 * @param step 1
 */
void UP_TcpSent::connectSlot(int step)
{
    switch (step) {
    case UP_CMD_CONNECT: // 连接成功 首先发送文件长度
        startSent();
        emit connectSig(UP_CMD_CONNECT); // 账号错误
        break;

    case UP_CMD_READ: // 读取验证信息，
         if(recvVerify()) {
            sentData();
         }
        break;

    default:
        if(isStart) { // 没发送完数据时，异常情况抛出
            mByFile.clear();
            emit connectSig(step);
        }
        break;
    }
}

void UP_TcpSent::timeoutDone()
{    
    if(isRun) {
        if(mTcpClient->getSentState()) {
            isRun = false;
            emit connectSig(UP_CMD_SENT_OK); // 发送完成
        }  else {
            QTimer::singleShot(200,this,SLOT(timeoutDone()));
        }
    }
}
