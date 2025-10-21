#include "ds_datashowwid.h"
#include "ui_ds_datashowwid.h"
#include "dev/devType/pdudtname.h"
#include "pdu/data/hash/read/pduhdckeck.h"
#include "listShow/devSetting/setthresholddlg.h"
#include <QDesktopServices>
extern bool usr_land_jur(void);

DS_DataShowWid::DS_DataShowWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DS_DataShowWid)
{
    ui->setupUi(this);

    QTimer::singleShot(1*1000,this,SLOT(initFun())); // 延时
}

DS_DataShowWid::~DS_DataShowWid()
{
    delete ui;
}


void DS_DataShowWid::initFun(void)
{
    mAlarm = 0;
    mDataPacket = NULL;
    mPduTG = new PduPacketTG();
    mPduData = new sPduTGDataStr;

    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(timeoutDone()));
    mTimer->start(1000);
}


/**
 * @brief 设备离线显示
 */
void DS_DataShowWid::clearShow(void)
{
    //   devLab->setText(tr("此设备已离线"));

    ui->curLab->setText("---");
    ui->volLab->setText("---");
    ui->powLab->setText("---");
    ui->eleLab->setText("---");
    ui->pfLab->setText("---");
    //    ui->temLab->setText("---");
    //    ui->humLab->setText("---");

    ui->curProBar->setValue(0);
    ui->volProBar->setValue(0);
    ui->powProBar->setValue(0);
    ui->eleProBar->setValue(0);
    ui->pfProBar->setValue(0);
    //    ui->temProBar->setValue(0);
    //    ui->humProBar->setValue(0);
}

/**
 * @brief 设备检查是否在线
 *      1、设备必须在线
 */
bool DS_DataShowWid::checkDev(void)
{
    bool ret = false;

    if(mDataPacket) {
        if(mDataPacket->offLine > 0)
            ret = true;
    }

    if(ret == false)
        clearShow();

    return ret;
}


/**
 * @brief 显示设备信息
 */
void DS_DataShowWid::dsyDevInfo(void)
{
    if(mDataPacket) {
        QString typeName;
        PduDTName pduTypeName;
        bool ret = pduTypeName.getName(mDataPacket->devType, typeName);
        if(ret)
            ui->typeLab->setText(typeName);
    }

    QString str = mDev.ip +"  ";
    if(mDev.num)
        str += tr(" 副机%1:").arg(mDev.num);
    else
        str += tr(" 主机");
    ui->ipLab->setText(str);

    QString name;
    mDataPacket->info->type->name->get(name);
    if(mDev.num) name = "";
    ui->nameLab->setText(name);

    str.clear();
    mDataPacket->info->addr->area->get(name);
    if(!name.isEmpty()) {
        str = tr("机房：")+name;
    }
    mDataPacket->info->addr->cab->get(name);
    if(!name.isEmpty()) {
        str += tr("  机柜：")+name;
    }
    ui->roomLab->setText(str);

    static int lineNum = 0; //刷新界面
    int phaseNum = mDataPacket->data->line->cur->value->size();
    if(lineNum != phaseNum) {
        lineNum = phaseNum;
        ui->comboBox->clear();
        for(int i = 1; i <= lineNum; i++){
            ui->comboBox->addItem(QString(tr("Line %1").arg(i)));
        }
    }

    QString numStr;
    if(phaseNum>1) {
        numStr = QString(tr("%1相").arg(phaseNum));
        ui->comboBox->setEnabled(true);
    } else {
        numStr = QString(tr("单相"));
        ui->comboBox->setEnabled(false);
    }

    ui->nameLab_3->setText(numStr);
}



void DS_DataShowWid::updateView()
{
    QString str, volStr;
    short line = mDev.line; // 0 表示统计
    if(line == 0) {
        volStr = tr("平均");
        str = tr("总");
    }

    ui->volItemLab->setText(tr("%1电压").arg(volStr));
    ui->curItemLab->setText(tr("%1电流").arg(str));
    ui->powItemLab->setText(tr("%1功率").arg(str));
    ui->eleItemLab->setText(tr("%1电能").arg(str));
}

