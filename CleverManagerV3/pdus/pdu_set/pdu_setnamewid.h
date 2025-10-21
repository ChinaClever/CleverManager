#ifndef SETDEVNAMEWID_H
#define SETDEVNAMEWID_H

#include "pdu_setsyscmdwid.h"

namespace Ui {
class Pdu_SetNameWid;
}

class Pdu_SetNameWid : public QWidget
{
    Q_OBJECT

public:
    explicit Pdu_SetNameWid(QWidget *parent = 0);
    ~Pdu_SetNameWid();

protected:
    void initWidget();
    bool checkDevName();
    bool sentDevName();
    void saveDevNameLog();
    void renameDev();
    bool sentData(Net_sDevData &pkt);

public slots:
    void updateSlot(sDataPacket *packet);

private slots:
    void on_saveBtn_clicked();

private:
    Ui::Pdu_SetNameWid *ui;
    sDataPacket *mPacket;
};

#endif // SETDEVNAMEWID_H
