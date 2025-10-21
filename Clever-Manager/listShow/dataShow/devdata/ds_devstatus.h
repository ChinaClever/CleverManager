#ifndef DS_DEVSTATUS_H
#define DS_DEVSTATUS_H

#include <QWidget>
#include "pdu/data/hash/read/pduhdckeck.h"

namespace Ui {
class DS_DevStatus;
}

class DS_DevStatus : public QWidget
{
    Q_OBJECT

public:
    explicit DS_DevStatus(QWidget *parent = 0);
    ~DS_DevStatus();

    void updateWidget(QString &ip, short num, short line);

protected:
    void updateSwTitle(int line);
    void updateSw();

    QString getStatus(PduDataUnit *);
    void updateStatus();

    void setHiddenWid(short line);

protected slots:
    void timeoutDone();

private:
    Ui::DS_DevStatus *ui;
    PduDataPacket *mDataPacket;
    int mLine;
    QTimer *mTimer;
    bool isAlarm;
};

#endif // DS_DEVSTATUS_H
