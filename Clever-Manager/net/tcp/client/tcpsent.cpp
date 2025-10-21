/*
 * tcpclient.cpp
 * Tcp发送类 主要用于TCP数据发送
 *      1、建立连接 newConnect()
 *      2、增加要发送的数据 tcp_queue_append()
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "tcpsent.h"
#include "zebra/zebra_client.h"
#include "snmp/snmpsetting.h"
#include "msgBox/msgbox.h"

static QReadWriteLock  *gLock = new QReadWriteLock();
static QQueue<QByteArray> *gArrayQue = new QQueue<QByteArray>; // 需要发送的数据都会保存至此队列中

/**
 * @brief 往列队中增加数据
 * @param data
 */
void tcp_queue_append(QByteArray &data)
{
    QWriteLocker locker(gLock);
    gArrayQue->enqueue(data);
}

/**
 * @brief 增加数据
 * @param buf 缓冲区
 * @param len 数据长度
 */
void tcp_queue_append(uchar *buf, int len)
{
    QByteArray data;
    data.append((char *)buf, len);
    tcp_queue_append(data);
}

void Delay_MSec(unsigned int msec)
{
    QTime _Timer = QTime::currentTime().addMSecs(msec);

    while( QTime::currentTime() < _Timer )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

bool get_tcp_connect(const QString &ip)
{
    bool ret = get_tcp_connect();
    if(!ret) {
        TcpSent::bulid()->connectCheck(ip);
        Delay_MSec(400);
    }

    ////2019-3-12 lzy gArrayQue大小为0，则为命令已将发送出去
    ret = get_tcp_connect();
    if(!ret) {if(gArrayQue->size() == 0) ret = true;}

    return  ret;
}


TcpSent::TcpSent(QObject *parent) : QObject(parent)
{
    mTcpClient = new TcpClient(this);

    mLock = gLock;
    mArrayQue = gArrayQue;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    timer->start(200);
}

TcpSent *TcpSent::bulid(QObject *parent)
{
    static TcpSent* sington = NULL;
    if(sington == NULL) {
        sington = new TcpSent(parent);
    }
    return sington;
}

bool TcpSent::onceSend(const QString &ip, uchar *buf, int len)
{
    bool ret = mTcpClient->newConnect(ip);
    if(ret) {
        QByteArray data((char *)buf, len);
        sentData(ip, data);
    } else {
        qDebug() << "TcpSent::onceSend err";
        // CriticalMsgBox box(0, tr("命令执行错误！"));
    }

    if(timer->isActive()) {
        timer->stop();
        QTimer::singleShot(150,this,SLOT(startTimerSlot()));
    }
    mHost = ip;

    return ret;
}

void TcpSent::startTimerSlot()
{
    //    mTcpClient->closeConnect();
    timer->start(800);
}

/**
 * @brief 连接服务器
 * @param host
 * @return
 */
void TcpSent::newConnect(const QString &host)
{
    if(host != mHost) {
        mHost = host;
        QWriteLocker locker(mLock);
        mArrayQue->clear();
    }

    mTcpClient->newConnect(mHost);
}

bool TcpSent::connectCheck(const QString &host)
{
    if(host != mHost)  mHost = host;
    mTcpClient->newConnect(mHost);

    return true;
}

/**
 * @brief 自动连接目标主机
 */
bool TcpSent::newConnect(void)
{
    bool ret = get_tcp_connect();
    if(ret == false)
    {
        if(!mHost.isEmpty())
        {
            mTcpClient->newConnect(mHost);
            //msleep(500); // 等待连接
        }
    }

    return get_tcp_connect();
}

/**
 * @brief 发送数据进行检查
 * @return true 可以发送数据
 */
bool TcpSent::sentCheck(void)
{
    bool ret = newConnect();
    if(ret)
        ret = mTcpClient->getSentStatue();
    return ret;
}

/**
 * @brief 发送数据
 */
void TcpSent::sentData(const QString &ip, QByteArray &data)
{
#if defined(ZEBRA_MODULE) || defined(SNMP_MODULE)
    int channel = zebra_client::get_instance()->get_send_cmd_channel(ip);
    if((3 == channel) ||  (2 == channel))  //   2 zebra+tcp  3 zebra
    {
        uchar *buf = (uchar *)data.data();
        zebra_client::get_instance()->send_new_protocol_data(ip ,buf,  data.size());
        if(3 == channel) return;
    }
#endif
    QWriteLocker locker(mLock);
    mTcpClient->sentMessage(data);
}

/**
 * @brief 定时发送数据
 */
void TcpSent::timeoutDone()
{
    static int count = 0;
    if(mArrayQue->size())
    {
        if(sentCheck()) {
            QByteArray data = mArrayQue->dequeue();
            sentData(mHost, data);
            count = 5;
        }
    } else {
        bool ret = get_tcp_connect();
        if(ret){
            if(count > 0) count--;
            else mTcpClient->closeConnect();
        }
    }
}

/**
 * @brief 发送测试数据 测试用
 */
void tcp_sent_test(void)
{
    net_dev_data pkt;
    uchar lzy[512] = {0};

    pkt.addr = 0;
    pkt.fn[0] = 0;
    pkt.fn[1] = 0;
    pkt.len = 10;
    pkt.data = (uchar *)"luozhiyong";

    int ret = net_data_packets(0x01010101, 1, &pkt, lzy);

    tcp_queue_append(lzy, ret);
}
