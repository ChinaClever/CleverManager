#ifndef PDUDATATGTHREAD_H
#define PDUDATATGTHREAD_H

#include "pdudatatg.h"

class PduDataTgThread : public QThread
{
    Q_OBJECT
public:
    explicit PduDataTgThread(QObject *parent = 0);
    ~PduDataTgThread();

    void tgCasData(PduDevHash *devHash);
    void tgDevData(PduHashIP *hashIP);
    void tgData(void);

    sPduTGDataStr *getData(void);

protected:
    void run(void);
    void dataTg(void);

signals:

public slots:
    void timeoutDone(void);

private:
    PduDevHash *mDevHash;
    PduHashIP *mHashIP;

    bool isRun;
    QTimer *mTimer;

    int mStep; // 统计数据类型
    PduDataTG *mPduDataTg;

    sPduTGDataStr *mData;
};

#endif // PDUDATATGTHREAD_H
