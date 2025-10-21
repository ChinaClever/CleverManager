#ifndef LOG_EXCELTHREAD_H
#define LOG_EXCELTHREAD_H

#include "excel_exportlogthread.h"

class Log_ExcelThread : public Excel_ExportLogThread
{
    Q_OBJECT
public:
    explicit Log_ExcelThread(QObject *parent = 0);

protected:
    virtual void readSql(QList<QStringList> &list);

signals:

public slots:
};

#endif // LOG_EXCELTHREAD_H
