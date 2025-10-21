#ifndef DC_IDCS_H
#define DC_IDCS_H

#include "dc_companysremarks.h"

struct DC_IdcItem{
    DC_IdcItem():id(-1),companys_id(-1){}
    bool unequal(const DC_IdcItem& item) const
    {
        return id!=item.id || companys_id!=item.companys_id || name!=item.name;
    }

    int id, companys_id;
    QString name;
    QDateTime create_time, update_time;
};

class DC_Idcs : public DC_BasicMySql<DC_IdcItem>
{
    Q_OBJECT
    DC_Idcs();
public:
    static DC_Idcs* bulid();

    QString tableName(){return QString("%1_%2").arg(TB_PREFIX).arg("idcs") ;}
    bool insertItem(const DC_IdcItem& item); // 插入
    bool updateItem(const DC_IdcItem& item); // 修改

protected:
    bool modifyItem(const DC_IdcItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query, DC_IdcItem &item);
    QString ridName(){ return "companys_id";}

private:
    DC_IdcItem mItem;
};

#endif // DC_IDCS_H
