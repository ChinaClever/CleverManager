/*
 * dialpan.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "dialpan.h"
#include <QPainter>
#include <QPen>
#include <QRectF>
#include <QLinearGradient>
#include <QPaintEvent>

dialpan::dialpan(QWidget *parent) :
    QLabel(parent)
{
    resize(170,120);
    this->setRateValue(0);
    update();
}

dialpan::~dialpan() {

}

//重写paintEvent函数
void dialpan::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    //移动坐标系,使得坐标原点在label的下方中间
    painter.translate(width() / 2, height());
    int side = qMin(width(), height());
    painter.scale(side / 120, side / 120.0);
    painter.save();

    QColor co1 = QColor(77, 106, 147);
    QColor co2 = QColor(255, 157, 85);

    /*pen1是最外边的大圆弧*/
    QLinearGradient linearGradient = QLinearGradient(-90, 0, 90, 0);
    linearGradient.setColorAt(0.0, Qt::black);
    linearGradient.setColorAt(0.8, co1);
    linearGradient.setColorAt(1, co2);

    painter.setPen(QPen(linearGradient, 5));
    QRectF rectangle(-82, -82, 164, 164);
    int startAngle = 0 * 16;
    int spanAngle = 180 *16;
    //画圆弧
    painter.drawArc(rectangle, startAngle, spanAngle);

    /*pen2是次里面的圆弧*/
    QPen pen2(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen2);
    QRectF rectangle2(-75.0, -75.0, 150.0, 150.0);
    int startAngle2 = 0 * 16;
    int spanAngle2 = 180 * 16;
    painter.drawArc(rectangle2, startAngle2, spanAngle2);

    /*pen3是最里面的那个圆弧*/
    QPen pen3(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen3);
    QRectF rectangle3(-60.0, -60.0, 120.0, 120.0);
    int startAngle3 = 0 * 16;
    int spanAngle3 = 180 * 16;
    painter.drawArc(rectangle3, startAngle3, spanAngle3);

    /*pen4用来处理动态显示*/
    QLinearGradient linearGradient3 = QLinearGradient(-68, 0, 68, 0);
    linearGradient3.setColorAt(0, Qt::cyan);
    linearGradient3.setColorAt(0.6, Qt::yellow);
    linearGradient3.setColorAt(1, Qt::red);

    painter.setPen(QPen(linearGradient3, 10));
    QRectF rectangle4(-68.0, -68.0, 136.0, 136.0);
    int startAngle4 = 0 * 16;
    int spanAngle4 = 0 * 16;

    //default status, the pan will be 0
    startAngle4 = 0 * 16;
    spanAngle4 = 0 * 16;

    startAngle4 = (1 - (this->getRateValue() / 65)) * 180 * 16;
    spanAngle4 = (this->getRateValue() / 65) * 180 * 16;

    painter.drawArc(rectangle4, startAngle4, spanAngle4);
   // painter.save();

    /*用画笔3,画表盘刻度*/
    painter.setPen(pen3);
    for (int i = 0; i < 11; i++) {
        painter.drawLine(68, 0, 75, 0);
        painter.rotate(-18);
    }
    painter.restore();
}

/*设置ratevalue*/
void dialpan::setRateValue(double ratevalue) {
    this->ratevalue = ratevalue;
}

/*得到ratevalue*/
double dialpan::getRateValue() {
    return this->ratevalue;
}
