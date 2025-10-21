#include "dt_dingtalk.h"

DT_DingTalk::DT_DingTalk(QObject *parent) : QObject(parent)
{

}

DT_DingTalk *DT_DingTalk::bulid(QObject *parent)
{
    static DT_DingTalk* sington = nullptr;
    if(sington == nullptr)
        sington = new DT_DingTalk(parent);
    return sington;
}

void DT_DingTalk::textJson(QJsonObject &json, const QString &msg)
{
    json.insert("msgtype", "text");

    QJsonObject obj;
    obj.insert("content", msg);
    json.insert("text", QJsonValue(obj));
}

void DT_DingTalk::atJosn(QJsonObject &json, const QStringList &mobiles, bool isAtAll)
{
    QJsonObject obj;
    if(!isAtAll) {
        QJsonArray jsonArray;
        for(int i=0; i<mobiles.size(); ++i) {
            jsonArray.append(mobiles.at(i));
        }

        if(mobiles.size())
            obj.insert("atMobiles", QJsonValue(jsonArray));
    }
    obj.insert("isAtAll",  isAtAll);

    json.insert("at", QJsonValue(obj));
}

bool DT_DingTalk::talk(const QString &text)
{
    Dt_sCfg *cfg = DT_Cfg::bulid()->cfg;
    cfg->content = text;
    return talk(cfg);
}

bool DT_DingTalk::talk(Dt_sCfg *d)
{
    if(d->en) {
        QJsonObject json;
        QString text = QString("【%1】").arg(d->key) + d->content;
        textJson(json, text);
        atJosn(json, d->mobiles, d->isAtAll);

        QJsonDocument doc(json);
        QString data = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
        post(d->webhook, data);
    }

    return d->en;
}


void DT_DingTalk::post(const QString &url, const QString &data)
{
    QNetworkAccessManager* naManager = new QNetworkAccessManager(this);
    QMetaObject::Connection connRet = QObject::connect(naManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished(QNetworkReply*)));
    Q_ASSERT(connRet);

    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");
    QNetworkReply* reply = naManager->post(request, data.toUtf8());
}

// 获取http状态码
void DT_DingTalk::requestFinished(QNetworkReply* reply)
{
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(statusCode.isValid()) {
        if(200 != statusCode.toInt())
            qDebug() << "status code=" << statusCode.toInt();
    }

    QVariant reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    if(reason.isValid()) {
        if("OK" != reason.toString())
            qDebug() << "reason=" << reason.toString();
    }

    QNetworkReply::NetworkError err = reply->error();
    if(err != QNetworkReply::NoError) {
        qDebug() << "Failed: " << reply->errorString();
    } else {  // 获取返回内容
        QString str = QString(reply->readAll());
        if(!str.contains("ok")) qDebug() << str;
    }
}
