#ifndef PDUDLSAVETHREAD_H
#define PDUDLSAVETHREAD_H

#include <QtCore>
#include "pdudevaddrthread.h"

class PduDLSaveThread : public QThread
{
    Q_OBJECT
public:
    explicit PduDLSaveThread(QObject *parent = 0);
    ~PduDLSaveThread();

protected:
    void run(void);

signals:

public slots:
    void timeoutDone(void);

private:
    bool isRun;
    QTimer *mTimer;

    PduDevAddrThread *devAddr;
};

#endif // PDUDLSAVETHREAD_H
