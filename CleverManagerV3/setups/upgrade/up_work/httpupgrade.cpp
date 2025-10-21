#include "httpupgrade.h"
extern void udp_sent_data(const QString &ip, uchar *buf, ushort len);

HttpUpgrade::HttpUpgrade(QObject *parent) : UpgradeThread(parent)
{
    quint16 port = 1180;
    mDaemon = new HttpDaemon(port, this);
    connect(mDaemon, SIGNAL(progressSig(float,QString)), this, SLOT(subProgressSlot(float,QString)));
}

HttpUpgrade::~HttpUpgrade()
{
    breakSent();
    wait();
}

bool HttpUpgrade::upload(const QString &file, const QString &ip)
{
    char *msg = "http upgrade";
    UdpSentSocket::bulid()->sentData(ip,(uchar *)msg , strlen(msg) , UDP_SENT_PORT);
    mDaemon->shareFile(file);
    mDaemon->Sleep(50000);
    int count = 0;
    while(!mDaemon->mRet && count < 100)
    {
        if(!mDaemon->mRet)
        {
            count++;
            mDaemon->Sleep(1000);
            continue;
        }
    }

    return mDaemon->mRet;
}

void HttpUpgrade::breakSent()
{
    mDaemon->breakDown();
}
