#ifndef LOG_ROOMHRSQUERYDLG_H
#define LOG_ROOMHRSQUERYDLG_H

#include <QDialog>
#include "logcomwid.h"
#include "dbcabhrs.h"

namespace Ui {
class Log_RoomHrsQueryDlg;
}

class Log_RoomHrsQueryDlg : public QDialog, public SqlQueryBase
{
    Q_OBJECT

public:
    explicit Log_RoomHrsQueryDlg(QWidget *parent = 0);
    ~Log_RoomHrsQueryDlg();

    virtual QString getCmd();
    int Exec() {return this->exec();}
    virtual bool inputCheck() {return true;}

protected slots:
    void on_okBtn_clicked();
    void on_quitBtn_clicked();

protected:
    Ui::Log_RoomHrsQueryDlg *ui;
    SqlDateBar *mDateBar;
};

#endif // LOG_ROOMHRSQUERYDLG_H
