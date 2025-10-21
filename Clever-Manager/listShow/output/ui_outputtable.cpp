/*
 * ui_outputtable.cpp
 * 设备输出位表格显示界面
 *   祥细的显示输出位的名称、开关状态、电流、功率、功率因数、最大值、最小值、临界状态等信息
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "ui_outputtable.h"
#include "common/icon/iconpath.h"
#include "listShow/devSetting/setswitchdlg.h"

UI_OutputTable::UI_OutputTable(QWidget *parent) : QWidget(parent)
{
    mTableWidget = new QTableWidget(this);
    layout = new QGridLayout(this);
    layout->addWidget(mTableWidget);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    //    mTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选择
    //    mTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(mTableWidget,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),
            this,SLOT(updateWidget(QTableWidgetItem *)));

    QTimer::singleShot(5,this,SLOT(initFunSlot())); // 延时
}


/**
 * @brief 初始化函数
 */
void UI_OutputTable::initFunSlot()
{
    mDataPacket = NULL;
    initWidget();
    initIcon();

    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    timer->start(2*1000);
}

/**
 * @brief 初始化图标
 */
void UI_OutputTable::initIcon(void)
{
    QString path;
    icon_get_opStatus(0, path);
    mIcon.addFile(path);

    icon_get_opStatus(1, path);
    mAlarmIcon.addFile(path);

    icon_get_opStatus(2, path);
    mOpenIcon.addFile(path);

    icon_get_opStatus(3, path);
    mCloseIcon.addFile(path);
}

/**
 * @brief 初始化窗口
 */
void UI_OutputTable::initWidget(void)
{
    mTableWidget->clear();
    mTableWidget->setRowCount(0);        //设置行数/
    mTableWidget->setFont(QFont("微软雅黑",10));
    mTableWidget->horizontalHeader()->setFont(QFont("微软雅黑",10));
    QStringList header;
    header <<tr("输出位名称")<<tr("开关状态") << tr("输出电流") << tr("输出功率")
          << tr("功率因数") << tr("最小值") << tr("最大值")
          << tr("临界下值") << tr("临界上值") << tr("接近临界");
    mTableWidget->setColumnCount(header.size());    //设置列数
    mTableWidget->setHorizontalHeaderLabels(header);
    mTableWidget->setStyleSheet("background-color:white");
    //    mTableWidget->horizontalHeader()->setFixedHeight(25);
    mTableWidget->horizontalHeader()->setStyleSheet("background-color:rgb(234,234,233)");
    //    mTableWidget->verticalHeader()->setFixedWidth(20);

    mTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mTableWidget->setColumnWidth(0,200);
    mTableWidget->horizontalHeader()->setStretchLastSection(true);
}


/**
 * @brief 增加Item
 * @param row 行
 * @param column 列
 * @param content 信息
 */
void UI_OutputTable::addItemContent(int row, int column, const QString &content)
{
    QTableWidgetItem *item = new QTableWidgetItem(content);
    if(column > 1)
        item->setTextAlignment(Qt::AlignCenter);
    else
        item->setTextAlignment(Qt::AlignVCenter);

    mTableWidget->setItem(row, column, item);
}

/**
 * @brief 增加一行Content
 * @param list 显示的内容
 */
void UI_OutputTable::addRowContent(QStringList &list)
{
    int row = mTableWidget->rowCount();
    mTableWidget->insertRow(row);
    //    mTableWidget->setRowHeight(row, 40);

    for(int i=0; i<list.size(); ++i)
        addItemContent(row,i,list[i]);

    list.clear();
}

/**
 * @brief 初始化一行表格
 * @param str
 */
void UI_OutputTable::initTable(void)
{
    QStringList list;
    int column = mTableWidget->columnCount();
    for(int j=0; j<column; ++j)
        list << "---";
    addRowContent(list);
}

/**
 * @brief 初始化表格窗口
 */
void UI_OutputTable::initTableWidget(void)
{
    initWidget();

    PduDataUnit *unit = mDataPacket->data->output->cur;
    int size = unit->value->size(); // 获取输出位的数量
    if(!size) {
        size = mDataPacket->data->output->sw->size();
    }
    for(int i=0; i<size; ++i)
        initTable();
}

/**
 * @brief 清空表格
 */
void UI_OutputTable::clearTable(void)
{
    QTableWidgetItem *item;
    int row = mTableWidget->rowCount();
    int column = mTableWidget->columnCount();

    for(int i=0; i<row; ++i)
    {
        for(int j=0; j<column; ++j)
        {
            item = mTableWidget->item(i, j);
            item->setText("---");
        }
    }
}

/**
 * @brief 检查设备是否在线
 * @return
 */
bool UI_OutputTable::checkDev(void)
{
    bool ret = false;
    if(mDataPacket)
    {
        if(mDataPacket->offLine > 0)
            ret = true;
        else
            clearTable();
    }
    return ret;
}

/**
 * @brief 表格检查 当输出位与表格行数不相等时，需要重新建立表
 * @return true 表示需要重新建立表格
 */
