#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "usr/usrlanddlg.h"
#include "common.h"

#include "zebra/zebra_client.h"
#include "zebra/zebra_delegates.h"
#include "autoaddfriend/auto_get_zebra_server_ip.h"
#include "signaldispatch.h"

#include "register/registerinfo.h"
#include "pdu/dev/pdudevspied.h"
#include "dc/storage/cloudsetdlg.h"
#include "autoaddfriend/zebrasecuritymanager.h"


extern bool db_connection(void);
extern void dl_read_db(void);


//void add_cabinets()
//{
//    DbCabinetList *cab = DbCabinetList::get();
//    if(cab->counts() > 100) return;

//    int id = 1;
//    QHostAddress ipAddr("192.168.2.0");
//    uint ip = ipAddr.toIPv4Address();
//    int num = 0;

//    QSqlDatabase::database().transaction();
//    for(int i=0; i<20; ++i) {
//        for(int j=0; j<25; ++j) {
//            CabinetItem item;
//            item.id = id++;
//            item.room_id = 1;
//            item.height = 42;
//            item.name = QString("%1_%2").arg(i).arg(j);
//            item.main_type = PDU_TYPE_ZPDU;
//            item.spare_type = PDU_TYPE_ZPDU;

//            item.main_num = num++;
//            item.main_ip = QHostAddress(ip).toString().remove("::ffff:");

//            item.spare_num = num++;
//            item.spare_ip = QHostAddress(ip).toString().remove("::ffff:");

//            if(num > 8) {
//                num = 0;
//                ip++;
//            }

//            item.row = i;
//            item.column = j;
//            cab->insertItem(item);
//        }
//    }
//    QSqlDatabase::database().commit();

//}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Clever Manager"));
    //this->setWindowTitle(tr("PDU Manager"));//中性
    //this->setWindowTitle(tr("OLABS Manager"));//中性

#if defined(CLOUD_MODULE)
    CloudSetDlg *wid = new CloudSetDlg(this); ///=== 只用来做试验 验证、测试用
    wid->exec();
#endif


    QTimer::singleShot(100,this,SLOT(initFunSLot())); //延时初始化
    mUserDlg = new UsrLandDlg();
    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    timer->start(1000);  //时间检测许可
    
    landWidget(); // 登录窗口
    QString currentusr=usr_land_name();
    ui->label_2->setText(currentusr);
    //ui->label->setText("OLABS Manager");//中性

#if defined(ZEBRA_MODULE)
    ui->label->setText(" Clever Manager Security");
#endif

//#if (UDP_HB_SENT == 0)
//    mProcess = new QProcess(this);
//    mProcess->start("UdpClient.exe");
//#endif
}


MainWindow::~MainWindow()
{
// #if (UDP_HB_SENT == 0)
//    if(mProcess) {
//        mProcess->close();
//    }
//#endif

#ifdef ZEBRA_MODULE  //clear zebra objs
    delete zebra_client::get_instance();
#endif
    delete ui;
}

void MainWindow::initZebra()
{
#ifdef ZEBRA_MODULE
    AutoGetServer::get(this);
    zebra_client::get_instance()->init();
    zebraSecurityManager::get_instance();
    zebraSecurityTable::get_instance();
#endif
}

void MainWindow::setBtndefault(QToolButton *btn, const QString &str)
{
    QString strNormal = tr("QToolButton{border-image: url(:/toolbutton/toolbutton/%1_normal.png);}").arg(str);
    QString strSelect = tr("QToolButton:hover{border-image: url(:/toolbutton/toolbutton/%1_select.png);}").arg(str);
    QString strClicked = tr("QToolButton:pressed{border-image: url(:/toolbutton/toolbutton/%1_clicked.png);}").arg(str);
    
    btn->setStyleSheet(strNormal + strSelect + strClicked);
    btn->setText("");
}

/**
 * @brief 初始化按键颜色
 */
void MainWindow::initBtnColour()
{
    setBtndefault(ui->topoBtn, "topo");
    setBtndefault(ui->TGBtn, "tg");
    setBtndefault(ui->DetailBtn, "detail");
    setBtndefault(ui->devBtn, "dev");
    setBtndefault(ui->serBtn, "ser");
    setBtndefault(ui->LogBtn, "log");
    setBtndefault(ui->usrBtn, "usr");
}

