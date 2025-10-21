#ifndef SETDEVNAMEWID_H
#define SETDEVNAMEWID_H

#include <QWidget>
#include "setswitchdlg.h"
#include "pdu/data/hash/read/pduhashread.h"

namespace Ui {
class SetDevNameWid;
}

class SetDevNameWid : public QWidget
{
    Q_OBJECT

public:
    explicit SetDevNameWid(QWidget *parent = 0);
    ~SetDevNameWid();

protected:
    void initWidget(void);
    bool checkDevName(void);
    bool sentDevName(void);
    void saveDevNameLog(void);
    void renameDev(void);

    bool checkDevMode(void);
    bool sentDevMode(void);
    void saveDevModeLog(void);
    void setDevMode(void);

public slots:
    void updateSlot(QString &ip, int num);

private slots:
    void on_saveBtn_clicked();

private:
    Ui::SetDevNameWid *ui;

    PduDataPacket *mDataPacket;
};

#endif // SETDEVNAMEWID_H
