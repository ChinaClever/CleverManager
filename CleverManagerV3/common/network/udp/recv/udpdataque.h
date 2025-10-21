#ifndef UDPDATAQUE_H
#define UDPDATAQUE_H
#include "udprecvsocket.h"

class UdpDataQueue
{
    UdpDataQueue();
public:
    static UdpDataQueue *bulid();

    void udp_data_add(UdpBaseData *data);
    int udp_data_size(void);
    UdpBaseData *udp_data_get(void);

private:
    QReadWriteLock  *gLock;
    QQueue<UdpBaseData*> *gUdpQueueData;
};



#endif // UDPDATAQUE_H
