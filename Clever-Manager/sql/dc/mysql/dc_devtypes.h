#ifndef DC_DEVTYPES_H
#define DC_DEVTYPES_H

#include "dc_basicmysql.h"

struct DC_DevTypeItem
{
    DC_DevTypeItem():id(-1),type_index(-1),name(" "){}
    int id, type_index;
    QString name;
    QDateTime create_time, update_time;
};

class DC_DevTypes : public DC_BasicMySql<DC_DevTypeItem>
{
    Q_OBJECT
    explicit DC_DevTypes();
public:
    static DC_DevTypes* bulid();

    QString tableName(){return QString("%1_%2").arg(TB_PREFIX).arg("dev_type_index") ;}
    bool insertItem(const DC_DevTypeItem& item); // 插入
    bool updateItem(const DC_DevTypeItem& item); // 修改

    QVector<int> listTypes();

protected:
    bool modifyItem(const DC_DevTypeItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,DC_DevTypeItem &item);
    QString ridName(){ return "type_index";}
};

#endif // DC_DEVTYPES_H
