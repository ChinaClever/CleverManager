#ifndef SETOUTPUTDLG_H
#define SETOUTPUTDLG_H

#include "pdu_setswitchdlg.h"

namespace Ui {
class Pdu_SetOutputDlg;
}

class Pdu_SetOutputDlg : public QDialog
{
    Q_OBJECT

public:
    explicit Pdu_SetOutputDlg(QWidget *parent = 0);
    ~Pdu_SetOutputDlg();

    void setMode(int id, sDataPacket *packet);

protected:
    void initData(int id);
    bool checkData();

    void saveLog();
    uchar *getData(uchar *buf);
    bool sentData();
    void saveData();

private slots:
    void on_saveBtn_clicked();

private:
    Ui::Pdu_SetOutputDlg *ui;

    int mId;
    sDataPacket *mPacket;
};

#endif // SETOUTPUTDLG_H
