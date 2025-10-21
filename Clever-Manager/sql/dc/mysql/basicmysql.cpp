/*
 * basicsql.cpp
 *  Mysql操作通用基类
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "basicmysql.h"

static QSqlDatabase *g_db=nullptr;


MySqlBasic::MySqlBasic(QObject *parent) : QObject(parent)
{
    initDb();
    mDb = g_db;
}


/**
 * @brief 删除
 * @param condition
 */
void MySqlBasic::remove(const QString &condition)
{
    if(mDb) {
        QSqlQuery query(*mDb);
        if(!query.exec(QString("DELETE FROM %1 WHERE %2").arg(tableName()).arg(condition)))
            throwError(query.lastError());
    }
}

/**
 * @brief 获取最大ID
 * @param idName
 * @return
 */
int MySqlBasic::maxId(const QString &idName, const QString &condition)
{
    int max_id = 0;
    if(mDb) {
        QSqlQuery query(*mDb);
        if(query.exec(QString("select max(%1) from %2 %3").arg(idName).arg(tableName()).arg(condition)))
        {
            if(query.next())
                max_id = query.value(0).toInt();
        } else
            qDebug()<< "maxId:" << query.lastError();
    }
    return max_id;
}

/**
 * @brief 函数返回指定列的值的数目
 * @param column_name
 * @return
 */
int MySqlBasic::count(const QString &column_name, const QString &condition)
{
    int count = -1;
    if(mDb) {
        QSqlQuery query(*mDb);
        if(query.exec(QString("select count(DISTINCT %1) from %2 %3").arg(column_name).arg(tableName()).arg(condition))){
            if(query.next())
                count = query.value(0).toInt();
        } else
            qDebug()<< "sql count:" << query.lastError();
    }
    return count;
}

/**
 * @brief 函数返回列的所有值
 * @param column_name 列名
 * @param condition 条件
 * @return
 */
QStringList MySqlBasic::listColumn(const QString &column_name, const QString &condition)
{
    QStringList list;
    if(mDb) {
        QSqlQuery query(*mDb);
        if(query.exec(QString("select DISTINCT %1 from %2 %3").arg(column_name).arg(tableName()).arg(condition))){
            while(query.next())
                list << query.value(0).toString();
        } else
            qDebug()<< "sql listColumnStr:" << query.lastError();
    }
    return list;
}


QVector<int> MySqlBasic::listColumnToInt(const QString &column_name, const QString &condition)
{
    QStringList list = listColumn(column_name, condition);
    QVector<int> items;
    for(int i=0; i<list.size(); ++i)
        items << list[i].toInt();

    return items;
}

/**
 * @brief 函数返回id列的所有值
 * @param condition 查询条件
 * @return
 */
QVector<int> MySqlBasic::selectIds(const QString &condition)
{
    return listColumnToInt("id", condition);
}

/**
 * @brief 更改修改时间
 * @param condition
 * @return
 */
bool MySqlBasic::updateTime(const QString &condition)
{
    bool ret = false;
    QString cmd = QString("update %1 set update_time=:update_time %2").arg(tableName()).arg(condition);
    if(mDb) {
        QSqlQuery query(*mDb);
        query.prepare(cmd);
        query.bindValue(":update_time", QTime::currentTime());
        ret = query.exec();
        if(!ret)
            throwError(query.lastError());
    }
    return ret;
}

bool MySqlBasic::updateColumn(const QString& column_name, double value, const QString &condition)
{
    bool ret = false;
    if(mDb) {
        QSqlQuery query(*mDb);
        ret = query.exec(QString("update  %1 set %2=%3 %4").arg(tableName()).arg(column_name).arg(value).arg(condition));
        if(ret)
            ret = updateTime(condition);
        else
            qDebug()<< "sql updateColumn 1:" << query.lastError();
    }
    return ret;
}

bool MySqlBasic::updateColumn(const QString& column_name, const QString& value, const QString &condition)
{
    bool ret = false;
    if(mDb) {
        QSqlQuery query(*mDb);
        ret = query.exec(QString("update  %1 set %2=\"%3\" %4").arg(tableName()).arg(column_name).arg(value).arg(condition));
        if(ret)
            ret = updateTime(condition);
        else
            qDebug()<< "sql updateColumn 2:" << query.lastError();
    }
    return ret;
}

/**
 * @brief 异常
 * @param err
 */
void MySqlBasic::throwError(const QSqlError &err)
{
//    throw MysqlExecption(err.text(),err.type());
     qCritical() << "Sql_Error:" << err.text() << err.type() << tableName();
}

/**
 * @brief 数据库初始化
 */
void MySqlBasic::initDb()
{
    static bool s_initDbFinshed = false;

#if defined(CLOUD_MODULE)
    if(s_initDbFinshed == false){
        QSqlDatabase *db = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL", "mysql")); // 没指定连接名字，则为默认库
        db->setDatabaseName("clever_dc");
        db->setHostName("127.0.0.1");
        db->setPort(3306);
        db->setUserName("root");
        db->setPassword("");

#if defined(CLOUD_MODULE)
        //打开数据库
        if (db->open()) {
            g_db = db;
            s_initDbFinshed = true;
        } else {
            qDebug()<< "sql initDb:" << db->lastError();
        }
#endif
    }
#endif
}


