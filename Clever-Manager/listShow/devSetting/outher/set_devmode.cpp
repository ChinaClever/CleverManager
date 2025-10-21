<<<<<<< HEAD:Clever-Manager/listShow/devSetting/outher/set_devmode.cpp
﻿/*
 * set_devmode.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "set_devmode.h"

SET_DevMode::SET_DevMode(QWidget *parent) : QWidget(parent)
{
    initConfigFile();
    initWidget();
    initLayout();
}

SET_DevMode::~SET_DevMode()
{

}


/**
 * @brief 初始化配置文件
 */
bool SET_DevMode::initConfigFile(void)
{
    bool ret = Sys_ConfigFile_Open();
    if(ret)
    {
        m_mode = Sys_ConfigFile_ReadInt("dev_mode");
        if(m_mode < 0)
            m_mode = 1;
    }
    else
        m_mode = 1;

    Sys_ConfigFile_Close();

    return ret;
}

/**
 * @brief 窗口初始化
 */
void SET_DevMode::initWidget(void)
{
    groupBox = new QGroupBox(tr("设备工作模式设置"),this);
    lab = new QLabel(tr("工作模式:"),groupBox);
    lab->setMaximumWidth(50);

    QStringList list;
    list << tr("主机") << tr("副机") ;
    comBox = new QComboBox(groupBox);
    comBox->setMinimumHeight(28);
    comBox->insertItems(0,list);
    comBox->setCurrentIndex(m_mode);

    btn = new QPushButton(tr("保存"),groupBox);
    btn->setMaximumWidth(80);
    btn->setMinimumHeight(27);

    connect(btn,SIGNAL(clicked()),this,SLOT(buttonClicked()));
}

/**
 * @brief 布局
 */
void SET_DevMode::initLayout(void)
{
    QWidget *widget = new QWidget(groupBox);
    widget->resize(260,60);
    widget->move(10,20);

    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    hLayout->addWidget(lab);
    hLayout->addWidget(comBox);
    hLayout->addWidget(btn);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(groupBox);
}

/**
 * @brief 更新IP
 * @param ip
 */
void SET_DevMode::updateIp(QString &ip)
{
    DataRead dataRead;

    Data_DevInfoType dev;
    bool ret = dataRead.readDevInfo(ip,dev);
    if(ret)
        comBox->setCurrentIndex(dev.UserDev.MasterSlave);

    m_ip.clear();
    m_ip << ip;
}


/**
 * @brief 更新IP
 * @param ip
 */
void SET_DevMode::updateDev(QStringList &ip)
{
    m_ip = ip;
    initConfigFile();
    comBox->setCurrentIndex(m_mode);
}


/**
 * @brief 模式设置
 */
bool SET_DevMode::setMode(char mode)
{
    SET_CmdClass cmd;
    if(m_ip.size())
    {
        for(int i=0; i<m_ip.size(); ++i) /*多个IP 依次设置*/
            cmd.setMode(mode,m_ip[i]);
    }
    else
    {
        cmd.setMode(mode);
        Sys_ConfigFile_WriteParam("dev_mode",QString::number(mode));
    }

    return true;
}

void SET_DevMode::buttonClicked(void)
{
    m_mode = comBox->currentIndex();

    bool ret = setMode(m_mode);
    if(ret)
    {
        InfoMsgBox box(this,tr("设备工作模式修改成功!"));

        /*增加日志信息*/
        s_RecordLog log;
        log.title = tr("设备配置");
        log.operation = tr("设备工作模式修改");

        QString str;
        if(m_ip.size() == 1)
            str = m_ip.at(0)+ " ";

        log.msg = str + tr("设备工作模式修改成功");
        sql_insert_record(log);
    }
}
=======
﻿/*
 * set_devmode.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "set_devmode.h"

SET_DevMode::SET_DevMode(QWidget *parent) : QWidget(parent)
{
    initConfigFile();
    initWidget();
    initLayout();
}

SET_DevMode::~SET_DevMode()
{

}


/**
 * @brief 初始化配置文件
 */
bool SET_DevMode::initConfigFile(void)
{
    bool ret = Sys_ConfigFile_Open();
    if(ret)
    {
        m_mode = Sys_ConfigFile_ReadInt("dev_mode");
        if(m_mode < 0)
            m_mode = 1;
    }
    else
        m_mode = 1;

    Sys_ConfigFile_Close();

    return ret;
}

/**
 * @brief 窗口初始化
 */
void SET_DevMode::initWidget(void)
{
    groupBox = new QGroupBox(tr("设备工作模式设置"),this);
    lab = new QLabel(tr("工作模式:"),groupBox);
    lab->setMaximumWidth(50);

    QStringList list;
    list << tr("主机") << tr("副机") ;
    comBox = new QComboBox(groupBox);
    comBox->setMinimumHeight(28);
    comBox->insertItems(0,list);
    comBox->setCurrentIndex(m_mode);

    btn = new QPushButton(tr("保存"),groupBox);
    btn->setMaximumWidth(80);
    btn->setMinimumHeight(27);

    connect(btn,SIGNAL(clicked()),this,SLOT(buttonClicked()));
}

/**
 * @brief 布局
 */
void SET_DevMode::initLayout(void)
{
    QWidget *widget = new QWidget(groupBox);
    widget->resize(260,60);
    widget->move(10,20);

    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    hLayout->addWidget(lab);
    hLayout->addWidget(comBox);
    hLayout->addWidget(btn);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(groupBox);
}

/**
 * @brief 更新IP
 * @param ip
 */
void SET_DevMode::updateIp(QString &ip)
{
    DataRead dataRead;

    Data_DevInfoType dev;
    bool ret = dataRead.readDevInfo(ip,dev);
    if(ret)
        comBox->setCurrentIndex(dev.UserDev.MasterSlave);

    m_ip.clear();
    m_ip << ip;
}


/**
 * @brief 更新IP
 * @param ip
 */
void SET_DevMode::updateDev(QStringList &ip)
{
    m_ip = ip;
    initConfigFile();
    comBox->setCurrentIndex(m_mode);
}


/**
 * @brief 模式设置
 */
bool SET_DevMode::setMode(char mode)
{
    SET_CmdClass cmd;
    if(m_ip.size())
    {
        for(int i=0; i<m_ip.size(); ++i) /*多个IP 依次设置*/
            cmd.setMode(mode,m_ip[i]);
    }
    else
    {
        cmd.setMode(mode);
        Sys_ConfigFile_WriteParam("dev_mode",QString::number(mode));
    }

    return true;
}

void SET_DevMode::buttonClicked(void)
{
    m_mode = comBox->currentIndex();

    bool ret = setMode(m_mode);
    if(ret)
    {
        InfoMsgBox box(this,tr("设备工作模式修改成功!"));

        /*增加日志信息*/
        s_RecordLog log;
        log.title = tr("设备配置");
        log.operation = tr("设备工作模式修改");

        QString str;
        if(m_ip.size() == 1)
            str = m_ip.at(0)+ " ";

        log.msg = str + tr("设备工作模式修改成功");
        sql_insert_record(log);
    }
}
>>>>>>> develop_beijing:Clever-Manager/listShow/devSetting/set_devmode.cpp
