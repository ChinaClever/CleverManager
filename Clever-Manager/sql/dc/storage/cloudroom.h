#ifndef CLOUDROOM_H
#define CLOUDROOM_H

#include <QObject>
#include "mysql/dc_rooms.h"
#include "dbroomlist.h"
#include "cloudsetparam.h"
#include "cloudmodule.h"

class CloudRoom : public QObject
{
    Q_OBJECT
    explicit CloudRoom(QObject *parent = 0);
public:
    static CloudRoom* bulid();

    int getRoomId(const QString &name);
    void del(int id);

protected:
    bool save(int id); // 保存机房
    void delRoom(const QString &name);

protected slots:
    void itemChangedSlot(int, int);
    void updateSlot(QString, QString);


private:
    DC_Rooms *dc;
    CloudSetParam *mParam;
    DbRoomList *mDbRoom;
    enum{Remove,Insert,Update};
};

#endif // CLOUDROOM_H
