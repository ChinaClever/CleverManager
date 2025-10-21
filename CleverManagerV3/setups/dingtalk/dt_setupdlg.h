#ifndef DT_SETUPDLG_H
#define DT_SETUPDLG_H

#include <QDialog>
#include "dt_dingtalk.h"

namespace Ui {
class DT_SetupDlg;
}

class DT_SetupDlg : public QDialog
{
    Q_OBJECT

public:
    explicit DT_SetupDlg(QWidget *parent = nullptr);
    ~DT_SetupDlg();

protected:
    void initWid();
    bool inputCheck();
    bool getCfg();

private slots:
    void on_oneRb_clicked(bool checked);
    void on_okBtn_clicked();
    void on_testBtn_clicked();
    void on_cancelBtn_clicked();

private:
    Ui::DT_SetupDlg *ui;
};

#endif // DT_SETUPDLG_H
