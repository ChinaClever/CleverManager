/*
 * sim900a_thread.cpp
 * sim 线程
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "sim900a_thread.h"

static QMutex *mutex = new QMutex;
static QQueue<SIM_smsInfo*> *sentSmsQue = new QQueue<SIM_smsInfo*>; /*发信息队列*/
static QQueue<QString> callQue;
extern bool usr_landFlag;
extern int usr_telephone_list(QStringList &phone);

void sim_call_phone(QString &phone)
{
    callQue.enqueue(phone);
}

/**
 * 发送信息
 */
void sim_sent_sms(QString &pohne,QString &msg)
{
    SIM_smsInfo *smsInfo = new SIM_smsInfo;
    smsInfo->pohneNum = pohne;
    smsInfo->msg = msg;

    QMutexLocker locker(mutex);
    sentSmsQue->enqueue(smsInfo);
}

void sim_sent_sms(QString &msg)
{
    QStringList phone;
    usr_telephone_list(phone);

    for(int i=0; i<phone.size(); ++i)
        sim_sent_sms(phone[i], msg);
}


SIM900A_Thread::SIM900A_Thread(QObject *parent) : QThread(parent)
{
    isRun = false;
    mErrSIM = new SIM900A_ERR(this);
    connect(mErrSIM,SIGNAL(simErrSig(int)),this,SIGNAL(simErrSig(int)));

    mCall = new SIM900A_Call;

    mSMS = new SIM900A_SMS(this);
    connect(mSMS,SIGNAL(newSMS()),this,SIGNAL(newSMS()));

    mMutex = mutex;
    mSentSmsQue = sentSmsQue;
    QTimer::singleShot(1000,this,SLOT(initFunction()));
}

SIM900A_Thread::~SIM900A_Thread()
{
    isRun = false;
    wait();
}


/**
 * @brief 延时启动线程
 */
void SIM900A_Thread::initFunction(void)
{
    mCount = 0;
    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(timeoutDone()));
    mTimer->start(1000);
}

/**
 * @brief 定时启动线程
 */
void SIM900A_Thread::timeoutDone(void)
{
    if(isRun == false) {
        isRun = true;
        start();
    }
}


/**
 * @brief 发送信息
 * @return
 */
bool SIM900A_Thread::sentSMS(void)
{
    bool ret = true;

    int rtn = mSentSmsQue->size();
    if(rtn>0)
    {
        QMutexLocker locker(mMutex);
        SIM_smsInfo *msg = mSentSmsQue->dequeue();
        ret = mSMS->sentSMS(msg);

        delete msg;
    }

    return ret;
}

/**
 * @brief 打电话
 */
void SIM900A_Thread::callPhone(void)
{
    int rtn = callQue.size();
    if(rtn>0)
    {
        QString phone = callQue.dequeue();
        mCall->callPhone(phone);
    }
}



void SIM900A_Thread::run(void)
{
    if(usr_landFlag) // 没登录不运行
    {
        if(g_sim_isOpen)
        {
            mCount++;
            if(mCount > 5)
            {
                mCount = 0;
                mErrSIM->timeoutDone();
                mCall->timeoutDone();
                //mSMS->timeoutDone();
            }
            callPhone();
            sentSMS();
            usleep(1);
        }
    }
    isRun = false;
}

