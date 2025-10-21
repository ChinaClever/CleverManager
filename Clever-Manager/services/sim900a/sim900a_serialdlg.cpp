#include "sim900a_serialdlg.h"
#include "db_sqlquery.h"

extern bool usr_land_jur(void);

SIM900A_SerialDlg::SIM900A_SerialDlg(QWidget *parent) : QWidget(parent)
{

    initWidget();
    initLayout();

//    QTimer::singleShot(2000,this,SLOT(openSerialSlot()));
}

SIM900A_SerialDlg::~SIM900A_SerialDlg()
{

}


/**
 * @brief SIM900A 端口列表显示
 */
void SIM900A_SerialDlg::initSerialPort(void)
{
    SerialPort port;
    QStringList com = port.portsList(); // 端口列表

    comBox->clear();
    comBox->addItems(com);

    SerialDev *simCom = sim_serial;
    if(simCom)
    {
        QString name = simCom->portName(); // 已打开端口
        if(!name.isEmpty())
            comBox->setCurrentText(name);
    }
}


void SIM900A_SerialDlg::initWidget(void)
{
    selectLab = new QLabel(tr("串口选择:"),this);

    //    selectLab->setMaximumSize(68,28);
    selectLab->adjustSize();

    comBox = new QComboBox(this);
    comBox->setMinimumHeight(27);
    //    comBox->setMaximumWidth(120);
    initSerialPort();
    connect(comBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(serialPortChanged(QString)));

    workLab = new QLabel(tr("串口操作:"),this);
    //    workLab->setMaximumSize(68,28);

    workBtn = new QPushButton(tr("打开串口"),this);
    workBtn->setMinimumHeight(27);
    //    workBtn->setMaximumWidth(120);
    workBtn->adjustSize();
    connect(workBtn,SIGNAL(clicked()),this,SLOT(workBtnSlot()));
    QPalette pal;
    pal.setColor(QPalette::ButtonText,QColor(0,90,171));
    workBtn->setPalette(pal);

    updateLab = new QLabel(tr("串口刷新:"),this);
    //    updateLab->setMaximumSize(68,28);

    updateBtn = new QPushButton(tr("刷新串口"),this);
    updateBtn->setMinimumHeight(27);
    updateBtn->adjustSize();
    //    updateBtn->setMaximumWidth(120);
    connect(updateBtn,SIGNAL(clicked()),this,SLOT(updateBtnSlot()));
    updateBtn->setPalette(pal);

    lab = new QLabel(tr("串口状态:"),this);
    //    lab->setMaximumSize(68,28);
    stateLab = new QLabel(this);
    //    stateLab->setFixedWidth(110);
}

void SIM900A_SerialDlg::initLayout(void)
{
    QGroupBox *groupBox = new QGroupBox(this);
    groupBox->setStyleSheet("background-color: rgb(255,255,255)");
    QWidget *widget = new QWidget(groupBox);
    //    widget->resize(200,160);
    widget->move(10,20);
    this->setMinimumSize(220,200);

    QGridLayout *layout = new QGridLayout(widget);
    layout->setSpacing(25);
    layout->addWidget(lab,0,0);
    layout->addWidget(stateLab,0,1);
    layout->addWidget(selectLab,1,0);
    layout->addWidget(comBox,1,1);
    layout->addWidget(workLab,2,0);
    layout->addWidget(workBtn,2,1);
    layout->addWidget(updateLab,3,0);
    layout->addWidget(updateBtn,3,1);

    ft.setFamily("微软雅黑");
    ft.setPointSize(12);

    lab->setFont(ft);
    stateLab->setFont(ft);
    selectLab->setFont(ft);
    comBox->setFont(ft);
    workLab->setFont(ft);
    workBtn->setFont(ft);
    updateLab->setFont(ft);
    updateBtn->setFont(ft);


    QGridLayout *pLayout = new QGridLayout(this);
    pLayout->setSpacing(0);
    QLabel *title=new QLabel(tr("  串口状态信息"),this);
    title->setFixedHeight(30);
    QPalette back;
    back.setColor(QPalette::WindowText,Qt::white);
    title->setPalette(back);
    title->setFont(ft);
    groupBox->setFont(ft);
    title->setStyleSheet("background-color: rgb(12,62,180)");
    pLayout->addWidget(title,0,0,1,1);
    pLayout->addWidget(groupBox,1,0,10,1);
}

/**
 * @brief 选择不同串口号时重新打开
 * @param com
 */
void SIM900A_SerialDlg::serialPortChanged(QString com)
{
    if(!com.isEmpty())
    {
        SIM900A_Serial serial;
        bool ret = serial.sim_serial_open(com);
        if(ret)
        {
            workBtn->setText(tr("关闭串口"));
            stateLab->setText(tr("串口已打开"));
        }
        else
        {
            workBtn->setText(tr("打开串口"));
            stateLab->setText(tr("串口打开失败"));
            CriticalMsgBox box(this, tr("打开串口%1失败").arg(com));
        }
    }
}

void SIM900A_SerialDlg::openSerialSlot(void)
{
    SIM900A_Serial serial;

    if(g_sim_isOpen) /*已打开，现在是关闭窗口*/
    {
        serial.sim_serial_close();
        workBtn->setText(tr("打开串口"));

        stateLab->setText(tr("串口已关闭"));
    }
    else /*未打开，现在打开*/
    {
        QString com = comBox->currentText();
        serialPortChanged(com);
    }
}

void SIM900A_SerialDlg::workBtnSlot(void)
{
    bool ret = usr_land_jur();
    if(ret==false){
        InfoMsgBox box(this,tr("您权限不够，无法操作"));
        return ;
    }

    openSerialSlot();

    /*增加日志信息*/
    s_RecordLog log;
    log.title = tr("设备配置");
    log.operation = tr("串口操作");
    if(g_sim_isOpen)
        log.msg = tr("打开串口:%1").arg(comBox->currentText());
    else
        log.msg = tr("关闭串口:%1").arg(comBox->currentText());

    sql_insert_record(log);
}

void SIM900A_SerialDlg::updateBtnSlot(void)
{
    disconnect(comBox,SIGNAL(currentIndexChanged(QString)),
               this,SLOT(serialPortChanged(QString)));  //断开信号

    initSerialPort();
    connect(comBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(serialPortChanged(QString)));

}
