#ifndef CLOUDSETDLG_H
#define CLOUDSETDLG_H

#include <QDialog>
#include "cloudsetparam.h"

namespace Ui {
class CloudSetDlg;
}

class CloudSetDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CloudSetDlg(QWidget *parent = 0);
    ~CloudSetDlg();

protected:
    bool checkDomain();
    bool checkName();
    bool checkIdc();
    bool checkModule();

private slots:
    void initFunSLot();
    void on_saveBtn_clicked();

    void on_domainBtn_clicked();

    void on_nameBtn_clicked();

    void on_idcBtn_clicked();

    void on_moduleBtn_clicked();

private:
    Ui::CloudSetDlg *ui;
    CloudSetParam *mParam;
};

#endif // CLOUDSETDLG_H
