#ifndef TG_LCDNUM_H
#define TG_LCDNUM_H

#include <QWidget>

namespace Ui {
class Room_LcdWid;
}

class Room_LcdWid : public QWidget
{
    Q_OBJECT

public:
    explicit Room_LcdWid(QWidget *parent = 0);
    ~Room_LcdWid();

    void setUnit(const QString &title, const QString &str);
    void display(double data, int dec=0);
    void display(const QString &str);
    void setTitle(const QString &str);
    void setSym(const QString &str);

protected:
    void lenChange(int len);

private:
    Ui::Room_LcdWid *ui;
};

#endif // TG_LCDNUM_H
