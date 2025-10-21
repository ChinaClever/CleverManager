/*
 * usrclass.cpp
 *  用户管理类
 *  用户信息保存以json格式进行保存
 *  对用户信息新增、读取、修改、保存
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "usrclass.h"
#include "common.h"

UsrMaptype g_usrMap;


/**
 * @brief 列出所有用户电话号码
 */
int usr_telephone_list(QStringList &phone)
{
    QMapIterator<QString, UsrStruct*> it(g_usrMap);
    while(it.hasNext())
        phone << it.next().value()->telephone;

    return phone.size();
}

/**
 * @brief 列出所有用户邮件地址
 */
int usr_email_list(QStringList &email)
{
    QMapIterator<QString, UsrStruct*> it(g_usrMap);
    while(it.hasNext())
        email << it.next().value()->email;

    return email.size();
}

UsrClass::UsrClass()
{
    m_usrMap = &g_usrMap;
    initUsr();
}

UsrClass::~UsrClass()
{

}

void UsrClass::initUsr(void)
{
    int rtn = g_usrMap.size();
    if(rtn == 0)
    {
        bool ret = m_usrJson.readJson();
        if(ret == false)
        {
            UsrStruct *usr = new UsrStruct;

            usr->jurisdiction = true;
            usr->name = "admin";
            usr->pwd = "admin";
            usr->telephone = "18576689472";
            usr->email = "luozhiyong131@qq.com";
            //            usr->remarks = QObject::tr("默认账号，请修改账号信息");
//                        usr->remarks = tr("默认账号，请修改账号信息");
//            usr->remarks = QObject::tr("默认账号，请修改账号信息");

            addUsr(usr);
        }
    }
}


/**
 * 功能：获得用户个数
 */
int UsrClass::getCount(void)
{
    return m_usrMap->size();
}

UsrMaptype * UsrClass::getUsrMap(void)
{
    return m_usrMap;
}

bool UsrClass::delUsr(QString &name)
{
    bool ret = m_usrMap->contains(name);
    if(ret)
    {
        UsrStruct *usr = getUsr(name);
        if(usr)
        {
            m_usrMap->remove(name);
            m_usrJson.writeJson();

            delete usr;
        }
    }

    return ret;
}

/**
 * @brief 新增用户
 * @param s_UsrStruct->用户结构体
 * @return
 */
bool UsrClass::addUsr(UsrStruct *s_UsrStruct)
{
    if(m_usrMap->contains(s_UsrStruct->name)) /*用户名已经存在*/
    {
        delete s_UsrStruct; // 删除
        return false;
    }
    else
    {
        m_usrMap->insert(s_UsrStruct->name, s_UsrStruct);
        m_usrJson.writeJson();
    }
    return true;
}

UsrStruct *UsrClass::getUsr(QString &name)
{
    UsrStruct *usr = NULL;

    if(m_usrMap->contains(name))
        usr = m_usrMap->find(name).value();

    return usr;
}


bool UsrClass::editUsr(UsrStruct *usr)
{
    bool rtn = true;

    if(m_usrMap->contains(usr->name))
    {
        (*m_usrMap)[usr->name] = usr;
        m_usrJson.writeJson();
    }
    else
        rtn = false;

    return rtn;
}
