#ifndef CLOUDPDUDEV_H
#define CLOUDPDUDEV_H

#include <QObject>
#include "mysql/dc_pdudevices.h"
#include "db_export/records/dbpdudevindex.h"

class CloudPduDev : public QObject
{
    Q_OBJECT
    explicit CloudPduDev(QObject *parent = 0);
public:
     static CloudPduDev* bulid();
     int getId(){return mID;}

     bool save(PduDevIndexItem &item);

protected:
    DC_PduDevItem toItem(PduDevIndexItem &item);
    bool updateStatus(int rid, int status);

signals:

public slots:

private:
    DC_PduDevices *dc;
    int mID;
};

#endif // CLOUDPDUDEV_H
