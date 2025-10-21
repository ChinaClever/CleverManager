/*
 * serialdev.cpp
 *  串口类
 *  对串口的常用操作：打开、读、写、关闭
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "serialdev.h"
#include <QApplication>

SerialDev::SerialDev(QObject *parent) : QThread(parent)
{
    isOpen = false;
    mSerial = NULL;
}

SerialDev::~SerialDev()
{
    close();
}

/**
 * @brief 打开串口
 * @param name 串口名  大写
 * @param baudRate 波特率
 * @return true
 */
bool SerialDev::open(QString &name,qint32 baudRate)
{
    bool ret=false;

    if(!isOpen)
    {
        mSerial = new QSerialPort(name);       //串口号，一定要对应好，大写！！！
        ret = mSerial->open(QIODevice::ReadWrite);      //读写打开
        if(ret)
        {
            mSerial->setBaudRate(baudRate);  //波特率
            mSerial->setDataBits(QSerialPort::Data8); //数据位
            mSerial->setParity(QSerialPort::NoParity);    //无奇偶校验
            mSerial->setStopBits(QSerialPort::OneStop);   //无停止位
            mSerial->setFlowControl(QSerialPort::NoFlowControl);  //无控制
            //     connect(mSerial,SIGNAL(readyRead()),this,SLOT(serialReadSlot()));    //连接槽
            isOpen = true;
        }
        else
            qDebug() << "open Serial:" << name << "Err";
    }

    return ret;
}

QString SerialDev::portName(void)
{
    QString com;

    if(mSerial)
        com =  mSerial->portName();

    return com;
}

/**
 * @brief 串口关闭
 * @return true
 */
bool SerialDev::close(void)
{
    if(isOpen)
    {
        QWriteLocker locker(&mRwLock); // 正在操作时不允许关闭
        isOpen = false;
        mSerial->close();
        delete mSerial;
        mSerial = NULL;
    }

    return true;
}

/**
 * @brief 读串口
 * @param array
 * @return 长度
 */
int SerialDev::read(QByteArray &array)
{
    int len = 0;
    if(!isOpen)
        return len;

    QWriteLocker locker(&mRwLock);
    for(int i=0; i<4; ++i)
    {
        /* 处理所有还没有被处理的各类事件，主要是不用用户感觉到ka */
        // QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        msleep(30);
        serialReadSlot();

        if(mSerialData.size() > len)
            len = mSerialData.size();
        else
            break;
    }

    if(len > 0)
    {
        array = mSerialData;
        mSerialData.clear();
    }

    return len;
}

int SerialDev::write(QByteArray &array)
{
    int len=0;

    if(isOpen)
        len = mSerial->write(array);

    return len;
}

/**
 * @brief 回环测试
 */
void SerialDev::test(void)
{
    QByteArray sentArray,recvArray;

    for(int i=0; i<255;++i)
        sentArray.append(i);
    write(sentArray);

    int ret = read(recvArray);
    if(ret != sentArray.size())
        qDebug() << "Serial test Err:" <<ret;
    else
        qDebug() << "Serial test OK";
}

/**
 * @brief 串口接收糟函数
 */
void SerialDev::serialReadSlot(void)
{
    if(isOpen)
    {
        QByteArray dataTemp;
        while (mSerial->waitForReadyRead(1)); // 等待窗口接收完全
        while (!mSerial->atEnd())
            dataTemp += mSerial->readAll();     //因为串口是不稳定的，也许读到的是部分数据而已，但也可能是全部数据

        mSerialData += dataTemp;
        //emit readyReadSig();
    }
}
