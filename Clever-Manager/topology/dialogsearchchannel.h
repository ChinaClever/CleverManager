#ifndef DIALOGSEARCHCHANNEL_H
#define DIALOGSEARCHCHANNEL_H

#include <QDialog>

namespace Ui {
class dialogsearchchannel;
}

class dialogsearchchannel : public QDialog
{
    Q_OBJECT

public:
    explicit dialogsearchchannel(QWidget *parent = 0);
    ~dialogsearchchannel();

private slots:
    void on_ok_pushButton_clicked();

    void on_cancle_pushButton_clicked();

private:
    Ui::dialogsearchchannel *ui;
};

#endif // DIALOGSEARCHCHANNEL_H
