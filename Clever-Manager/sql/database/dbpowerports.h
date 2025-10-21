#ifndef DBPOWERPORTS_H
#define DBPOWERPORTS_H

#include "basicsql.h"

struct PowerPortItem
{
    int id,device_id,port_index;
    float l1_ampere,l1_voltag,l2_ampere,l2_voltag,l3_ampere,l3_voltag;
    QDateTime update_time;
};

class DbPowerPorts:public BasicSql1<PowerPortItem>
{
    Q_OBJECT
    DbPowerPorts();
public:
    static DbPowerPorts *get();
    QString tableName(){return "power_ports";}
    void insertItem(const PowerPortItem& item);
    void updateItem(const PowerPortItem& item);
protected slots:
    void pduDevicesItemChange(int id,int type);
protected:
    inline void modifyItem(const PowerPortItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,PowerPortItem &item);
};

#endif // DBPOWERPORTS_H
