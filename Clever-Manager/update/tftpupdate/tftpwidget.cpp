/*
 * tftpwidget.cpp
 * tftp上传界面类
 *  1、在界面可以设置文件
 *  2、在界面可以启动升级
 *  3、发送过程中，根据实际情况，及时更新至界面
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "tftpwidget.h"
#include "ui_tftpwidget.h"
#include "common.h"

TftpWidget::TftpWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TftpWidget)
{
    ui->setupUi(this);

    mTftpThread = new TftpThread(this);
    connect(mTftpThread, SIGNAL(sentSig(int,QString)), this, SLOT(sentSlot(int,QString)));
    connect(mTftpThread, SIGNAL(progressSig(int)), this,SLOT(progressSlot(int)));
    connect(mTftpThread, SIGNAL(sentFinshSig(int,int,int)), this, SLOT(sentFinshSlot(int,int,int)));

    mProMax = 100;
    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));

    groupBox_background_icon(this);
}

TftpWidget::~TftpWidget()
{
    delete ui;
}

void TftpWidget::timeoutDone(void)
{
    ui->progressBar->setValue(mProgress);
    mProgress += 2;
    if(mProgress>=mProMax)
        mProgress=0;
}

void TftpWidget::setMax(int max)
{
     mProMax = max;
     ui->progressBar->setMaximum(max);
}

/**
 * @brief 增加设备IP地址， 首先得设置IP地址
 * @param list
 */
void TftpWidget::append(QStringList &list)
{
     mTftpThread->append(list);
}

void TftpWidget::on_openBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(0,tr("文件选择"),"/","",0);
    if (!fileName.isNull()) {
        ui->lineEdit->setText(fileName);
    }
    else{ //点的是取消

    }
}

/**
 * @brief 发送状态改变
 * @param id 状态代码
 * @param ip
 */
void TftpWidget::sentSlot(int id, QString ip)
{
    QString str;
    switch (id) {
    case 1:
        str = tr("开始传输 ");
        break;
    case 2:
        str = tr("传输完成 ");
        break;
    case 3:
        str = tr("传输错误 ");
        break;
    case 4:
        str = tr("没有目标设备");
        break;
    default:
        break;
    }
    str += ip;
    ui->stateLab->setText(str);
}

/**
 * @brief 进度
 * @param value
 */
void TftpWidget::progressSlot(int value)
{
    ui->progressBar->setValue(value);
}

/**
 * @brief 开始升级
 */
void TftpWidget::on_updateBtn_clicked()
{
    QString file = ui->lineEdit->text();
    if(!file.isEmpty()) {
        QString str = file.section(".", -1);
        if(str == "bin") {
            mProgress=0;
            timer->start(200);

            mTftpThread->setFile(file);
            mTftpThread->startSent();

            ui->quitBtn->setEnabled(false);
            ui->updateBtn->setEnabled(false);
        } else {
             CriticalMsgBox box(this, tr("升级文件格式错误!, 请重新选择"));
        }
    } else {
         CriticalMsgBox box(this, tr("请选择升级文件"));
    }
}



/**
 * @brief 发送结束时，显示状态信息
 * @param all 升级个数
 * @param num 成功个数
 * @param err 失败个数
 */
void TftpWidget::sentFinshSlot(int all, int num, int err)
{
    QString str = tr("升级个数%1 成功个数%2 失败个数%3").arg(all).arg(num).arg(err);
    ui->numLab->setText(str);

    timer->stop();
    ui->progressBar->setValue(mProMax);

    ui->quitBtn->setEnabled(true);
    InfoMsgBox box(this, str);
}

void TftpWidget::on_quitBtn_clicked()
{
    emit quitSig();
}
