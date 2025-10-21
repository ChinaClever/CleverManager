/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "pdu_linewid.h"
#include "ui_pdu_linewid.h"
#include <QDesktopServices>
#include "pdu_set/pdu_setthresholddlg.h"

extern bool usr_land_jur();

Pdu_LineWid::Pdu_LineWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Pdu_LineWid)
{
    ui->setupUi(this);

    mLine = 0;
    mPacket = NULL;
    mGraph = new Pdu_LineGraph(ui->graphWid);


    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(timeoutDone()));
    mTimer->start(1000);
}

Pdu_LineWid::~Pdu_LineWid()
{
    delete ui;
}



/**
 * @brief 设备离线显示
 */
void Pdu_LineWid::clearShow()
{
    ui->curLab->setText("---");
    ui->volLab->setText("---");
    ui->powLab->setText("---");
    ui->eleLab->setText("---");
    ui->pfLab->setText("---");

    ui->curProBar->setValue(0);
    ui->volProBar->setValue(0);
    ui->powProBar->setValue(0);
    ui->eleProBar->setValue(0);
    ui->pfProBar->setValue(0);
}

/**
 * @brief 设备检查是否在线
 *      1、设备必须在线
 */
bool Pdu_LineWid::checkDev()
{
    bool ret = false;

    if(mPacket) {
        if((mPacket->offLine > 0) &&(mPacket->en))
            ret = true;
    }

    if(ret == false)
        clearShow();

    return ret;
}


void Pdu_LineWid::initLineName()
{
    static int lineNum = 0; //刷新界面
    int phaseNum = mPacket->data.line.size;
    if(lineNum != phaseNum) {
        lineNum = phaseNum;
        ui->comboBox->clear();
        for(int i = 1; i <= lineNum; i++){
            ui->comboBox->addItem(QString("Line %1").arg(i));
        }
    }

    QString numStr;
    if(phaseNum>1) {
        numStr = tr("%1相").arg(phaseNum);
        ui->comboBox->setEnabled(true);
    } else {
        numStr = tr("单相");
        ui->comboBox->setEnabled(false);
    }

    ui->nameLab_3->setText(numStr);
}

/**
 * @brief 显示设备信息
 */
void Pdu_LineWid::dsyDevInfo()
{
    ui->typeLab->setText(mPacket->dev_type);
    QString str = mPacket->net.ip +"  " + mPacket->dev_num;
    ui->ipLab->setText(str);

    ui->nameLab->setText(mPacket->name);
    str = tr("机房：%1 机柜：%2").arg(mPacket->room).arg(mPacket->cab);
    ui->roomLab->setText(str);

    initLineName();
}



QString Pdu_LineWid::getValueStr(int data, double rate, int bit)
{
    QString str;

    if(data >= 0)
        str = QString::number((data/rate), 'f', bit);
    else
        str = "---";
    return str;
}

void Pdu_LineWid::setProBarValue(QProgressBar *pro, int min, int max, int value)
{
    if(value > max) {
        value = max;
    }
    if(max == 0) max = 100;

    pro->setRange(min, max);
    pro->setValue(value);
}


void Pdu_LineWid::updateCur()
{
    int line = mLine;
    double rate = COM_RATE_CUR;
    sDataUnit *unit = &(mPacket->data.line.cur);

    int data = unit->value[line];
    QString str = getValueStr(data, rate, 1) + "A";
    ui->curLab->setText(str);

    if(data >= 0) {
        int minData = unit->min[line];
        str = getValueStr(minData, rate);
        ui->curMinLab->setText(str);

        int maxData = unit->max[line];
        str = getValueStr(maxData, rate);
        ui->curMaxLab->setText(str);
        setProBarValue(ui->curProBar, minData, maxData, data);
    } else {
        ui->curProBar->setValue(0);
    }
}

void Pdu_LineWid::updateVol()
{
    int line = mLine;
    double rate = COM_RATE_VOL;
    sDataUnit *unit = &(mPacket->data.line.vol);

    int data =unit->value[line];
    QString str = getValueStr(data, rate) + "V";
    ui->volLab->setText(str);

    if(data >= 0) {
        int minData = unit->min[line];
        str = getValueStr(minData, rate);
        ui->volMinLab->setText(str);

        int maxData = unit->max[line];
        str = getValueStr(maxData, rate);
        ui->volMaxLab->setText(str);
        setProBarValue(ui->volProBar, minData, maxData, data);
    } else {
        ui->volProBar->setValue(0);
    }
}

