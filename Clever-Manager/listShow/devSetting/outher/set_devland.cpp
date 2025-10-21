<<<<<<< HEAD:Clever-Manager/listShow/devSetting/outher/set_devland.cpp
﻿/*
 * set_devland.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "set_devland.h"

SET_DevLand::SET_DevLand(QWidget *parent) : QWidget(parent)
{
    initConfigFile();
    initWidget();
    initLayout();

  //  this->setMaximumSize(240,180);
}

SET_DevLand::~SET_DevLand()
{

}

/**
 * @brief 初始化配置文件
 * @return
 */
bool SET_DevLand::initConfigFile(void)
{
    bool ret = Sys_ConfigFile_Open();
    if(ret)
    {
        m_usr = Sys_ConfigFile_ReadStr("dev_usr");
        m_pwd = Sys_ConfigFile_ReadStr("dev_pwd");
    }

    if(m_usr.isEmpty() || m_pwd.isEmpty())
        m_usr = m_pwd = "admin";

    Sys_ConfigFile_Close();

    return ret;
}


/**
 * @brief 窗口初始化
 */
void SET_DevLand::initWidget(void)
{
    usrLab = new QLabel(tr("账户："),this);
    pwdLab = new QLabel(tr("密码："),this);

    usrEdit = new QLineEdit(m_usr,this);
    usrEdit->setMinimumHeight(28);

    pwdEdit = new QLineEdit(m_pwd,this);
    pwdEdit->setMinimumHeight(28);

    btn = new QPushButton(tr("保存"),this);
    btn->setMinimumHeight(30);
    btn->setMaximumWidth(120);

    connect(btn,SIGNAL(clicked()),this,SLOT(buttonClicked()));
}


/**
 * @brief 布局
 */
void SET_DevLand::initLayout(void)
{
    QGroupBox *groupBox = new QGroupBox(tr("设备用户登陆设置"),this);
    QWidget *widget = new QWidget(groupBox);
    widget->resize(240,180);
    widget->move(10,10);

    QGridLayout *layout = new QGridLayout(widget);
    layout->addWidget(usrLab,1,0);
    layout->addWidget(usrEdit,1,1);

    layout->addWidget(pwdLab,2,0);
    layout->addWidget(pwdEdit,2,1);

    layout->addWidget(btn,3,1);

    QGridLayout *pLayout = new QGridLayout(this);
    pLayout->addWidget(groupBox);
}


void SET_DevLand::updateIp(QString &ip)
{
    DataRead dataRead;

    Data_DevInfoType dev;
    bool ret = dataRead.readDevInfo(ip,dev);
    if(ret)
    {
        QMap<QString, QString> *map = &dev.UserDev.loginMap;
        QMap<QString,QString>::const_iterator it;
        for (it = map->constBegin(); it != map->constEnd(); ++it)
        {
            m_usr = it.key();
            m_pwd = it.value();
        }
        usrEdit->setText(m_usr);
        pwdEdit->setText(m_pwd);
    }

    m_ip.clear();
    m_ip << ip;
}

/**
 * @brief 更新IP
 * @param ip
 */
void SET_DevLand::updateDev(QStringList &ip)
{
    m_ip = ip;
    initConfigFile();
    usrEdit->setText(m_usr);
    pwdEdit->setText(m_pwd);
}

/**
 * @brief 设置登陆名
 */
bool SET_DevLand::setLand(void)
{
    m_usr = usrEdit->text();
    if(m_usr.size() >= 24)
    {
        CriticalMsgBox box(this,tr("用户名太长!"));
        return false;
    }

    m_pwd = pwdEdit->text();
    if(m_pwd.size() >= 20)
    {
        CriticalMsgBox box(this,tr("密码太长!"));
        return false;
    }

    SET_CmdClass cmd;
    if(m_ip.size())
    {
        for(int i=0; i<m_ip.size(); ++i) /*多个IP 依次设置*/
            cmd.setLand(m_usr,m_pwd,m_ip[i]);
    }
    else
    {
        cmd.setLand(m_usr,m_pwd);
        Sys_ConfigFile_WriteParam("dev_usr",m_usr);
        Sys_ConfigFile_WriteParam("dev_pwd",m_pwd);
    }

    return true;
}

void SET_DevLand::buttonClicked(void)
{
    bool ret = setLand();
    if(ret)
    {
        InfoMsgBox box(this,tr("账户修改成功!"));

        /*增加日志信息*/
        s_RecordLog log;
        log.title = tr("设备配置");
        log.operation = tr("设备登陆信息修改");

        QString str;
        if(m_ip.size() == 1)
            str = m_ip.at(0)+ " ";

        log.msg = str + tr("设备登陆名修改为%1").arg(m_usr);
        sql_insert_record(log);
    }
}
=======
﻿/*
 * set_devland.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "set_devland.h"

SET_DevLand::SET_DevLand(QWidget *parent) : QWidget(parent)
{
    initConfigFile();
    initWidget();
    initLayout();

  //  this->setMaximumSize(240,180);
}

SET_DevLand::~SET_DevLand()
{

}

/**
 * @brief 初始化配置文件
 * @return
 */
