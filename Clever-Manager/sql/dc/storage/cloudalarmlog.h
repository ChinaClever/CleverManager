#ifndef CLOUDALARMLOG_H
#define CLOUDALARMLOG_H
#include "mysql/dc_devlogs.h"

struct CloudAlarmItem {
    QString room, cab, ip;
    int devType, num;
    QString alarmType, msg;
};



class CloudAlarmLog
{
    CloudAlarmLog();
public:
    static CloudAlarmLog* bulid();
    void saveLog(CloudAlarmItem &item);

protected:
    int getAlarmType(QString &str);
    int getRid(CloudAlarmItem &item);
};

#endif // CLOUDALARMLOG_H
