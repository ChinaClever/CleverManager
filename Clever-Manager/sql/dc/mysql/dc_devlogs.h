#ifndef DC_DEVLOGS_H
#define DC_DEVLOGS_H
#include "basicmysql.h"
struct DC_DevLogItem
{
    DC_DevLogItem():id(-1),pdu_devices_id(0),item_type(0),contents(" "){}
    int id, pdu_devices_id, item_type;
    QString contents;
    QDateTime create_time, update_time;
};
Q_DECLARE_METATYPE(DC_DevLogItem)

class DC_DevLogs : public BasicMySql<DC_DevLogItem>
{
    Q_OBJECT
    explicit DC_DevLogs();
public:
    static DC_DevLogs* bulid();

    QString tableName(){return QString("%1_%2").arg(TB_PREFIX).arg("dev_logs") ;}
    bool insertItem(const DC_DevLogItem& item); // 插入
    bool updateItem(const DC_DevLogItem& item); // 修改

protected:
   bool modifyItem(const DC_DevLogItem& item,const QString& cmd);
   void selectItem(QSqlQuery &query,DC_DevLogItem &item);
};

#endif // DC_DEVLOGS_H
