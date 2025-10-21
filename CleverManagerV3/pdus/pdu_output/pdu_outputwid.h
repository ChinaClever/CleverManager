#ifndef PDU_OUTPUTWID_H
#define PDU_OUTPUTWID_H

#include "comtablewid.h"


class Pdu_OutputWid : public ComTableWid
{
    Q_OBJECT
public:
    explicit Pdu_OutputWid(QWidget *parent = nullptr);

protected:
    void initWid();
    void setAlarm(int id, sObjData &unit);
    void setObjUnit(int id, sObjData &unit);
    int updateDev(sDevData &dev);
    void updateData();

    void setOutput(int row);
    void switchControl(int row);

public slots:
    void timeoutDone();
    void itemDoubleSlot();
    void packetSlot(sDataPacket *p) {mPacket=p;}

private:
    sDataPacket *mPacket;
};

#endif // PDU_OUTPUTWID_H
