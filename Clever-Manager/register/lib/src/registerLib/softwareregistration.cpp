<<<<<<< HEAD
﻿/**
 * 思路：
 * 1、保存activeCode到HKEY_CURRENT_USER\\Software\\qCleverManager\\activeCode中
 * 2、程序每次打开时，先读取 注册表HKEY_CURRENT_USER\\Software\\qCleverManager\\activeCode，
 * 3、如果为空，则提示需要激活，如果存在，进行下一步：
 * 4、获取机器信息getMachineInfo(),计算calMachineCode，
 * 5、计算激活码calActiveCode与activeCode进行对比，一致说明已激活，否则提示激活。
 * 6、有效期限制：首次激活时，validRecord到HKEY_CURRENT_USER\\Software\\qCloudSecChk\\validRecordTime
 * 7、程序每次打开，确认已激活后，打开上述位置，读取第一次激活时间，
 * 8、与当前时间对比，根据激活码匹配情况（120，180，360，730），选择对应时间阈，提示软件是否过期，是否需要重新注册。
 * 9、另外不能单纯依赖注册表，还应加上一些隐藏文件的判断等
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "softwareregistration.h"
#include <QObject>
#include <QSettings>
#include <QDebug>
#include <QMessageBox>
#include <QAxObject>
#include <windows.h>
#include <stdio.h>
#include <QUuid>
#include <tchar.h>
#include <QFile>
#include <QNetworkInterface>
#include <QCryptographicHash>
#include <QClipboard>
#include <QApplication>

SoftwareRegiste::SoftwareRegiste()
{
}

/**
 * @brief 通过WMI接口获取硬件信息，但这些信息有些未必是唯一的（不同机器的数据可能类似)
 * @param type
 *      1.当前原生网卡地址：
 *      2.硬盘序列号
 *      3.获取主板序列号
 *      4.处理器ID)
 *      5.BIOS序列号
 *      6.主板型号
 * @return
 */
QString SoftwareRegiste::getWMIHWInfo(int type)
{
    QString hwInfo=("");
    QStringList sqlCmd;
    sqlCmd.clear();
    sqlCmd<<("SELECT MACAddress  FROM Win32_NetworkAdapter WHERE (MACAddress IS NOT NULL) AND (NOT (PNPDeviceID LIKE 'ROOT%'))");
    //注意qt调用 wmi时，对查询语句要求很严格，所以 like之类的句子务必精确才能有结果出来
    sqlCmd<<("SELECT PNPDeviceID  FROM  Win32_DiskDrive WHERE( PNPDeviceID IS NOT NULL) AND (MediaType LIKE 'Fixed%')");
    sqlCmd<<("SELECT SerialNumber FROM Win32_BaseBoard WHERE (SerialNumber IS NOT NULL)");
    sqlCmd<<("SELECT ProcessorId  FROM Win32_Processor WHERE (ProcessorId IS NOT NULL)");
    sqlCmd<<("SELECT SerialNumber  FROM Win32_BIOS WHERE (SerialNumber IS NOT NULL)");
    sqlCmd<<("SELECT Product FROM Win32_BaseBoard WHERE (Product IS NOT NULL)");

    QStringList columnName;
    columnName.clear();
    columnName<<("MACAddress");
    columnName<<("PNPDeviceID");
    columnName<<("SerialNumber");
    columnName<<("ProcessorId");
    columnName<<("SerialNumber");
    columnName<<("Product");

    QAxObject *objIWbemLocator = new QAxObject("WbemScripting.SWbemLocator");
    QAxObject *objWMIService = objIWbemLocator->querySubObject("ConnectServer(QString&,QString&)",QString("."),QString("root\\cimv2"));
    QString query=("");
    if(type<sqlCmd.size())
        query=sqlCmd.at(type);

    QAxObject *objInterList = objWMIService->querySubObject("ExecQuery(QString&))", query);
    QAxObject *enum1 = objInterList->querySubObject("_NewEnum");
    //需要 include windows.h
    IEnumVARIANT* enumInterface = 0;
    enum1->queryInterface(IID_IEnumVARIANT, (void**)&enumInterface);
    enumInterface->Reset();

    //qDebug()<<"number is "<<QString::number(objInterList->dynamicCall("Count").toInt());
    for (int i = 0; i < objInterList->dynamicCall("Count").toInt(); i++)
    {
        //qDebug()<<__FILE__<<__LINE__;
        VARIANT *theItem = (VARIANT*)malloc(sizeof(VARIANT));
        if (enumInterface->Next(1,theItem,NULL) != S_FALSE)
        {
            //qDebug()<<__FILE__<<__LINE__;
            QAxObject *item = new QAxObject((IUnknown *)theItem->punkVal);
            //qDebug()<<__FILE__<<__LINE__;
            if(item){
                if(type<columnName.size()){
                    QString str = columnName.at(type);
                    QByteArray datagram(str.toLatin1()); //Or
                    const char* tempConstChar = datagram.data();
                    //qDebug()<<"the query is "<<query<<" and cn is "<<tempConstChar;
                    hwInfo=item->dynamicCall(tempConstChar).toString();
                }
                //qDebug() <<" string is "<<hwInfo;
            }else{
                qDebug() <<" item is null";
            }
        }else{
            qDebug() <<" item is false";
        }
    }
    //this->getCPUManID();
    return hwInfo;
}

