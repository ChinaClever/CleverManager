/*
 * sim900a_interface.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "sim900a_interface.h"

extern QString usr_land_telephone(void);

SIM900A_Interface::SIM900A_Interface(QWidget *parent) : QWidget(parent)
{
    initWidget();
    initLayout();

//    mSimThread = new SIM900A_Thread(this);
//    connect(mSimThread,SIGNAL(simErrSig(int)),this,SLOT(simErrSlot(int)));
}

SIM900A_Interface::~SIM900A_Interface()
{

}

///**
// * @brief 信息显示
// */
//void SIM900A_Interface::setModuLab(void)
//{
//    extern SIM_ModuInfo sim_moduInfo;
//    extern SIM_gsmInfo  sim_gsmInfo;

//    SIM_ModuInfo *modu = &sim_moduInfo;
//    SIM_gsmInfo *gsm = &sim_gsmInfo;

//    manufacturerLab->setText(modu->manufacturer);
//    modular->setText(modu->modular);
//    //    qDebug()<<"本机号码："<<modu->localNumbers;
//    localNumbers->setText(modu->localNumbers);
//    operators->setText(gsm->operators);

//    /*信号质量转化为百分数*/
//    QString str= "--";
//    if(!gsm->signalQuality.isEmpty())
//    {
//        int sig = gsm->signalQuality.remove(0,2).toInt()*100/31;
//        str = tr("%1%").arg(sig);
//    }
//    signalQuality->setText(str);
//}

//void SIM900A_Interface::initLabel(void)
//{
//    QString str = "----";
//    manufacturerLab->setText(str);
//    modular->setText(str);
//    localNumbers->setText(str);
//    operators->setText(str);
//}

///**
// * @brief 错误提示
// * @param err
// */
//void SIM900A_Interface::simErrSlot(int err)
//{
//    static int errNo = -1;

//    if(errNo == -1) {
//        if(err <= SIM_MODU_ERR)
//            return;
//    }

//    if(errNo != err)
//    {
//        errNo = err;
//        setModuLab();

//        QString str;
//        switch (err)
//        {
//        case SIM_OK:
//            break;
//        case SIM_COM_ERR: // 串口错误
//            str =  tr("串口错误！");
//            break;
//        case SIM_MODU_ERR: //模块错误
//            str =  tr("短信模块错误！");
//            break;
//        case SIM_PIN_ERR: //卡错误
//            str = tr("手机卡错误！");
//            break;
//        case SIM_COPS_ERR: //运营商错误
//            str = tr("运营商错误！");
//            break;
//        case SIM_SMS_ERR: //短信初始化错误
//            str = tr("短信初始化错误！");
//            break;
//        case SIM_MODU_INFO_ERR: // 读模块信息错误
//            str = tr("读模块信息错误!");
//            break;
//        case SIM_PIN_INFO_ERR: //读卡信息错误
//            str = tr("读SIM卡信息错误!");
//            break;
//        default:
//            break;
//        }

//        if(!str.isEmpty())
//        {
//            initLabel();
//            stateLab->setText(str);
//            smsBtn->setDisabled(true);
//            CriticalMsgBox box(this, str);
//        } else {
//            smsBtn->setEnabled(true);
//            stateLab->setText(tr("正常"));
//        }
//    }else {
//    }
//}


/**
 * @brief 窗口初始化
 */
