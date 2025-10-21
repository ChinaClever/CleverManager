#include "ls_rightheadwid.h"
#include "ui_ls_rightheadwid.h"
//#include<qdebug.h>

static bool is_off=false;

LS_RightHeadWid::LS_RightHeadWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LS_RightHeadWid)
{
    number=0;
//    ui->pushButton->setVisible(false);
    //QPixmap img(":/images/other/switch-on-icon.png");
    //img.scaled(ui->pushButton_2->width(),ui->pushButton_2->height());
    //ui->pushButton_2->setIcon(QIcon(img));
    //    ui->pushButton_2->setIcon(QIcon(":/images/other/switch-on-icon.png"));
    //    ui->pushButton_2->setIconSize(QSize(75,23));
    //         ui->pushButton_2->setStyleSheet("border-image: url(:/images/other/switch-on-icon.png);");
    ui->setupUi(this);
    ui->pushButton_2->setStyleSheet("QPushButton{background-color:rgb(0,45,130);}"
                                      "QPushButton:hover{background-color:rgb(191, 237, 238);}"
                                  "QPushButton:pressed{background-color:rgb(143, 185, 181);}");
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(selectSig(int)));
}

LS_RightHeadWid::~LS_RightHeadWid()
{
    delete ui;
}

//void LS_RightHeadWid::on_pushButton_clicked()
//{
//    ui->comboBox->setCurrentIndex(3);
//    //    emit selectSig(3);

//}


/**关闭报警按钮，点一次关，再点一次为开
 * @brief LS_RightHeadWid::on_pushButton_2_clicked
 */
void LS_RightHeadWid::on_pushButton_2_clicked()
{

    number++;
    if(number%2)
    {
        is_off=true;
        ui->pushButton_2->setText(tr("打开报警"));
    }
    else
    {
         ui->pushButton_2->setText(tr("关闭报警"));
        //         ui->pushButton_2->setStyleSheet("QToolButton{border-image: url(:/images/other/switch-on-icon.png);}");
        is_off=false;
        number=0;
    }
    //    qDebug()<<number<<"and"<<is_off;
}

bool getAlarmSignal()
{
    return is_off;
}



