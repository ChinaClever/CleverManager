#include "ui_envtable.h"
#include "common/icon/iconpath.h"
#include "listShow/devSetting/setthresholddlg.h"
#include<qlabel.h>
#include "common.h"

UI_EnvTable::UI_EnvTable(QWidget *parent) : QWidget(parent)
{
    mGroupBox = new QGroupBox(this);


    mTableWidget = new QTableWidget(mGroupBox);//tr("环境状态 -----"),
    mTableWidget->setStyleSheet("background-color:white");
    //    set_background_color(mTableWidget, QColor(255,255,255));
    mTableWidget->horizontalHeader()->setHighlightSections(false);
    mTableWidget->verticalHeader()->setHighlightSections(false);
    mTableWidget->horizontalHeader()->setStyleSheet("background-color:rgb(235,235,235)");
    mTableWidget->verticalHeader()->setHighlightSections(false);
    mTableWidget->verticalHeader()->setStyleSheet("background-color:rgb(235,235,235)");
    mTableWidget->horizontalHeader()->setFixedHeight(25);
    mTableWidget->verticalHeader()->setFixedWidth(20);
    initLayout();

    //    mTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选择
    //    mTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(mTableWidget,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),
            this,SLOT(updateWidget(QTableWidgetItem *)));

    QTimer::singleShot(5,this,SLOT(initFunSlot())); // 延时
}


/**
 * @brief 布局
 */
void UI_EnvTable::initLayout(void)
{
    QGridLayout *layout = new QGridLayout(mGroupBox);
    layout->addWidget(mTableWidget);
    //    mTableWidget->setStyleSheet("background-color:white");

    //mGroupBox->setStyleSheet("background-color:white");
    mGroupBox->setStyleSheet("border:none");
    little_widget=new QWidget(this);

    little_widget->setStyleSheet("background-color:rgb(0,17,55)");
    little_layout=new QVBoxLayout(little_widget);
    little_layout->addWidget(mGroupBox);
    little_layout->setContentsMargins(8,0,15,0);

    //    layout->setSpacing(0);
    //    layout->setContentsMargins(5, 11, 0, 0);

    QGridLayout *pLayout = new QGridLayout(this);
    pLayout->setContentsMargins(0,0,0,0);
    QLabel *title=new QLabel(tr("  环境状态"),this);
    title->setStyleSheet("background-color:black");
    QPalette pal;
    pal.setColor(QPalette::WindowText,Qt::white);
    title->setPalette(pal);
    title->setFixedHeight(0);
    title->setFont(QFont("微软雅黑",12));
    pLayout->addWidget(title);
    pLayout->addWidget(little_widget);


    // groupBox_background_icon(this);
}



/**
 * @brief 初始化函数
 */
void UI_EnvTable::initFunSlot()
{
    mDataPacket = NULL;
    initTableWidget();
    initIcon();

    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    timer->start(2*1000);
}

/**
 * @brief 初始化图标
 */
void UI_EnvTable::initIcon(void)
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
void UI_EnvTable::initWidget(void)
{
    mTableWidget->clear();
    mTableWidget->setFont(QFont("微软雅黑",10));
    mTableWidget->horizontalHeader()->setFont(QFont("微软雅黑",10));
    mTableWidget->setRowCount(0);        //设置行数/
    mItemList.clear();

    QStringList header;
    //    header <<tr("传感器名称")<<tr("传感器状态")<<tr("最大值")<<tr("最小值");
    header << tr("传感器名称") << tr("传感器状态") <<tr("最小值") << tr("最大值");
    mTableWidget->setColumnCount(header.size());    //设置列数
    mTableWidget->setHorizontalHeaderLabels(header);

    mTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mTableWidget->horizontalHeader()->setStretchLastSection(true);
    mTableWidget->verticalHeader()->setFixedWidth(25);

}


/**
 * @brief 增加Item
 * @param row 行
 * @param column 列
 * @param content 信息
 */
