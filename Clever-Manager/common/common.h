#ifndef COMMON_H
#define COMMON_H
#include <QtCore>
//#include "msgBox/qumsgbox.h"
#include "msgBox/msgbox.h"
#include <QTableView>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <QString>
#include <QDir>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QList>
#include <QNetworkAddressEntry>

#define ZEBRA_DEBUG_OUTPUT(X) qDebug() << X;

//#define ZEBRA_DEBUG_OUTPUT(X)

//#define PDU_DEBUG_OUTPUT(X) qDebug() << X;
#define PDU_DEBUG_OUTPUT(X)

#define UDP_HB_SENT 1

namespace cm {

inline bool isValidIpAddress(const QString& ipAddress)
{
    QHostAddress addr(ipAddress);
    switch (addr.protocol()) {
    case QAbstractSocket::IPv4Protocol:
        return ipAddress.count('.')==3;
    case QAbstractSocket::IPv6Protocol:
    case QAbstractSocket::AnyIPProtocol:
        return true;
    }
    return false;
//    return ipAddress.contains(QRegExp("^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$"));
}

inline QString getLocalIP()
{
    QList<QHostAddress> AddressList = QNetworkInterface::allAddresses();
    foreach(QHostAddress address, AddressList){
        if(address.protocol() == QAbstractSocket::IPv4Protocol &&
                address != QHostAddress::Null &&
                address != QHostAddress::LocalHost){
            if (address.toString().contains("127.0.")){
                continue;
            }
            return address.toString();
        }
    }
    return "192.168.1.8";
}

inline QString getLocalIP(QString netInterfaceName)
{
    QString ipAddr;

    //qDebug() << "********************NetInterfaceName : " << netInterfaceName;
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface netInterface, list) {
        if (!netInterface.isValid())
            continue;

        QNetworkInterface::InterfaceFlags flags = netInterface.flags();
        if (!(flags.testFlag(QNetworkInterface::IsRunning)
                    && !flags.testFlag(QNetworkInterface::IsLoopBack))) {
            continue;
        }
        if (netInterface.humanReadableName()/*device readable name*/ == netInterfaceName)
        {
            QList<QNetworkAddressEntry> allIP = netInterface.addressEntries();  //Returns the list of IP addresses that this interface possesses along with their associated netmasks and broadcast addresses.
            foreach(QNetworkAddressEntry ip, allIP)
            {
                if (ip.ip().protocol() == QAbstractSocket::IPv4Protocol &&
                        ip.ip() != QHostAddress::Null &&
                        ip.ip() != QHostAddress::LocalHost)
                {
                    ipAddr = ip.ip().toString();
                    if (ipAddr.contains("127.0.")){
                        continue;
                    }
                    //qDebug() << "locai ip : " << ipAddr;
                    return ipAddr;
                }
            }
        }
    }
    return "192.168.1.8";
}


inline int oidNameToDeviceNum(const QString& name)
{
    if(name.contains(QRegExp("^Master")))return 0;
    if(name.contains(QRegExp("^slaveOne")))return 1;
    if(name.contains(QRegExp("^slaveTwo")))return 2;
    if(name.contains(QRegExp("^slaveThree")))return 3;
    if(name.contains(QRegExp("^slaveFour")))return 4;
    if(name.contains(QRegExp("^slaveFive")))return 5;
    if(name.contains(QRegExp("^slaveSix")))return 6;
    if(name.contains(QRegExp("^slaveSeven")))return 7;
    if(name.contains(QRegExp("^slaveEight")))return 8;
    if(name.contains(QRegExp("^slaveNine")))return 9;
}

static QColor color(int index)
{
    static QColor colorArray[] =
    {QColor(52,233,0),QColor(220,223,0),QColor(255,162,0),QColor(0,147,138),QColor(0,240,226),
     QColor(0,158,240),QColor(0,96,145),QColor(203,161,255),QColor(119,80,168),QColor(248,127,136),
     QColor(169,65,72),QColor(138,196,139),QColor(81,120,82)};

    index = index%(sizeof(colorArray)/sizeof(colorArray[0]));
    return colorArray[index];

}

static QString pathOfData(const QString& name)
{
#if 1
    QDir dataDir = QDir::home();
#else
    QDir dataDir = QDir::current();//2019-9-16 by peng//北京科华定制需要把数据库放在软件文件夹的当前目录
#endif
    QString dirName = ".CleverManager";
    if(!dataDir.exists(dirName))
        dataDir.mkdir(dirName);
    dataDir.cd(dirName);
    return dataDir.absoluteFilePath(name);
}

template <typename T>
inline const bool bInBound(const T & min, const T & value, const T & max)
{
    return value >= min && value <= max;
}

const QColor gray(174,225,254);//空设备,原为灰色，现为天蓝色
const QColor green(1,255,102);//正常
const QColor orange(255,0,0);//报警，原为橘红，现为红色
const QColor blue(200,200,200);//离线，原为蓝色，现为灰色
const QColor yellow(255, 255, 0);//预警，黄色
inline QColor temp2Color(int value){
    if(value>35)return QColor(255,90,0,255);
    if(value>30)return QColor(255,255,0,255);
    if(value>25)return QColor(100,200,105,255);
    if(value>22)return QColor(100,128,255,255);
    if(value>0)return QColor(0,128,255,255);
    return QColor(200,200,200,255);
}

}

// 倍率定义
#define COM_RATE_VOL	10.0    // 电压
#define COM_RATE_CUR	10.0    // 电流
#define COM_RATE_POW	1000.0  // 功率
#define COM_RATE_ELE	10.0    // 电能
#define COM_RATE_PF		100.0   // 功率因数
#define COM_RATE_TEM	10.0    // 温度
#define COM_RATE_HUM	10.0    // 湿度


bool isIPaddress(QString &);
bool isDigitStr(QString);

extern void set_background_icon(QWidget *widget, QString icon);
extern void set_background_color(QWidget *widget, QColor color);
extern void groupBox_background_icon(QWidget *target);
extern void set_mainTitle_back(QWidget *widget);
extern void splitter_style_sheet(QWidget *target);
extern void splitter_style_color(QWidget *target);
extern void button_style_sheet(QWidget *target);
extern void btnBlue_style_sheet(QWidget *target);
extern void btnyellow_style_sheet(QWidget *target);
extern void tableview_style_png(QTableView *view);
extern bool db_getInitDb(void);
extern long ApplicationCrashHandler(EXCEPTION_POINTERS *pException);
#endif // COMMON_H
