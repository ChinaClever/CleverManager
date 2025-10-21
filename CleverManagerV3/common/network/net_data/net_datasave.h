#ifndef NET_DATASAVE_H
#define NET_DATASAVE_H

#include "net_devdata.h"

class Net_DataSave
{
    Net_DataSave();
public:
    static Net_DataSave *bulid();

    void dataSave(Net_sPacket *packet);

protected:
    void dataFun(sDataPacket *dev,Net_sDevData *data);
    void outputName(QString *name, Net_sDevData *data);
    void user(QString &user, QString &pwd, Net_sDevData *data);
    void iPAddr(sIpAddr &ip,Net_sDevData *data);
    void devName(QString &name,Net_sDevData *data);

private:
    Net_DevData *mDevData;
    Net_DataCom *mDataCom;
};

#endif // NET_DATASAVE_H
