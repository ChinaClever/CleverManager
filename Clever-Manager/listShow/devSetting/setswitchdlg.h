#ifndef SETSWITCHDLG_H
#define SETSWITCHDLG_H

#include "setoutputdlg.h"

namespace Ui {
class setSwitchDlg;
}

class setSwitchDlg : public QDialog
{
    Q_OBJECT

public:
    explicit setSwitchDlg(QWidget *parent = 0);
    ~setSwitchDlg();

    void setMode(int, QString &, PduDataPacket *);
protected:
    void saveData(void);
    bool sentData(void);
    uchar *getData(uchar *buf);
    void saveLog(void);

private slots:
    void on_saveBtn_clicked();

private:
    Ui::setSwitchDlg *ui;

    int mLine;
    bool oldSw;
    PduSWControl *mSw;
    PduDataPacket *mPacket;
};
bool set_pdu_outputSw(QString &ip, int devNum, int bit, int sw);
#endif // SETSWITCHDLG_H
