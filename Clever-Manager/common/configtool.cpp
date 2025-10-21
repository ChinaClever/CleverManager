#include "configtool.h"
#include <QStandardPaths>
#include <QApplication>
#include <QCryptographicHash>
#include "common.h"
#include "services/config/sysconfigfile.h"

ConfigTool::ConfigTool(const QString &fileName, Format format, QObject *parent)
    :QSettings(fileName,format,parent)
{
}

ConfigTool *ConfigTool::get()
{
    static ConfigTool* s_configTool = nullptr;
    if (s_configTool == nullptr)
        s_configTool = new ConfigTool(cm::pathOfData("CleverManager.ini"),IniFormat);
    return s_configTool;
}

void ConfigTool::setValue(const QString &key, const QVariant &value)
{
    QSettings::setValue(key,value);
    sync();
}

QStringList ConfigTool::communicationGet()
{
    return value("get_communication", QStringList()<<"tcp").toStringList();
}

void ConfigTool::setCommunicationGet(const QStringList &strs)
{
    setValue("get_communication",strs);
}

void ConfigTool::setMainWindowGeometry(const QByteArray &ary)
{
    setValue("mainWindowGeometry",ary);
}

QByteArray ConfigTool::mainWindowGeometry()
{
    return value("mainWindowGeometry").toByteArray();
}

QString ConfigTool::getZebraServerIP()
{
    return value("server_address",SERVER_ADDRESS).toString();
}

void ConfigTool::setZebraServerIP(const QString &ipAddr)
{
    setValue("server_address",ipAddr);
}

int ConfigTool::getManaulStatus()
{
    return value("manual_status", 0).toInt();
}

void ConfigTool::setManaulStatus(int status)
{
    setValue("manual_status",status);
}

QString ConfigTool::zebraServerGet()
{
    Sys_ConfigFile_Open();
    QString netName = Sys_ConfigFile_ReadStr("NET_NAME");
    if(netName.length()<=0){
        netName = "本地连接";
        netName.toUtf8();
        Sys_ConfigFile_WriteParam("NET_NAME", netName);
    }
    Sys_ConfigFile_Close();
    QString local_point = cm::getLocalIP(netName)+":20111";
    QString server_address = getZebraServerIP()+":27053";
    return local_point + QString(" ") + server_address;
}

int ConfigTool::getLogRecordTime()
{
    return value("logrecord_time", 24*60).toInt();
}

void ConfigTool::setLogRecordTime(int time)
{
    setValue("logrecord_time",time);
}
