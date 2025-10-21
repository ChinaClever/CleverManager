#ifndef DC_ROOMS_H
#define DC_ROOMS_H

#include "dc_basicmysql.h"

struct DC_RoomItem
{
    DC_RoomItem():id(-1),modules_id(-1){}
    bool unequal(const DC_RoomItem& item) const
    {
        return  modules_id!=item.modules_id || name!=item.name;
    }

    int id, modules_id ;
    QString name; // 机房名
    QDateTime create_time, update_time;
};

class DC_Rooms : public DC_BasicMySql<DC_RoomItem>
{
    Q_OBJECT
    explicit DC_Rooms();
public:
    static DC_Rooms* bulid();

    QString tableName(){return QString("%1_%2").arg(TB_PREFIX).arg("rooms") ;}
    bool insertItem(const DC_RoomItem& item); // 插入
    bool updateItem(const DC_RoomItem& item); // 修改

protected:
    bool modifyItem(const DC_RoomItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,DC_RoomItem &item);
    QString ridName(){ return "modules_id";}
};

#endif // DC_ROOMS_H
