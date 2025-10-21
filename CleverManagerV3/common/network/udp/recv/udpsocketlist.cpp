/*
 * UdpSocketList.cpp
 * UDP 接收套接字
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "udpsocketlist.h"
#define UDP_RECV_PORT  28720  /* 接收套接字起始端口 */
#define UDP_PORT_SIZE   20 /*UDP监听端口最大个数*/

UdpSocketList::UdpSocketList(QObject *parent) : QThread(parent)
{
    isRun = false;
    mQue = UdpDataQueue::bulid();
    mSocketList = new QList<UdpRecvSocket *>();
    initSocket();
    start(); // 启动线程
}


UdpSocketList::~UdpSocketList()
{
    isRun = false;
    wait();
}


UdpSocketList *UdpSocketList::bulid(QObject *parent)
{
    static UdpSocketList* sington = NULL;
    if(sington == NULL) {
        sington = new UdpSocketList(parent);
    }
    return sington;
}

/**
 * @brief 初始化UDP监听端口号，
 * @param num　创建端口个数
 */
void UdpSocketList::initSocket()
{
    UdpRecvSocket *socket = NULL;

    for(int i=0; i<UDP_PORT_SIZE; ++i)
    {
        socket = new UdpRecvSocket(this);
        socket->initSocket(UDP_RECV_PORT+i);
        mSocketList->append(socket); // 保存套接字
    }
}


/**
 * @brief 读取套接字中的数据
 */
int UdpSocketList::recvData()
{
    int ret = 0;

    for(int i=0; i<mSocketList->size(); ++i)
    {
        mSocket = mSocketList->at(i);
        mData = mSocket->getData();
        if(mData) {
            mQue->udp_data_add(mData);
            ret++;
        }
    }

    return ret;
}



void UdpSocketList::run()
{
    int ret;
    isRun = true;

    while(isRun){
        ret = recvData();
        if(ret <= 0)
            usleep(1);
    }
}




