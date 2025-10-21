#ifndef DP_ALARMSLAVE_H
#define DP_ALARMSLAVE_H
#include "datapacket.h"
#include "dbpdualarm.h"

class Dp_PduAlarm : public QObject
{
    Dp_PduAlarm(QObject *parent);
public:
    static Dp_PduAlarm *bulid(QObject *parent=0);
    bool alarm(sDataPacket *pack);

protected:
    bool alarmSave(const QString &str, uchar value);
    bool alarmDataUnit(sDataUnit &unit, int size);
    bool alarmObjDataUnit(sObjData &obj);
    bool alarmEnvData(sEnvData &env);
    bool alarmDevData(sDevData &dev);
    sDbAlarmItem initItem();
    bool saveItem(sDbAlarmItem &item);
    bool offlineSave();

private:
    sDataPacket *mPack;
    QString mItemStr;
};

#endif // DP_ALARMSLAVE_H