void SIM900A_Interface::initWidget(void)
{
    QFont ft;
    ft.setPointSize(12);
    ft.setFamily("微软雅黑");

    //lab = new QLabel(tr("     软件版本:V2.0.1"),this);
//    lab = new QLabel(tr("     软件版本:V1.0"),this);
//    lab = new QLabel(tr("     Software version:V1.1"),this);//English
    lab = new QLabel(tr("     Software version:V1.3"),this);//English
    //    lab->setMaximumSize(70,30);
    lab->setFont(ft);

//    stateLab = new QLabel(tr("  未开启:"),this);
    //    stateLab->setMinimumHeight(28);
    //    stateLab->setMinimumWidth(200);
//    stateLab->setFont(ft);

//    manuLab = new QLabel(tr("     更新日期:2020-05-26"),this);
    //需要注释离线邮件pdu/dev/offLine/pdu_offlinedev.cpp Line180
//    manuLab = new QLabel(tr("     Update time:2022-08-30"),this);//English
    manuLab = new QLabel(tr("     Update time:2024-07-23"),this);//English//开放离线邮件
    //    manuLab->setMaximumSize(70,30);
    manuLab->setFont(ft);

//    manufacturerLab = new QLabel(this);
//    //    manufacturerLab->setMinimumHeight(28);
//    manufacturerLab->setFont(ft);

//    modu = new QLabel(tr("  模块名:"),this);
//    //    modu->setMaximumSize(70,30);
//    modu->setFont(ft);

//    modular = new QLabel(this);
//    //    modular->setMinimumHeight(28);
//    modular->setFont(ft);

//    local = new QLabel(tr("  本机号:"),this);
//    //    local->setMaximumSize(70,30);
//    //    local->setMinimumWidth(200);
//    local->setFont(ft);

//    localNumbers = new QLabel(this);
//    //    localNumbers->setMinimumHeight(28);
//    localNumbers->setFont(ft);

//    opera = new QLabel(tr("  运营商:"),this);
//    //    opera->setMaximumSize(70,30);
//    opera->setFont(ft);

//    operators = new QLabel(this);
//    //    operators->setMinimumHeight(28);
//    operators->setFont(ft);

//    signal = new QLabel(tr("信号强度:"),this);
//    signal->setMinimumWidth(120);
//    signal->setAlignment(Qt::AlignLeft);
//    //    signal->setMaximumSize(70,30);
//    signal->setFont(ft);

//    signalQuality = new QLabel(this);
//    //    signalQuality->setMinimumHeight(28);
//    signalQuality->setFont(ft);

//    smsLab = new QLabel(tr("模块操作:"),this);
//    //    smsLab->setMinimumWidth(180);
//    //   smsLab->setMaximumSize(70,30);
//    smsLab->setFont(ft);

//    smsBtn = new QPushButton(tr("短信测试"),this);
//    //     smsBtn->setMaximumWidth(30);
//    //    smsBtn->setMinimumHeight(28);
//    //    smsBtn->setMaximumWidth(120);
//    smsBtn->setFont(ft);
//    smsBtn->setDisabled(true);
//    QPalette pal;
//    pal.setColor(QPalette::ButtonText,QColor(0,90,171));
//    smsBtn->setPalette(pal);


//    connect(smsBtn, SIGNAL(clicked()),this,SLOT(smsBtnSlot()));
}

/**
 * @brief 布局
 */
