/*
 * dc_companys.cpp
 *  公司表：公司名称，域名，
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dc_companys.h"
#include <time.h>
#include <iostream>

DC_Companys::DC_Companys()
{
    mId = -1;
}

DC_Companys *DC_Companys::bulid()
{
    static DC_Companys* sington = nullptr;
    if(sington == nullptr)
        sington = new DC_Companys();
    return sington;
}


bool DC_Companys::insertItem(const DC_CompanyItem &item)
{
    QString cmd = "insert into %1 (name,domain) values(:name,:domain)";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Insert);
    return ret;
}

bool DC_Companys::updateItem(const DC_CompanyItem &item)
{
    QString cmd = "update %1 set "
                  "name         = :name,"
                  "domain         = :domain,"
                  "update_time    = :update_time"
                  " where id    = :id";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Update);
    return ret;
}

bool DC_Companys::modifyItem(const DC_CompanyItem &item, const QString &cmd)
{
    bool ret = false;
    if(mDb) {
        QSqlQuery query(*mDb);
        query.prepare(cmd);
        query.bindValue(":id",item.id);
        query.bindValue(":name",item.name);
        query.bindValue(":domain",item.domain);
        query.bindValue(":update_time", QTime::currentTime());
        ret = query.exec();
        if(!ret)
            throwError(query.lastError());
    }
    return ret;
}

void DC_Companys::selectItem(QSqlQuery &query,DC_CompanyItem &item)
{
    item.id = query.value("id").toInt();
    item.name = query.value("name").toString();
    item.domain = query.value("domain").toString();
    item.create_time = query.value("create_time").toDateTime();
    item.update_time = query.value("update_time").toDateTime();
}

DC_CompanyItem DC_Companys::findById(int id)
{
    QVector<DC_CompanyItem> items = findItemById(id);
    return items.first();
}

DC_CompanyItem DC_Companys::findByName(const QString& name)
{
    QVector<DC_CompanyItem> items = selItemsByName(name);
    return items.first();
}

DC_CompanyItem DC_Companys::findByDomain(const QString& domain)
{
    QVector<DC_CompanyItem> items = selItemsByDomain(domain);
    return items.first();
}

/**
 * @brief 根据公司名称查找
 * @param name 公司名称
 * @return
 */
QVector<DC_CompanyItem> DC_Companys::selItemsByName(const QString& name)
{
    return selectItems(QString("where name = \"%1\"").arg(name));
}

/**
 * @brief 根据公司域名查找
 * @param domain 公司域名
 * @return
 */
QVector<DC_CompanyItem> DC_Companys::selItemsByDomain(const QString& domain)
{
    return selectItems(QString("where domain = \"%1\"").arg(domain));
}

/**
 * @brief 公司名称是否已存在
 * @param name 公司名称
 * @return
 */
bool DC_Companys::containName(const QString& name)
{
    int ret = count("name", QString("where name = \"%1\"").arg(name));
    return (ret > 0) ? true:false;
}

/**
 * @brief 公司域名是否已存在
 * @param domain 公司域名
 * @return
 */
bool DC_Companys::containDomain(const QString& domain)
{
    int ret = count("domain", QString("where domain = \"%1\"").arg(domain));
    return (ret > 0) ? true:false;
}

bool DC_Companys::contain(const QString& name,const QString& domain)
{
    return containName(name) || containDomain(domain);
}

void DC_Companys::setName(const QString& name)
{
    DC_CompanyItem item = findById(mId);
    item.name = name;
    updateItem(item);
}

void DC_Companys::setDomain(const QString& domain)
{
    DC_CompanyItem item = findById(mId);
    item.domain = domain;
    updateItem(item);
}

/**
 * @brief 根据公司名，域名 获取公司索引ID
 * @param name  公司名
 * @param domain    域名
 * @return
 */
int DC_Companys::getId(const QString& name,const QString& domain)
{
    int id = -1;
    if(contain(name, domain))
    {
        QString cmd = QString("where name = \"%1\" and domain = \"%2\"").arg(name).arg(domain);
        QVector<int> items = selectIds(cmd);
        if(items.size() > 0) {
            mId = id = items.first();
        }
    }
    return id;
}

int DC_Companys::getId(const QString& domain)
{
    int id = -1;
    if(containDomain(domain))
    {
        QString cmd = QString("where domain = \"%1\"").arg(domain);
        QVector<int> items = selectIds(cmd);
        if(items.size() > 0) {
            mId = id = items.first();
        }
    }
    return id;
}
