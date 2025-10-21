#ifndef SETTHRESHOLDDLG_H
#define SETTHRESHOLDDLG_H

#include "net_dataanalyze.h"
#include "dbuserlog.h"


namespace Ui {
class Pdu_SetThresholdDlg;
}

enum _SET_CMD_ID{
    SET_CMD_LINE_VOL=1, // 相电压
    SET_CMD_LINE_CUR, // 相电流
    SET_CMD_ENV_TEM, // 温度
    SET_CMD_ENV_HUM, // 湿度
};

class Pdu_SetThresholdDlg : public QDialog
{
    Q_OBJECT

public:
    explicit Pdu_SetThresholdDlg(QWidget *parent = 0);
    ~Pdu_SetThresholdDlg();

    void setMode(int mode, int id, sDataPacket *packet);

protected:
    void initData(int id);
    bool checkData();
    void saveData();
    void saveLog();
    bool sentData();
    uchar *getData(uchar *buf);
    void setSpin(const QString &str, int bit, int max);

private:
    QString getModeStr();


private slots:
    void on_saveBtn_clicked();

private:
    Ui::Pdu_SetThresholdDlg *ui;

    double mRate;
    int mLine, mMode;
    sDataUnit *mUnit;
    sDataPacket *mPacket;
};
extern bool usr_land_jur(void);

#endif // SETTHRESHOLDDLG_H
