#ifndef LOG_PDUDEVDB_H
#define LOG_PDUDEVDB_H

#include <QObject>
#include "sql/db_export/records/dbpdudevindex.h"
#include "sql/db_export/records/dbpduelecparams.h"

class LOG_PduDevDb : public QObject
{
    Q_OBJECT
public:
    explicit LOG_PduDevDb(QObject *parent = 0);

    void setDev(const QString &ip, int num);
    void setDate(const QDate &start, const QDate &end);
    void setTime(const QTime &start, const QTime &end);

    QVector<PduDevIndexItem> getResult(QVector<PduElecParamsItem> &start,
                                       QVector<PduElecParamsItem> &end);

protected:
    QVector<PduDevIndexItem> selItems();
    QVector<PduDevIndexItem> getIndexResult();

signals:

public slots:

private:
    DbPduDevIndex *mDevIndex;
    DbPduElecParams *mDevParams;

    QString mSqlCmd;
};

#endif // LOG_PDUDEVDB_H
