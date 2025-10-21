#ifndef LOG_HEXCELTHREAD_H
#define LOG_HEXCELTHREAD_H

#include "excel_exportlogthread.h"

class Log_HExcelThread : public Excel_ExportLogThread
{
    Q_OBJECT
public:
    explicit Log_HExcelThread(QObject *parent = 0);

protected:
    virtual void readSql(QList<QStringList> &list);

signals:

public :
    QList<QStringList> strList;
};

#endif // LOG_HEXCELTHREAD_H
