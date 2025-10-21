#ifndef NETDATAANALYZE_H
#define NETDATAANALYZE_H
#include "udp/recv/udpsocketlist.h"
#include "net_data/net_datasave.h"
#include "udp/sent/udpheartbeat.h"

class NetDataAnalyze : public QThread
{    
    Q_OBJECT
    explicit NetDataAnalyze(QObject *parent = 0);
public:
    static NetDataAnalyze *bulid(QObject *parent = 0);
    ~NetDataAnalyze();

protected:
    void run(void);
    void recvData(void);

private:
    bool isRun;
    UdpBaseData *mUdpData; /*UDP数据包*/
    UdpDataQueue *mDataQue;
    UdpSocketList *mSocketList;
    NetAnalyticData *mNetAnalytic;

    QString mIP;
    uchar *mRecvBuf; //数据缓冲区
    Net_DataSave *mDataSave;
    Net_sDataPacket *mPacket; //网络数据包
    Net_sDevData *mDevData; //设备数据结构体
    Net_sPacket *mPduData; //设备数据包结构体
};

extern void pdu_hashData_save(Net_sPacket *packet);

#endif // NETDATAANALYZE_H
