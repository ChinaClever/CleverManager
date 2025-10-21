#ifndef EMAIL_SETDLG_H
#define EMAIL_SETDLG_H

#include <QDialog>
#include "services/config/sysconfigfile.h"
#include "emailthread.h"
#include "common.h"

namespace Ui {
class Email_SetDlg;
}

class Email_SetDlg : public QDialog
{
    Q_OBJECT

public:
    explicit Email_SetDlg(QWidget *parent = 0);
    ~Email_SetDlg();

    void setData(email_setInfo *);

protected:
    void initWidget(void);
    bool dataCheck(void);

    void saveData(void);

private slots:
    void on_saveBtn_clicked();

    void on_checkBox_clicked(bool checked);

private:
    Ui::Email_SetDlg *ui;

    email_setInfo *mSet;
};

#endif // EMAIL_SETDLG_H
