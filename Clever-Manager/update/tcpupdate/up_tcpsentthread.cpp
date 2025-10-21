/*
 * up_tcpsent.cpp
 * TCP升级部分  升级多台设备
 * 1、设置好发送结构体，和要目标IP
 * 2、启动定时器，给依次给设备进行升级
 * 3、给一台设备升级过程中，会发出三个信号，开始信号，验证结果信号，完成信号
 * 4、发送进度信号
 * 5、升级结束后，会发出结束信号，void sentFinshSig(int,int, int);
 * 6、升级失败，会发出异常信号
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "up_tcpsentthread.h"

UP_TcpSentThread::UP_TcpSentThread(QObject *parent) : QThread(parent)
{
    mTcpSent = new UP_TcpSent(this);
    connect(mTcpSent, SIGNAL(connectSig(int)), this, SLOT(connectSlot(int)));
}

void UP_TcpSentThread::startSent(void)
{
    mNum = mIpAddr.size();
    mSucNum = 0;
    mFailNum = 0;
    emit progressSig(0);
    QTimer::singleShot(500,this,SLOT(timeoutDone())); //延时发送
}

/**
 * @brief 升级完成
 */
void UP_TcpSentThread::sentFinsh(void)
{
    int errNum = mIpErr.size();
    if(mFailNum == errNum)
        mSucNum++;
    mFailNum = errNum;
    emit sentFinshSig(mNum, mSucNum, errNum);
    mTcpSent->close();
}

/**
 * @brief 计算进度
 */
void UP_TcpSentThread::progress(void)
{
    int num = mIpAddr.size();
    int p = (((mNum - num)*1.0)/mNum) * 100;
    emit progressSig(p);
}

void UP_TcpSentThread::timeoutDone()
{
    if(mIpAddr.size() > 0) {
        mIP = mIpAddr.first();
        bool ret = mTcpSent->connectServer(mIP);
        if(ret) {
            emit sentSig(UP_ID_START, mIP);
        } else {
            mIpErr.append(mIP);
        }
    } else {
        sentFinsh();
    }
    progress();
}

void UP_TcpSentThread::connectSlot(int step)
{
    switch (step) {
    case UP_CMD_SENT_OK:        
        mIpAddr.removeFirst();
        QTimer::singleShot(500,this,SLOT(timeoutDone()));
        emit sentSig(UP_ID_SENT_OK, mIP); // 发送完成;
        break;

    case UP_CMD_VERIFY:
        emit sentSig(UP_ID_VERIFY, mIP); // 验证正确
        break;

    case UP_CMD_DIS: // 只有当断开、链接错误时，才发出相应信号出来
    case UP_CMD_ERR:
    case UP_CMD_PWDERR:
        sentFinsh();
        emit connectSig(step);
        break;

    default:
        break;
    }
}

