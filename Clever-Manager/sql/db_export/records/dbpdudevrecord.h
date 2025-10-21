#ifndef DBPDUDEVRECORD_H
#define DBPDUDEVRECORD_H

#include <QObject>
#include "pdu/data/hash/read/pduhashread.h"
#include "dbpdudevindex.h"
#include "dbpduelecparams.h"
#include "dbpduenvsensor.h"
#include "pdupackettg.h"

class DbPduDevRecord : public QThread
{
    Q_OBJECT
public:
    explicit DbPduDevRecord(QObject *parent = 0);

     static DbPduDevRecord *get(QObject *parent = 0);
    int slave(int devType, QString &ip, int devNum, bool save=false);

protected:
    int slaveDevIndex(PduDataPacket *dataPacket);
    void slaveTgData(int index, int type, PduDataPacket *dataPacket);
    void slaveElecParams(int index, PduDataPacket *devData);
    void slaveElecDataUnit(int index, int type, PduObjData *objData);

    void slaveThUnit(int index, int type,PduDataUnit *unitData);
    void slaveSensor(int index,int type,PduDataBase *baseData);
    void slaveEnvData(int index,PduEnvData *envData);

    void changeItem(PduElecParamsItem &item);
    void slaveEnvCloud(PduEnvSensorItem &item);

protected: // 为了实现多态
     virtual bool insertItem(PduDevIndexItem &item);
     virtual bool insertItem(PduElecParamsItem &item);
     virtual void insertItem(PduEnvSensorItem &item);

signals:

public slots:

protected:
    int mSqlId; ////=== 测试用
    bool isSave;
};

#endif // DBPDUDEVRECORD_H
