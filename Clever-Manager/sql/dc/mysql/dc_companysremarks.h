#ifndef DC_COMPANYSREMARKS_H
#define DC_COMPANYSREMARKS_H

#include "dc_companys.h"

struct DC_CompanyRemarkItem{
    DC_CompanyRemarkItem():id(-1),companys_id(-1){}
    bool unequal(const DC_CompanyRemarkItem& item) const
    {
        return id!=item.id || companys_id!=item.companys_id;
    }

    int id, companys_id;
    QString name,addr,phone,fax,email,zipcode;
    QDateTime create_time, update_time;
};
Q_DECLARE_METATYPE(DC_CompanyRemarkItem)


class DC_CompanysRemarks : public DC_BasicMySql<DC_CompanyRemarkItem>
{
     Q_OBJECT
    DC_CompanysRemarks();
public:
    static DC_CompanysRemarks *bulid();

    QString tableName(){return QString("%1_%2").arg(TB_PREFIX).arg("companys_remarks") ;}
    bool insertItem(const DC_CompanyRemarkItem& item); // 插入
    bool updateItem(const DC_CompanyRemarkItem &item); // 修改

protected:
    bool modifyItem(const DC_CompanyRemarkItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query, DC_CompanyRemarkItem &item);
    QString ridName(){ return "companys_id";}
};

#endif // DC_COMPANYSREMARKS_H
