#ifndef UP_TCPSENT_H
#define UP_TCPSENT_H

#include "up_tcpclient.h"

/*代号段结构体*/
typedef struct
{
   QString file; // 升级文件
   QString usr; // 用户名
   QString pwd; // 密码
}sTcpUpdateStr;

class UP_TcpSent : public QThread
{
    Q_OBJECT
public:
    explicit UP_TcpSent(QObject *parent = 0);
    ~UP_TcpSent();

    void set(sTcpUpdateStr &tcpStr);
    bool connectServer(const QString &ip);
    void close(){mTcpClient->closeConnect();}

protected:
    bool readFile(const QString &fn);
    bool sentLen(void);
    bool startSent(void);

    bool recvVerify(void);
    bool sentData(void);
    void stepMode(void);

signals:
    void connectSig(int);

public slots:
    void connectSlot(int);
    void timeoutDone();

private:
    UP_TcpClient *mTcpClient;
    sTcpUpdateStr mTcpUpdateStr;
    QByteArray mByFile;
    bool isRun, isStart;
};

#endif // UP_TCPSENT_H
