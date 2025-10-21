#ifndef SETDEVSYSCMDWID_H
#define SETDEVSYSCMDWID_H

#include <QWidget>
#include "setdevsmtpwid.h"

namespace Ui {
class SetDevSysCmdWid;
}

class SetDevSysCmdWid : public QWidget
{
    Q_OBJECT

public:
    explicit SetDevSysCmdWid(QWidget *parent = 0);
    ~SetDevSysCmdWid();

protected:
    void initWidget(void);
    void sentCmdData(int fn);
    bool saveNetData(void);
    void saveLog(void);
    void setDevCmd(void);
    bool isChecked();

public slots:
    void updateSlot(QString &ip, int num);

private slots:
    void on_saveBtn_clicked();

    void on_updateBtn_clicked();

private:
    Ui::SetDevSysCmdWid *ui;
    PduDataPacket *mDataPacket;
};

#endif // SETDEVSYSCMDWID_H