void UI_EnvTable::addItemContent(int row, int column, const QString &content)
{
    QTableWidgetItem *item = new QTableWidgetItem(content);
    if(column > 0)
        item->setTextAlignment(Qt::AlignCenter);
    else
        item->setTextAlignment(Qt::AlignVCenter);
    mTableWidget->setItem(row, column, item);

    if(column > 0)
        mItemList.append(item);
}

/**
 * @brief 增加一行Content
 * @param list 显示的内容
 */
void UI_EnvTable::addRowContent(QStringList &list)
{
    int row = mTableWidget->rowCount();
    mTableWidget->insertRow(row);
    mTableWidget->setRowHeight(row, 40);

    for(int i=0; i<list.size(); ++i)
        addItemContent(row,i,list[i]);

    list.clear();
}

/**
 * @brief 初始化一行表格
 * @param str
 */
void UI_EnvTable::initTable(const QString &str)
{
    QStringList list;
    list << str;
    int column = mTableWidget->columnCount();
    for(int j=1; j<column; ++j)
        list << "---";
    addRowContent(list);
}

/**
 * @brief 初始化表格窗口
 */
void UI_EnvTable::initTableWidget(void)
{
    initWidget();

    for(int i=0; i<4; ++i)
        initTable(tr("温度 %1").arg(i+1));

    for(int i=0; i<4; ++i)
        initTable(tr("湿度 %1").arg(i+1));

    for(int i=0; i<2; ++i)
    {
        QStringList list;
        list << tr("门禁 %1").arg(i+1) << "---";
        addRowContent(list);
    }

    {
        QStringList list;
        list << tr("水浸") << "---";
        addRowContent(list);
    }

    {
        QStringList list;
        list << tr("烟雾") << "---";
        addRowContent(list);
    }
}


/**
 * @brief 设置基本数据
 * @param id 传感器编号
 * @param data 传感器数据
 * @param row 行数
 * @param column 列数
 * @param str 单位
 */
void UI_EnvTable::setDataBase(int id, PduDataBase *data, int row, int column, double rate, const QString &str)
{
    QTableWidgetItem *item = mTableWidget->item(row, column);

    char value = (char)(data->get(id) / rate);
    if(value >= 0)
        item->setText(QString::number(value, 'f', 1)+str);
    else
        item->setText("---");
}

/**
 * @brief 设置报警颜色
 * @param id 传感器编号
 * @param data 传感器数据
 * @param row 行数
 */
void UI_EnvTable::setAlarmColor(int id, PduDataBase *data, int row)
{
    QTableWidgetItem *item = mTableWidget->item(row, 1);

    int value = data->get(id);
    if(value > 0) // 报警
    {
        item->setIcon(mAlarmIcon);
        item->setTextColor(QColor(Qt::red));
    }
    else
    {
        if(value == 0)
            item->setIcon(mIcon);
        else
            item->setIcon(mCloseIcon);

        item->setTextColor(QColor(Qt::black));
    }
}

/**
 * @brief 显示一行的温湿度信息
 * @param id 传感器编号
 */
void UI_EnvTable::setOneRow(int id, PduDataUnit *unit, int row, double rate, const QString &str)
{
    int column = 1;

    setDataBase(id, unit->value, row, column++, rate, str);
    setDataBase(id, unit->min, row, column++, rate, str);
    setDataBase(id, unit->max, row, column++, rate, str);

    setAlarmColor(id, unit->alarm, row);
}

/**
 * @brief 设置温度
 */
void UI_EnvTable::setTemp(void)
{
    PduDataUnit *unit = mDataPacket->data->env->tem;
    for(int i=0; i<4; ++i)
        setOneRow(i, unit, i, COM_RATE_TEM, "℃");
}

/**
 * @brief 设置湿度
 */
void UI_EnvTable::setHum(void)
{
    PduDataUnit *unit = mDataPacket->data->env->hum;
    for(int i=0; i<4; ++i)
        setOneRow(i, unit, 4+i, COM_RATE_HUM, "%");
}