void Pdu_LineWid::updatePf()
{
    double rate = COM_RATE_PF;
    sObjData *obj = &(mPacket->data.line);

    int data = obj->pf[mLine];
    QString str = getValueStr(data, rate,2);
    ui->pfLab->setText(str);

    if(data >= 0) {
        ui->pfProBar->setValue(data);
    } else {
        ui->pfProBar->setValue(0);
    }
}


void Pdu_LineWid::updatePow()
{
    int line = mLine;
    double rate = COM_RATE_POW;
    sObjData *obj = &(mPacket->data.line);

    int data = obj->pow[line];
    QString str = QString::number((data/rate), 'f', 3)+ "kW";
    ui->powLab->setText(str);
    if(data >= 0) {
        int curMaxData = obj->cur.max[line];
        int volMaxData = obj->vol.max[line];
        int maxPow = volMaxData * curMaxData;
        setProBarValue(ui->powProBar, 0, maxPow, data);
    } else {
        ui->powProBar->setValue(0);
    }
}

void Pdu_LineWid::updateEle()
{
    int line = mLine;
    sObjData *obj = &(mPacket->data.line);
    int data = obj->ele[line];

    QString str = QString::number((data/COM_RATE_ELE), 'f', 3)+ "kWh";
    ui->eleLab->setText(str);
}

/**
 * @brief 刷新数据显示
 */
void Pdu_LineWid::updateData()
{
    updateCur();
    updateVol();
    updatePf();
    updatePow();
    updateEle();
}


/**
 * @brief 设置默认颜色
 */
void Pdu_LineWid::setDefaultColor()
{
    QString str = "color: rgb(255, 255, 255);";
    ui->curLab->setStyleSheet(str);
    ui->volLab->setStyleSheet(str);
}


/**
 * @brief 检查设备的报警状态
 * @param line
 */
void Pdu_LineWid::checkStatus()
{
    int line = mLine;

    setDefaultColor();
    sObjData *obj = &(mPacket->data.line);
    int ret = obj->cur.alarm[line];
    if(ret) ui->curLab->setStyleSheet("color: red");

    ret = obj->vol.alarm[line];
    if(ret) ui->volLab->setStyleSheet("color: red");
}


/**
 * @brief 定时器响应函数
 */
void Pdu_LineWid::timeoutDone()
{
    bool ret = checkDev();
    if(ret) {
        dsyDevInfo();
        updateData();
//        checkStatus();
    }
}

void Pdu_LineWid::packetSlot(sDataPacket *p)
{
    mPacket = p;
    mLine = 0;

    timeoutDone();
//    mGraph->packetSlot(p, mLine);
}



void Pdu_LineWid::on_comboBox_currentIndexChanged(int index)
{
    mLine = index;
    mGraph->packetSlot(mPacket, mLine);
}

void Pdu_LineWid::on_webBtn_clicked()
{    
    if(mPacket) {
        if(!mPacket->net.ip.isEmpty()) {
            QString addr = "http://"+mPacket->net.ip+"/";
            QDesktopServices::openUrl(QUrl(addr)); // 打开浏览器
        }
    }
}


void Pdu_LineWid::setThreshold(int mode, int id)
{
    bool ret = usr_land_jur();
    if(ret==false){
        InfoMsgBox box(this,tr("您权限不够，无法操作"));
        return;
    }
    if(!mPacket) return;
    else if(!mPacket->en) return;

    ret = checkDev();
    if(ret) {
        Pdu_SetThresholdDlg dlg(this);
        dlg.setMode(mode, id, mPacket);
        int ret = dlg.exec();
        if(ret == QDialog::Accepted)
            updateData();
    }
}

void Pdu_LineWid::on_curBtn_clicked()
{
    setThreshold(SET_CMD_LINE_CUR, mLine);
}

void Pdu_LineWid::on_volBtn_clicked()
{
    if(mPacket) {
        if(mPacket->devType ==  PDU_TYPE_RPDU || mPacket->devType ==  PDU_TYPE_NPM_PDU) { // 为RPDU NPM不能设置电压
            InfoMsgBox box(0,tr("PDU设备不支持此功能!"));
            return;
        }
    }
    setThreshold(SET_CMD_LINE_VOL, mLine);
}

