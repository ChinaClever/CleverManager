#ifndef LOG_CABELECQUERYDLG_H
#define LOG_CABELECQUERYDLG_H
#include "dbcabelec.h"
#include "log_cabalarmquerydlg.h"

namespace Ui {
class Log_CabHrsQueryDlg;
}

class Log_CabElecQueryDlg : public Log_CabHrsQueryDlg
{
    Q_OBJECT

public:
    explicit Log_CabElecQueryDlg(QWidget *parent = 0);
    ~Log_CabElecQueryDlg();

    QString getCmd();

protected:
    bool inputCheck();
    QStringList getCabs();
    QStringList getCmds();

};

#endif // LOG_CABELECQUERYDLG_H
