#ifndef NETANALYZEDATA_H
#define NETANALYZEDATA_H

#include "net_dataformat.h"

class NetAnalyticData
{
public:
    NetAnalyticData() {}
    static NetAnalyticData *bulid();
    uchar net_data_xor(uchar *buf, ushort len);
    int net_data_analytic(uchar *buf, ushort len, Net_sDataPacket *msg);
    int dev_data_analytic(uchar *buf, ushort len, Net_sDevData *pkt);

protected:
    int data_packet_getDomain(uchar *buf, Net_sDataPacket *msg);
    int data_packet_getCode(uchar *buf, Net_sDevCode *code);
    int data_packet_sync(uchar *buf, ushort len);
};

#endif // NETANALYZEDATA_H
