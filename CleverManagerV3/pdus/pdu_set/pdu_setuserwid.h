#ifndef SETDEVUSRWID_H
#define SETDEVUSRWID_H

#include "pdu_setnetwid.h"

namespace Ui {
class Pdu_SetUserWid;
}

class Pdu_SetUserWid : public QWidget
{
    Q_OBJECT

public:
    explicit Pdu_SetUserWid(QWidget *parent = 0);
    ~Pdu_SetUserWid();

protected:
    void initWidget();
    bool checkLand();
    bool sentLand();
    void saveLandLog();
    void setLand();
    bool sentData(Net_sDevData &pkt);

public slots:
    void updateSlot(sDataPacket *packet);

private slots:
    void on_saveBtn_clicked();

private:
    Ui::Pdu_SetUserWid *ui;
    sDataPacket *mPacket;
    QString mUsrName, mPwd;
};

#endif // SETDEVUSRWID_H
