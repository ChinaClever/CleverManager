/*
 * ds_slavebtn.cpp
 *  副机选择
 *      1、根据级联数据会显示对应副机按钮
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "ds_slavebtn.h"


DS_SlaveBtn::DS_SlaveBtn(QWidget *parent) : QWidget(parent)
{
    setMaximumHeight(260);

    initWidget();
    initLayout();

    m = new QSignalMapper(this);
    connect(masterBtn,SIGNAL(clicked()),m,SLOT(map()));
    connect(slaveBtn_1,SIGNAL(clicked()),m,SLOT(map()));
    connect(slaveBtn_2,SIGNAL(clicked()),m,SLOT(map()));
    connect(slaveBtn_3,SIGNAL(clicked()),m,SLOT(map()));
    connect(slaveBtn_4,SIGNAL(clicked()),m,SLOT(map()));

    m->setMapping(masterBtn,0);
    m->setMapping(slaveBtn_1,1);
    m->setMapping(slaveBtn_2,2);
    m->setMapping(slaveBtn_3,3);
    m->setMapping(slaveBtn_4,4);

    connect(m,SIGNAL(mapped(int)),this,SIGNAL(btnClickedSig(int)));
    connect(comboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(comChangeSlot(int)));

    allHidden(true);
}

DS_SlaveBtn::~DS_SlaveBtn()
{

}

/**
 * @brief 初始化窗口
 */
void DS_SlaveBtn::initWidget(void)
{
    QFont ft;
    ft.setPointSize(15);
    ft.setBold(true);

    lab = new QLabel("---", this);
    lab->setMinimumHeight(30);
    lab->setAlignment(Qt::AlignCenter);
    lab->setFont(ft);

    masterBtn = new QPushButton(tr("主机"),this);
    slaveBtn_1 = new QPushButton(tr("副机 1"),this);
    slaveBtn_2 = new QPushButton(tr("副机 2"),this);
    slaveBtn_3 = new QPushButton(tr("副机 3"),this);
    slaveBtn_4 = new QPushButton(tr("副机 4"),this);
    comboBox = new QComboBox(this);
}

/**
 * @brief 初始化布局
 */
void DS_SlaveBtn::initLayout(void)
{
//     QGroupBox *groupBox = new QGroupBox(tr("设备选择"),this);
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->addWidget(lab);
    vLayout->addWidget(masterBtn);
    vLayout->addWidget(slaveBtn_1);
    vLayout->addWidget(slaveBtn_2);
    vLayout->addWidget(slaveBtn_3);
    vLayout->addWidget(slaveBtn_4);
    vLayout->addWidget(comboBox);
}

/**
  * @brief 初始化ComboBox
  * @param num
  */
void DS_SlaveBtn::initComboBox(int num)
{
    disconnect(comboBox, SIGNAL(currentIndexChanged(int)),
               this, SLOT(comChangeSlot(int)));

    QStringList texts;
    for(int i=5; i<num; ++i)
        texts<<("    副机 "+QString::number(i));
    comboBox->clear();
    comboBox->addItems(texts);

    connect(comboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(comChangeSlot(int)));
}

/**
  * @brief 控件隐藏
  * @param hid
  */
void DS_SlaveBtn::allHidden(bool hid)
{
    lab->setHidden(hid);
    masterBtn->setHidden(hid);
    slaveBtn_1->setHidden(hid);
    slaveBtn_2->setHidden(hid);
    slaveBtn_3->setHidden(hid);
    slaveBtn_4->setHidden(hid);
    comboBox->setHidden(hid);
}

/**
 * @brief 初始化控件
 * @param ip
 */
int DS_SlaveBtn::initBtn(QString &ip)
{
    int ret = pdu_hd_getSlaveNum(ip);
    if(ret>1)
    {
        allHidden(false);
        switch(ret)
        {
        case 2:
            slaveBtn_2->setHidden(true);
        case 3:
            slaveBtn_3->setHidden(true);
        case 4:
            slaveBtn_4->setHidden(true);
        case 5:
            comboBox->setHidden(true);
            break;
        default:
            initComboBox(ret);
            break;
        }
    }
    return ret;
}

/**
 * @brief 更新设备 ip 和设备号
 * @param ip
 * @param num
 */
void DS_SlaveBtn::updateBtn(QString &ip,int num)
{
    allHidden(true);
    int ret = initBtn(ip);
    if(ret>1)
    {
        lab->setHidden(false); /*显示标签*/

        QString str;
        if(num)
            str = tr("副机 %1").arg(num);
        else
            str = tr("主机");

        lab->setText(str);
    }
}


void DS_SlaveBtn::comChangeSlot(int id)
{
    emit btnClickedSig(id+5);
}
