#ifndef JSON_PUSH_H
#define JSON_PUSH_H
#include "json_build.h"
#include "websocketclient.h"

class Json_Push : public QObject
{
    Json_Push(QObject *parent = nullptr);
public:
    static Json_Push *bulid(QObject *parent = nullptr);
    bool push(sDataPacket *packet);

protected:
    void udpPush(QString &str);
    void wsPush(QString &str);
    bool check(int count);

private:
    Json_Build *mJson;
    sConfigItem *mItem;
    WebSocketClient *mSocket;
};

#endif // JSON_PUSH_H
