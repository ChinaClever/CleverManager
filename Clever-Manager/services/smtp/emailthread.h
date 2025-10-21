#ifndef EMAILTHREAD_H
#define EMAILTHREAD_H

#include "email.h"


class EmailThread : public QThread
{
    Q_OBJECT
public:
    explicit EmailThread(QObject *parent = 0);
    ~EmailThread();

protected:
    void run(void);

    bool getMessage(QString &subject, QString &body);
    void setEmail(void);
    bool checkIp(const QString& ip);

signals:

public slots:
     void finishedSlot(int ret);
     void initFunction();
     void timeoutDone();

private:
    Email *mEmail;
    bool isRun,isStart;

    QTimer *mTimer;
};

#endif // EMAILTHREAD_H
