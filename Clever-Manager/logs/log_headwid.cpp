#include "log_headwid.h"
#include "ui_log_headwid.h"

LOG_HeadWid::LOG_HeadWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LOG_HeadWid)
{
    ui->setupUi(this);
//    button_style_sheet(ui->pushButton);
    connect(ui->comboBox,SIGNAL(currentIndexChanged(QString)), this,SLOT(selectSlot(QString)));
    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)), this,SIGNAL(selectSig(int)));

    ////====  lzy 2018/6/7
    ui->comboBox->removeItem(ui->comboBox->count()-1);
}

LOG_HeadWid::~LOG_HeadWid()
{
    delete ui;
}


void LOG_HeadWid::selectSlot(QString str)
{
    ui->titleLab->setText(str);
}