/**
 * @brief 读取设备数据信息
 */
void DS_DataShowWid::readData(void)
{
    memset(mPduData, 0, sizeof(sPduTGDataStr));
    short line = mDev.line; // 0 表示统计

    if(line) {
        line--;
        PduObjData *lineData = mDataPacket->data->line;
        mPduData->vol = lineData->vol->value->get(line); // 电压数据
        mPduData->cur = lineData->cur->value->get(line); // 电流数据
        mPduData->pow = lineData->pow->get(line); // 功率数据
        mPduData->ele = lineData->ele->get(line); // 电能数据
        mPduData->pf = lineData->pf->get(line); // 功率因数

        PduEnvData *envData = mDataPacket->data->env; // 温湿度
        mPduData->tem = envData->tem->value->get(0);
        mPduData->hum = envData->hum->value->get(0);
    } else {
        sPduTGDataStr *data = mPduTG->getTgData(mDataPacket); // 获取统计数据
        if(data) {
            memcpy(mPduData, data, sizeof(sPduTGDataStr));
            delete data;
        }
    }
    updateView();
}



QString DS_DataShowWid::getValueStr(int data, double rate, int bit)
{
    QString str;

    if(data >= 0)
        str = QString::number((data/rate), 'f', bit);
    else
        str = "---";
    return str;
}

void DS_DataShowWid::setProBarValue(QProgressBar *pro, int min, int max, int value)
{
    if(value > max) {
        value = max;
    }

    pro->setRange(min, max);
    pro->setValue(value);
}


void DS_DataShowWid::updateCur()
{
    double rate = COM_RATE_CUR;
    int line = mDev.line - 1;

    int data = mPduData->cur;
    QString str = getValueStr(data, rate, 1) + "A";
    ui->curLab->setText(str);

    if(data >= 0) {
        PduDataUnit *dataUnit = mDataPacket->data->line->cur;
        int minData = dataUnit->min->get(line);
        str = getValueStr(minData, rate);
        ui->curMinLab->setText(str);

        int maxData = dataUnit->max->get(line);
        str = getValueStr(maxData, rate);
        ui->curMaxLab->setText(str);
        if(minData != maxData)
            setProBarValue(ui->curProBar, minData, maxData, data);
        else
            ui->curProBar->setValue(0);
    } else {
        ui->curProBar->setValue(0);
    }
}

void DS_DataShowWid::updateVol()
{
    double rate = COM_RATE_VOL;
    int line = mDev.line - 1;

    int data = mPduData->vol;
    QString str = getValueStr(data, rate) + "V";
    ui->volLab->setText(str);

    if(data >= 0) {
        PduDataUnit *dataUnit = mDataPacket->data->line->vol;
        int minData = dataUnit->min->get(line);
        str = getValueStr(minData, rate);
        ui->volMinLab->setText(str);

        int maxData = dataUnit->max->get(line);
        str = getValueStr(maxData, rate);
        ui->volMaxLab->setText(str);
        if(minData != maxData)
            setProBarValue(ui->volProBar, minData, maxData, data);
        else
            ui->volProBar->setValue(0);
    } else {
        ui->volProBar->setValue(0);
    }
}

void DS_DataShowWid::updatePf()
{
    double rate = COM_RATE_PF;

    int data = mPduData->pf;
    QString str = getValueStr(data, rate,2);
    ui->pfLab->setText(str);

    if(data >= 0) {
        ui->pfProBar->setValue(data);
    } else {
        ui->pfProBar->setValue(0);
    }
}

void DS_DataShowWid::updateTem()
{
    double rate = COM_RATE_TEM;
    int line = mDev.line - 1;

    int data = mPduData->tem;
    QString str = getValueStr(data, rate) + "℃";
    //    ui->temLab->setText(str);

    if(data >= 0) {
        PduDataUnit *dataUnit = mDataPacket->data->env->tem;
        int minData = dataUnit->min->get(line);
        str = getValueStr(minData, rate);
        //        ui->temMinLab->setText(str);

        int maxData = dataUnit->max->get(line);
        str = getValueStr(maxData, rate);
        //        ui->temMaxLab->setText(str);
        //        setProBarValue(ui->temProBar, minData, maxData, data);
    } else {
        //        ui->temProBar->setValue(0);
    }
}


