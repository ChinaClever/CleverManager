#ifndef SIM900A_ERR_H
#define SIM900A_ERR_H

#include "sim900a.h"


typedef enum {
    SIM_OK,
    SIM_COM_ERR, // 串口错误
    SIM_MODU_ERR, //模块错误
    SIM_PIN_ERR, //卡错误
    SIM_COPS_ERR, //运营商错误
    SIM_SMS_ERR, //短信初始化错误
    SIM_MODU_INFO_ERR, // 读模块信息错误
    SIM_PIN_INFO_ERR, //读卡信息错误
}SIM900A_RETURN;


class SIM900A_ERR : public QThread
{
    Q_OBJECT
public:
    explicit SIM900A_ERR(QObject *parent = 0);
    ~SIM900A_ERR();

    void timeoutDone(void);

protected:
   int cleckSim(void);
   int getSimInfo(void);

signals:
    void simErrSig(int);

public slots:

private:
    SIM900A *mSim;
    QMutex *mutex;   

    bool isRun;
};

#endif // SIM900A_ERR_H
