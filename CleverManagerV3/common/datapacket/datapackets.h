#ifndef DATAPACKETS_H
#define DATAPACKETS_H

#include <QObject>
#include "datapacket.h"
#include "dp_basicthread.h"

class DataPackets : public Dp_BasicThread<sDataPacket *>
{
    Q_OBJECT
public:
    explicit DataPackets(QObject *parent = nullptr);

    sDataPacket *get(const QString &ip, int devNum);
    sDataPacket *get(const QString &ip, const QString &dev_num);
    int tgDataPackts(sTgObjData *tg, QVector<sDataPacket *> &packs);

    sDataPacket *getByPdu(uint id);
    QVector<sDataPacket *> getByCab(uint id);
    QVector<sDataPacket *> getByRoom(uint id);
    void dels(QVector<sDataPacket *> &packs);

    void delPdu(uint id);
    void delCab(uint id);
    void delRoom(uint id);

protected:
    virtual void initFun()=0;
    virtual void workDown(sDataPacket *pack)=0;
    int averData(ushort *data, int len);
    void tgDevData(sDevData &dev);
};

#endif // DATAPACKETS_H
