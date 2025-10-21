#ifndef UDPSENTSOCKET_H
#define UDPSENTSOCKET_H

#include <QtCore>
#include <QtNetwork>

#define UDP_SENT_PORT   18750 /*UDP发送端口*/

class UdpSentSocket : public QObject
{
    Q_OBJECT
    explicit UdpSentSocket(QObject *parent = 0);
public:
    static UdpSentSocket *bulid(QObject *parent = 0);
    ~UdpSentSocket();

    bool sentData(const QString &addr,uchar *buf, ushort len, quint16 port=UDP_SENT_PORT);
    bool sentData(const QString &addr, const QByteArray &msg, quint16 port=UDP_SENT_PORT);
    bool sentBroadcastData(uchar *buf, ushort len, quint16 port);
    bool sentBroadcastData(QByteArray &msg, quint16 port);

private:
    QUdpSocket *mUdpSocket;
    QReadWriteLock  *sentLock;
};

#endif // UDPSENTSOCKET_H
