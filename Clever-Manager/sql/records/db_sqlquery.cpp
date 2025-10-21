/*
 * db_sqlquery.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "db_sqlquery.h"
#include "common.h"
#include <QSqlRecord>
#include "storage/clouduseractions.h"

static bool gInitDbFinshed = false;
extern bool db_createDevListTable(void);
extern QString usr_land_name(void);

/**
 * @brief 数据库初始化状态
 * @return
 */
bool db_getInitDb(void)
{
    return gInitDbFinshed;
}

/**
 * @brief 创建记录表
 * @return
 */
bool db_createRecordTable(void)
{
    QSqlQuery query;
    bool ret =  query.exec("create table recordLog (id int primary key,date vchar, time vchar,usr vchar, title vchar, operation vchar, description vchar)");
    //   if(ret == false)
    //       qDebug() << "create Record Table err";

    return ret;
}

/**
 * @brief 用户登录表
 * @return
 */
bool db_createLandTable(void)
{
    QSqlQuery query;
    bool ret =  query.exec("create table landLog (id int primary key,date vchar, time vchar,usr vchar, title vchar, operation vchar, description vchar)");
    //   if(ret == false)
    //       qDebug() << "create land Table err";

    return ret;
}


/**
 * @brief 创建报警表
 * @return
 */
bool db_createAlarmTable(void)
{
    QSqlQuery query;
    bool ret =  query.exec("create table alarmLog (id int primary key,date vchar, time vchar, devIP vchar, devName vchar,title vchar, alarmtype vchar, line vchar, description vchar)");
    //   if(ret == false)
    //       qDebug() << "create Alarm Table err";

    return ret;
}

/**
 * @brief 创建离线表
 * @return
 */
bool db_createOffLineTable(void)
{
    QSqlQuery query;
    bool ret =  query.exec("create table offLineLog (id int primary key,date vchar, time vchar, devIP vchar, devName vchar,title vchar, description vchar)");
    //   if(ret == false)
    //       qDebug() << "create offLineLog Table err";

    return ret;
}


/**
 * @brief 数据库连接
 * @return
 */
bool db_connection(void)
{
//    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//    db.setDatabaseName("database.db");
//    if(!db.open())
//        return false;

    if(gInitDbFinshed == false){
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(cm::pathOfData("cleverManager.db"));
        if (db.open()) //打开数据库
            gInitDbFinshed = true;
        else
            return false;
    }

    //创建表 -- 不存在的情况才会创建
    db_createRecordTable();
    db_createAlarmTable();
    db_createDevListTable();
    db_createOffLineTable();
    db_createLandTable();

    return true;
}



DB_SqlQuery::DB_SqlQuery()
{

}

DB_SqlQuery::~DB_SqlQuery()
{

}

int DB_SqlQuery::tableSize(const QString &name)
{
    static QReadWriteLock lock;
    QWriteLocker locker(&lock);
//    QString cmd = "select * from "+name;
    QString cmd = "select max(id) from " + name;
    int num = -1;

    query.setForwardOnly(true); // 为了节省内存开销
    bool ret = query.exec(cmd);
    if(ret) {
        if(query.next()) {
            ret = query.last();
            if(ret)
                num = query.value(0).toInt();
        }
    }
    else
        qDebug() << cmd << "err";

    return num;
}

/**
 * @brief 插入操作记录
 * @param log
 * @return
 */
bool DB_SqlQuery::insertRecord(s_RecordLog &log, const QString &table)
{
    bool ret = false;
    QDateTime dateTime = QDateTime::currentDateTime();
    QString date = dateTime.toString("yyyy-MM-dd");
    QString time = dateTime.toString("hh:mm:ss");
    int num = tableSize(table);

    query.prepare("insert into " + table +" (id, date,time, usr, title, operation, description) "
                  "values (:id, :date, :time, :usr, :title, :operation, :description)");
    query.bindValue(0, num+1);
    query.bindValue(1, date);
    query.bindValue(2, time);
    query.bindValue(3, log.usr);
    query.bindValue(4, log.title);
    query.bindValue(5, log.operation);
    query.bindValue(6, log.msg);
    ret = query.exec();
    if(!ret)
    {
            qDebug() << "insert Record err";
    }


    return  ret;
}

/**
 * @brief 插入报警记录
 * @param log
 * @return
 */
bool DB_SqlQuery::insertAlarm(s_AlarmLog &log)
{
    bool ret = false;
    QString table = LOG_ALARM_TABLE;
    QDateTime dateTime = QDateTime::currentDateTime();
    QString date = dateTime.toString("yyyy-MM-dd");
    QString time = dateTime.toString("hh:mm:ss");
    int num = tableSize(table);

    query.prepare("insert into alarmLog (id, date, time, devIP, devName, title, alarmtype, line, description)"
                  "values (:id, :date, :time, :devIP, :devName, :title, :alarmtype, :line, :description)");
    query.bindValue(0, num+1);
    query.bindValue(1, date);
    query.bindValue(2, time);
    query.bindValue(3, log.devIP);
    query.bindValue(4, log.devName);
    query.bindValue(5, log.title);
    query.bindValue(6, log.type);
    query.bindValue(7, log.line);
    query.bindValue(8, log.msg);
    ret = query.exec();
    if(!ret)
        qDebug() << "insert Alarm err";

    return  ret;
}

