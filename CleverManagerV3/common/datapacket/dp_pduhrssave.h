#ifndef DP_PDUHRSSAVE_H
#define DP_PDUHRSSAVE_H

#include "dp_pdualarm.h"
#include "dbpduhrs.h"

class Dp_PduHrsSave : public QObject
{
    Q_OBJECT
    explicit Dp_PduHrsSave(QObject *parent = nullptr);
public:
    static Dp_PduHrsSave *bulid(QObject *parent=0);

    bool save(sDataPacket *pack);

protected:
    bool saveItem(sDbPduHrsItem &item);
    sDbPduHrsItem initItem();
    QString getSw(int sw);
    bool objData(sObjData &obj);
    void devData(sDevData &dev);
    bool checkCount(int count);

private:
     sDataPacket *mPack;
};

#endif // DP_PDUHRSSAVE_H
