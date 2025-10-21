#ifndef DBRTPDUSAVETHREAD_H
#define DBRTPDUSAVETHREAD_H

#include <QtCore>
#include "dbrtpdudevrecord.h"

struct sDbRtPduDevList
{
    int devType;
    QString ip;
    int devNum;
};

class DbRtPduSaveThread : public QThread
{
    Q_OBJECT
public:
    explicit DbRtPduSaveThread(QObject *parent = 0);
    ~DbRtPduSaveThread();
    static DbRtPduSaveThread *get(QObject *parent = 0);

protected:
    void run();
    void getDevList();
    void slaveData();

signals:

public slots:
    void timeoutDone();

private:
    bool isRun;
    QTimer *timer;
    QList<sDbRtPduDevList *> mDevList;

    DbRtPduDevRecord *mRecord;
};

#endif // DBRTPDUSAVETHREAD_H
