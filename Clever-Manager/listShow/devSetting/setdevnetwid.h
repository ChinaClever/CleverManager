#ifndef SETDEVNETWID_H
#define SETDEVNETWID_H

#include <QWidget>
#include "setdevusrwid.h"

namespace Ui {
class SetDevNetWid;
}

class SetDevNetWid : public QWidget
{
    Q_OBJECT

public:
    explicit SetDevNetWid(QWidget *parent = 0);
    ~SetDevNetWid();

protected:
    void initWidget(void);
    void setMode(char);

    bool sentNetMode(void);
    bool checkMode(void);
    bool saveNetMode(void);

    bool checkNetData(void);
    void sentNetData(int fn,QString &str);
    bool saveNetData(void);

    bool saveNetAddr(void);
    void saveLog(void);

public slots:
    void updateSlot(QString &ip, int num);

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_saveBtn_clicked();

private:
    Ui::SetDevNetWid *ui;
    PduDataPacket *mDataPacket;
    PduNetIPAddr *mNetIPAddr;
};

#endif // SETDEVNETWID_H
