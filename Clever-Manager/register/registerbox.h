#ifndef REGISTERBOX_H
#define REGISTERBOX_H

#include <QDialog>

namespace Ui {
class RegisterBox;
}

class RegisterBox : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterBox(QPixmap map,QString str,QWidget *parent = 0);
    ~RegisterBox();

    bool is_ok;
    bool is_no;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::RegisterBox *ui;
};

#endif // REGISTERBOX_H
