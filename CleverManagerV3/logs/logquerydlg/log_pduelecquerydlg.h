#ifndef LOG_PDUELECQUERYDLG_H
#define LOG_PDUELECQUERYDLG_H
#include "dbpduelec.h"
#include "log_pdualarmquerydlg.h"

namespace Ui {
class Log_PduAlarmQueryDlg;
}

class Log_PduElecQueryDlg : public Log_PduAlarmQueryDlg
{
    Q_OBJECT
public:
    explicit Log_PduElecQueryDlg(QWidget *parent = nullptr);

    QString getCmd();

protected:
    bool inputCheck();
    QStringList getItems();
    QStringList getCmds();

protected slots:
    void on_devnumCmb_currentIndexChanged(int index);
};

#endif // LOG_PDUELECQUERYDLG_H
