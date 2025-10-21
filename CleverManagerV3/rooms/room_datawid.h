#ifndef ROOM_DATAWID_H
#define ROOM_DATAWID_H

#include "room_lcdwid.h"
#include "room_powergraph.h"

namespace Ui {
class Room_DataWid;
}

class Room_DataWid : public QWidget
{
    Q_OBJECT

public:
    explicit Room_DataWid(QWidget *parent = 0);
    ~Room_DataWid();

protected:
    void initView();
    void changeCurMode(long double);
    void changePowMode(long double);
    void updateData(sTgObjData &);
    bool checkPack();

public slots:
    void timeoutDone();
    void packetSlot(sRoomPacket *p) {mPacket=p;}

private:
    Ui::Room_DataWid *ui;
    QTimer *timer;
    Room_LcdWid *mCurLcd;
    Room_LcdWid *mVolLcd;
    Room_LcdWid *mPowLcd;
    Room_LcdWid *mPfLcd;
    sRoomPacket *mPacket;
};

#endif // ROOM_DATAWID_H
