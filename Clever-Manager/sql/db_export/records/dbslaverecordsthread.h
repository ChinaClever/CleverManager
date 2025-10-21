#ifndef DBSLAVERECORDSTHREAD_H
#define DBSLAVERECORDSTHREAD_H

#include <QThread>
#include <QTimer>
#include"services/ser_servicdlg.h"
#include "dbtgroom.h"

struct sDbPduDevList
{
    int devType;
    QString ip;
    int devNum;
};

class DbSlaveRecordsThread : public QThread
{
    Q_OBJECT
public:
    explicit DbSlaveRecordsThread(QObject *parent = 0);
    ~DbSlaveRecordsThread();

    static DbSlaveRecordsThread *get(QObject *parent = 0);
    void appendRoomItem(TgRoomItem &item) {mRoomItems.append(item);}
    void appendCabItem(TgCabItem &item) {mCabItems.append(item);}

protected:
    void run();
    void saveSleep(int t);
    int getDevList(QList<sDbPduDevList *> &devs);
    int slaveData();
    void slaveRoomData();
    void slaveCabData();
    bool getSlaveTime();

signals:

public slots:
    void timeoutDone();
    void dbTimeoutDone();

private:
    bool isRun;
    int mTimeCount;
    QTimer *timer;
    QList<TgRoomItem> mRoomItems;
    QList<TgCabItem> mCabItems;
};

#endif // DBSLAVERECORDSTHREAD_H
