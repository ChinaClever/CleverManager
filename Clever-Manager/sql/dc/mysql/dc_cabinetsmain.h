#ifndef DC_CABINETSMAIN_H
#define DC_CABINETSMAIN_H
#include "dc_cabinets.h"

struct DC_CabinetPduItem{
    int id,cabinets_id,dev_type,dev_num ;
    QString ip;
    QDateTime create_time, update_time;

    DC_CabinetPduItem():id(-1),cabinets_id(-1),dev_type(0),dev_num(0){}
};

class DC_CabinetsMain : public DC_BasicMySql<DC_CabinetPduItem>
{
public:
    DC_CabinetsMain();
    static DC_CabinetsMain* bulid();

    QString tableName(){return QString("%1_%2").arg(TB_PREFIX).arg("cabinets_pdu_main") ;}
    bool insertItem(const DC_CabinetPduItem &item); // 插入
    bool updateItem(const DC_CabinetPduItem& item); // 修改

protected:
    bool modifyItem(const DC_CabinetPduItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,DC_CabinetPduItem &item);
    QString ridName(){ return "cabinets_id";}
};

#endif // DC_CABINETSMAIN_H