=======
﻿/**
 * 思路：
 * 1、保存activeCode到HKEY_CURRENT_USER\\Software\\qCleverManager\\activeCode中
 * 2、程序每次打开时，先读取 注册表HKEY_CURRENT_USER\\Software\\qCleverManager\\activeCode，
 * 3、如果为空，则提示需要激活，如果存在，进行下一步：
 * 4、获取机器信息getMachineInfo(),计算calMachineCode，
 * 5、计算激活码calActiveCode与activeCode进行对比，一致说明已激活，否则提示激活。
 * 6、有效期限制：首次激活时，validRecord到HKEY_CURRENT_USER\\Software\\qCloudSecChk\\validRecordTime
 * 7、程序每次打开，确认已激活后，打开上述位置，读取第一次激活时间，
 * 8、与当前时间对比，根据激活码匹配情况（120，180，360，730），选择对应时间阈，提示软件是否过期，是否需要重新注册。
 * 9、另外不能单纯依赖注册表，还应加上一些隐藏文件的判断等
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "softwareregistration.h"
#include <QObject>
#include <QSettings>
#include <QDebug>
#if defined(Q_OS_WIN32)
#include <QMessageBox>
#include <QAxObject>
#include <windows.h>
#include <tchar.h>
#include <QApplication>
#elif defined(Q_OS_LINUX)

#endif
#include <stdio.h>
#include <QUuid>
#include <QFile>
#include <QNetworkInterface>
#include <QCryptographicHash>
#include <QClipboard>

SoftwareRegiste::SoftwareRegiste()
{
}

/**
 * @brief 通过WMI接口获取硬件信息，但这些信息有些未必是唯一的（不同机器的数据可能类似)
 * @param type
 *      1.当前原生网卡地址：
 *      2.硬盘序列号
 *      3.获取主板序列号
 *      4.处理器ID)
 *      5.BIOS序列号
 *      6.主板型号
 * @return
 */
QString SoftwareRegiste::getWMIHWInfo(int type)
{
    QString hwInfo=("");
#if defined(Q_OS_WIN32)
#include <QMessageBox>
#include <QAxObject>
#include <windows.h>
#include <tchar.h>
#include <QApplication>
    QStringList sqlCmd;
    sqlCmd.clear();
    sqlCmd<<("SELECT MACAddress  FROM Win32_NetworkAdapter WHERE (MACAddress IS NOT NULL) AND (NOT (PNPDeviceID LIKE 'ROOT%'))");
    //注意qt调用 wmi时，对查询语句要求很严格，所以 like之类的句子务必精确才能有结果出来
    sqlCmd<<("SELECT PNPDeviceID  FROM  Win32_DiskDrive WHERE( PNPDeviceID IS NOT NULL) AND (MediaType LIKE 'Fixed%')");
    sqlCmd<<("SELECT SerialNumber FROM Win32_BaseBoard WHERE (SerialNumber IS NOT NULL)");
    sqlCmd<<("SELECT ProcessorId  FROM Win32_Processor WHERE (ProcessorId IS NOT NULL)");
    sqlCmd<<("SELECT SerialNumber  FROM Win32_BIOS WHERE (SerialNumber IS NOT NULL)");
    sqlCmd<<("SELECT Product FROM Win32_BaseBoard WHERE (Product IS NOT NULL)");

    QStringList columnName;
    columnName.clear();
    columnName<<("MACAddress");
    columnName<<("PNPDeviceID");
    columnName<<("SerialNumber");
    columnName<<("ProcessorId");
    columnName<<("SerialNumber");
    columnName<<("Product");

    QAxObject *objIWbemLocator = new QAxObject("WbemScripting.SWbemLocator");
    QAxObject *objWMIService = objIWbemLocator->querySubObject("ConnectServer(QString&,QString&)",QString("."),QString("root\\cimv2"));
    QString query=("");
    if(type<sqlCmd.size())
        query=sqlCmd.at(type);

    QAxObject *objInterList = objWMIService->querySubObject("ExecQuery(QString&))", query);
    QAxObject *enum1 = objInterList->querySubObject("_NewEnum");
    //需要 include windows.h
    IEnumVARIANT* enumInterface = 0;
    enum1->queryInterface(IID_IEnumVARIANT, (void**)&enumInterface);
    enumInterface->Reset();

    //qDebug()<<"number is "<<QString::number(objInterList->dynamicCall("Count").toInt());
    for (int i = 0; i < objInterList->dynamicCall("Count").toInt(); i++)
    {
        //qDebug()<<__FILE__<<__LINE__;
        VARIANT *theItem = (VARIANT*)malloc(sizeof(VARIANT));
        if (enumInterface->Next(1,theItem,NULL) != S_FALSE)
        {
            //qDebug()<<__FILE__<<__LINE__;
            QAxObject *item = new QAxObject((IUnknown *)theItem->punkVal);
            //qDebug()<<__FILE__<<__LINE__;
            if(item){
                if(type<columnName.size()){
                    QString str = columnName.at(type);
                    QByteArray datagram(str.toLatin1()); //Or
                    const char* tempConstChar = datagram.data();
                    //qDebug()<<"the query is "<<query<<" and cn is "<<tempConstChar;
                    hwInfo=item->dynamicCall(tempConstChar).toString();
                }
                //qDebug() <<" string is "<<hwInfo;
            }else{
                qDebug() <<" item is null";
            }
        }else{
            qDebug() <<" item is false";
        }
    }
    //this->getCPUManID();

#elif defined(Q_OS_LINUX)

#endif
    return hwInfo;
}

>>>>>>> refs/remotes/origin/linux_lzy
