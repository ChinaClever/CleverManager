#ifndef LOG_CABALARMQUERYDLG_H
#define LOG_CABALARMQUERYDLG_H

#include "log_cabhrsquerydlg.h"
#include "dbcabalarm.h"

class Log_CabAlarmQueryDlg : public Log_CabHrsQueryDlg
{
    Q_OBJECT
public:
    explicit Log_CabAlarmQueryDlg(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // LOG_CABALARMQUERYDLG_H
