#ifndef TFTPTHREAD_H
#define TFTPTHREAD_H

#include <QThread>
#include "tftp.h"

class TftpThread : public QThread
{
    Q_OBJECT
public:
    explicit TftpThread(QObject *parent = 0);
    ~TftpThread();

    void clear(void);
    void setFile(const QString &file);
    void append(QStringList &ip);
    void set(const QString &ip, const QString &file);

    void startSent(void);
    int getErrList(QStringList &errList);

protected:
    void run(void);
    bool sentFile(void);
    void sentProgress(void);
    void sentFinsh(void);

signals:
    void sentSig(int, QString ip); // 1开始，2发送成功，3发送失败, 4没有目标地址
    void sentFinshSig(int,int, int); // 升级个数，成功个数，失败个数
    void progressSig(int); // 百分制

public slots:

private:
    Tftp *mTftp;

    int mCount;
    QStringList mIpAddr;
    QString mFile; // 需要发送的IP地址和文件

    QStringList mErrList;
};

#endif // TFTPTHREAD_H
