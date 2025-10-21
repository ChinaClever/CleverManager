#ifndef DC_USERACTIONS_H
#define DC_USERACTIONS_H
#include "dc_basicmysql.h"

struct DC_UserActionsItem{
    DC_UserActionsItem():id(-1),modules_id(-1){}
    int id, modules_id;
    QString name, operation, content;
    QDateTime create_time, update_time;
};

class DC_UserActions: public DC_BasicMySql<DC_UserActionsItem>
{
    DC_UserActions();
public:
    static DC_UserActions* bulid();

    QString tableName(){return QString("%1_%2").arg(TB_PREFIX).arg("user_actions") ;}
    bool insertItem(const DC_UserActionsItem& item); // 插入
    bool updateItem(const DC_UserActionsItem& item); // 修改

protected:
    bool modifyItem(const DC_UserActionsItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,DC_UserActionsItem &item);
    QString ridName(){ return "modules_id";}
};

#endif // DC_USERACTIONS_H
