#ifndef DC_PDUDEVICES_H
#define DC_PDUDEVICES_H

#include "basicmysql.h"

struct DC_PduDevItem
{
    DC_PduDevItem():id(-1),cabinets_id(0),type_index(0),num(0),name(""){}
    bool unequal(const DC_PduDevItem& item) const
    {
        return  cabinets_id!=item.cabinets_id || type_index!=item.type_index ||
                num!=item.num || ip!=item.ip|| name!=item.name;
    }
    int id, cabinets_id, type_index, num;
    QString name, ip;
    QDateTime create_time, update_time;
};


class DC_PduDevices : public BasicMySql<DC_PduDevItem>
{
    Q_OBJECT
    explicit DC_PduDevices();
public:
    static DC_PduDevices* bulid();

    QString tableName(){return QString("%1_%2").arg(TB_PREFIX).arg("pdu_devices") ;}
    bool insertItem(const DC_PduDevItem& item); // 插入
    bool updateItem(const DC_PduDevItem& item); // 修改

    int countDev(int cabinets_id); // 设备个数
    bool containDev(const DC_PduDevItem& item); // 设备是否存在
    int getId(DC_PduDevItem& item);

    QVector<DC_PduDevItem> selItemsByCabinetId(int cabinets_id); // 根据机柜查找设备
    QVector<DC_PduDevItem> selItemsByIp(int cabinets_id, const QString& ip);

protected:
    bool modifyItem(const DC_PduDevItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,DC_PduDevItem &item);
};

#endif // DC_PDUDEVICES_H