void SIM900A_Interface::initLayout(void)
{
    QHBoxLayout *hLayout_1 = new QHBoxLayout;
//    hLayout_1->addStretch();
    hLayout_1->addWidget(lab);
    hLayout_1->addStretch();
    //hLayout_1->addWidget(stateLab);
//    hLayout_1->addStretch();
    hLayout_1->setSpacing(10);

    QHBoxLayout *hLayout_2 = new QHBoxLayout;
//    hLayout_2->addStretch();
    hLayout_2->addWidget(manuLab);
    hLayout_2->addStretch();
//    hLayout_2->addWidget(manufacturerLab);
//   // hLayout_2->addStretch();
//    hLayout_2->setSpacing(10);

//    QHBoxLayout *hLayout_3 = new QHBoxLayout;
//  //  hLayout_3->addStretch();
//    hLayout_3->addWidget(modu);
//    hLayout_3->addStretch();
//    hLayout_3->addWidget(modular);
//    //hLayout_3->addStretch();
//    hLayout_3->setSpacing(10);

//    QHBoxLayout *hLayout_4 = new QHBoxLayout;
//  //  hLayout_4->addStretch();
//    hLayout_4->addWidget(local);
//    hLayout_4->addStretch();
//    hLayout_4->addWidget(localNumbers);
//  //  hLayout_4->addStretch();
//    hLayout_4->setSpacing(10);

//    QHBoxLayout *hLayout_5 = new QHBoxLayout;
//   // hLayout_5->addStretch();
//    hLayout_5->addWidget(opera);
//    hLayout_5->addStretch();
//    hLayout_5->addWidget(operators);
//   // hLayout_5->addStretch();
//    hLayout_5->setSpacing(10);

//    QHBoxLayout *hLayout_6 = new QHBoxLayout;
//   // hLayout_6->addStretch();
//    hLayout_6->addWidget(signal);
//    hLayout_6->addStretch();
//    hLayout_6->addWidget(signalQuality);
//   // hLayout_6->addStretch();
//    hLayout_6->setSpacing(10);

//    QHBoxLayout *hLayout_7 = new QHBoxLayout;
//   // hLayout_7->addStretch();
//    hLayout_7->addWidget(smsLab);
//    hLayout_7->addStretch();
//    hLayout_7->addWidget(smsBtn);
//  //  hLayout_7->addStretch();
//    hLayout_7->setSpacing(10);

    QGroupBox *groupBox = new QGroupBox(this);
    //后续优化增加代码
    groupBox->setStyleSheet("background-color: rgb(255,255,255)");  //背景白色

    QWidget *widget = new QWidget(groupBox);
    //    widget->resize(200,260);
    //    widget->move(10,10);
    this->setMinimumSize(220,300);

    QGridLayout *layout = new QGridLayout(widget);


    layout->addWidget(lab,0,0);
    //layout->addWidget(stateLab,0,1);
    layout->addWidget(manuLab,1,0);
    //    layout->addWidget(manufacturerLab,1,1);
    //    layout->addWidget(modu,2,0);
    //    layout->addWidget(modular,2,1);
    //    layout->addWidget(local,3,0);
    //    layout->addWidget(localNumbers,3,1);
    //    layout->addWidget(opera,4,0);
    //    layout->addWidget(operators,4,1);
    //    layout->addWidget(signal,5,0);
    //    layout->addWidget(signalQuality,5,1);
    //    layout->addWidget(smsLab,6,0);
    //    layout->addWidget(smsBtn,6,1);
    layout->addLayout(hLayout_1,0,0);
//    layout->addLayout(hLayout_2,1,0);
//    layout->addLayout(hLayout_3,2,0);
//    layout->addLayout(hLayout_4,3,0);
//    layout->addLayout(hLayout_5,4,0);
//    layout->addLayout(hLayout_6,5,0);
//    layout->addLayout(hLayout_7,6,0);
    //    layout->addLayout(hLayout_1,0,0);

    layout->setContentsMargins(10,10,0,0);
    layout->setSpacing(15);

    m_layout = new QGridLayout(this);
    m_layout->setSpacing(0);
    QFont ft;
    ft.setPointSize(12);
    ft.setFamily("微软雅黑");
    QLabel *title=new QLabel(this);
    title->setFont(ft);
    //title->setText(tr("  短信测试"));
//    title->setText(tr("  软件版本"));
    title->setText(tr("  Software version"));//English
    title->setFixedHeight(30);
    title->setStyleSheet("background-color: rgb(12,62,180)");
    QPalette back;
    back.setColor(QPalette::WindowText,Qt::white);
    title->setPalette(back);
    m_layout->addWidget(title,0,0,1,1);
    m_layout->addWidget(groupBox,1,0,16,1);
}

///**
// * @brief 短信测试
// */
//void SIM900A_Interface::smsBtnSlot(void)
//{
//    QString phone = usr_land_telephone();  //获取当前登录用户手机

//    if(phone.isEmpty())
//    {
//        bool ok;
//        QString text = QInputDialog::getText(this,tr("短信测试"),tr("请输入手机号"),
//                                             QLineEdit::Normal,NULL,&ok);
//        if(ok && !text.isEmpty())
//            phone = text;
//        else
//            return ;
//    }
//    QString msg = tr("这是一条测试信息");
//    sim_sent_sms(phone, msg);

//    InfoMsgBox box(this,tr("信息已发送至：%1，请等待!").arg(phone));
//}
