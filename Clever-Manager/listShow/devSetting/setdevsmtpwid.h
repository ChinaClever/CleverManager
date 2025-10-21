#ifndef SETDEVSMTPWID_H
#define SETDEVSMTPWID_H

#include <QWidget>
#include "setdevsnmpwid.h"

namespace Ui {
class SetDevSmtpWid;
}

class SetDevSmtpWid : public QWidget
{
    Q_OBJECT

public:
    explicit SetDevSmtpWid(QWidget *parent = 0);
    ~SetDevSmtpWid();

protected:
    void initWidget(void);
    bool checkData(void);
    void sentNetData(int fn,QString &str);
    void sentNetData(int fn, short port);
    bool saveNetData(void);
    void saveLog(void);
    bool saveSMTP(void);

public slots:
    void updateSlot(QString &ip, int num);

private slots:
    void on_saveBtn_clicked();

private:
    Ui::SetDevSmtpWid *ui;

    PduDataPacket *mDataPacket;
    PduNetSMTP *mSMTP;
};

#endif // SETDEVSMTPWID_H
