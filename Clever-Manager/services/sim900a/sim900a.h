#ifndef SIM900A_H
#define SIM900A_H

#include "sim900a_serial.h"

typedef struct
{
    QString manufacturer; // 制造商
    QString modular;    //模块名字
    QString serialNumber; //产品序列号
    QString localNumbers; //本地号码
}SIM_ModuInfo;

typedef struct
{
    bool flag;  //是否有卡
    QString operators; //运营商名字
    QString signalQuality;  //信号质量
    QString batteryPower; //电池电量
    QString dateTime; // 日期
}SIM_gsmInfo;


typedef struct
{
    bool flag;  //读状态
    QString pohneNum; //来自
    QString dateTime;  //时间
    QString msg; //信息
}SIM_smsInfo;

class SIM900A: public QObject
{
public:
    SIM900A(QObject *parent = 0);
    ~SIM900A();

    void test(void);

    bool sim900a_AT_cmd(void);
    bool simp00a_cpin(void);
    bool simp900a_cops(void);

    bool sim900a_modu_info(SIM_ModuInfo &);
    bool sim900a_gsm_info(SIM_gsmInfo &);

    bool sim900a_call_init(void);
    bool sim900a_call_hangUp(void);
    bool sim900a_call_Connect(void);
    bool sim900a_call_Phone(QString&);
    bool sim900a_answer_Phone(QString &);

    bool sim900a_sms_init(void);
    bool sim900a_sms_count(short &, short &);
    bool sim900a_sms_read(short, SIM_smsInfo *);
    bool sim900a_sms_send(SIM_smsInfo *);
    bool sim900a_sms_del(short,short);

protected:
    void serialSent(QString &);
    void serialSent(uchar );
    bool serialRecv(QString &);

    bool sim900a_check_cmd(QString);
    bool sim900a_check_ack(QString , short );
    bool sim900a_send_cmd(QString ,QString , short );
    bool sim900a_send_cmd(uchar ,QString , short );

    void sim900a_unigbk_GBK(QString &);
    void sim900a_unigbk_exchange(QString , QString &);

private:
    SIM900A_Serial serialCom;// SIM串口类
    SerialDev *mSerial;
    QString mAckStr;
};

extern QMutex g_sim_mutex;

#endif // SIM900A_H
