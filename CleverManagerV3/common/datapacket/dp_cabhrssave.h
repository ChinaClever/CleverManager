#ifndef DP_CABHRSSAVE_H
#define DP_CABHRSSAVE_H


#include "dp_cabalarm.h"

class Dp_CabHrsSave : public QObject
{
    Q_OBJECT
    explicit Dp_CabHrsSave(QObject *parent = nullptr);
public:
    static Dp_CabHrsSave *bulid(QObject *parent);
    void save(sCabPacket *cab);

protected:
    sDbCabHrsItem initItem();
    bool checkCount(int count);
    bool saveItem(sDbCabHrsItem &item);
    bool tgObjData(const QString &road, sTgObjData &obj);
    void dataPacket(const QString &road, sDataPacket *pack);

private:
    sCabPacket *mCab;
};

#endif // DP_CABHRSSAVE_H
