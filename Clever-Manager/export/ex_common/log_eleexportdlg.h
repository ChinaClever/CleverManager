#ifndef LOG_ELEEXPORTDLG_H
#define LOG_ELEEXPORTDLG_H

#include "log_exportdlg.h"
#include "log_excelthread.h"
#include "log_exportbarwid.h"

class Log_EleExportDlg : public LOG_ExportDlg
{
    Q_OBJECT
public:
    explicit Log_EleExportDlg(QWidget *parent = 0);

    void setEle(int mode);
    void init(Log_ExportBarWid *bar, int mode);

protected:
     virtual int getProgress(){ return mExcelThread->getProgress();}
     virtual void setEx(Excel_ExportStr &ex) {mExcelThread->set(ex);}

signals:

public slots:

private:
    Log_ExcelThread *mExcelThread;
};

#endif // LOG_ELEEXPORTDLG_H
