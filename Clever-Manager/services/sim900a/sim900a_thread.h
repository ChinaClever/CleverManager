#ifndef SIM900A_THREAD_H
#define SIM900A_THREAD_H

#include "sim900a_call.h"

class SIM900A_Thread : public QThread
{
    Q_OBJECT
public:
    explicit SIM900A_Thread(QObject *parent = 0);
    ~SIM900A_Thread();

    void run(void);
protected:
    bool sentSMS(void);
    void callPhone(void);

signals:
    void simErrSig(int);
    void newSMS(void);

public slots:
    void initFunction(void);
    void timeoutDone(void);

private:
    bool isRun;
    QMutex *mMutex;
    QQueue<SIM_smsInfo *> *mSentSmsQue;

    SIM900A_Call *mCall;
    SIM900A_ERR *mErrSIM;
    SIM900A_SMS *mSMS;

    QTimer *mTimer;
    int mCount;
};

extern void sim_call_phone(QString &phone);
extern void sim_sent_sms(QString &pohne,QString &msg);
extern void sim_sent_sms(QString &msg);

#endif // SIM900A_THREAD_H
