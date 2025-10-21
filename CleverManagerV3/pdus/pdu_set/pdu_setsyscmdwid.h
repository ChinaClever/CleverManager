#ifndef SETDEVSYSCMDWID_H
#define SETDEVSYSCMDWID_H

#include "pdu_setuserwid.h"

namespace Ui {
class Pdu_SetSysCmdWid;
}

class Pdu_SetSysCmdWid : public QWidget
{
    Q_OBJECT

public:
    explicit Pdu_SetSysCmdWid(QWidget *parent = 0);
    ~Pdu_SetSysCmdWid();

protected:
    void initWidget();
    void sentCmdData(int fn);
    bool saveNetData();
    void saveLog();
    void setDevCmd();
    bool isChecked();
    bool sentData(Net_sDevData &pkt);

public slots:
    void updateSlot(sDataPacket *packet);

private slots:
    void on_saveBtn_clicked();

private:
    Ui::Pdu_SetSysCmdWid *ui;
    sDataPacket *mPacket;
};

#endif // SETDEVSYSCMDWID_H
