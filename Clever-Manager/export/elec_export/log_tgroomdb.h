#ifndef LOG_TGROOMDB_H
#define LOG_TGROOMDB_H

#include "db_export/records/dbtgroom.h"
#include "ex_common/log_common.h"

class LOG_TgRoomDb : public QObject
{
    Q_OBJECT
public:
    explicit LOG_TgRoomDb(QObject *parent = 0);

    QVector<TgRoomItem> getResult();

    void setRoomName(const QString &name);
    void setDate(const QDate &start, const QDate &end);
    void setTime(const QTime &start, const QTime &end);

    QStringList getRoomList();

protected:
    QString getRoomNameStr(const QString &name);
    QVector<TgRoomItem> selItems();

signals:

public slots:

private:
    DbTgRoom *mDbTgRoom;
    QString mSqlCmd;
};

#endif // LOG_TGROOMDB_H
