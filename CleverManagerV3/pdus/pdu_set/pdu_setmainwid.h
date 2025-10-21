#ifndef SETDEVMAINWID_H
#define SETDEVMAINWID_H

#include "pdu_setnamewid.h"

namespace Ui {
class Pdu_SetMainWid;
}

class Pdu_SetMainWid : public QWidget
{
    Q_OBJECT

public:
    explicit Pdu_SetMainWid(QWidget *parent = 0);
    ~Pdu_SetMainWid();

protected:
    void initWidget();

signals:
    void selectSig(sDataPacket *packet);


private:
    Ui::Pdu_SetMainWid *ui;

    Pdu_SetNameWid *mDevName;
    Pdu_SetUserWid *mDevUsr;
    Pdu_SetNetWid *mDevNet;
    Pdu_SetSysCmdWid *mSysCmd;
};

#endif // SETDEVMAINWID_H
