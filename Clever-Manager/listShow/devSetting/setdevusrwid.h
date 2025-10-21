#ifndef SETDEVUSRWID_H
#define SETDEVUSRWID_H

#include <QWidget>
#include "setdevnamewid.h"

namespace Ui {
class SetDevUsrWid;
}

class SetDevUsrWid : public QWidget
{
    Q_OBJECT

public:
    explicit SetDevUsrWid(QWidget *parent = 0);
    ~SetDevUsrWid();

protected:
    void initWidget(void);
    bool checkLand(void);
    bool sentLand(void);
    void saveLandLog(void);
    void setLand(void);

public slots:
    void updateSlot(QString &ip, int num);

private slots:
    void on_saveBtn_clicked();

private:
    Ui::SetDevUsrWid *ui;
    PduDataPacket *mDataPacket;
    QString mUsrName, mPwd;
};

#endif // SETDEVUSRWID_H
