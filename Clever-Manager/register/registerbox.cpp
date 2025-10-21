#include "registerbox.h"
#include "ui_registerbox.h"
#include<qpixmap.h>
#include<QPalette>

RegisterBox::RegisterBox(QPixmap map,QString str,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterBox)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::WindowTitleHint);
    QPixmap img1(":/images/3.png");
    QPixmap img=img1.scaled(396,149);
    QPalette pal;
    pal.setBrush(QPalette::Background,QBrush(img));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    is_ok=false;
    is_no=false;

    //QPixmap map(":/images/doubt4.png");
    QPixmap msg=map.scaled(50,50);
    ui->label->setPixmap(msg);
    //ui->label_2->setText(tr("是否马上激活"));
    ui->label_2->setText(str);
}

RegisterBox::~RegisterBox()
{
    delete ui;
}

void RegisterBox::on_pushButton_clicked()
{
    is_ok=true;
    is_no=false;
    this->close();
}


void RegisterBox::on_pushButton_2_clicked()
{
    is_no=true;
    is_ok=false;
    this->close();
}
