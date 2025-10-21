/*
 * tftpthread.cpp
 * tftp上传线程类
 *  1、设置相应的目标IP，及文件
 *  2、启动线程发送文件，void startSent(void);
 *  3、发送过程中，根据实际情况，会发出相应的信号及进度信号
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "tftpthread.h"
#include"pdu/data/packet/pdudatapacket.h"
#include "pdu/data/hash/read/pduhashread.h"

TftpThread::TftpThread(QObject *parent) : QThread(parent)
{
    mTftp = new Tftp(this);
}

TftpThread::~TftpThread()
{
    clear();
    wait();
}

/**
 * @brief 清空发送IP、文件
 */
void TftpThread::clear(void)
{
    mIpAddr.clear();
    mFile.clear();
}

/**
 * @brief 设置需要发送的IP，和文件
 * @param ip
 * @param file
 */
void TftpThread::set(const QString &ip, const QString &file)
{
    clear();
    mIpAddr.append(ip);
    mFile.append(file);
}

/**
 * @brief 设置发送的文件
 * @param file
 */
void TftpThread::setFile(const QString &file)
{
    mFile = file;
}

/**
 * @brief 增加发送的目标地址
 * @param ip
 */
void TftpThread::append(QStringList &ip)
{
    mIpAddr.append(ip);
    mCount = mIpAddr.size();
}

/**
 * @brief 发送文件
 * @return
 */
bool TftpThread::sentFile(void)
{    
    bool ret = false;
    QString ip = mIpAddr.first();
    // PduDataPacket *mDataPacket = pdu_hd_getPacket(ip,0);
    // if(mDataPacket) {
    //  if(!ip.isEmpty() && mDataPacket->offLine > 0)
    if(!ip.isEmpty())
    {
        emit sentSig(1,ip);
        ret = mTftp->upload(mFile, ip);
        if(ret){
            emit sentSig(2, ip);
        } else {
            mErrList << ip;
            emit sentSig(3, ip);
        }
    } else {
        mErrList << ip;
        ret = false;
    }
    // }
    mIpAddr.removeFirst();

    return ret;
}


/**
 * @brief 开始发送
 */
void TftpThread::startSent(void)
{
    if(mIpAddr.size() > 0) {
        mErrList.clear();
        start();
    } else {
        emit sentSig(4, "");
    }
}

/**
 * @brief 计数 100 分制
 */
void TftpThread::sentProgress(void)
{
    int size = mIpAddr.size();
    int x = (((mCount-size) *1.0) / mCount) *100;
    emit progressSig(x);
}

void TftpThread::run(void)
{
    while (mIpAddr.size() > 0) {
        sentFile();
        sentProgress();
        msleep(500);
    }
    sentFinsh();
}

int TftpThread::getErrList(QStringList &errList)
{
    errList = mErrList;
    return mErrList.size();
}

void TftpThread::sentFinsh(void)
{
    int num = mCount - mErrList.size(); // 发送成功的个数，
    emit sentFinshSig(mCount, num, mErrList.size());
}
