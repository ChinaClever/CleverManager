#ifndef UDPSOCKETLIST_H
#define UDPSOCKETLIST_H

#include "udpdataque.h"


class UdpSocketList : public QThread
{
    Q_OBJECT
    explicit UdpSocketList(QObject *parent = 0);
public:
    static UdpSocketList *bulid(QObject *parent = 0);
    ~UdpSocketList();

protected:
    void run();
    int recvData();
    void initSocket();

signals:

public slots:

private:
    bool isRun;
    UdpBaseData *mData;
    UdpDataQueue *mQue;
    UdpRecvSocket *mSocket;
    QList<UdpRecvSocket *> *mSocketList;
};

#endif // UDPSOCKETLIST_H
