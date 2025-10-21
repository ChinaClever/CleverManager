#ifndef DC_MODULES_H
#define DC_MODULES_H
#include "dc_idcsremarks.h"

struct DC_ModuleItem
{
    DC_ModuleItem():id(-1),idcs_id(-1){}
    bool unequal(const DC_ModuleItem& item) const
    {
        return id!=item.id || idcs_id!=item.idcs_id || name!=item.name;
    }
    int id, idcs_id;
    QString name; // 模块名
    QDateTime create_time, update_time;
};

class DC_Modules: public DC_BasicMySql<DC_ModuleItem>
{
     Q_OBJECT
    DC_Modules();
public:
    static DC_Modules* bulid();

    QString tableName(){return QString("%1_%2").arg(TB_PREFIX).arg("modules") ;}
    bool insertItem(const DC_ModuleItem& item); // 插入
    bool updateItem(const DC_ModuleItem& item); // 修改

protected:
    bool modifyItem(const DC_ModuleItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,DC_ModuleItem &item);
    QString ridName(){ return "idcs_id";}

private:
    DC_ModuleItem mItem;
};

#endif // DC_MODULES_H
