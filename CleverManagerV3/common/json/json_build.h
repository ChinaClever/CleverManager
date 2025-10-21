#ifndef BUILDJSON_H
#define BUILDJSON_H
#include "datapacket.h"
#include "configbase.h"

#define JSON_VERSION   1

class Json_Build
{
    Json_Build();
public:
    static Json_Build *bulid();
    virtual bool getJson(sDataPacket *packet, QJsonObject &json);
    bool saveJson(const QString &name, QJsonObject &json);

protected:
    virtual void head(sDataPacket *packet,QJsonObject &obj);
    virtual void uutInfo(sDataPacket *packet, QJsonObject &json);
    virtual void pduInfo(sDataPacket *packet, QJsonObject &json);

    int objData(const QString &str, sObjData &ObjData, QJsonObject &obj);
    void thresholdItem(const QString &str, int i, sDataUnit &unit, double rate, QJsonObject &obj);

    void envItem(const QString &str, sDataUnit &unit, double rate, QJsonObject &json);
    void envItem(const QString &str, uchar *index, QJsonObject &obj);
    void envs(sEnvData &ObjData, QJsonObject &json);
    void netAddr(sIpAddr &net, QJsonObject &json);
    void devData(sDataPacket *packet, QJsonObject &obj);
};

#endif // BUILDJSON_H