/**
 * @brief 显示门禁、烟雾的报警状态
 * @param value 状态值
 * @param item
 */
void UI_EnvTable::setOtherColor(int value, QTableWidgetItem *item)
{
    if(value == 2) // 报警
    {
        item->setIcon(mAlarmIcon);
        item->setTextColor(QColor(Qt::red));
    }
    else
    {
        if(value == 1)
            item->setIcon(mIcon);
        else
            item->setIcon(mCloseIcon);
        item->setTextColor(QColor(Qt::black));
    }
}


/**
 * @brief 门禁
 */
void UI_EnvTable::setDoor(void)
{
    PduDataBase *data = mDataPacket->data->env->door;

    for(int i=0; i<2; ++i)
    {
        QTableWidgetItem *item = mTableWidget->item(8+i, 1);
        int value = data->get(i);
        if(value == 1)
            item->setText(tr("关"));
        else if(value == 2)
            item->setText(tr("开"));
        else
            item->setText("---");

        setOtherColor(value, item);
    }
}



/**
 * @brief 水浸、烟雾数据
 * @param data 数据
 * @param row 所在行
 */
void UI_EnvTable::setDataOther(PduDataBase *data, int row)
{
    QTableWidgetItem *item = mTableWidget->item(row, 1);
    int value = data->get(0);
    if(value == 1)
        item->setText(tr("正常"));
    else if(value == 2)
        item->setText(tr("报警"));
    else
        item->setText("---");

    setOtherColor(value, item);
}

/**
 * @brief 设置水浸
 */
void UI_EnvTable::setWater(void)
{
    PduDataBase *data = mDataPacket->data->env->water;
    setDataOther(data, 10);
}

/**
 * @brief 设置烟雾
 */
void UI_EnvTable::setSmoke(void)
{
    PduDataBase *data = mDataPacket->data->env->smoke;
    setDataOther(data, 11);
}


/**
 * @brief 清空表格
 */
void UI_EnvTable::clearTable(void)
{
    for(int i=0; i<mItemList.size(); ++i)
        mItemList.at(i)->setText("---");

    int row = mTableWidget->rowCount();
    for(int i=0; i<row; ++i)
        mTableWidget->item(i, 1)->setIcon(mCloseIcon);
}

/**
 * @brief 检查设备是否在线
 * @return
 */
bool UI_EnvTable::checkDev(void)
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
 * @brief 数据更新
 */
void UI_EnvTable::updateData(void)
{
    bool ret = checkDev();
    if(ret)
    {
        setTemp();
        setHum();
        setDoor();
        setWater();
        setSmoke();
    }
}

/**
 * @brief 更新
 * @param ip
 * @param num
 */
void UI_EnvTable::updateWidget(QString &ip, int num)
{
    mDataPacket = pdu_hd_getPacket(ip, num);
    updateData();
}


/**
 * @brief 定时器响应函数
 */
void UI_EnvTable::timeoutDone(void)
{
    updateData();
}

void UI_EnvTable::updateWidget(QTableWidgetItem *it)
{
    bool ret = usr_land_jur();  //管理权限
    if(ret==false)
        return ;

    int coulumn = mTableWidget->currentColumn();
    if((coulumn==3) || (coulumn==2))
    {
        int row = mTableWidget->currentRow();

        int mode = 0;
        if(row < 4)
            mode = SET_CMD_ENV_TEM;
        else if(row < 8)
        {
            mode = SET_CMD_ENV_HUM;
            row -= 4;
        }

        if(mDataPacket)
        {
            SetThresholdDlg dlg(this);
            dlg.setMode(mode, row, mDataPacket);
            int ret = dlg.exec();
            if(ret == QDialog::Accepted)
                updateData();
        }
    }
    else
        it->setFlags(it->flags() & (~Qt::ItemIsEditable)); // 单元格不能编辑
}
