#ifndef SNMPDEMO_H
#define SNMPDEMO_H

#include <QObject>
#include "QtSnmpClient.h"
#include <QTimer>

class SnmpDemo : public QObject
{
    Q_OBJECT
public:
    explicit SnmpDemo(QObject *parent = nullptr);

signals:

protected slots:
    void onResponseReceived(const qint32 request_id, const QtSnmpDataList& );
    void onRequestFailed(const qint32 request_id );
    void makeRequest();

private:
    QHostAddress m_address;
    QtSnmpClient *m_snmp_client;
    QTimer  *m_timer;
};

#endif // SNMPDEMO_H
