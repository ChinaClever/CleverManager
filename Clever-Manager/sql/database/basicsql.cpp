/*
 * basicsql.cpp
 *  sql通用类
 *
 *  Created on: 2016年10月11日
 *      Author:
 */
#include "basicsql.h"
#include "common.h"

BasicSql::BasicSql()
{
    initDb();
    //标记表
    QString cmd = "create table if not exists markingtable("
                  "name TEXT primary key not null,"
                  "marking TEXT not null"
                  ");";
    QSqlQuery query;
    if(!query.exec(cmd))
        throwError(query.lastError());
}

/**
 * @brief 删除
 * @param condition
 */
bool BasicSql::remove(const QString &condition)
{
    QSqlQuery query;
    bool ret = query.exec(QString("DELETE FROM %1 WHERE %2").arg(tableName()).arg(condition));
    if(!ret)throwError(query.lastError());
    return ret;
}

bool BasicSql::removeMinIds(int id)
{
    bool ret = remove(QString("id < %1").arg(id));
    if(ret) {
        emit itemChanged(id, Remove);
    }
    return ret;
}

bool BasicSql::clear()
{
    return remove("id > 0");
}

/**
 * @brief 获取最大ID
 * @param idName
 * @return
 */
int BasicSql::maxId(const QString &idName)
{
    int max_id = 0;
    QSqlQuery query;
    if(query.exec(QString("select max(%1) from %2").arg(idName).arg(tableName())))
    {
        if(query.next())
            max_id = query.value(0).toInt();
    } else {
        qDebug()<< "maxId:" << query.lastError();
    }
    return max_id;
}


int BasicSql::minId(const QString &idName, const QString &condition)
{
    int id = 0;
    QSqlQuery query;
    if(query.exec(QString("select MIN(%1) from %2 %3").arg(idName).arg(tableName()).arg(condition)))
    {
        if(query.next())
            id = query.value(0).toInt();
    } else
        throwError(query.lastError());
    return id;
}

int BasicSql::minId(const QString &condition)
{
    return minId("id", QString("where %1").arg(condition));
}

bool BasicSql::remove(int id)
{
    bool ret =  remove(QString("id = %1").arg(id));
    if(ret) {
        emit itemChanged(id, Remove);
    }
    return ret;
}

int BasicSql::minId()
{
    return minId("id", "");
}

bool BasicSql::minIdRemove()
{
    bool ret = false;
    int id = minId();
    if(id > 0) {
        ret = remove(id);
    }

    return true;
}

bool BasicSql::minIdsRemove(int id)
{
   return removeMinIds(id);
}

/**
 * @brief 函数返回指定列的值的数目
 * @param column_name
 * @return
 */
int BasicSql::count(const QString &column_name, const QString &condition)
{
    int count = -1;
    QSqlQuery query;
    if(query.exec(QString("select count(DISTINCT %1) from %2 %3").arg(column_name).arg(tableName()).arg(condition))){
        if(query.next())
            count = query.value(0).toInt();
    } else
       throwError(query.lastError());
    return count;
}

int BasicSql::counts()
{
    return count("id", "");
}

/**
 * @brief 数据库最多保存多少条数据，删除掉其它的
 * @param count
 * @return
 */
bool BasicSql::countsRemove(int count)
{
    bool ret = false;
    int rtn = counts();
    if(rtn > count) {
        int id = minId() + (rtn-count);
        ret = minIdsRemove(id);
    }

    return ret;
}
/**
 * @brief 异常
 * @param err
 */
void BasicSql::throwError(const QSqlError &err)
{
#ifdef SQL_THROW_ERROR
    throw SqlExecption(err.text(),err.type());
#else
    qCritical() << "Sql_Error:" << err.text() << err.type();
#endif
}

/**
 * @brief BasicSql::tableMarking
 * @return
 */
QString BasicSql::tableMarking()
{
    QString cmd = QString("SELECT * from markingtable where name = \"%1\"").arg(tableName());
    QSqlQuery query(cmd);
    if(query.exec(cmd)){
        if(query.next())
            return query.value("marking").toString();
    } else {
        throwError(query.lastError());
    }
    return "";
}
/**
 * @brief BasicSql::setTableMarking
 * @param marking
 */
void BasicSql::setTableMarking(const QString &marking)
{
    QString ori = tableMarking();
    QString cmd = ori.isEmpty()?"insert into markingtable (name,marking) values(%1,%2)":"update markingtable set marking = %2 where name = \"%1\"";
    QSqlQuery query;
    if(!query.exec(cmd.arg(tableName()).arg(marking)))
        throwError(query.lastError());
}

/**
 * @brief 数据库初始化
 */
void BasicSql::initDb()
{
    static bool s_initDbFinshed = false;

    s_initDbFinshed = db_getInitDb();
    if(s_initDbFinshed == false){
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(cm::pathOfData("cleverManager.db"));
        if (db.open())
        {
            qDebug() << "initDb success !!!";
        }
        else
        {
            qDebug() << "initDb error !!!";
        }
        //打开数据库
//        if (!db.open())
//            throwError(db.lastError());///===
        s_initDbFinshed = true;
    }
}
