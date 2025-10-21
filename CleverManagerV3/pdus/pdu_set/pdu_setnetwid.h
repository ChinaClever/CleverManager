#ifndef SETDEVNETWID_H
#define SETDEVNETWID_H

#include "pdu_setthresholddlg.h"

namespace Ui {
class Pdu_SetNetWid;
}

class Pdu_SetNetWid : public QWidget
{
    Q_OBJECT

public:
    explicit Pdu_SetNetWid(QWidget *parent = 0);
    ~Pdu_SetNetWid();

protected:
    void initWidget();
    void setMode(char);
    bool sentData(Net_sDevData &pkt);

    bool sentNetMode();
    bool checkMode();
    bool saveNetMode();

    bool checkNetData();
    void sentNetData(int fn,QString &str);
    bool saveNetData();

    bool saveNetAddr();
    void saveLog();
    void clearWidget();

public slots:
    void updateSlot(sDataPacket *packet);

private slots:
    void on_comboBox_currentIndexChanged(int index);
    void on_saveBtn_clicked();

private:
    Ui::Pdu_SetNetWid *ui;
    sDataPacket *mPacket;
};

#endif // SETDEVNETWID_H
