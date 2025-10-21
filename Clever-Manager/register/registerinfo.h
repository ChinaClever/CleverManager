#ifndef REGISTERINFO_H
#define REGISTERINFO_H

#include <QDialog>
#include"activeprocess.h"
#include"activecode.h"
#include<common.h>

namespace Ui {
class RegisterInfo;
}

class RegisterInfo : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterInfo(QWidget *parent = 0);
    ~RegisterInfo();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::RegisterInfo *ui;

    ActiveProcess act;
    ActiveCode active;

    void chande_info();
    void register_info();

    bool is_late;
    bool is_numright;
    bool is_actcode;


    QDate edit_date;
    QString edit_num;
    QString edit_actcode;  
};

#endif // REGISTERINFO_H
