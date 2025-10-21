#ifndef LOG_HEXPORTDLG_H
#define LOG_HEXPORTDLG_H

#include "log_exportdlg.h"
#include "log_hexcelthread.h"
#include "ex_common/log_exportbarwid.h"

class Log_HExportDlg : public LOG_ExportDlg
{
    Q_OBJECT
public:
    explicit Log_HExportDlg(QWidget *parent = 0);

    void init(Log_ExportBarWid *bar);
    void init();

protected:
     virtual int getProgress(){ return mExcelThread->getProgress();}
     virtual void setEx(Excel_ExportStr &ex) {mExcelThread->set(ex);}

signals:

public slots:

public:
    Log_HExcelThread *mExcelThread;
};

#endif // LOG_HEXPORTDLG_H
