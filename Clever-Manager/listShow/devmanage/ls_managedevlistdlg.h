#ifndef LS_MANAGEDEVLISTDLG_H
#define LS_MANAGEDEVLISTDLG_H

#include <QDialog>
#include "ls_devmainspl.h"

namespace Ui {
class LS_ManageDevListDlg;
}

class LS_ManageDevListDlg : public QDialog
{
    Q_OBJECT

public:
    explicit LS_ManageDevListDlg(QWidget *parent = 0);
    ~LS_ManageDevListDlg();

protected:
    void initWidget(void);

private slots:

private:
    Ui::LS_ManageDevListDlg *ui;

    LS_DevMainSpl *mSpl;
};

#endif // LS_MANAGEDEVLISTDLG_H
