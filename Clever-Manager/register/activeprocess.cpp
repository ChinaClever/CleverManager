#include "activeprocess.h"
#include "ui_activeprocess.h"
#include<QDateTime>
#include<QDate>
#include<QApplication>
#include"activecode.h"
#include"registrykey.h"
#include<QDebug>
#include <QDesktopWidget>
#include<QFile>
#include<QTextStream>
#include "common.h"
#include"machinecode.h"
#include<QPalette>
#include<QString>
#include"registerbox.h"
#include"registrykey.h"


ActiveProcess::ActiveProcess(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ActiveProcess)
{
    ui->setupUi(this);
    //  this->setStyleSheet("background-image:url(:/images/im.png)");
    this->setWindowFlags(Qt::WindowTitleHint);

    //设置背景图片
    this->setAutoFillBackground(true);
    QPalette palette;
    //palette.setColor(QPalette::Background,QColor(192,253,123));
    palette.setBrush(QPalette::Background,QBrush(QPixmap(":/images/im.png")));
    this->setPalette(palette);

    //设置字体颜色
    QPalette pal;
    pal.setBrush(QPalette::WindowText,Qt::red);
    ui->label_5->setPalette(pal);
    ui->label_7->setPalette(pal);
    ui->label_8->setPalette(pal);

    codestr=code.getMachineCode();
    //write_file(codestr);//将激活码写入文件
    ui->label_6->setText(codestr);

    printf_info();
}

ActiveProcess::~ActiveProcess()
{
    is_close=true;
    delete ui;

}

/*
 *获取编辑框中的日期，并与当前时间比较
 */
void ActiveProcess::getdate()
{
    QDate currentdate,date1;

    QDateTime time=QDateTime::currentDateTime();
    currentdate=time.date();

    date1=ui->dateEdit->date();
    is_late=date1.operator >(currentdate);
    if(is_late)
    {
        date=date1;
    }else{
        //InfoMsgBox(this,tr("information"),tr("时间已失效"));

        // ui->label_6->setPalette(pal);
        ui->label_5->setText(tr("日期已失效！"));
        ui->lineEdit_2->clear();
        sleep(800);
        ui->label_5->clear();
    }
}

/*
 * 延迟函数
 */

void ActiveProcess::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


/*
 *获取数量，判断是否为空以及是否为数字格式
 */

void ActiveProcess::getenum()
{
    QString numstr;
    num=0;

    numstr=ui->lineEdit->text();

    if(!numstr.isEmpty()&&is_number(numstr))
    {
        num=numstr.toInt();
    }else{
        //弹出对话框，提示输入数量
        if(numstr.isEmpty())
        {
            ui->label_7->setText(tr("请输入数量!"));
            ui->lineEdit_2->clear();
        }else {
            ui->label_7->setText(tr("数量输入错误!"));
            ui->lineEdit->clear();
            ui->lineEdit_2->clear();
        }
        //sleep(3000);
        sleep(1000);
        ui->label_7->clear();
    }
}

/*
   *获取数量输入是否为纯数字格式
   */
bool ActiveProcess::is_number(QString src)
{
    QByteArray ba = src.toLatin1();//QString 转换为 char*
    const char *s = ba.data();

    while(*s && *s>='0' && *s<='9') s++;

    if (*s)
    {
        return false;
    }
    else
    {
        return true;
    }
}


/*
 * 将相应信息注册到本地注册表中
 */
void ActiveProcess::registe_info(QString str)
{

    key.writeDate(date);
    key.writeCount(num);
    key.writeMachineCode(codestr);
    key.writeActiveCode(str);//激活码
}

void ActiveProcess::on_pushButton_clicked()
{
    QString activestr;
    ActiveCode active;
    num=0;

    getdate();
    if(is_late)
        getenum();
    if(num&&is_late&&num>5)   //如果数量不为空，且时间有效
    {
        get_code();
        activestr=active.createActiveCode(date,num,codestr);  //后台计算的激活码
    }
//    ui->label_14->setText(activestr);

    if(currentstr==activestr&&(!activestr.isEmpty())&&num>5)
    {
        registe_info(currentstr); //将相应信息注册到本地注册表中
        QMessageBox::question(this,tr("information"),tr("软件激活成功，是否关闭该窗口?"),QMessageBox::Ok);
        is_act=true;
        this->close();

    }
    else if(currentstr!=activestr&&num&&(!currentstr.isEmpty())&&is_late)
        QMessageBox::critical(this,tr("information"),tr("对不起，激活码不正确，请重新再试！"));
    else if(num&&num<=5)
    {
        InfoMsgBox(this,tr("请选择免费激活！"));
        is_no=true;//免费激活则重新再来
        this->close();
    }
}

