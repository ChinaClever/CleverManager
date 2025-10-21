#ifndef CONFIGBASH
#define CONFIGBASH
#include "sysconfigfile.h"

#define PUSH_UDP_SIZE 2

struct sConfigItem
{
    int sound;
    double elePrice;
    int logTimes; // 日志间隔时间分钟
    int logCount;

    int wsEn;
    QString wsUrl;
    int wsPort;
    int reTime;

    int udpEn[PUSH_UDP_SIZE];
    QString udpUrl[PUSH_UDP_SIZE];
    int udpPort[PUSH_UDP_SIZE];

    int envEn;
    int netAddrEn;
    int thresholdEn;
    int pushTime;
};

class ConfigBase
{
    ConfigBase();
public:
    static ConfigBase *bulid();

    sConfigItem *item;

    int getPushTime();
    void setPushTime(int s);

    int getReTime();
    void setReTime(int s);

    int getThresholdEn();
    void setThresholdEn(int s);

    int getEnvEn();
    void setEnvEn(int s);

    int getNetEn();
    void setNetEn(int s);

    int getUdpPort(int id);
    void setUdpPort(int id, int s);

    QString getUdpUrl(int id);
    void setUdpUrl(int id, QString &url);

    int getUdpEn(int id);
    void setUdpEn(int id, int s);

    int getWsPort();
    void setWsPort(int s);

    QString getWsUrl();
    void setWsUrl(QString &url);

    int getLogTime();
    void setLogTime(int num);

    int getLogCount();
    void setLogCount(int count);

    double getElePrice();
    void setElePrice(double p);

    int getSound();
    void setSound(int s);

    int getWsEn();
    void setWsEn(int s);

    virtual QString getPrefix(){return "con";}
};

#endif // CONFIGBASH
