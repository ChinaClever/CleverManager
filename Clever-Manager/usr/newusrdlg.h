#ifndef NEWUSRDLG_H
#define NEWUSRDLG_H

#include <QDialog>
#include "usrclass.h"
#include "msgBox/msgbox.h"
#include "common.h"

namespace Ui {
class NewUsrDlg;
}

class NewUsrDlg : public QDialog
{
    Q_OBJECT

public:
    explicit NewUsrDlg(QWidget *parent = 0);
    ~NewUsrDlg();

    void editTitle(QString &);
    void loadUsrInfo(UsrStruct *);

    virtual bool saveUsrInfo(UsrStruct *);
    void checkUsr(void);

protected:
    bool informationCheck(void);

private slots:
    void on_saveBtn_clicked();

private:
    Ui::NewUsrDlg *ui;
};

#endif // NEWUSRDLG_H