/**
 * @brief 插入离线日志
 * @param log
 * @return
 */
bool DB_SqlQuery::insertOffLine(sOffLineLog &log)
{
    bool ret = false;
    QString table = LOG_OFFLINE_TABLE;
    QDateTime dateTime = QDateTime::currentDateTime();
    QString date = dateTime.toString("yyyy-MM-dd");
    QString time = dateTime.toString("hh:mm:ss");
    int num = tableSize(table);

    query.prepare("insert into offLineLog (id, date, time, devIP, devName, title, description)"
                  "values (:id, :date, :time, :devIP, :devName, :title, :description)");
    query.bindValue(0, num+1);
    query.bindValue(1, date);
    query.bindValue(2, time);
    query.bindValue(3, log.devIP);
    query.bindValue(4, log.devName);
    query.bindValue(5, log.title);
    query.bindValue(6, log.msg);
    ret = query.exec();
    if(!ret)
        qDebug() << "insert offLineLog err";

    return  ret;
}

void DB_SqlQuery::selectTable(QString &name)
{
    QString cmd = "select * from "+name;
    bool ret = query.exec(cmd);
    if(ret)
    {
        while(query.next())
        {
            qDebug() << query.value(0).toInt() << query.value(1).toString();
        }
    }
    else
        qDebug() << cmd << "Err";
}

/**
 * @brief 根据日期来查询
 */
void DB_SqlQuery::selectTableByTime(QString &name, QString &start, QString &end, QString &q, QList<QStringList> &list)
{
    QString cmd = QString("select * from %1 where date Between \'%2\' and  \'%3\'").arg(name).arg(start).arg(end);
    if(!q.isEmpty()) cmd += QString(" and %1").arg(q);

    bool ret = query.exec(cmd);
    if(ret)
    {
        while(query.next())
        {
            QStringList strList;
            int columnNum = query.record().count(); //获取每条记录中属性（即列）的个数
            for(int i=0; i<columnNum; ++i)
                strList << query.value(i).toString();
            list.append(strList);
        }
    }
    else
        qDebug() << cmd << "Err";
}

/**
 * @brief 根据日期来查询
 */
int DB_SqlQuery::selectTableByTime(QString &name, QString &start, QString &end)
{
    QString cmd = QString("select * from %1 where date Between \'%2\' and  \'%3\'").arg(name).arg(start).arg(end);
    int ret = 0;
    if(query.exec(cmd)){
        ret = query.size();
    } else
        qDebug() << cmd << "Err";

    return ret;
}

bool DB_SqlQuery::delTable(const QString &table)
{
    QString cmd = QString("drop table %1").arg(table);
    bool ret = query.exec(cmd);
    if(!ret)
        qDebug() << cmd << "Err";
    return ret;
}


/**
 * @brief 清除离线表
 * @return
 */
bool DB_SqlQuery::clearTable(const QString &table)
{
    QString cmd = QObject::tr("delete from %1 ").arg(table);
    bool ret = query.exec(cmd);
    if(!ret)
        qDebug() << cmd << "Err" << "#################";
    return ret;
}




/**
 * @brief 插入操作日志记录
 * @param log
 * @return
 */
bool sql_insert_record(s_RecordLog &log)
{
    DB_SqlQuery sql;

    log.usr = usr_land_name();
    CloudUserActions::bulid()->slaveLog(log.usr, log.operation, log.msg);
    return sql.insertRecord(log);
}

bool sql_insert_land(sLandLog &log)
{
    DB_SqlQuery sql;

    log.usr = usr_land_name();
    CloudUserActions::bulid()->slaveLog(log.usr, log.operation, log.msg);
    return sql.insertLand(log);
}


/**
 * @brief 插入报警日志记录
 * @param log
 * @return
 */
bool sql_insert_alarm(s_AlarmLog &log)
{
    DB_SqlQuery sql;

    return sql.insertAlarm(log);
}

/**
 * @brief 插入离线日志
 * @param log
 * @return
 */
bool sql_insert_offLine(sOffLineLog &log)
{
    DB_SqlQuery sql;

    return sql.insertOffLine(log);
}


/**
 * @brief 获取日志时间链表
 * @return
 */
bool db_select_dateList(const QString &tableName, QStringList &dateList)
{
    QString str = QString("select * from %1").arg(tableName);

    QSqlQuery query; //数据库对象
    query.setForwardOnly(true); // 为了节省内存开销
    bool ret = query.exec(str);
    if(ret)
    {
        while(query.next()) //开始就先执行一次next()函数，那么query 指向结果集的第一条记录
        {
            str = query.value(1).toString();  //日期
            if(!dateList.contains(str))
                dateList << str; // 加入链表中
        }
    }
    else
        qDebug() << "db_select_dateList err: " << str;

    return ret;
}
