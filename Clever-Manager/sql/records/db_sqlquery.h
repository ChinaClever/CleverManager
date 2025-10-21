#ifndef DB_SQLQUERY_H
#define DB_SQLQUERY_H

#include <QtCore>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlError>


#define LOG_ReCORD_TABLE    "recordLog"
#define LOG_ALARM_TABLE    "alarmLog"
#define LOG_OFFLINE_TABLE    "offLineLog"
#define LOG_LAND_TABLE    "landLog"

typedef struct
{
    QString usr;
    QString title;
    QString operation;
    QString msg;
}s_RecordLog;

typedef s_RecordLog sLandLog;

typedef struct
{
    QString devIP;
    QString devName;
    QString title;
    QString type;
    QString line;
    QString msg;
}s_AlarmLog;

typedef struct
{
    QString devIP;
    QString devName;
    QString title;
    QString msg;
}sOffLineLog;

class DB_SqlQuery
{
public:
    DB_SqlQuery();
    ~DB_SqlQuery();

    int tableSize(const QString &);
    void selectTable(QString &);
    bool insertRecord(s_RecordLog &,const QString &table = LOG_ReCORD_TABLE);
    bool insertLand(sLandLog &log){return insertRecord(log, LOG_LAND_TABLE);}
    bool insertAlarm(s_AlarmLog &);
    bool insertOffLine(sOffLineLog &);

    int selectTableByTime(QString &name, QString &start, QString &end);
    void selectTableByTime(QString &, QString &, QString &, QString &, QList<QStringList> &);
    bool clearTable(const QString &table);
    bool delTable(const QString &table);

private:
     QSqlQuery query;
};

extern bool db_connection(void);
extern bool sql_insert_record(s_RecordLog &log);
extern bool sql_insert_alarm(s_AlarmLog &log);
extern bool sql_insert_offLine(sOffLineLog &log);
extern bool sql_insert_land(sLandLog &log);
extern bool db_select_dateList(const QString &tableName, QStringList &dateList);
#endif // DB_SQLQUERY_H
