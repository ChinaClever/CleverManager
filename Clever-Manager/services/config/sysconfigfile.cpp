/*
 * sysconfigfile.cpp
 *  配置文件的操作接口
 *  配置参数的保存与读取
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "sysconfigfile.h"
#include "common.h"

static QSettings *pConfigIni = NULL;

/**
 * 功 能：打开系统配置文件
 * 开发人员：Lzy     2016 - 七夕
 */
bool Sys_ConfigFile_Open(void)
{
    bool ret = true;

    QString fileName = "sysconfig.ini";
    QString strFilename = cm::pathOfData(fileName);

   // QString strFilename = QCoreApplication::applicationDirPath();
   // strFilename += "/" + fileName;

    if(!QFileInfo(strFilename).exists())
        ret = false;

    if(pConfigIni==NULL)
    {
        pConfigIni = new QSettings(strFilename, QSettings::IniFormat);
        //pConfigIni->setIniCodec(QTextCodec::codecForName("gb18030"));
        pConfigIni->setIniCodec("UTF-8");
    }

    return ret;
}

/**
 * 功 能：关闭系统配置文件
 * 开发人员：Lzy     2016 - 七夕
 */
void Sys_ConfigFile_Close(void)
{
    delete pConfigIni;
    pConfigIni = NULL;
   // sync();
}

/**
 * 功 能：读字符串配置文件
 * 开发人员：Lzy     2013 - 七夕
 */
QString Sys_ConfigFile_ReadStr(QString strParameterName, QString strGroup)
{
    QString strParameter = "";
    strParameterName = "/" + strGroup + "/" + strParameterName;
    strParameter = pConfigIni->value(strParameterName).toString();
    return strParameter;
}

/**
 * 功 能：读整形串配置文件
 * 开发人员：Lzy     2016 - 七夕
 */
int Sys_ConfigFile_ReadInt(QString strParameterName, QString strGroup)
{
    int nParameter = -1;
    strParameterName = "/" + strGroup + "/" + strParameterName;
    nParameter = pConfigIni->value(strParameterName).toInt();

    return nParameter;
}

/**
 * 功 能：读浮点形串配置文件
 * 开发人员：Lzy     2013 - 七夕
 */
double Sys_ConfigFile_ReadDouble(QString strParameterName, QString strGroup)
{
    double fParameter = 0;
    strParameterName = "/" + strGroup + "/" + strParameterName;
    fParameter = pConfigIni->value(strParameterName).toDouble();

    return fParameter;
}

/**
 * 功 能：参数写入配置文件
 * 开发人员：Lzy     2016 - 七夕
 */
void Sys_ConfigFile_Write(QString strParameterName, QString strParameter, QString strGroup)
{
    strParameterName = "/" + strGroup + "/" + strParameterName;
    pConfigIni->setValue(strParameterName, strParameter);
}

/**
 * 功 能：写入参数
 * 开发人员：Lzy     2016 - 七夕
 */
void Sys_ConfigFile_WriteParam(QString name, QString value)
{
    Sys_ConfigFile_Open();
    Sys_ConfigFile_Write(name, value);
    Sys_ConfigFile_Close();
}
