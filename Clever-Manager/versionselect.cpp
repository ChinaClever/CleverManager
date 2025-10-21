#include "versionselect.h"
#include "ui_versionselect.h"

VersionSelect::VersionSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VersionSelect)
{
    number=0;
    ui->setupUi(this);


}

VersionSelect::~VersionSelect()
{
    delete ui;
}



void VersionSelect::on_pushButton_clicked()
{
    QString str=ui->comboBox->currentText();
    if(str == "中文")
    {
        number=1;
    }
    //    else if(str == "英文")
    //    {
    //        number=2;
    //    }
    number=ui->comboBox->currentIndex()+1;
    this->close();
}
