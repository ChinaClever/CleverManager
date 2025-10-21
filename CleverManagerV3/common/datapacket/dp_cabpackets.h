#ifndef DP_CABPACKETS_H
#define DP_CABPACKETS_H

#include "dp_cabhrssave.h"

class Dp_CabPackets : public Dp_BasicThread<sCabPacket *>
{
    Q_OBJECT
    explicit Dp_CabPackets(QObject *parent = nullptr);
public:
    static Dp_CabPackets *bulid(QObject *parent = nullptr);

    sCabPacket *get(const QString &room, const QString &cab);
    sCabPacket *getByCab(uint id);
    QVector<sCabPacket *> getByRoom(uint id);

    void delCab(uint id);
    void delRoom(uint id);

protected:
    void initFun();
    void workDown(sCabPacket *pack);
    void setPacket(CabinetItem &it, sDataPacket *pack);

    void initPacket(CabinetItem &it);
    void dels(QVector<sCabPacket *> &packs);

    void tgCabData(sCabPacket *cab);
    int getStatus(sDataPacket *pack);
    QColor getColor(int status);
    sCabPacket *newDataPacket() {return new sCabPacket;}


private slots:
    void cabinetItemChange(int, int);

private:
    int mCount;
    Dp_PduPackets *mPdus;
    DbCabinetList *mDb;
    Dp_CabHrsSave *mCabHrs;
    Dp_CabAlarm *mAlarm;
};

#endif // DP_CABPACKETS_H