void MainWindow::setBtnClicked(QToolButton *btn, const QString &str)
{
    initBtnColour();
    QString strNormal = tr("QToolButton{border-image: url(:/toolbutton/toolbutton/%1_clicked.png);}").arg(str);
    QString strSelect = tr("QToolButton:hover{border-image: url(:/toolbutton/toolbutton/%1_select.png);}").arg(str);
    QString strClicked = tr("QToolButton:pressed{border-image: url(:/toolbutton/toolbutton/%1_clicked.png);}").arg(str);
    
    btn->setStyleSheet(strNormal + strSelect + strClicked);
}

void MainWindow::addToolBtnIcon(QToolButton *btn, const QString &icon)
{
    QString str = ":/toolbutton/toolbutton/" + icon +".png";
    btn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    btn->setIcon(QIcon(str));
    btn->setIconSize(QSize(100, 100));
}


void MainWindow::initFunSLot()
{
    set_mainTitle_back(ui->widget); /*设置主窗口标题图片*/
    button_style_sheet(ui->widget_2);/*设置按键样式*/
    
    QColor maincolor(132,209,251);
    set_background_color(this,maincolor);
    
    initFunction();  //--data
    initWidget();    //--widget
    
    QColor color(0,17,55);  //黑色界面
    set_background_color(ui->widget_2,color);
    on_topoBtn_clicked();
}

void MainWindow::timeoutDone(void)
{
 // /*1为需要注册 0为不需要注册
#if 1
    num_out=false;
    QString str;
    ActiveCode act;
    int ret=act.verifyLocalCode(); //*/
    pdu_devNumStr *current_pdunum=get_pdu_num();
    int current_num=current_pdunum->line;
     RegistryKey key;
    int register_num=key.readCount();
    str=tr("当前在线数量:%1,注册数量：%2，在线数量超过注册上限！").arg(current_num).arg(register_num); //////========
    if(current_num > register_num && (ret==ACTIVE_RET_OK ||ret==ACTIVE_RET_FREE))//
    {
        num_out=true;//超过上限
        int value = QMessageBox::warning(this,tr("报警信息"),str,QMessageBox::Ok|QMessageBox::No);
        if(QMessageBox::Ok == value)
        {
            RegisterInfo info(this); //这个操作会造成内存泄漏【------】
            //重新修改注册信息
            info.exec();
        }else if(QMessageBox::No == value)
            //退出该软件
            this->close();
        else{

        }
    }
#else
    QDate date = QDate::currentDate();
    if(date > QDate(2024,9,1)) {
        WaringMsgBox box(this, tr("software expired"));
        box.Exec();

        exit(0);
    }
#endif
}


/**
 * @brief 初始化
 */
void MainWindow::initFunction(void)
{
    db_connection(); // 连接数据库
    //add_cabinets();
    dl_read_db(); // 设备列表信息，先从数据库中读取，
    
    /* 创建网络数据解析对象、它分创建UDP接收套接字，广播心跳包，接收数据并进行解析，把数据保存至Hash中 */
    mNetDataAnalyze = new NetDataAnalyze(this);
    mDevSpied = new PduDevSpied(this); // 设备掉线处理
}

/**
 * @brief 初始化布局
 */
void MainWindow::initLayout(void)
{
    QGridLayout *layout = new QGridLayout(ui->widget_2);
    layout->addWidget(mStack);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
}


/**
 * @brief 初始化 窗口
 */
void MainWindow::initWidget(void)
{
    mStack = new QStackedWidget(ui->widget_2); //堆叠界面
    splitter_style_sheet(mStack); //样式
    
    mTopology = new TopologyWidget(mStack); // 机房拓扑
    mStack->addWidget(mTopology);
    
    mTGShow = new TG_WidgetMain(mStack); // 统计信息界面
    mStack->addWidget(mTGShow);

    mLSMainSpl = new LS_MainSpl(mStack); //设备信息
    mStack->addWidget(mLSMainSpl);
    
#if 1
    mDetailList = new DT_MainWidget(mStack); // 祥细列表信息
    mStack->addWidget(mDetailList);
    connect(mDetailList,SIGNAL(selectSig(QString,short)),this,SLOT(selectSlot(QString,short)));
    connect(mDetailList,SIGNAL(selectSig(QString,short)),mLSMainSpl,SLOT(selectSlot(QString,short)));
#else
    // ui->DetailBtn->setHidden(true);
#endif 
    
    mLog = new LOG_ShowClass(mStack); // 日志界面
    mStack->addWidget(mLog);
    
    mServiceDlg = new Ser_ServiceDlg(mStack); //服务设置
    mStack->addWidget(mServiceDlg);
    
    initLayout();
}


