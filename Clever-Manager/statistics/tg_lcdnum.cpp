#include "tg_lcdnum.h"
#include "ui_tg_lcdnum.h"

TG_LcdNum::TG_LcdNum(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TG_LcdNum)
{
    ui->setupUi(this);
}

TG_LcdNum::~TG_LcdNum()
{
    delete ui;
}

/**
 * @brief 设置单位
 * @param str
 */
void TG_LcdNum::setUnit(const QString &title,const QString &str)
{
    ui->titleLab->setText(title);
    ui->label->setText(str);
}

/**
 * @brief 显示数字
 * @param data
 */
void TG_LcdNum::display(double data, int dec)
{
    QString str = QString::number(data, 'f', dec);
    display(str);
}

/**
 * @brief 根据长度调整字体大小
 * @param len
 */
void TG_LcdNum::lenChange(int len)
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

void TG_LcdNum::display(const QString &str)
{
    ui->valueLab->setText(str);
    lenChange(str.length());
}


void TG_LcdNum::setTitle(const QString &str)
{
    ui->titleLab->setText(str);
}


void TG_LcdNum::setSym(const QString &str)
{
    ui->label->setText(str);
}
