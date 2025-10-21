#ifndef LOG_TGCABDB_H
#define LOG_TGCABDB_H

#include "db_export/records/dbtgcabinet.h"
#include "ex_common/log_common.h"

class LOG_TgCabDb : public QObject
{
    Q_OBJECT
public:
    explicit LOG_TgCabDb(QObject *parent = 0);

    QVector<TgCabItem> getResult();
    QVector<TgCabNameCountItem> getCabNameCount();

    void setRoomName(const QString &name);
    void setRoomCabStr(const QString &roomName , const QString &cabName);
    void setDate(const QDate &start, const QDate &end);
    void setTime(const QTime &start, const QTime &end);
    void setCabName(const QString &name);
    QString getCabNameStr(const QString &name);

    QStringList getCabList();

protected:
    QString getRoomNameStr(const QString &name);
    QVector<TgCabItem> selItems();
    QString getCabByRoomCabStr(const QString &room, const QString &cab);

signals:

public slots:

private:
    DbTgCabinet *mDbTgCab;
    QString mSqlCmd;
};

#endif // LOG_TGCABDB_H
