#ifndef DS_DATASHOWWID_H
#define DS_DATASHOWWID_H

#include <QWidget>
#include <QProgressBar>
#include "pdupackettg.h"
#include "devdata/ds_datashow.h"

namespace Ui {
class DS_DataShowWid;
}

//typedef struct
//{
//    QString ip; //ip
//    short num;  //副机
//    short line;   //相数 0 表示统计，
//}DS_DsyDev;

class DS_DataShowWid : public QWidget
{
    Q_OBJECT

public:
    explicit DS_DataShowWid(QWidget *parent = 0);
    ~DS_DataShowWid();

signals:
    void selectDevSig(DS_DsyDev &);
    void updateSig(void);

private:
    bool checkDev(void);
    void clearShow(void);
    void dsyDevInfo(void);
    void updateView();
    void readData(void);
    QString getValueStr(int data, double rate, int bit=0);

    void setProBarValue(QProgressBar *, int, int, int);
    void updateCur();
    void updateVol();
    void updatePf();
    void updateTem();
    void updateHum();
    void updataTh();
    void updatePow();
    void updateEle();
    void updateData(void);

    void setDefaultColor(void);
    void setAlarmColor(int alarm);
    void checkAlarm(void);
    void checkStatus(void);
    void setThreshold(int mode, int id=0);

public slots:
    void initFun(void);
    void timeoutDone(void);
    void updateData(QString &, int);

private slots:
    void on_comboBox_currentIndexChanged(int index);
    void on_webBtn_clicked();
    void on_curBtn_clicked();
    void on_volBtn_clicked();
    void on_temBtn_clicked();
    void on_humBtn_clicked();

private:
    Ui::DS_DataShowWid *ui;

    QTimer *mTimer;
    short mAlarm; // 报警 1位 电流报警 2位 电压报警 3位温度报警 4位湿度报警

    DS_DsyDev mDev;
    PduDataPacket *mDataPacket;

    PduPacketTG *mPduTG;
    sPduTGDataStr *mPduData;
};

#endif // DS_DATASHOWWID_H
