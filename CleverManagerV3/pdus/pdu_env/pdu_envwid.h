#ifndef PDU_ENVWID_H
#define PDU_ENVWID_H

#include "pdu_output/pdu_outputwid.h"

class Pdu_EnvWid : public ComTableWid
{
    Q_OBJECT
public:
    explicit Pdu_EnvWid(QWidget *parent = nullptr);

protected:
    void initWid();
    void initTableWidget();
    void setAlarm(int row, int id, sDataUnit &unit);
    int setDataUnit(int row, sDataUnit &unit, const QString &str);
    int setDoor(int row, uchar *data);
    int setDataOther(int row, uchar *data);
    int updateDev(sEnvData &env);
    void updateData();

public slots:
    void timeoutDone();
    void itemDoubleSlot();
    void packetSlot(sDataPacket *p) {mPacket=p;}

private:
    sDataPacket *mPacket;
};

#endif // PDU_ENVWID_H
