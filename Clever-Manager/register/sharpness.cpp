#include "sharpness.h"
#include<QApplication>
#include<qdesktopwidget.h>


sharpness::sharpness()
{

}


/*
 * 根据原有控件尺寸大小，重新根据比例设置大小
 */
void sharpness::set_geromy(int widgetX,int widgetY,int widgetWid,int widgetHei,QWidget *widget)
{
    currentScreenWid = QApplication::desktop()->width();
    currentScreenHei = QApplication::desktop()->height();

    factorx =  currentScreenWid /1920.0;
    factory = currentScreenHei/1080.0;

    int nWidgetX = (int)(widgetX*factorx);
    int nWidgetY = (int)(widgetY*factory);
    int nWidgetWid = (int)(widgetWid*factorx);
    int nWidgetHei = (int)(widgetHei*factory);
    widget->setGeometry(nWidgetX,nWidgetY,nWidgetWid,nWidgetHei);

}
