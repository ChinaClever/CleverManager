#ifndef DS_DATASHOWWID_H
#define DS_DATASHOWWID_H

#include <QProgressBar>
#include "pdu_linegraph.h"

namespace Ui {
class Pdu_LineWid;
}


class Pdu_LineWid : public QWidget
{
    Q_OBJECT

public:
    explicit Pdu_LineWid(QWidget *parent = 0);
    ~Pdu_LineWid();

signals:
    void selectDevSig(sDataPacket *p, int line);

private:
    void clearShow();
    bool checkDev();
    void dsyDevInfo();
    void initLineName();

    QString getValueStr(int data, double rate, int bit=0);
    void setProBarValue(QProgressBar *, int, int, int);

    void updateCur();
    void updateVol();
    void updatePf();
    void updatePow();
    void updateEle();
    void updateData();

    void setDefaultColor();
    void checkStatus();
    void setThreshold(int mode, int id=0);

public slots:
    void timeoutDone();
    void packetSlot(sDataPacket *p);

private slots:
    void on_comboBox_currentIndexChanged(int index);
    void on_webBtn_clicked();
    void on_curBtn_clicked();
    void on_volBtn_clicked();

private:
    Ui::Pdu_LineWid *ui;

    int mLine;
    QTimer *mTimer;
    sDataPacket *mPacket;
    Pdu_LineGraph *mGraph;
};
#endif // DS_DATASHOWWID_H
