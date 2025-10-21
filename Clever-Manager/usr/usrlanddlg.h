#ifndef USRLANDDLG_H
#define USRLANDDLG_H

#include <QDialog>
#include "usrmanagedlg.h"

namespace Ui {
class UsrLandDlg;
}

class UsrLandDlg : public QDialog
{
    Q_OBJECT

public:
    explicit UsrLandDlg(QWidget *parent = 0);
    ~UsrLandDlg();

    bool landWidget(void);
    bool quitWidget(void);
    int selectWork(void);

protected:
    void usrLand(void);

private slots:
    void on_landBtn_clicked();
    void on_pushButton_clicked();

private:
    Ui::UsrLandDlg *ui;
    UsrClass m_usr;

    UsrMaptype *m_usrMap;
};

//typedef enum{
//    Chinese=1,
//    English,
//}version;

extern QString usr_land_name(void);
extern QString usr_land_telephone(void);
extern QString usr_land_email(void);
extern bool usr_land_jur(void);

extern bool usr_landFlag;
//extern bool is_close;
//extern int getVersion(void);
#endif // USRLANDDLG_H