bool UI_OutputTable::checkTable(void)
{
    bool ret = false;

    PduDataUnit *unit = mDataPacket->data->output->cur;
    int size = unit->value->size();
    if(!size) {
        size = mDataPacket->data->output->sw->size();
    }

    int row = mTableWidget->rowCount();
    if(size != row)
        ret = true;

    return ret;
}

/**
 * @brief 设置输出位状态图标
 * @param id 输出位
 */
void UI_OutputTable::setIcon(int id, int column)
{
    QTableWidgetItem *item = mTableWidget->item(id, column);

    PduDataUnit *unit = mDataPacket->data->output->cur;
    int data = unit->alarm->get(id);
    if(data > 0) // 报警
        item->setIcon(mAlarmIcon);
    else
        item->setIcon(mIcon);
}

/**
 * @brief 设置输出位名字
 * @param id
 */
void UI_OutputTable::setOutputName(int id, int column)
{
    QTableWidgetItem *item = mTableWidget->item(id, column);

    QString name = tr("Output %1").arg(id+1);
    mDataPacket->output->name->get(id, name);
    item->setText(name);

    setIcon(id, column); // 设置图标
}

/**
 * @brief 设置开关状态
 * @param id
 */
void UI_OutputTable::setSwState(int id, int column)
{
    QTableWidgetItem *item = mTableWidget->item(id, column);
    bool sw = true;
    int ret = mDataPacket->data->output->sw->get(id);

    if(ret == 1){
        item->setText(tr("关"));
        item->setIcon(mCloseIcon); // 关
    }
    else if(ret == 2)
    {
        item->setText(tr("开"));
        item->setIcon(mOpenIcon);
    }
}

/**
 * @brief 输出位电流 电流变成红色
 * @param id
 */
void UI_OutputTable::setCur(int id, int column)
{
    QTableWidgetItem *item = mTableWidget->item(id, column);
    PduDataUnit *unit = mDataPacket->data->output->cur;

    // 显示值
    double value = unit->value->get(id) / COM_RATE_CUR;
    if(value >= 0)
        item->setText(QString::number(value, 'f', 1) + "A");
    else
        item->setText("---");

    // 颜色
    int data = unit->alarm->get(id);
    if(data > 0) // 报警
        item->setTextColor(QColor(Qt::red));
    else
        item->setTextColor(QColor(Qt::black));
}

/**
 * @brief 功率显示
 * @param id 输出位
 * @param column
 */
void UI_OutputTable::setPow(int id, int column)
{
    QTableWidgetItem *item = mTableWidget->item(id, column);
    PduDataBase *value = mDataPacket->data->output->pow;

    double data = value->get(id) / COM_RATE_POW;
    if(data >= 0)
        item->setText(QString::number(data, 'f', 3) + "kW");
    else
        item->setText("---");
}

/**
 * @brief 功率因数
 * @param id 输出位
 * @param column
 */
void UI_OutputTable::setPf(int id, int column)
{
    QTableWidgetItem *item = mTableWidget->item(id, column);
    PduDataBase *value = mDataPacket->data->output->pf;

    double data = value->get(id) / COM_RATE_PF;
    if(data >= 0)
        item->setText(QString::number(data, 'f', 2));
    else
        item->setText("---");
}

/**
 * @brief 电流最小值
 * @param id
 * @param column
 */
void UI_OutputTable::setCurMin(int id, int column)
{
    QTableWidgetItem *item = mTableWidget->item(id, column);
    PduDataUnit *unit = mDataPacket->data->output->cur;

    double value = unit->min->get(id) / COM_RATE_CUR;
    if(value >= 0)
        item->setText(QString::number(value, 'f', 1) + "A");
    else
        item->setText("---");
}

/**
 * @brief 电流最大值
 * @param id
 * @param column
 */
void UI_OutputTable::setCurMax(int id, int column)
{
    QTableWidgetItem *item = mTableWidget->item(id, column);
    PduDataUnit *unit = mDataPacket->data->output->cur;

    double value = unit->max->get(id) / COM_RATE_CUR;
    if(value >= 0)
        item->setText(QString::number(value, 'f', 1) + "A");
    else
        item->setText("---");
}


/**
 * @brief 临界最小值
 * @param id
 * @param column
 */
void UI_OutputTable::setCrMin(int id, int column)
{
    QTableWidgetItem *item = mTableWidget->item(id, column);
    PduDataUnit *unit = mDataPacket->data->output->cur;

    double value = unit->crMin->get(id) / COM_RATE_CUR;
    if(value >= 0)
        item->setText(QString::number(value, 'f', 1) + "A");
    else
        item->setText("---");
}

/**
 * @brief 临界最大值
 * @param id
 * @param column
 */
