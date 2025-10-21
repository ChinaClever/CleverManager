#ifndef JSON_RECV_H
#define JSON_RECV_H
#include "json_build.h"
#include <QUdpSocket>

struct sJsonCmd
{
    sJsonCmd() {}
    bool status;
    int dev_num, pduType, dev_spec;
    QString ip, dev_type, dev_name;

    int id, sw, fn, len;
    double min, cr_min;
    double max, cr_max;
    double rate;
};

class Json_Recv :  public QObject
{
    Q_OBJECT
    explicit Json_Recv(QObject *parent = nullptr);
public:
    ~Json_Recv();
    static Json_Recv *bulid(QObject *parent = nullptr);
    bool recv(const QString &msg);

protected:
    QJsonValue getValue(QJsonObject &object, const QString &key);
    QJsonObject getObject(QJsonObject &object, const QString &key);
    QJsonArray getArray(QJsonObject &object, const QString &key);
    QString getString(QJsonObject &object, const QString &key);
    double getData(QJsonObject &object, const QString &key);

    bool company(QJsonObject &object);
    bool versionNumber(QJsonObject &object);
    bool headInfo(QJsonObject &object, sJsonCmd &cmd);

    uchar *getData(sJsonCmd &cmd);
    bool sentData(sJsonCmd &cmd);
    bool itemValues(QJsonObject &obj, sJsonCmd &cmd, const QString &key);
    bool lineItem(QJsonObject &obj, sJsonCmd &cmd);
    bool lineItemList(QJsonObject &object, sJsonCmd &cmd);

    bool setSwitch(sJsonCmd &cmd);
    bool outputItem(QJsonObject &obj, sJsonCmd &cmd);
    bool outputItemList(QJsonObject &object, sJsonCmd &cmd);

    bool temItemList(QJsonObject &object, sJsonCmd &cmd);
    bool humItemList(QJsonObject &object, sJsonCmd &cmd);


    bool envInfo(QJsonObject &object, sJsonCmd &cmd);
    bool analyticalData(QJsonObject &object);
    void Sleep(int msec);


protected slots:
    void processPendingDatagram();

private:
    QUdpSocket *mUdpSocket;
};

#endif // JSON_RECV_H
