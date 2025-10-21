/*
 * barslider.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "barslider.h"

BarSlider::BarSlider(QWidget *parent) : QFrame(parent)
{
    setMaximumSize(160,100);
    setMaxValue(100);
    setMinValue(0);

    setValue(0);
}

BarSlider::~BarSlider()
{

}

void BarSlider::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    //移动坐标系
    painter.translate(20, height() / 2);
    QPen pen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);
    painter.drawLine(0, 40, 0, -40);

    painter.drawLine(0, -30, 110, -30); //画上边直线
    //painter.drawLine(0, 30, 110, 30);  //画下边直线

    //画上下直线刻度以及文字
    int x, y;
    QPen pen2(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPoint point;
    painter.setPen(pen2);
    for (int num = 1; num < 11; num++) {
        x = 10 * num;
        if (num % 2 == 0) {
            y = -20;
            point.setX(x - 8);
            point.setY(y + 10);
            painter.drawText(point, QString::number(10 * num));
        } else {
            y = -25;
        }
        painter.drawLine(x, -30, x, y);
    }

    //设置线性渐变
    QLinearGradient linearGradient = QLinearGradient(0, 0, 200, 0);
    linearGradient.setColorAt(0.0, Qt::cyan);
    linearGradient.setColorAt(0.6, Qt::yellow);
    linearGradient.setColorAt(1, Qt::red);
    //设置画刷
    QBrush qsh(linearGradient);
    painter.setBrush(qsh);

    painter.setPen(Qt::cyan);
    painter.drawRect(0, -5, this->getValue() / this->getMaxValue() * 100, 34);//画矩形

}


void BarSlider::setMinValue(double minValue)
{
    this->minvalue = minValue;
}

double BarSlider::getMinValue()
{
    return minvalue;
}

void BarSlider::setMaxValue(double maxValue)
{
    maxvalue = maxValue;
}

double BarSlider::getMaxValue() //得到最大值
{
    return maxvalue;
}

void BarSlider::setValue(double Value)
{
    this->value = Value;
}

double BarSlider::getValue()
{
    return value;
}
