#ifndef DC_ENVSENSORS_H
#define DC_ENVSENSORS_H

#include "basicmysql.h"
struct DC_EnvSensorItem{
    DC_EnvSensorItem():id(-1),pdu_devices_id(-1),type_index(-1),
        sensor_num(-1),sensor_value(-1){}

    int id, pdu_devices_id, type_index, sensor_num; // (类型 1:温度 2:湿度 3门禁 4水濅 5烟雾)
    float sensor_value;
    QDateTime create_time, update_time;
};
Q_DECLARE_METATYPE(DC_EnvSensorItem)

class DC_EnvSensors : public BasicMySql<DC_EnvSensorItem>
{
    Q_OBJECT
    explicit DC_EnvSensors();
public:
    static DC_EnvSensors* bulid();

    QString tableName(){return QString("%1_%2").arg(TB_PREFIX).arg("env_sensors") ;}
    bool insertItem(const DC_EnvSensorItem& item); // 插入
    bool updateItem(const DC_EnvSensorItem& item); // 修改

    bool containRid(int rid, int type, int bit);
    int getMaxId(int rid, int type, int bit);

protected:
   bool modifyItem(const DC_EnvSensorItem& item,const QString& cmd);
   void selectItem(QSqlQuery &query,DC_EnvSensorItem &item);
   QString ridName(){return QString("pdu_devices_id") ;}
};

#endif // DC_ENVSENSORS_H
