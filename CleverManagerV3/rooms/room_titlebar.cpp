/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "room_titlebar.h"
#include "ui_room_titlebar.h"
#include "common.h"

Room_TitleBar::Room_TitleBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Room_TitleBar)
{
    ui->setupUi(this);

}

Room_TitleBar::~Room_TitleBar()
{
    delete ui;
}

void Room_TitleBar::setTitle(const QString &title)
{
    QString str = "   " + title;
    ui->label->setText(str);
    ui->label->setFont(QFont("微软雅黑",12,75));
    ui->label->setStyleSheet("color:white;");
    ui->label->setTextFormat(Qt::AutoText);
    //   ui->label->setAlignment(Qt::AlignRight);
    //   ui->label->setMinimumHeight(25);

    /*根据长度来调整长度*/
    int len = str.length();
    if(len < 11);
    else if(len < 13)
        len -= 1;
    else
        len -= 2;
    int w = len * 14;
    ui->label->resize(w, 25);
}
