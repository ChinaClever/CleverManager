#ifndef LOG_CABHRSQUERYDLG_H
#define LOG_CABHRSQUERYDLG_H

#include <QDialog>
#include "logcomwid.h"
#include "dbcabhrs.h"

namespace Ui {
class Log_CabHrsQueryDlg;
}

class Log_CabHrsQueryDlg : public QDialog, public SqlQueryBase
{
    Q_OBJECT

public:
    explicit Log_CabHrsQueryDlg(QWidget *parent = 0);
    ~Log_CabHrsQueryDlg();

    virtual QString getCmd();
    int Exec() {return this->exec();}
    virtual bool inputCheck() {return true;}

protected:
    void setRoadHidden();

protected slots:
    void on_okBtn_clicked();
    void on_quitBtn_clicked();

protected:
    Ui::Log_CabHrsQueryDlg *ui;
    SqlDateBar *mDateBar;
};

#endif // LOG_CABHRSQUERYDLG_H
