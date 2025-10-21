#ifndef SETSWITCHDLG_H
#define SETSWITCHDLG_H

#include "datapacket.h"
#include "net_dataanalyze.h"
#include "dbuserlog.h"

namespace Ui {
class Pdu_SetSwitchDlg;
}

class Pdu_SetSwitchDlg : public QDialog
{
    Q_OBJECT

public:
    explicit Pdu_SetSwitchDlg(QWidget *parent = 0);
    ~Pdu_SetSwitchDlg();

    void setMode(int id, sDataPacket *);

protected:
    void saveData();
    bool sentData();
    uchar *getData(uchar *buf);
    void saveLog();

private slots:
    void on_saveBtn_clicked();

private:
    Ui::Pdu_SetSwitchDlg *ui;

    int mId;
    bool oldSw;
    sDataPacket *mPacket;
};
bool set_pdu_outputSw(QString &ip, int devNum, int bit, int sw);
#endif // SETSWITCHDLG_H
