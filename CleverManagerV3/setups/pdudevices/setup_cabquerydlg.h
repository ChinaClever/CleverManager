#ifndef SETUP_CABQUERYDLG_H
#define SETUP_CABQUERYDLG_H

#include "setup_pdusquerydlg.h"

namespace Ui {
class Setup_CabQueryDlg;
}

class Setup_CabQueryDlg : public QDialog , public SqlQueryBase
{
    Q_OBJECT

public:
    explicit Setup_CabQueryDlg(QWidget *parent = 0);
    ~Setup_CabQueryDlg();

    void setRoom();
    QString getCmd();
    int Exec() {return this->exec();}

private slots:
    void on_okBtn_clicked();
    void on_quitBtn_clicked();

private:
    Ui::Setup_CabQueryDlg *ui;
};

#endif // SETUP_CABQUERYDLG_H
