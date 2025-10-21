#ifndef DC_BASICSTATUS_H
#define DC_BASICSTATUS_H
#include "basicmysql.h"

struct DC_StatusItem
{
    DC_StatusItem():id(-1),rid(0),status(0){}
    int id, rid, status;
    QDateTime create_time, update_time;
};

class DC_BasicStatus : public BasicMySql<DC_StatusItem>
{
public:
    DC_BasicStatus();
    virtual QString ridName()=0;
    bool insertItem(const DC_StatusItem& item); // 插入
    bool updateItem(const DC_StatusItem& item); // 修改

    bool containRid(int rid);
    bool setStatus(int rid, int status); // 设置的状态
    bool updateStatus(int rid, int status);

protected:
   bool modifyItem(const DC_StatusItem& item,const QString& cmd);
   void selectItem(QSqlQuery &query,DC_StatusItem &item);
};

#endif // DC_BASICSTATUS_H