void UI_OutputTable::setCrMax(int id, int column)
{
    QTableWidgetItem *item = mTableWidget->item(id, column);
    PduDataUnit *unit = mDataPacket->data->output->cur;

    double value = unit->crMax->get(id) / COM_RATE_CUR;
    if(value >= 0)
        item->setText(QString::number(value, 'f', 1) + "A");
    else
        item->setText("---");
}
/**
 * @brief 临界状态
 * @param id 输出位
 * @param column 所在列数
 */
void UI_OutputTable::setCritical(int id, int column)
{
    QTableWidgetItem *item = mTableWidget->item(id, column);
    PduDataUnit *unit = mDataPacket->data->output->cur;

    int value = unit->crAlarm->get(id);
    if(value > 0)
        item->setText(tr("已临界"));
    else
        item->setText(tr("没临界"));
}

/**
 * @brief 数据更新入口函数
 */
void UI_OutputTable::updateData(void)
{
    bool ret = checkDev();
    if(ret)
    {
        ret = checkTable();
        if(ret)
            initTableWidget(); // 重新建立表格

        int row = mTableWidget->rowCount();
        for(int i=0; i<row; ++i)
        {
            int k=0;

            setOutputName(i, k++); // 设置输出位名称
            setSwState(i, k++); // 设置开关状态
            setCur(i, k++); // 设置电流值

            setPow(i, k++); // 功率
            setPf(i, k++); // 功率因数

            setCurMin(i, k++); //电流最小值
            setCurMax(i, k++); // 电流最大值

            setCrMin(i, k++); // 临界最小值
            setCrMax(i, k++); // 临界最大值
            setCritical(i, k++); // 临界状态
        }
        reconizeSerial(mDataPacket->devSpec);
    }
}

void UI_OutputTable::reconizeSerial(int type)
{
     switch (type) {
    case 1:
        //  mTableWidget->clear();
        //  mDataPacket->data->loop->sw->setAll(-1);
        aSerialSetting();
        break;
    case 2: //只监测，无开关，隐藏开关状态
        //  mTableWidget->setColumnHidden(1,true);
        bSerialSetting();
        break;
    case 3: //只有开关，而无阈值
        //  for(int i = 2 ; i < 10 ; i++)
        //     mTableWidget->setColumnHidden(i,true);
        //   mTableWidget->setColumnHidden(1,false);
        cSerialSetting();
        break;
    case 4:
        //        mTableWidget->setColumnHidden(1,false);
        break;
    default:
        break;
    }
}

void UI_OutputTable::cSerialSetting()
{
    for(int j = 0 ; j < mTableWidget->rowCount() ; j++)
        for(int i = 2 ; i < mTableWidget->columnCount(); i++)
        {
            QTableWidgetItem *item = mTableWidget->item(j,i);
            item->setText("---");
        }
}

void UI_OutputTable::aSerialSetting()
{
    for(int j = 0 ; j < mTableWidget->rowCount() ; j++)
        for(int i = 0 ; i < mTableWidget->columnCount(); i++)
        {
            QTableWidgetItem *item = mTableWidget->item(j,i);
            item->setText("---");
        }
}

void UI_OutputTable::bSerialSetting()
{
    for(int j = 0 ; j < mTableWidget->rowCount() ; j++)
    {
        QTableWidgetItem *item = mTableWidget->item(j,1);
        item->setText("---");
    }
}

/**
 * @brief 更新
 * @param ip
 * @param num
 */
void UI_OutputTable::updateWidget(QString &ip,int num)
{
    mDataPacket = pdu_hd_getPacket(ip, num);
    updateData();
}

/**
 * @brief 定时器响应函数
 */
void UI_OutputTable::timeoutDone(void)
{
    updateData();
}

/**
 * @brief 输出位开关控制
 */
void UI_OutputTable::switchControl(void)
{
    int row = mTableWidget->currentRow();
    QString name = mTableWidget->item(row, 0)->text(); // 输出位名称

    if(mDataPacket)
    {
        setSwitchDlg dlg(this);
        dlg.setMode(row, name,mDataPacket);
        int ret = dlg.exec();
        if(ret == QDialog::Accepted)
            updateData();
    }
}

/**
 * @brief 设置输出位阈值
 */
void UI_OutputTable::setOutput(void)
{
    int row = mTableWidget->currentRow();
    QString name = mTableWidget->item(row, 0)->text(); // 输出位名称

    if(mDataPacket)
    {
        SetOutputDlg dlg(this);
        dlg.setMode(row, name,mDataPacket);
        int ret = dlg.exec();
        if(ret == QDialog::Accepted)
            updateData();
    }
}

void UI_OutputTable::updateWidget(QTableWidgetItem *it)
{
    bool ret = usr_land_jur();  //管理权限
    if(ret==false)
        return ;

    int coulumn = mTableWidget->currentColumn();
    if((coulumn == 1) && (mDataPacket->devSpec != 2)) //B系列无开关控制
        switchControl();
    else if(((coulumn>4) && (coulumn<9)) && (mDataPacket->devSpec != 3)) //C系列无阈值设置
        setOutput();
    else
        it->setFlags(it->flags() & (~Qt::ItemIsEditable)); // 单元格不能编辑
}
