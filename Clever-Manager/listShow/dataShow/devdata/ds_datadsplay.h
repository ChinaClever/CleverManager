#ifndef DS_DATADSPLAY_H
#define DS_DATADSPLAY_H

#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLCDNumber>
#include "pdupackettg.h"
#include "ds_lcdnumwid.h"
#include "ds_devstatus.h"

typedef struct
{
    QString ip; //ip
    short num;  //副机
    short line;   //相数 0 表示统计，
}DS_DsyDev;

class DS_DataDsplay : public QWidget
{
    Q_OBJECT
public:
    explicit DS_DataDsplay(QWidget *parent = 0);
    ~DS_DataDsplay();

    void updateDsp(DS_DsyDev &);

signals:

public slots:
    void initFun(void);
    void timeoutDone(void);

protected:
    void updateView();
    void createControl(void);
    void layoutControl(void);
    void dsyDevInfo(void);
    void updateData(void);
    void updataTh();

    void checkAlarm(void);
    void setDefaultColor(void);
    void setAlarmColor(int);

    void clearShow(void);

    bool checkDev(void);
    void checkStatus(void);
    void readData(void);

private:
    QLabel *devLab; // 设备名 IP
    QLabel *ipLab;
    QLabel *groupLab;
    QLabel *lineLab; //相数

    DS_LcdNumWid *mCurLcd;
    DS_LcdNumWid *mVolLcd;
    DS_LcdNumWid *mPowLcd;
    DS_DevStatus *mDevStatus;

    QLabel *eleLab;

    QLabel *tempLab; //温度
    QLabel *humLab;

    QVBoxLayout *dspVLayout;

    QTimer *mTimer;    
    short mAlarm; // 报警 1位 电流报警 2位 电压报警 3位温度报警 4位湿度报警

    DS_DsyDev mDev;
    PduDataPacket *mDataPacket;

    PduPacketTG *mPduTG;
    sPduTGDataStr *mPduData;
};

#endif // DS_DATADSPLAY_H
