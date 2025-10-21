#ifndef PDU_DATAPACKETS_H
#define PDU_DATAPACKETS_H

#include "datapackets.h"
#include "dp_pduhrssave.h"
#include "dbpdudevices.h"
#include "json_push.h"

class Dp_PduPackets : public DataPackets
{
    Q_OBJECT
    explicit Dp_PduPackets(QObject *parent = nullptr);
public:
    static Dp_PduPackets *bulid(QObject *parent=0);

protected:
    void initFun();
    void initPacket(PduDeviceItem &it);
    void workDown(sDataPacket *pack);
    void sendHeartBeat(sDataPacket *pack);
    void pushData(sDataPacket *pack);
    sDataPacket *newDataPacket();
    int getDevNum(const QString &dev_num);

protected slots:
    void pduItemChange(int,int);

private:
    DbPduDevices *mDb;
    Dp_PduAlarm *mAlarm;
    Dp_PduHrsSave *mHrs;
    Json_Push *mPush;
};

#endif // PDU_DATAPACKETS_H
