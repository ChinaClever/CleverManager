#ifndef DC_PDUTG_H
#define DC_PDUTG_H
#include "basicmysql.h"

struct DC_PduTgItem{
    DC_PduTgItem():id(-1),pdu_devices_id(-1),vol(-1),cur(-1),pow(-1),ele(-1),pf(-1){}

    int id, pdu_devices_id;
    double vol, cur, pow, ele,pf;
    QDateTime create_time, update_time;
};

class DC_PduTg : public BasicMySql<DC_PduTgItem>
{
    DC_PduTg();
public:    
    static DC_PduTg* bulid();

    QString tableName(){return QString("%1_%2").arg(TB_PREFIX).arg("pdu_tg") ;}
    bool insertItem(const DC_PduTgItem& item); // 插入
    bool updateItem(const DC_PduTgItem& item); // 修改

    bool containRid(int rid);
    int getMaxId(int rid); // 根据父ID查找最大的ID

protected:
   bool modifyItem(const DC_PduTgItem& item,const QString& cmd);
   void selectItem(QSqlQuery &query,DC_PduTgItem &item);
   QString ridName(){return QString("pdu_devices_id") ;}
};

#endif // DC_PDUTG_H
