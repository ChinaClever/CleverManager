#include "titlebar.h"
#include "ui_titlebar.h"
#include "common.h"

TitleBar::TitleBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TitleBar)
{
    ui->setupUi(this);

}

TitleBar::~TitleBar()
{
    delete ui;
}

void TitleBar::setTitle(const QString &title)
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