bool SET_DevLand::initConfigFile(void)
{
    bool ret = Sys_ConfigFile_Open();
    if(ret)
    {
        m_usr = Sys_ConfigFile_ReadStr("dev_usr");
        m_pwd = Sys_ConfigFile_ReadStr("dev_pwd");
    }

    if(m_usr.isEmpty() || m_pwd.isEmpty())
        m_usr = m_pwd = "admin";

    Sys_ConfigFile_Close();

    return ret;
}


/**
 * @brief 窗口初始化
 */
void SET_DevLand::initWidget(void)
{
    usrLab = new QLabel(tr("账户："),this);
    pwdLab = new QLabel(tr("密码："),this);

    usrEdit = new QLineEdit(m_usr,this);
    usrEdit->setMinimumHeight(28);

    pwdEdit = new QLineEdit(m_pwd,this);
    pwdEdit->setMinimumHeight(28);

    btn = new QPushButton(tr("保存"),this);
    btn->setMinimumHeight(30);
    btn->setMaximumWidth(120);

    connect(btn,SIGNAL(clicked()),this,SLOT(buttonClicked()));
}


/**
 * @brief 布局
 */
void SET_DevLand::initLayout(void)
{
    QGroupBox *groupBox = new QGroupBox(tr("设备用户登陆设置"),this);
    QWidget *widget = new QWidget(groupBox);
    widget->resize(240,180);
    widget->move(10,10);

    QGridLayout *layout = new QGridLayout(widget);
    layout->addWidget(usrLab,1,0);
    layout->addWidget(usrEdit,1,1);

    layout->addWidget(pwdLab,2,0);
    layout->addWidget(pwdEdit,2,1);

    layout->addWidget(btn,3,1);

    QGridLayout *pLayout = new QGridLayout(this);
    pLayout->addWidget(groupBox);
}


void SET_DevLand::updateIp(QString &ip)
{
    DataRead dataRead;

    Data_DevInfoType dev;
    bool ret = dataRead.readDevInfo(ip,dev);
    if(ret)
    {
        QMap<QString, QString> *map = &dev.UserDev.loginMap;
        QMap<QString,QString>::const_iterator it;
        for (it = map->constBegin(); it != map->constEnd(); ++it)
        {
            m_usr = it.key();
            m_pwd = it.value();
        }
        usrEdit->setText(m_usr);
        pwdEdit->setText(m_pwd);
    }

    m_ip.clear();
    m_ip << ip;
}

/**
 * @brief 更新IP
 * @param ip
 */
void SET_DevLand::updateDev(QStringList &ip)
{
    m_ip = ip;
    initConfigFile();
    usrEdit->setText(m_usr);
    pwdEdit->setText(m_pwd);
}

/**
 * @brief 设置登陆名
 */
bool SET_DevLand::setLand(void)
{
    m_usr = usrEdit->text();
    if(m_usr.size() >= 24)
    {
        CriticalMsgBox box(this,tr("用户名太长!"));
        return false;
    }

    m_pwd = pwdEdit->text();
    if(m_pwd.size() >= 20)
    {
        CriticalMsgBox box(this,tr("密码太长!"));
        return false;
    }

    SET_CmdClass cmd;
    if(m_ip.size())
    {
        for(int i=0; i<m_ip.size(); ++i) /*多个IP 依次设置*/
            cmd.setLand(m_usr,m_pwd,m_ip[i]);
    }
    else
    {
        cmd.setLand(m_usr,m_pwd);
        Sys_ConfigFile_WriteParam("dev_usr",m_usr);
        Sys_ConfigFile_WriteParam("dev_pwd",m_pwd);
    }

    return true;
}

void SET_DevLand::buttonClicked(void)
{
    bool ret = setLand();
    if(ret)
    {
        InfoMsgBox box(this,tr("账户修改成功!"));

        /*增加日志信息*/
        s_RecordLog log;
        log.title = tr("设备配置");
        log.operation = tr("设备登陆信息修改");

        QString str;
        if(m_ip.size() == 1)
            str = m_ip.at(0)+ " ";

        log.msg = str + tr("设备登陆名修改为%1").arg(m_usr);
        sql_insert_record(log);
    }
}
>>>>>>> develop_beijing:Clever-Manager/listShow/devSetting/set_devland.cpp