void MainWindow::selectSlot(QString ip,short num)
{
    on_devBtn_clicked(); // 显示设备信息
}

/**
 * @brief 机房拓图
 */
void MainWindow::on_topoBtn_clicked()
{
    mStack->setCurrentWidget(mTopology);
    setBtnClicked(ui->topoBtn, "topo");
    set_background_color(ui->widget, QColor(12,62,180));
}

/**
 * @brief 统计界面
 */
void MainWindow::on_TGBtn_clicked()
{
    mStack->setCurrentWidget(mTGShow);
    setBtnClicked(ui->TGBtn, "tg");
    set_background_color(ui->widget, QColor(12,62,180));
}

/**
 * @brief 明细列表
 */
void MainWindow::on_DetailBtn_clicked()
{
    mStack->setCurrentWidget(mDetailList);
    setBtnClicked(ui->DetailBtn, "detail");
    set_background_color(ui->widget, QColor(12,62,180));
}

/**
 * @brief 设备界面
 */
void MainWindow::on_devBtn_clicked()
{
    mStack->setCurrentWidget(mLSMainSpl);
    setBtnClicked(ui->devBtn, "dev");
    set_background_color(ui->widget, QColor(12,62,180));
}

/**
 * @brief 主界面按键使能、锁定
 * @param en
 */
void MainWindow::enableBtn(bool en)
{
    ui->TGBtn->setEnabled(en);
    ui->devBtn->setEnabled(en);
    ui->DetailBtn->setEnabled(en);
    ui->serBtn->setEnabled(en);
    ui->LogBtn->setEnabled(en);
    //    ui->usrBtn->setEnabled(en);
}

/**
 * @brief 用户退出
 */
void MainWindow::usrQuit(void)
{
    UsrLandDlg dlg(this);
    QString usrName = usr_land_name();
    bool ret = dlg.quitWidget();
    if(ret) {
        enableBtn(false);
        on_topoBtn_clicked();
        
        //增加日志信息
        sLandLog log;
        log.title = tr("退出");
        log.operation = tr("用户退出");
        log.msg = tr("用户:%1退出").arg(usrName);
        sql_insert_land(log);
    }
    SignalDispatch::get()->changeLoginState();
}

/**
 * @brief 用户登录
 */
void MainWindow::landWidget(void)
{
    bool ret = mUserDlg->landWidget();
    
    if(ret)
    {
        enableBtn(true);
        QString usrName = usr_land_name();
        
        /*增加日志信息*/
        sLandLog log;
        log.title = tr("登录");
        log.operation = tr("用户登录");
        log.msg = tr("登录用户名:%1").arg(usrName);
        sql_insert_land(log);

#ifdef ZEBRA_MODULE
        initZebra();     //--zebra
#endif

    }
    SignalDispatch::get()->changeLoginState();
}

/**
 * @brief 用户登录
 */
void MainWindow::on_usrBtn_clicked()
{
    if(usr_landFlag == false) // 用户登陆
        landWidget();
    else {
        UsrLandDlg dlg(this);
        int ret = dlg.selectWork();
        if(ret == 1) // 用户切换
            landWidget();
        else if(ret == 2) // 用户退出
            usrQuit();
    }
    SignalDispatch::get()->changeLoginState();
    
    QString currentusr=usr_land_name();
    ui->label_2->setText(currentusr);
    
}



void MainWindow::on_LogBtn_clicked()
{
    mStack->setCurrentWidget(mLog);
    setBtnClicked(ui->LogBtn, "log");
    //    set_background_color(ui->widget, QColor(67,67,67));
}

void MainWindow::on_serBtn_clicked()
{
    mStack->setCurrentWidget(mServiceDlg);
    setBtnClicked(ui->serBtn, "ser");
    set_background_color(ui->widget, QColor(12,62,180));
}

/*
 * 本机注册信息
 */
void MainWindow::on_pushButton_clicked()
{
    RegisterInfo dia(this);
    dia.exec();
}



void MainWindow::closeEvent(QCloseEvent *event)
{
    switch (QMessageBox::information(this,tr("warning"),tr("您是否确定关闭管理软件？"),tr("确定"),tr("取消"),0,1)) {
    case 0:
        event->accept();
        break;
    case 1:
        event->ignore();
        break;
    }
}
