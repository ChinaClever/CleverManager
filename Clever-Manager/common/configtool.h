#ifndef CONFIGTOOL_H
#define CONFIGTOOL_H

#include <QObject>
#include <QSettings>

#define SERVER_ADDRESS "192.168.0.164"

class ConfigTool : public QSettings
{
    Q_OBJECT
    explicit ConfigTool(const QString &fileName, Format format, QObject *parent = 0);
public:
    static ConfigTool* get();
    void setValue(const QString &key, const QVariant &value);
//customize need
    QStringList communicationGet();
    void setCommunicationGet(const QStringList& str);
    //主窗口图形
    void setMainWindowGeometry(const QByteArray& ary);
    QByteArray mainWindowGeometry();

    QString getZebraServerIP();
    void setZebraServerIP(const QString& ipAddr);
    int getManaulStatus();
    void setManaulStatus(int status);
    int getLogRecordTime();
    void setLogRecordTime(int time);
    //zebra server info
    QString zebraServerGet();
protected:
    QSettings* m_setting;
};

#endif // CONFIGTOOL_H
