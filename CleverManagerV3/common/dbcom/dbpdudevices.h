#ifndef DBPDUDEVICES_H
#define DBPDUDEVICES_H
#include "dbcabinetlist.h"

struct PduDeviceItem
{
    PduDeviceItem() {id=room_id=cab_id=-1;}
    int id, room_id,cab_id;
    QString room, cab, road;
    QString ip, dev_num, dev_type;
};

class DbPduDevices : public SqlBasic<PduDeviceItem>
{
    Q_OBJECT
    DbPduDevices();
public:
    static DbPduDevices *get();
    QString tableName(){return "pdu_devices";}

    QStringList listRoom();
    QStringList listCabinet(const QString &room);
    QStringList listDevTypes();
    QStringList listDevNums(const QString &ip);
    int getDevNums(const QString &ip);

    QStringList listIps();
    QStringList listIps(const QString &dev_type);
    QStringList listIps(const QString &room,const QString &cab);

protected:
    void insertItem(CabinetItem &item);
    void insertItem(PduDeviceItem &item);
    void updateItem(const PduDeviceItem& item);

    void removeItemsByCabinetId(int id);
    QVector<PduDeviceItem> selectItemsByCabinetId(int id);

protected slots:    
    void roomItemChange(int roomId, int type);
    void cabinetItemChange(int cabinet_id,int type);

protected:
    void createTable();
    inline void modifyItem(const PduDeviceItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,PduDeviceItem &item);
};

#endif // DBPDUDEVICES_H
