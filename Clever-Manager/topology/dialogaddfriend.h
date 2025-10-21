#ifndef DIALOGADDFRIEND_H
#define DIALOGADDFRIEND_H

#include <QDialog>

namespace Ui {
class DialogAddFriend;
}

class DialogAddFriend : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddFriend(QWidget *parent = 0);
    ~DialogAddFriend();
    QString get_invite_code();
    QString get_ipaddr();
private slots:
    void on_pushButton_ok_clicked();
private:
    Ui::DialogAddFriend *ui;
};

#endif // DIALOGADDFRIEND_H
