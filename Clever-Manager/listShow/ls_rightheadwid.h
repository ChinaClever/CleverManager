#ifndef LS_RIGHTHEADWID_H
#define LS_RIGHTHEADWID_H

#include <QWidget>
#include"common.h"

namespace Ui {
class LS_RightHeadWid;
}

class LS_RightHeadWid : public QWidget
{
    Q_OBJECT

public:
    explicit LS_RightHeadWid(QWidget *parent = 0);
    ~LS_RightHeadWid();

signals:
    void selectSig(int);

private slots:
//    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::LS_RightHeadWid *ui;
    int number;

};

extern bool getAlarmSignal();


#endif // LS_RIGHTHEADWID_H
