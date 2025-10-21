#ifndef UP_MAINDLG_H
#define UP_MAINDLG_H

#include <QDialog>
#include "up_ipswid.h"
#include "upgradewid.h"
#include "up_okstabwid.h"
#include "up_devselectwid.h"

namespace Ui {
class Up_MainDlg;
}

class Up_MainDlg : public QDialog
{
    Q_OBJECT

public:
    explicit Up_MainDlg(QWidget *parent = 0);
    ~Up_MainDlg();

protected slots:
    void initFunSLot();

private:
    Ui::Up_MainDlg *ui;

    QTimer* timer;
    Up_IpsWid *mIpsWid;
    UpgradeWid *mUpgradeWid;
    Up_IpsTabWid *mIpsTabWid;
    Up_OksTabWid *mOksTabWid;
    Up_ErrsTabWid *mErrsTabWid;
    Up_DevSelectWid *mDevSelectWid;
};

#endif // UP_MAINDLG_H
