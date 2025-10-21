#ifndef DIALPAN_H
#define DIALPAN_H

#include <QLabel>
#include <QWidget>
#include <QPaintEvent>
#include "common.h"

class dialpan : public QLabel
{
    Q_OBJECT
public:
    explicit dialpan(QWidget *parent = 0);
    ~dialpan();
    void setRateValue(double ratevalue);
    double getRateValue();
signals:
private:
    /*ratevalue为表盘的rate值*/
    int ratevalue;
protected:
    void paintEvent(QPaintEvent *);

public slots:
};

#endif // DIALPAN_H