/*
 * 获取填入的激活码
 */
void ActiveProcess::get_code()
{
    currentstr=ui->lineEdit_2->text(); //获取填入激活码
    if(currentstr.isEmpty())
        ui->label_8->setText(tr("请填入激活码"));
    sleep(800);
    ui->label_8->clear();

}

/*
 * 判断是否已经激活
 */
void ActiveProcess::is_active()
{
    ActiveCode code;
    int ret;
    is_no=false;
    is_act=false;
    is_close=false;

    ret=code.verifyLocalCode();
    /*
   * 如果激活码正确，则正常登录，如果激活码错误，则弹出激活框，进行选择
   */
    RegistryKey key;
//    qDebug()<<key.readMachineCode();
    if(ret==ACTIVE_RET_EXPIRYDATE)  //激活码已过期   3
    {
        QPixmap map1(":/images/1203596.png");
        QString str1=tr("激活码已过期，请选择激活方式！"); /////////////===================

        RegisterBox *mybox=new RegisterBox(map1,str1);
        mybox->exec();

        if(mybox->is_ok)
        {
            this->exec();
        }
        else
        {
            if(!is_close)
            {
                free_register();
                QApplication::setQuitOnLastWindowClosed(false);
                InfoMsgBox(mybox,tr("免费激活成功！"));
            }
        }
    }

    else if(ret==ACTIVE_RET_UNREGISTERED)  //未注册   0
    {
        QPixmap map2(":/images/doubt4.png");
        QString str2=tr("软件尚未激活，请选择激活方式！");

        RegisterBox *mybox=new RegisterBox(map2,str2);

        QApplication::setQuitOnLastWindowClosed(false);
        mybox->exec();

        if(mybox->is_ok)
        {
            this->exec();
        }
        else
        {
            if(!is_close)  //如果强行关闭，不选择，不会免费注册
            {
                free_register();
                InfoMsgBox(mybox,tr("免费激活成功！"));
            }
        }
    }

    else if(ret==ACTIVE_RET_ERR)  //激活码错误    4
    {
        QPixmap map3(":/images/1205806.png");
        QString str3=tr("激活码错误，请选择激活方式！");  /////////////===================

        RegisterBox *mybox=new RegisterBox(map3,str3);
        mybox->exec();
        if(mybox->is_ok)
        {
            this->exec();
        }
        else
        {
            if(!is_close)
            {
                free_register();
                InfoMsgBox(mybox,tr("免费激活成功！"));
            }
        }
    }
    else  //免费使用或注册成功
        is_act=true;

}

/*
 * 将机器码写入文件
 */
void ActiveProcess::write_file(QString &strr)
{
    QFile data("C:/Users/hongzhunzhun/Desktop/file.txt");      //定义文件
    if(data.open(QFile::Append))    //以写的及重写的方式打开文件，这里面涉及到QIODevice类，是输入/输出设备的基类。
    {
        QTextStream out(&data);                                                //QTextStream类提供了读写文件的基本功能
        out <<strr<<"\n";                                            //正常情况下输出到终端，这里输出到文件，将文件替换终端
    }
}



/*
 * 注销激活码
 */

void ActiveProcess::clearinfo()
{
    RegistryKey key;
//    key.writeMachineCode("helloworld");//当注销时，设置注册表中的机器码为helloworld作为标志位
    key.writeActiveCode("--------------------------");
}

/*
 * 显示激活码等相关信息
 */
void ActiveProcess::printf_info()
{
    //    RegistryKey *key;
    //    int i=key->readCount();
    //    QString strr=QString::number(i,10);
    //    QString strr1=key->readMachineCode();
    //    QString strr2=key->readActiveCode();
    //    QString strr3=key->readDate().toString();

    //    ui->label_9->setText(strr);
    //    ui->label_10->setText(strr1);
    //    ui->label_11->setText(strr2);
    //    ui->label_12->setText(strr3);
}


/*
 * 免费激活
 */
void ActiveProcess::free_register()
{
    ActiveCode active;
    date = QDate::currentDate().addMonths(3);
    num=5;

    codestr=code.getMachineCode();
    QString activestr=active.createActiveCode(date,num,codestr);
    registe_info(activestr);
    is_act=true;
}

