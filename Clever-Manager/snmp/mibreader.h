#ifndef MIBREADER_H
#define MIBREADER_H

#include <QString>
#include <QThread>

class MibReader : public QThread
{
    Q_OBJECT
public:
    MibReader(QObject* parent = nullptr);
    void setPath(const QString &path);
    int reading(const QString &path);
    QString errMsg(int errType);
protected:
    void run();
signals:
    void progressChange(int pos);
    void progressDbChange(int pos);
    void result(int type);
protected:
    inline bool isNotOid(const QString &oid);
private:
    int m_lineIndex,m_progress;
    qint64 m_currentSize;
    QString m_path;
};

#endif // MIBREADER_H
