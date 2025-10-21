/*
 * dc_companysremarks.cpp
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dc_companysremarks.h"

DC_CompanysRemarks::DC_CompanysRemarks()
{

}

DC_CompanysRemarks *DC_CompanysRemarks::bulid()
{
    static DC_CompanysRemarks* sington = nullptr;
    if(sington == nullptr)
        sington = new DC_CompanysRemarks();
    return sington;
}


bool DC_CompanysRemarks::insertItem(const DC_CompanyRemarkItem &item)
{
    QString cmd = "insert into %1 (companys_id,name,addr,phone,fax,email,zipcode) "
                  "values(:companys_id,:name,:addr,:phone,:fax,:email,:zipcode)";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Insert);
    return ret;
}

bool DC_CompanysRemarks::updateItem(const DC_CompanyRemarkItem &item)
{
    QString cmd = "update %1 set "
                  "companys_id  = :companys_id,"
                  "name         = :name,"
                  "addr         = :addr,"
                  "phone        = :phone,"
                  "fax          = :fax,"
                  "email        = :email,"
                  "zipcode      = :zipcode,"
                  "update_time  = :update_time"
                  " where id    = :id";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Update);
    return ret;
}

bool DC_CompanysRemarks::modifyItem(const DC_CompanyRemarkItem &item, const QString &cmd)
{
    bool ret = false;
    if(mDb) {
        QSqlQuery query(*mDb);
        query.prepare(cmd);
        query.bindValue(":id",item.id);
        query.bindValue(":companys_id",item.companys_id);
        query.bindValue(":name",item.name);
        query.bindValue(":addr",item.addr);
        query.bindValue(":phone",item.phone);
        query.bindValue(":fax",item.fax);
        query.bindValue(":email",item.email);
        query.bindValue(":zipcode",item.zipcode);
        query.bindValue(":update_time", QTime::currentTime());
        ret = query.exec();
        if(!ret)
            throwError(query.lastError());
    }
    return ret;
}

void DC_CompanysRemarks::selectItem(QSqlQuery &query,DC_CompanyRemarkItem &item)
{
    item.id = query.value("id").toInt();
    item.companys_id = query.value("companys_id").toInt();

    item.name = query.value("name").toString();
    item.addr = query.value("addr").toString();
    item.phone = query.value("phone").toString();
    item.fax = query.value("fax").toString();
    item.email = query.value("email").toString();
    item.zipcode = query.value("zipcode").toString();

    item.create_time = query.value("create_time").toDateTime();
    item.update_time = query.value("update_time").toDateTime();
}




