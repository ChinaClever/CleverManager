#ifndef DS_LCDNUMWID_H
#define DS_LCDNUMWID_H

#include <QWidget>

namespace Ui {
class DS_LcdNumWid;
}

class DS_LcdNumWid : public QWidget
{
    Q_OBJECT

public:
    explicit DS_LcdNumWid(QWidget *parent = 0);
    ~DS_LcdNumWid();

    void setUnit(const QString &title, const QString &str);
    void display(const QString &str);
    void display(double data);
    void setDigitCount(int);
    void setStyleSheet(const QString &str);
    void setTitle(const QString &str);

private:
    Ui::DS_LcdNumWid *ui;
};

#endif // DS_LCDNUMWID_H
