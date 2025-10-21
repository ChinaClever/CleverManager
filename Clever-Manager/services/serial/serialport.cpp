/*
 * serialport.cpp
 *  串口端口链表
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "serialport.h"

SerialPort::SerialPort(QObject *parent) : QObject(parent)
{
    initSeialPort();
}

SerialPort::~SerialPort()
{

}

/**
 * @brief 端口是否存在
 * @param name
 * @return
 */
bool SerialPort::isContains(QString &name)
{
    return mPortNameList.contains(name);
}

/**
 * @brief 获取端口链表
 * @return
 */
QStringList &SerialPort::portsList(void)
{
    return mPortNameList;
}

void SerialPort::initSeialPort()
{
    QList<QSerialPortInfo>  infos = QSerialPortInfo::availablePorts();
    if(!infos.isEmpty())
    {
        foreach (QSerialPortInfo info, infos) {
            mPortNameList <<  info.portName();
        }
    }
}
