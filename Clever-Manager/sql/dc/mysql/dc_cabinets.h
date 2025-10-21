#ifndef DC_CABINETS_H
#define DC_CABINETS_H

#include "dc_basicmysql.h"
struct DC_CabinetItem{
    int id,rooms_id,height,row_index,column_index;
    QString name;
    QDateTime create_time, update_time;

    DC_CabinetItem():id(-1),rooms_id(-1),row_index(0),column_index(0),height(42){}
};


class DC_Cabinets : public DC_BasicMySql<DC_CabinetItem>
{
    Q_OBJECT
    explicit DC_Cabinets();
public:
    static DC_Cabinets* bulid();

    QString tableName(){return QString("%1_%2").arg(TB_PREFIX).arg("cabinets") ;}
    bool insertItem(const DC_CabinetItem &item); // 插入
    bool updateItem(const DC_CabinetItem& item); // 修改

protected:
    bool modifyItem(const DC_CabinetItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,DC_CabinetItem &item);
    QString ridName(){ return "rooms_id";}
};

#endif // DC_CABINETS_H
