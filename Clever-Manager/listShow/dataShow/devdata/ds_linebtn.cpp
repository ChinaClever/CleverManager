/*
 * ds_linebtn.cpp
 *  相数选择按钮
 *      第一个按键，是统计，对应的键值0
 *      相数按钮，分别对应1\2\3
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "ds_linebtn.h"

DS_LineBtn::DS_LineBtn(QWidget *parent) : QWidget(parent)
{
    setMaximumHeight(160);
    initWidget();
    initLayout();

    m = new QSignalMapper(this);
    connect(mBtn,SIGNAL(clicked()),m,SLOT(map()));
    connect(lineBtn_1,SIGNAL(clicked()),m,SLOT(map()));
    connect(lineBtn_2,SIGNAL(clicked()),m,SLOT(map()));
    connect(lineBtn_3,SIGNAL(clicked()),m,SLOT(map()));

    m->setMapping(mBtn,0);
    m->setMapping(lineBtn_1,1);
    m->setMapping(lineBtn_2,2);
    m->setMapping(lineBtn_3,3);

    connect(m,SIGNAL(mapped(int)),this,SIGNAL(btnClickedSig(int)));
}

DS_LineBtn::~DS_LineBtn()
{

}

/**
 * @brief 初始化窗口
 */
void DS_LineBtn::initWidget(void)
{
    QFont ft;
    ft.setPointSize(15);
    ft.setBold(true);
    this->setStyleSheet("QPushButton{background-color:rgb(102, 199, 249);}"
                        "QPushButton:hover{background-color:rgb(91, 237, 238);}"
                        "QPushButton:pressed{background-color:rgb(143, 185, 181);}"
                    "QComboBox{background-color:rgb(255,255,255);}" );

    lab = new QLabel("L:1", this);
    lab->setMinimumHeight(30);
    lab->setAlignment(Qt::AlignCenter);
    lab->setFont(ft);
QPalette pal;
pal.setColor(QPalette::ButtonText,Qt::white);
    mBtn = new QPushButton(tr("统计"),this);
     mBtn->setFont(QFont("微软雅黑",11));
      mBtn->setPalette(pal);

    lineBtn_1 = new QPushButton(tr("Line 1"),this);
    lineBtn_1->setFont(QFont("微软雅黑",10));
    lineBtn_1->setFixedHeight(25);
//    lineBtn_1->setStyleSheet("background-color:rgb(88,162,130)");
    lineBtn_1->setPalette(pal);

    lineBtn_2 = new QPushButton(tr("Line 2"),this);
    lineBtn_2->setFont(QFont("微软雅黑",10));
    lineBtn_2->setFixedHeight(25);
//    lineBtn_2->setStyleSheet("background-color:rgb(88,162,130)");
    lineBtn_2->setPalette(pal);

    lineBtn_3 = new QPushButton(tr("Line 3"),this);
    lineBtn_3->setFont(QFont("微软雅黑",10));
    lineBtn_3->setFixedHeight(25);
//    lineBtn_3->setStyleSheet("background-color:rgb(88,162,130)");
    lineBtn_3->setPalette(pal);
}

/**
 * @brief 初始化布局
 */
void DS_LineBtn::initLayout(void)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(lab);
    layout->addWidget(mBtn);
    layout->addWidget(lineBtn_1);
    layout->addWidget(lineBtn_2);
    layout->addWidget(lineBtn_3);
}

/**
 * @brief 设置相数
 * @param line
 */
void DS_LineBtn::setLine(int line)
{
     QString str;
    if(line)
         str = "L:" +QString::number(line);
    else
        str = tr("统计");

    lab->setText(str);
}
