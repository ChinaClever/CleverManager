#ifndef TG_LCDNUM_H
#define TG_LCDNUM_H

#include <QWidget>

namespace Ui {
class TG_LcdNum;
}

class TG_LcdNum : public QWidget
{
    Q_OBJECT

public:
    explicit TG_LcdNum(QWidget *parent = 0);
    ~TG_LcdNum();

    void setUnit(const QString &title, const QString &str);
    void display(double data, int dec=0);
    void display(const QString &str);
    void setTitle(const QString &str);
    void setSym(const QString &str);

protected:
    void lenChange(int len);

private:
    Ui::TG_LcdNum *ui;
};

#endif // TG_LCDNUM_H
