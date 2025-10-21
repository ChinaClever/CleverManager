#ifndef BARSLIDER_H
#define BARSLIDER_H

#include <QWidget>
#include <QFrame>
#include <QPainter>
#include "common.h"

class BarSlider : public QFrame
{
    Q_OBJECT
public:
    explicit BarSlider(QWidget *parent = 0);
    ~BarSlider();

    void setMaxValue(double maxValue); //设置最大值
    double getMaxValue(); //得到最大值

    void setMinValue(double minvalue);  //设置最小值
    double getMinValue();  //得到最小值

    void setValue(double Value); //设置值
    double getValue();  //取得值

protected:
    void paintEvent(QPaintEvent *);

signals:

public slots:

private:
    double maxvalue;   //滑动条最大值
    double minvalue;   //滑动条最小值
    double value;   //设置显示的值
};

#endif // BARSLIDER_H
