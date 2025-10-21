#ifndef SHARPNESS_H
#define SHARPNESS_H
#include<QWidget>


class sharpness
{
public:
    sharpness();
    void set_geromy(int widgetX,int widgetY,int widgetWid,int widgetHei,QWidget *widget);

    int currentScreenWid; //当前分辨率的长宽
    int currentScreenHei;
    float factorx;//比例
    float factory;
};

#endif // SHARPNESS_H
