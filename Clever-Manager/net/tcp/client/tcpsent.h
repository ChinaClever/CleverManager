#ifndef TCPSENT_H
#define TCPSENT_H

#include "tcpclient.h"

class TcpSent : public QObject
{
    Q_OBJECT
    explicit TcpSent(QObject *parent = 0);
public:
     static TcpSent *bulid(QObject *parent = nullptr);

    void newConnect(const QString &host); //连接服务器
    bool onceSend(const QString &ip, uchar *buf, int len);
    bool connectCheck(const QString &ip);
protected:
    bool newConnect(void);
    bool sentCheck(void);
    void sentData(const QString &ip, QByteArray &data);

signals:

protected slots:
    void startTimerSlot();
    void timeoutDone();

private:
    TcpClient *mTcpClient;
    QTimer *timer;

    QString mHost;
    QReadWriteLock  *mLock;
    QQueue<QByteArray>  *mArrayQue;
};
void tcp_queue_append(uchar *buf, int len);
bool get_tcp_connect(const QString &ip);
#endif // TCPSENT_H
