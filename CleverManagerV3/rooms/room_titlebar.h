#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>

namespace Ui {
class Room_TitleBar;
}

class Room_TitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit Room_TitleBar(QWidget *parent = 0);
    ~Room_TitleBar();

    void setTitle(const QString &title);

private:
    Ui::Room_TitleBar *ui;
};

#endif // TITLEBAR_H
