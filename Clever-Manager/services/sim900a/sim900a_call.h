#ifndef SIM900A_CALL_H
#define SIM900A_CALL_H

#include "sim900a_sms.h"

class SIM900A_Call : public QThread
{
    Q_OBJECT
public:
    SIM900A_Call();
    ~SIM900A_Call();

    bool callPhone(QString phone);
    void timeoutDone(void);

signals:
    void callPhoneSig(QString);

public slots:

private:
    SIM900A *mSim;
    QMutex *mutex;
};

#endif // SIM900A_CALL_H
