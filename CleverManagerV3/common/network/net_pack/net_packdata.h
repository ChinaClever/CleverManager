#ifndef NETPACKDATA_H
#define NETPACKDATA_H
#include "net_analyzedata.h"

class NetPackData
{
public:
    NetPackData() {}
    static NetPackData *bulid();
    int net_data_packets(int num, Net_sDevData *pkt, uchar *buf, uchar type=TRA_TYPR_UDP);

protected:
    Net_sDataPacket *get_data_packet(Net_sDevCode *code, uchar *buf, ushort len);
    Net_sDevCode *get_dev_code(int num, uchar type);
    int int_to_uchar(int form, uchar *to);
    int dev_data_pack(Net_sDevData *pkt, uchar *buf);
    int data_msg_packet(Net_sDataPacket *pkt, uchar *buf);
    int data_packet_domain(Net_sDataPacket *msg, uchar *buf);
    int data_packet_code(Net_sDevCode *msg, uchar *buf);
    void data_packet_head(Net_sDataPacket *pkt);

private:
    QReadWriteLock lock;
    uchar mSentBuf[DATA_MSG_SIZE];
};


#endif // NETPACKDATA_H
