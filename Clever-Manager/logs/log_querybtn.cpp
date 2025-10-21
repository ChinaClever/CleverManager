#include "log_querybtn.h"

LOG_QueryBtn::LOG_QueryBtn(QWidget *parent) : LOG_DateTimeBtn(parent)
{
    //    this->setStyleSheet( "QPushButton{color:black;}"
    //                         "QPushButton{background-color:rgb(255,168,3)}");
    cabEdit = nullptr;
}

LOG_QueryBtn::~LOG_QueryBtn()
{

}



void LOG_QueryBtn::initRoom()
{
    roomLab = new QLabel(tr("机房名："),this);
    roomLab->setFont(QFont("微软雅黑",12));
    roomLab->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    roomLab->setMaximumSize(65,30);
    roomLab->setStyleSheet("color:white");

    roomEdit = new QLineEdit(this);
    roomEdit->setMaximumSize(140,30);
    hLayout->addWidget(roomLab);
    hLayout->addWidget(roomEdit);
}

void LOG_QueryBtn::initCab()
{
    initRoom();
    cabLab = new QLabel(tr("机柜名："),this);
    cabLab->setFont(QFont("微软雅黑",12));
    cabLab->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    cabLab->setMaximumSize(65,30);
    cabLab->setStyleSheet("color:white");

    cabEdit = new QLineEdit(this);
    cabEdit->setMaximumSize(140,30);
    hLayout->addWidget(cabLab);
    hLayout->addWidget(cabEdit);
}

void LOG_QueryBtn::initWidget(void)
{
    btn = new QPushButton(tr("查找"),this);
    //    btn->setStyleSheet("color:white;" "background-color:rgb(0,91,172)");
    btn->setFont(QFont("微软雅黑",12));
    btn->setMaximumSize(60,30);
    hLayout->addWidget(btn);
    hLayout->addStretch();
    connect(btn, SIGNAL(clicked()),this,SLOT(btnSlot()));
}


void LOG_QueryBtn::btnSlot(void)
{
    QString cmd;
    QString room = roomEdit->text();
    if(!room.isEmpty())
        cmd = tr("roomName like '%%1%'").arg(room);

    if(cabEdit) {
        QString cab = cabEdit->text();
        if(!cab.isEmpty()) {
            if(!room.isEmpty()) cmd += " and ";
            cmd += tr("cabName like '%%1%'").arg(cab);
        }
    }

    emit querySig(cmd);
}
