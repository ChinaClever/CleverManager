#ifndef DT_DINGTALK_H
#define DT_DINGTALK_H
#include "dt_cfg.h"
#include <QObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


class DT_DingTalk : public QObject
{
    Q_OBJECT
    explicit DT_DingTalk(QObject *parent = nullptr);
public:
    static DT_DingTalk *bulid(QObject *parent);

    bool talk(const QString &text);
    bool talk(Dt_sCfg *d);
signals:

protected:
    void post(const QString &url, const QString &data);
    void textJson(QJsonObject &json, const QString &msg);
    void atJosn(QJsonObject &json, const QStringList &mobiles, bool isAtAll);

protected slots:
    void requestFinished(QNetworkReply* reply);
    void alarmDingSlot(QString str);
};

#endif // DT_DINGTALK_H