void DS_DataShowWid::updateHum()
{
    double rate = COM_RATE_HUM;
    int line = mDev.line - 1;

    int data = mPduData->hum;
    QString str = getValueStr(data, rate) + "%";
    //    ui->humLab->setText(str);

    if(data >= 0) {
        PduDataUnit *dataUnit = mDataPacket->data->env->hum;
        int minData = dataUnit->min->get(line);
        str = getValueStr(minData, rate);
        //        ui->humMinLab->setText(str);

        int maxData = dataUnit->max->get(line);
        str = getValueStr(maxData, rate);
        //        ui->humMaxLab->setText(str);
        //        setProBarValue(ui->humProBar, minData, maxData, data);
    } else {
        //        ui->humProBar->setValue(0);
    }
}

/**
 * @brief
 */
void DS_DataShowWid::updataTh()
{
    if(mDataPacket->devType == 0)  // IP设备才显示温湿度
    {
        //        ui->temLab->setHidden(false);
        //        ui->humLab->setHidden(false);
        //        ui->widget_5->setHidden(false);
        //        ui->widget_10->setHidden(false);
    } else {
        //        ui->temLab->setHidden(true);
        //        ui->humLab->setHidden(true);
        //        ui->widget_5->setHidden(true);
        //        ui->widget_10->setHidden(true);
    }
}

void DS_DataShowWid::updatePow()
{
    double rate = COM_RATE_POW;
    int line = mDev.line - 1;
    QString str;

    int data = mPduData->pow;
    //    QString str = getValueStr(data, rate) + "kW";
    if(data >= 0)
        str = QString::number((data/rate), 'f', 3)+ "kW";
    else
        str = "---";
    ui->powLab->setText(str);

    if(data >= 0) {
        PduDataUnit *curDataUnit = mDataPacket->data->line->cur;
        int curMaxData = curDataUnit->max->get(line);

        PduDataUnit *volDataUnit = mDataPacket->data->line->vol;
        int volMaxData = volDataUnit->max->get(line);
        int maxPow = volMaxData * curMaxData;
        if(curDataUnit->min->get(line)!=curDataUnit->max->get(line)&&volDataUnit->min->get(line)!=volDataUnit->max->get(line))
            setProBarValue(ui->powProBar, 0, maxPow, data);
        else
            ui->powProBar->setValue(0);
    } else {
        ui->powProBar->setValue(0);
    }
}

void DS_DataShowWid::updateEle()
{
    QString str;
    int data = mPduData->ele;
    if(data >= 0)
        str = QString::number((data/COM_RATE_ELE), 'f', 3)+ "kWh";
    else
        str = "---";
    ui->eleLab->setText(str);
}

/**
 * @brief 刷新数据显示
 */
void DS_DataShowWid::updateData(void)
{
    readData(); // 读取设备数据
    updateCur();
    updateVol();
    updatePf();

    updatePow();
    updateTem();
    updateHum();
    updataTh(); // 更新温湿度数据
    updateEle();
}


/**
 * @brief 设置默认颜色
 */
void DS_DataShowWid::setDefaultColor(void)
{
    QString str = "color: rgb(255, 255, 255);";
    ui->curLab->setStyleSheet(str);
    ui->volLab->setStyleSheet(str);
    //    ui->temLab->setStyleSheet(str);
    //    ui->humLab->setStyleSheet(str);
}



/**
 * @brief 设置报警色
 * @param alarm
 */
void DS_DataShowWid::setAlarmColor(int alarm)
{
    QLabel *lcd=NULL;

    if(alarm == 1)
        lcd = ui->curLab;
    else if(alarm == 2)
        lcd = ui->volLab;
    //    else if(alarm == 3)
    //        lcd = ui->temLab;
    //    else if(alarm == 4)
    //        lcd = ui->humLab;

    if(lcd)
        lcd->setStyleSheet("color: red");
}


/**
 * @brief 检查报警类型
 */
