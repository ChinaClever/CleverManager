#ifndef CLOUDOFFLINE_H
#define CLOUDOFFLINE_H
#include "mysql/dc_devofflines.h"

struct CloudOffLineItem {
    QString room, cab, ip;
    int devType, num;
    QString msg;
};

class CloudOffLine
{
    CloudOffLine();
public:
    static CloudOffLine* bulid();

    void saveLog(CloudOffLineItem &item);

protected:
    int getRid(CloudOffLineItem &item);
};

#endif // CLOUDOFFLINE_H
