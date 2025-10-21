#ifndef DC_COMPANYS_H
#define DC_COMPANYS_H

#include "dc_basicmysql.h"

struct DC_CompanyItem{
    DC_CompanyItem():id(-1){}
    bool unequal(const DC_CompanyItem& item) const
    {
        return id!=item.id || name!=item.name || domain!=item.domain;
    }

    int id;
    QString name,domain;
    QDateTime create_time, update_time;
};
Q_DECLARE_METATYPE(DC_CompanyItem)


class DC_Companys : public BasicMySql<DC_CompanyItem>
{
    Q_OBJECT
    DC_Companys();

public:
    static DC_Companys* bulid();

    QString tableName(){return QString("%1_%2").arg(TB_PREFIX).arg("companys") ;}
    bool insertItem(const DC_CompanyItem& item); // 插入
    bool updateItem(const DC_CompanyItem& item); // 修改

    DC_CompanyItem findById(int id); // 根据索引ID查找
    DC_CompanyItem findByName(const QString& name);
    DC_CompanyItem findByDomain(const QString& domain);

    QVector<DC_CompanyItem> selItemsByName(const QString& name); // 根据公司名来查找
    QVector<DC_CompanyItem> selItemsByDomain(const QString& domain); // 根据公司域名来查找

    bool containName(const QString& name); // 公司名是否已存在
    bool containDomain(const QString& domain); // 公司域名是否已存在
    bool contain(const QString& name,const QString& domain);

    int getId(const QString& name,const QString& domain); // 根据公司名获取索引ID
    int getId(const QString& domain);
    int getId(){return mId;}

    void setName(const QString& name); // 修改公司名
    void setDomain(const QString& domain); // 修改公司域名

protected:
    bool modifyItem(const DC_CompanyItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,DC_CompanyItem &item);

private:
    int mId;
};

#endif // DC_COMPANYS_H
