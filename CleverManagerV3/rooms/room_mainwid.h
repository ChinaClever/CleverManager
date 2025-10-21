#ifndef TG_ROOMWIDGET_H
#define TG_ROOMWIDGET_H

#include <QWidget>
#include "room_datawid.h"
#include "room_bargraph.h"

namespace Ui {
class Room_MainWid;
}

class Room_MainWid : public QWidget
{
    Q_OBJECT

public:
    explicit Room_MainWid(QWidget *parent = 0);
    ~Room_MainWid();


protected:
    void initWid();


signals:
    void roomSelectSig(sRoomPacket *p);

protected slots:
    void itemChangedSlot(int,int);
    void initFunSLot();

private slots:
    void on_upBtn_clicked();
    void on_downBtn_clicked();
    void on_comboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::Room_MainWid *ui;

    Room_BarGraph *mBar;
    Room_DataWid *mData;
    Room_PowerGraph *mPow;
    Room_VcGraph *mVc;
};

//extern QString comtext;

#endif // TG_ROOMWIDGET_H
