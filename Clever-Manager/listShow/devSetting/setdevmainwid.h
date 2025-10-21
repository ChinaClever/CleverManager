#ifndef SETDEVMAINWID_H
#define SETDEVMAINWID_H

#include <QWidget>
#include <QGroupBox>
#include <QGridLayout>
#include "setdevsyscmdwid.h"

namespace Ui {
class SetDevMainWid;
}

class SetDevMainWid : public QWidget
{
    Q_OBJECT

public:
    explicit SetDevMainWid(QWidget *parent = 0);
    ~SetDevMainWid();

    void updateWidget(QString &ip, int num);

protected:
    void initWidget(void);
    void initLayout(void);

signals:
    void selectSig(QString &ip,int num);


private:
    Ui::SetDevMainWid *ui;
    QString mIP;

    SetDevNameWid *mDevName;
    SetDevUsrWid *mDevUsr;
    SetDevNetWid *mDevNet;
    SetDevSnmpWid *mDevSnmp;
    SetDevSmtpWid *mDevSmtp;
    SetDevSysCmdWid *mSysCmd;
};

#endif // SETDEVMAINWID_H
