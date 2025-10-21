#ifndef UDPHEARTBEAT_H
#define UDPHEARTBEAT_H

#include "udpbdsent.h"

class UdpHeartBeat : public QObject
{
    Q_OBJECT
    explicit UdpHeartBeat(QObject *parent = 0);
public:
    static UdpHeartBeat *bulid(QObject *parent);

    void sent();
    void sent(const QString &ip);

private:
    UdpSentSocket *mSocket;
};

#endif // UDPHEARTBEAT_H
