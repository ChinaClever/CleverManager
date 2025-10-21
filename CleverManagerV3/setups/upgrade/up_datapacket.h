#ifndef UP_DATAPACKET_H
#define UP_DATAPACKET_H
#include <QtCore>

/**
 * 数据包
 */
struct Up_sDataPacket
{
    bool isRun;
    int devtype;
    int progress, subPorgress;
    QString usr,pwd;

    QString file, dev_type;
    QString status, subStatus;

    QStringList ips;
    QStringList errs;
    QStringList oks;
    QList<QStringList> logs;
};


class Up_DataPacket
{
    Up_DataPacket();
public:
    static Up_DataPacket *bulid();

    Up_sDataPacket *data;
};

#endif // DATAPACKET_H
