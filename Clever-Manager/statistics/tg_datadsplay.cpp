/*
 * tg_datadsplay.cpp
 * 统计数据实时显示
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "tg_datadsplay.h"
#include "titlebar.h"

TG_DataDsplay::TG_DataDsplay(QWidget *parent) : QWidget(parent)
{
    initView();
    //    set_background_color(this, Qt::white);
}

TG_DataDsplay::~TG_DataDsplay()
{

}

void TG_DataDsplay::initView()
{
#if 0
    groupBox = new QGroupBox(tr("设备状态信息"),this);

    dspVLayout = new QVBoxLayout(groupBox);
    createControl();
    layoutControl();

    m_pLayout = new QGridLayout(this);
    m_pLayout->addWidget(groupBox);
#else
    TitleBar *bar = new TitleBar(this);
    bar->setTitle(tr("设备状态信息"));
    //    bar->setTitle(tr("状态信息"));
    bar->setFont(QFont("微软雅黑",12,75));
    //    bar->setStyleSheet("color:white; background-color:rgb(0,17,55)");

    bar->setFixedHeight(25);

    groupBox = new QWidget(this);
    //    groupBox->setMaximumHeight(320);
    //    groupBox->setStyleSheet("border:none");
    groupBox->setStyleSheet("background-color:white");

    //    groupBox->setFont(QFont("幼圆",12));

    QWidget *out_widget=new QWidget(this);
    out_widget->setStyleSheet("background-color:rgb(0,17,55)");
    //   out_widget->setStyleSheet("background-color:white");
    //    QPalette pal;
    //    pal.setColor();
    QVBoxLayout *out_layout=new QVBoxLayout(out_widget);
    out_layout->addWidget(bar);
    out_layout->addWidget(groupBox);
    out_layout->setSpacing(13);

    dspVLayout = new QVBoxLayout(groupBox);
    createControl();
    layoutControl();

    m_pLayout = new QGridLayout(this);
    m_pLayout->addWidget(out_widget, 0, 0);
    //    m_pLayout->addWidget(groupBox, 1, 0);
    m_pLayout->setSpacing(0);
    m_pLayout->setContentsMargins(0, 0, 0, 0);
#endif
}


/**
 * @brief 创建控件
 */
void TG_DataDsplay::createControl(void)
{
    //    devLab = new QLabel(this); // 设备名
    //    devLab->setMaximumHeight(50);

    mVolLcd = new TG_LcdNum(this);
    mVolLcd->setUnit(tr("平均电压"), "V");

    mCurLcd = new TG_LcdNum(this);
    mCurLcd->setUnit(tr("总电流"), "A");

    mPowLcd = new TG_LcdNum(this);
    mPowLcd->setUnit(tr("总功率"), "kW");

    mEleLcd = new TG_LcdNum(this);
    mEleLcd->setUnit(tr("总电能"), "kWh");

    mTemLcd = new TG_LcdNum(this);
    mTemLcd->setUnit(tr("温度"), "°C");

    //    tempLab = new QLabel(this);
    //    tempLab->setAlignment(Qt::AlignCenter);
    //    tempLab->setMaximumHeight(110);
    //    m_pDialPan = new DialpanDlg(tempLab);

    mHumLcd = new TG_LcdNum(this);
    mHumLcd->setUnit(tr("湿度"), "%");
    //    humLab = new QLabel(this);
    //    humLab->setAlignment(Qt::AlignCenter);
    //    humLab->setMaximumHeight(110);
    //    m_pBarSliderDlg = new BarSliderDlg(humLab);

    //    eleLab = new QLabel(this); // 设备名
    //    eleLab->setMaximumHeight(10);
}
/**
 * @brief 布局
 */
void TG_DataDsplay::layoutControl(void)
{
    //    QHBoxLayout *devHlayout = new QHBoxLayout;
    //    devHlayout->addWidget(devLab);

    QHBoxLayout *linelayout_2 = new QHBoxLayout;
    linelayout_2->setSpacing(5);
    linelayout_2->addWidget(mVolLcd);
    linelayout_2->addWidget(mCurLcd);
    linelayout_2->addWidget(mPowLcd);
    linelayout_2->setContentsMargins(0,10,0,0);

    //    QHBoxLayout *linelayout_3 = new QHBoxLayout;
    //    linelayout_3->addWidget(eleLab);

    QHBoxLayout *linelayout_4 = new QHBoxLayout;
    linelayout_4->setSpacing(5);
    linelayout_4->addWidget(mHumLcd);
    linelayout_4->addWidget(mTemLcd);
    linelayout_4->addWidget(mEleLcd);
    linelayout_2->setContentsMargins(0,0,0,10);

    //     dspVLayout->setMargin(15);
    //     dspVLayout->setSpacing(15);
    //    dspVLayout->addLayout(devHlayout);
    dspVLayout->addStretch();
    dspVLayout->addLayout(linelayout_2);
    dspVLayout->addLayout(linelayout_4);
    dspVLayout->addStretch();
    //    dspVLayout->addLayout(linelayout_3);
    dspVLayout->setSpacing(0);
    dspVLayout->setContentsMargins(10,0,0,0);

}

void TG_DataDsplay::changeVolMode(long double value)
{
    if(value >= 1000){
        mVolLcd->setSym("KV");
        value /= 1000;
    } else {
        mVolLcd->setSym("V");
    }

    mVolLcd->display((double)value);
}

/**
 * @brief 电流值来调整显示
 * @param value
 */
void TG_DataDsplay::changeCurMode(long double value)
{    
    int dec = 1;
    QString str = "A";

    if(value > 9000) {
        value /= 1000;
        str = "KA";
    } else if(value > 1000){
        dec = 0;
    }

    mCurLcd->setSym(str);
    mCurLcd->display((double)value, dec);
}

/**
 * @brief 功率显示
 * @param value
 */
void TG_DataDsplay::changePowMode(long double value)
{
    int dec = 0;
    QString str = "kW";

    if(value < 10)
        dec = 3;
    else if(value < 100)
        dec = 2;
    else if(value < 1000)
        dec = 1;
    else if(value < 9000)
        dec = 0;
    else {
        value /= 1000;
        dec = 3;
        str = "MW";
    }

    mPowLcd->setSym(str);
    mPowLcd->display((double)value, dec);
}


void TG_DataDsplay::changeEleMode(long double value)
{
    int dec = 1;
    QString str = "kWh";

    if(value < 100)
        dec = 1;
    else if(value < 1000){
        dec = 0;
    } else  if(value > 9000) {
        value /= 1000;
        str = "MWh";
    }

    mEleLcd->setSym(str);
    mEleLcd->display((double)value, dec);
}


/**
 * @brief 刷新数据
 */
void TG_DataDsplay::updateData(TG_DataType *data)
{
    long double value;

    value = data->vol/COM_RATE_VOL;
    changeVolMode(value);

    value = data->cur/COM_RATE_CUR;
    changeCurMode(value);

    value = data->pow/COM_RATE_POW;
    changePowMode(value);

    value = data->ele/COM_RATE_ELE;
    changeEleMode(value);

    value = data->tem / COM_RATE_TEM;
    mTemLcd->display(value);
    //    m_pDialPan->setValue(value); //平均温度

    value = data->hum / COM_RATE_HUM;
    mHumLcd->display(value);
    //    m_pBarSliderDlg->setValue(value); //平均湿度
}

