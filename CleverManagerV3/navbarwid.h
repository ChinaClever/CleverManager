#ifndef NAVBARWID_H
#define NAVBARWID_H

#include "usermainwid.h"

namespace Ui {
class NavBarWid;
}

class NavBarWid : public QWidget
{
    Q_OBJECT

public:
    explicit NavBarWid(QWidget *parent = 0);
    ~NavBarWid();
    UsrLandDlg* mUserLand;


signals:
    void navBarSig(int);

protected:
    void enableBtn();

public slots:
    void recvUserNameSlot(QString str);

private slots:
    void on_userBtn_clicked();
    void on_setupBtn_clicked();
    void on_logBtn_clicked();

    void on_pduBtn_clicked();

    void on_roomBtn_clicked();

    void on_tpBtn_clicked();

    void on_cabBtn_clicked();

    void changeToCabSlot();

private:
    void setBtndefault(QPushButton *btn, const QString &str);
    void initBtnColour();
    void setBtnClicked(QPushButton *btn, const QString &str);
    void initLabText();
    void initBtnQStr();

private:
    Ui::NavBarWid *ui;
    QMap<QPushButton*,QString> mBtnQStr;
    QLabel* mUserLab;
    QString mUserName;
};

#endif // NAVBARWID_H
