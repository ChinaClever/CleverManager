#ifndef SETTHRESHOLDDLG_H
#define SETTHRESHOLDDLG_H

#include <QDialog>
#include "pduhashdata.h"
#include "net/tcp/client/tcpsent.h"
#include "net/udp/sent/udpbdsent.h"
#include "sql/records/db_sqlquery.h"
#include "common.h"

namespace Ui {
class SetThresholdDlg;
}

typedef enum {
    SET_CMD_LINE_VOL=1, // 相电压
    SET_CMD_LINE_CUR, // 相电流
    SET_CMD_ENV_TEM, // 温度
    SET_CMD_ENV_HUM, // 湿度
}_SET_CMD_ID;

class SetThresholdDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SetThresholdDlg(QWidget *parent = 0);
    ~SetThresholdDlg();

    void setMode(int mode, int line,PduDataPacket *);
protected:
    void initData(int line);
    bool checkData(void);
    void saveData(void);
    void saveLog(void);
    bool sentData(void);
    uchar *getData(uchar *buf);

private:
    void getModeStr(QString &);


private slots:
    void on_saveBtn_clicked();

private:
    Ui::SetThresholdDlg *ui;

    double mRate;
    int mLine, mMode;
    QString mSymbol;
    PduDataUnit *mUnit;
    PduDataPacket *mPacket;
};
extern bool usr_land_jur(void);

#endif // SETTHRESHOLDDLG_H
