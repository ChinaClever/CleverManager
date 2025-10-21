#ifndef UDPBDSENT_H
#define UDPBDSENT_H

#include "udpsentsocket.h"

class UdpBDSent : public QObject
{
    Q_OBJECT
    explicit UdpBDSent(QObject *parent = 0);
public:
    static UdpBDSent *bulid(QObject *parent = 0);

    bool sent(uchar *buf, ushort len, quint16 port=UDP_SENT_PORT);
    bool sent(QByteArray &msg, quint16 port=UDP_SENT_PORT);

private:
    UdpSentSocket *mSocket;
};

#endif // UDPBDSENT_H
