#ifndef DBCONSUMERDEVICES_H
#define DBCONSUMERDEVICES_H

#include "basicsql.h"

struct ConsumerDeviceItem
{
    ConsumerDeviceItem():id(-1),cabinet_id(-1),main_pdu_port(0),spare_pdu_port(0),address(1),occupy_height(2){}
    int id,cabinet_id,main_pdu_port,spare_pdu_port,
        address,occupy_height;
    QString name;
};
Q_DECLARE_METATYPE(ConsumerDeviceItem)

class DbConsumerDevices : public SqlBasic<ConsumerDeviceItem>
{
    Q_OBJECT
    DbConsumerDevices();
public:
    static DbConsumerDevices *get();
    QString tableName(){return "consumer_devices";}
    void insertItem(ConsumerDeviceItem &item);
    void updateItem(const ConsumerDeviceItem& item);
    void removeItemsByCabinetId(int cabinetId);
    bool containName(int cabinetId, const QString &name);
    QVector<ConsumerDeviceItem> selectItemsByCabinetId(int id);
    QVector<ConsumerDeviceItem> selectItemsByRackAddress(int address,int cabinetId);
signals:
    void updateSig(int ,QString);
protected slots:
    void cabinetItemChange(int cabinet_id,int type);
protected:
    void createTable();
    inline void modifyItem(const ConsumerDeviceItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,ConsumerDeviceItem &item);
};

#endif // DBCONSUMERDEVICES_H
