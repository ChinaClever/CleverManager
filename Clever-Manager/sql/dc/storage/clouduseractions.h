#ifndef CLOUDUSERACTIONS_H
#define CLOUDUSERACTIONS_H
#include "mysql/dc_useractions.h"

class CloudUserActions
{
    CloudUserActions();
public:
    static CloudUserActions *bulid();

    void slaveLog(QString &name, QString &op, QString &msg);
};

#endif // CLOUDUSERACTIONS_H
