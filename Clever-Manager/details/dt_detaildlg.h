#ifndef DT_DETAILDLG_H
#define DT_DETAILDLG_H

#include <QDialog>
#include <QGridLayout>
#include "ds_devinfotable.h"

namespace Ui {
class DT_DetailDlg;
}

class DT_DetailDlg : public QDialog
{
    Q_OBJECT

public:
    explicit DT_DetailDlg(QWidget *parent = 0);
    ~DT_DetailDlg();

     void updateWidget(QString &,short);

private:
    Ui::DT_DetailDlg *ui;

    DS_DevInfoTable *m_pDevInfo;
};

#endif // DT_DETAILDLG_H
