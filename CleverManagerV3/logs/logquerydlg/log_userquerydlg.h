#ifndef LOG_LANDQUERYDLG_H
#define LOG_LANDQUERYDLG_H

#include <QDialog>
#include "logcomwid.h"
#include "dbuserlog.h"

namespace Ui {
class Log_UserQueryDlg;
}

class Log_UserQueryDlg : public QDialog, public SqlQueryBase
{
    Q_OBJECT
public:
    explicit Log_UserQueryDlg(QWidget *parent = 0);
    ~Log_UserQueryDlg();

    QString getCmd();
    int Exec() {return this->exec();}

private slots:
    void on_quitBtn_clicked();
    void on_okBtn_clicked();

private:
    Ui::Log_UserQueryDlg *ui;
    SqlDateBar *mDateBar;
};

#endif // LOG_LANDQUERYDLG_H
