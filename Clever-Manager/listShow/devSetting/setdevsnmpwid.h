#ifndef SETDEVSNMPWID_H
#define SETDEVSNMPWID_H

#include <QWidget>
#include "setdevnetwid.h"

namespace Ui {
class SetDevSnmpWid;
}

class SetDevSnmpWid : public QWidget
{
    Q_OBJECT

public:
    explicit SetDevSnmpWid(QWidget *parent = 0);
    ~SetDevSnmpWid();

protected:
    void initWidget(void);
    bool checkData(void);
    void sentNetData(int fn,QString &str);
    bool saveNetData(void);
    void saveLog(void);
    bool saveSnmp(void);

public slots:
    void updateSlot(QString &ip, int num);

private slots:
    void on_saveBtn_clicked();

private:
    Ui::SetDevSnmpWid *ui;
    PduDataPacket *mDataPacket;
    PduNetSNMP *mSnmp;
};

#endif // SETDEVSNMPWID_H
