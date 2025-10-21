#ifndef CLOUDSERDEV_H
#define CLOUDSERDEV_H

#include <QObject>
#include "mysql/dc_serverdevices.h"
#include "sql/database/dbconsumerdevices.h"

class CloudSerDev : public QObject
{
    Q_OBJECT
    explicit CloudSerDev(QObject *parent = 0);
public:
    static CloudSerDev* bulid();

    void del(int id);

protected:
    DC_SerDevItem toItem(int id);
    void modify(DC_SerDevItem &dcItem, QString &name);
    void save(int id); // 保存机房

protected slots:
    void itemChangedSlot(int,int);
    void updateSlot(int id, QString name);

private:
    DC_ServerDevices *dc;
    DbConsumerDevices *mDev;
    enum{Remove,Insert,Update};
};

#endif // CLOUDSERDEV_H