void DS_DataShowWid::checkAlarm(void)
{
    static short alarm=0;

    if(alarm != mAlarm)
    {
        alarm = mAlarm;

        setDefaultColor();
        if(alarm &0x01)
            setAlarmColor(1);
        if(alarm &0x02)
            setAlarmColor(2);
        if(alarm &0x04)
            setAlarmColor(3);
        if(alarm &0x08)
            setAlarmColor(4);
    }
}


/**
 * @brief 检查设备的报警状态
 * @param line
 */
void DS_DataShowWid::checkStatus(void)
{
    mAlarm = 0;
    int line = mDev.line;
    if(line > 0)
    {
        line--;
        PduObjData *lineData = mDataPacket->data->line;
        int ret = pdu_data_alarmCheck(lineData->cur, line);
        if(ret > 0)
            mAlarm |= (1<<0);
        else
            mAlarm &= ~(1<<0);

        ret = pdu_data_alarmCheck(lineData->vol, line);
        if(ret > 0)
            mAlarm |= (1<<1);
        else
            mAlarm &= ~(1<<1);
    }

    PduEnvData *envData = mDataPacket->data->env;
    int ret = pdu_data_checkAlarm(envData->tem, 0);  // 检查温度数据
    if(ret > 0)
        mAlarm |= (1<<2);
    else
        mAlarm &= ~(1<<2);

    ret = pdu_data_checkAlarm(envData->hum, 0);  // 检查温度数据
    if(ret > 0)
        mAlarm |= (1<<3);
    else
        mAlarm &= ~(1<<3);

    checkAlarm();
}


/**
 * @brief 定时器响应函数
 */
void DS_DataShowWid::timeoutDone(void)
{
    bool ret = checkDev();
    if(ret) {
        dsyDevInfo();
        updateData();

        checkStatus();
    }
}



/**
 * @brief 数据更新
 * @param ip
 */
void DS_DataShowWid::updateData(QString &ip, int num)
{
    mDev.ip = ip;
    mDev.num = num; //获取第一个机器编号：0 -1
    mDev.line = 1;
    ui->comboBox->setCurrentIndex(0);  // 设置第一相

    mDataPacket = pdu_hd_getPacket(ip, num);
    timeoutDone();

    if(!mDev.ip.isEmpty())
        emit selectDevSig(mDev);
}


void DS_DataShowWid::on_comboBox_currentIndexChanged(int index)
{
    mDev.line = index + 1;
    if(!mDev.ip.isEmpty())
        emit selectDevSig(mDev);
}

void DS_DataShowWid::on_webBtn_clicked()
{
    bool ret = usr_land_jur();
    if(ret==false){
        InfoMsgBox box(this,tr("您权限不够，无法操作"));
        return ;
    }

    if(!mDev.ip.isEmpty()) {
        QString addr = "http://"+mDev.ip+"/";
        QDesktopServices::openUrl(QUrl(addr)); // 打开浏览器
    }
}


void DS_DataShowWid::setThreshold(int mode, int id)
{
    bool ret = checkDev();
    if(ret) {
        SetThresholdDlg dlg(this);
        dlg.setMode(mode, id, mDataPacket);
        int ret = dlg.exec();
        if(ret == QDialog::Accepted)
            updateData();
    }
}

void DS_DataShowWid::on_curBtn_clicked()
{
    setThreshold(SET_CMD_LINE_CUR,mDev.line-1);
}

void DS_DataShowWid::on_volBtn_clicked()
{
    if(mDataPacket) {
        if(mDataPacket->devType ==  PDU_TYPE_RPDU || mDataPacket->devType ==  PDU_TYPE_NPM_PDU) { // 为RPDU NPM不能设置电压
            InfoMsgBox box(0,tr("PDU设备不支持此功能!"));
            return;
        }
    }
    setThreshold(SET_CMD_LINE_VOL,mDev.line-1);
}


void DS_DataShowWid::on_temBtn_clicked()
{
    setThreshold(SET_CMD_ENV_TEM);
}

void DS_DataShowWid::on_humBtn_clicked()
{
    setThreshold(SET_CMD_ENV_HUM);
}
