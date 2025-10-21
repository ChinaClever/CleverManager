/*
 * emailthread.cpp
 *  邮件发送线程
 *      1、实现，一份一份发送邮件功能
 *      2、使用定时器发送邮件
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "emailthread.h"
static QReadWriteLock *gLock = new QReadWriteLock();
static QStringList gSubjectList, gBodyList; // 所有的邮件信息，都会保存至此链表中

/**
 * @brief 发送邮件  调用此函数会把信息保存至发送区 邮件得一条一条的发送
 * @param subject 标题
 * @param body 内容
 */
void email_sent_message(QString &subject, QString &body)
{
    QWriteLocker locker(gLock);
    gSubjectList.append(subject);
    gBodyList.append(body);
}


EmailThread::EmailThread(QObject *parent) : QThread(parent)
{
    isRun = isStart = false;
    mEmail = new Email(this);
    mSet = email_get_setInfo();
    connect(mEmail, SIGNAL(finishedSig(int)),this, SLOT(finishedSlot(int)));

    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(timeoutDone()));
    mTimer->start(1300);
    //    QTimer::singleShot(100,this,SLOT(initFunction()));
}


EmailThread::~EmailThread()
{
    isRun = false;
    wait();
}

void EmailThread::initFunction()
{
    start();
}

/**
 * @brief 获取发送内容
 * @param subject 标题
 * @param body  内容
 * @return true 表示需要发送邮件
 */
bool EmailThread::getMessage(QString &subject, QString &body)
{
    bool ret = true;
    if((gSubjectList.size() > 0) && (gBodyList.size() > 0))
    {
        QWriteLocker locker(gLock);
        subject = gSubjectList.first();
        body = gBodyList.first();

    }
    else
        ret = false;

    return ret;
}

/**
 * @brief 邮件发送完成响应函数
 */
void EmailThread::finishedSlot(int ret)
{
    QWriteLocker locker(gLock);
    if(ret == 0)
    {
        if((gSubjectList.size() > 0) && (gBodyList.size() > 0))
        {
            gSubjectList.removeFirst();
            gBodyList.removeFirst();
        }
    }
    if((gSubjectList.size() > 1000) && (gBodyList.size() > 1000))
    {
        gSubjectList.clear();
        gBodyList.clear();
    }
    isStart = false;
}

/**
 * @brief 发送邮件
 */
void EmailThread::setEmail(void)
{
    QString subject,body;
    bool ret = getMessage(subject,body);
    if(ret)
    {
        if(!mSet->to.isEmpty()) {
            isStart = true;
            QStringList recipient;
            recipient << mSet->to;
            mEmail->sentEmail(recipient,subject,body);
        }
    }
}

/**
 * @brief 定时器发送邮件
 */
void EmailThread::timeoutDone()
{
    if(isStart == false)
    {
        bool online = false;
        online = checkIp(mSet->server);
        if(online) {
            setEmail();
        } else {
            QWriteLocker locker(gLock);
            if((gSubjectList.size() > 0) && (gBodyList.size() > 0))
            {
                gSubjectList.clear();
                gBodyList.clear();
            }
        }
    }
}

/**
 * @brief 线程发送邮件
 */
void EmailThread::run(void)
{
    isRun = true;
    while(isRun)
    {
        timeoutDone();
        msleep(100);
    }
}

/**
 * @brief ping 检测邮件服务器是否能通讯
 */
bool EmailThread::checkIp(const QString& ip)
{
    QProcess pingProcess;
    QString strArg = "ping " + ip + " -n 1 -i 2";  //strPingIP 为设备IP地址
    pingProcess.start(strArg,QIODevice::ReadOnly);
    pingProcess.waitForFinished(-1);

    QString p_stdout = QString::fromLocal8Bit( pingProcess.readAllStandardOutput());
    bool bPingSuccess = false;

    if(p_stdout.contains("TTL=")) //我采用这个字符来判断 对不对？
    {
        bPingSuccess = true;
    }else
    {
        bPingSuccess = false;
    }
    return bPingSuccess;
}
