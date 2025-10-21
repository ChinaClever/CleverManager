#ifndef SIM900A_INTERFACE_H
#define SIM900A_INTERFACE_H

#include "sim900a_thread.h"
#include "sim900a_serialdlg.h"

class SIM900A_Interface : public QWidget
{
    Q_OBJECT
public:
    explicit SIM900A_Interface(QWidget *parent = 0);
    ~SIM900A_Interface();

protected:
    void initWidget(void);
    void initLayout(void);
//    void initLabel(void);
//    void setModuLab(void);

signals:

public slots:
//    void simErrSlot(int);
//    void smsBtnSlot(void);

private:
    QLabel *lab;
//    QLabel *stateLab; /*模块状态*/

    QLabel *manuLab;
//    QLabel *manufacturerLab; /*制造商*/

//    QLabel *modu;
//    QLabel *modular; // 模块名

//    QLabel *local;
//    QLabel *localNumbers; //本机号码

//    QLabel *opera;
//    QLabel *operators; //运营商

//    QLabel *signal;
//    QLabel *signalQuality; //信号强度

//    QLabel *smsLab;
//    QPushButton *smsBtn;

    QGridLayout *m_layout;

//    SIM900A_Thread *mSimThread;
};

#endif // SIM900A_INTERFACE_H
