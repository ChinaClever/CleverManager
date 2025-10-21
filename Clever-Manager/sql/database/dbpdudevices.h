#ifndef DBPDUDEVICES_H
#define DBPDUDEVICES_H

#include "basicsql.h"
#include <QDateTime>

struct PduDeviceItem
{
    //device_type 0表示主，1表示备，其他无效
    PduDeviceItem():
        id(-1),cabinet_id(-1),pdu_index(-1),socket_count(0),device_type(-1),
        l1_ampere(0),l2_ampere(0),l3_ampere(0),
        l1_voltag(0),l2_voltag(0),l3_voltag(0),
        zebra_channel_state(false),clever_channel_state(false){}
    int id,cabinet_id,pdu_index,socket_count,device_type;
    float l1_ampere,l2_ampere,l3_ampere,l1_voltag,l2_voltag,l3_voltag;
    QString name,pdu_address,channel_id;
    bool zebra_channel_state,clever_channel_state;
    QDateTime update_time;
};

class DbPduDevices : public BasicSql1<PduDeviceItem>
{
    Q_OBJECT
    DbPduDevices();
public:
    static DbPduDevices *get();
    QString tableName(){return "power_devices_units";}
    void insertItem(const PduDeviceItem& item);
    void updateItem(const PduDeviceItem& item);
    void removeItemsByCabinetId(int id);
    QVector<PduDeviceItem> selectItemsByCabinetId(int id);
protected slots:
    void cabinetItemChange(int cabinet_id,int type);
protected:
    inline void modifyItem(const PduDeviceItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,PduDeviceItem &item);
};

#endif // DBPDUDEVICES_H
