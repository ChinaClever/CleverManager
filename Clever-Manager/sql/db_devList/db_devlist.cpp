/*
 * db_devlist.cpp
 * 设备链表保存至数据库中
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "db_devlist.h"
#include "db_sqlquery.h"
#include <QObject>

/**
 * @brief 创建记录表
 * @return
 */
bool db_createDevListTable(void)
{
    QSqlQuery query;
    bool ret =  query.exec("create table devList (ip vchar primary key,type int, num int, name vchar, addrArea vchar, addrGroup vchar, addrCab vchar)");
    // if(ret == false)
    //    qDebug() << "-----------------------------------create Dev List Table err";

    return ret;
}



/**
 * @brief 设备是否存在
 * @param list
 * @return
 */
bool db_exist_dev(QString &ip)
{
    QString cmd = QObject::tr("select * from devList where ip = '%1'").arg(ip);
    QSqlQuery query;
    query.setForwardOnly(true); // 为了节省内存开销
    bool ret = query.exec(cmd);
    if(ret)
    {
        if(query.next())
            ret = true;
        else
            ret = false;
    }
    else
        qDebug() << "db dev exit err:" << ip;

    return ret;
}

/**
 * @brief 插入操作记录
 * @param log
 * @return
 */
bool db_insert_devList(db_devStruct *dev)
{
    bool ret = db_exist_dev(dev->IP);
    if(ret)
        db_del_devList(dev->IP); // 先删除再记录

    int offset = 0;
    QSqlQuery query;
    query.prepare("insert into devList (ip, type, num, name, addrArea, addrGroup, addrCab) "
                  "values (:ip, :type, :num, :name, :addrArea, :addrGroup, :addrCab)");
    query.bindValue(offset++, dev->IP);
    query.bindValue(offset++, dev->type);
    query.bindValue(offset++, dev->num);
    query.bindValue(offset++, dev->name);
    query.bindValue(offset++, dev->area);
    query.bindValue(offset++, dev->group);
    query.bindValue(offset++, dev->cab);
    ret = query.exec();
    if(!ret)
        qDebug() << "insert devList err";

    return  ret;
}



/**
 * @brief 删除一个设备
 * @param ip
 * @return
 */
bool db_del_devList(QString &ip)
{
    QString cmd = QObject::tr("delete from devList where ip = '%1'").arg(ip);
    QSqlQuery query;
    bool ret = query.exec(cmd);
    if(!ret)
        qDebug() << "db del dev err:" << ip;

    return  ret;
}

/**
 * @brief 获取设备链表
 * @param list
 * @return
 */
bool db_select_devList(QList<db_devStruct*> &list)
{
    QSqlQuery query;
    query.setForwardOnly(true); // 为了节省内存开销
    bool ret = query.exec("select * from devList");
    if(ret)
    {
        while(query.next()) //开始就先执行一次next()函数，那么query 指向结果集的第一条记录
        {
            //int rowNum = query.at();  //获取query 所指向的记录在结果集中的编号
            //int columnNum = query.record().count(); //获取每条记录中属性（即列）的个数

            int offset = 0;
            db_devStruct *dev = new db_devStruct;
            dev->IP = query.value(offset++).toString();  //获取IP
            dev->type = query.value(offset++).toInt();  //获取设备类型
            dev->num = query.value(offset++).toInt(); // 副机总数
            dev->name = query.value(offset++).toString(); // 获取设备名

            dev->area = query.value(offset++).toString();
            dev->group = query.value(offset++).toString(); // 获取设备所属组
            dev->cab = query.value(offset++).toString();

            list << dev; // 加入链表中
        }
    }
    else
        qDebug() << "db_select_devList err";

    return ret;
}


