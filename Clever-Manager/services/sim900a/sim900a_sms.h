#ifndef SIM900A_SMS_H
#define SIM900A_SMS_H

#include "sim900a_err.h"


class SIM900A_SMS : public QThread
{
    Q_OBJECT
public:
    explicit SIM900A_SMS(QThread *parent = 0);
    ~SIM900A_SMS();

    bool sentSMS(SIM_smsInfo *);
    SIM_smsInfo *readSMS(void);

    void timeoutDone(void);

protected:
    bool clearSms(void);
    bool checkNewSms(int);

signals:
    void sentSmsFailed(void);
    void newSMS(void);

public slots:

private:
    SIM900A *mSim;
    QMutex *mutex;

    QQueue<SIM_smsInfo*> mRecvSmsQue;
};

#endif // SIM900A_SMS_H
