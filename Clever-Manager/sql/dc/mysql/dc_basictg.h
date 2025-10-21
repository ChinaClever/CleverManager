#ifndef DC_BASICTG_H
#define DC_BASICTG_H
#include "basicmysql.h"

struct DC_TgItem{
    DC_TgItem():id(0),rid(0),
        cur(-1),vol(-1),pow(-1),ele(-1),pf(-1),tem(-1),hum(-1),
        all_num(0),line_num(0),alarm_num(0),offLine_num(0){}

    int id, rid;
    double cur,vol,pow,ele,pf,tem,hum;
    int all_num,line_num,alarm_num,offLine_num;
    QDateTime create_time, update_time;
};

class DC_BasicTg : public BasicMySql<DC_TgItem>
{
public:
    DC_BasicTg();
    virtual QString ridName()=0;

    bool insertItem(const DC_TgItem& item); // 插入
    bool updateItem(const DC_TgItem& item); // 修改

    bool containRid(int rid);
    int getMaxId(int rid); // 根据父ID查找最大的ID

protected:
   bool modifyItem(const DC_TgItem& item,const QString& cmd);
   void selectItem(QSqlQuery &query,DC_TgItem &item);
};

#endif // DC_BASICTG_H
