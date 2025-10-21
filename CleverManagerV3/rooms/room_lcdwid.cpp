/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "room_lcdwid.h"
#include "ui_room_lcdwid.h"

Room_LcdWid::Room_LcdWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Room_LcdWid)
{
    ui->setupUi(this);
}

Room_LcdWid::~Room_LcdWid()
{
    delete ui;
}

/**
 * @brief 设置单位
 * @param str
 */
void Room_LcdWid::setUnit(const QString &title,const QString &str)
{
    ui->titleLab->setText(title);
    ui->label->setText(str);
}

/**
 * @brief 显示数字
 * @param data
 */
void Room_LcdWid::display(double data, int dec)
{
    QString str = QString::number(data, 'f', dec);
    display(str);
}

/**
 * @brief 根据长度调整字体大小
 * @param len
 */
void Room_LcdWid::lenChange(int len)
{
    int size = 18;
    if(len < 4)
        size = 20;
    else if(len < 6)
        size = 18;
    else
        size = 14;

    QFont ft("微软雅黑", size, 75);
    //    ft.setPointSize(size);
    ui->valueLab->setFont(ft);

}

void Room_LcdWid::display(const QString &str)
{
    ui->valueLab->setText(str);
    lenChange(str.length());
}


void Room_LcdWid::setTitle(const QString &str)
{
    ui->titleLab->setText(str);
}


void Room_LcdWid::setSym(const QString &str)
{
    ui->label->setText(str);
}
