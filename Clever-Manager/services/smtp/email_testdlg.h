#ifndef EMAIL_TESTDLG_H
#define EMAIL_TESTDLG_H

#include <QLabel>
#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QGridLayout>
#include <QInputDialog>
#include<QPalette>
#include "email_setdlg.h"

class Email_TestDlg : public QWidget
{
    Q_OBJECT
public:
    explicit Email_TestDlg(QWidget *parent = 0);
    ~Email_TestDlg();

protected:
    void initWidget(void);
    void initLayout(void);
    void initData(void);

signals:

public slots:
    void clickedBtn(void);
    void setClickedBtn(void);

private:
     QPalette back;
    QLabel *title_emil;

    QLabel *lab;
    QPushButton *btn;

    QLabel *setLab;
    QPushButton *setBtn;

    EmailThread *mEmailThread;
    email_setInfo *mSet;
};
extern QString usr_land_email(void);

#endif // EMAIL_TESTDLG_H
