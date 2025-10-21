#ifndef UP_TCPSENTTHREAD_H
#define UP_TCPSENTTHREAD_H

#include "up_tcpsent.h"

typedef enum {
    UP_ID_START, // 开始发送
    UP_ID_VERIFY, // 验证正确
    UP_ID_SENT_OK, // 发送完成
}TCP_UP_CMD;

class UP_TcpSentThread : public QThread
{
    Q_OBJECT
public:
    explicit UP_TcpSentThread(QObject *parent = 0);

    void set(sTcpUpdateStr &tcpStr){mTcpSent->set(tcpStr);}
    void append(QStringList &ip){mIpAddr.append(ip);}
    void startSent(void);
    int getErrIp(QStringList &ip){ip = mIpAddr; return mIpAddr.size();}

protected:
    void sentFinsh(void);
    void progress(void);

signals:
    void connectSig(int);
    void sentSig(int, QString &ip); // 1开始，2发送成功
    void sentFinshSig(int,int, int); // 升级个数，成功个数，失败个数
    void progressSig(int); // 百分制

public slots:
    void timeoutDone();
    void connectSlot(int);

private:
    UP_TcpSent *mTcpSent;

    int mNum;
    int mFailNum;
    int mSucNum;
    QString mIP;
    QStringList mIpAddr, mIpErr;
};

#endif // UP_TCPSENTTHREAD_H
