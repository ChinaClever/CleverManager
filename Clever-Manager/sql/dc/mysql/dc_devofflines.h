#ifndef DC_DEVOFFLINES_H
#define DC_DEVOFFLINES_H

#include "basicmysql.h"

struct DC_DevOfflineItem
{
    DC_DevOfflineItem():id(-1),pdu_devices_id(-1){}
    int id, pdu_devices_id ;
    QString contents;
    QDateTime create_time, update_time;
};
Q_DECLARE_METATYPE(DC_DevOfflineItem)

class DC_DevOfflines : public BasicMySql<DC_DevOfflineItem>
{
    Q_OBJECT
    explicit DC_DevOfflines();
public:
    static DC_DevOfflines* bulid();

    QString tableName(){return QString("%1_%2").arg(TB_PREFIX).arg("dev_offlines") ;}
    bool insertItem(const DC_DevOfflineItem& item); // 插入
    bool updateItem(const DC_DevOfflineItem& item); // 修改

protected:
    bool modifyItem(const DC_DevOfflineItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,DC_DevOfflineItem &item);
};

#endif // DC_DEVOFFLINES_H
