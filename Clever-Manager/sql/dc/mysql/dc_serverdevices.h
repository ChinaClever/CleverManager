#ifndef DC_SERVERDEVICES_H
#define DC_SERVERDEVICES_H

#include "dc_basicmysql.h"
struct DC_SerDevItem
{
    DC_SerDevItem():id(-1),cabinets_id(-1),name(" "),
    main_pdu_port(-1), spare_pdu_port(-1), address(0),height(2){}
    int id, cabinets_id;
    QString name;
    int main_pdu_port, spare_pdu_port, address, height;
    QDateTime create_time, update_time;
};

class DC_ServerDevices : public DC_BasicMySql<DC_SerDevItem>
{
    Q_OBJECT
    explicit DC_ServerDevices();
public:
    static DC_ServerDevices* bulid();

    QString tableName(){return QString("%1_%2").arg(TB_PREFIX).arg("server_devices") ;}
    bool insertItem(const DC_SerDevItem& item); // 插入
    bool updateItem(const DC_SerDevItem& item); // 修改

protected:
    bool modifyItem(const DC_SerDevItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,DC_SerDevItem &item);
    QString ridName(){ return "cabinets_id";}
};

#endif // DC_SERVERDEVICES_H
