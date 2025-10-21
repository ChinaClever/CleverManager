/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include <atomic>
#include "tp_topologywid.h"
#include "ui_tp_topologywid.h"
#include "tp_questdlg.h"

extern bool usr_land_jur(void);

Tp_TopologyWid::Tp_TopologyWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tp_TopologyWid)
{
    ui->setupUi(this);

    mCabAdd = new Cab_AddDlg(this);
    mCabs = Dp_CabPackets::bulid(this);
    mPdus = Dp_PduPackets::bulid(this);

    initWidget();
    initMenu();
    initData();

    timer = new QTimer(this);
    timer->start(5*1000);
    connect(timer, SIGNAL(timeout()),this, SLOT(updateBackgroundColor()));
    connect(ui->pushButton_state,SIGNAL(clicked()),SLOT(updateAllBackgroundColor()));
    connect(LandingUser::get(),SIGNAL(landSig()),SLOT(loginStateChange()));
}

Tp_TopologyWid::~Tp_TopologyWid()
{
    delete ui;
}


void Tp_TopologyWid::initWidget()
{
    ui->tableWidget_cabinetMap->setColumnCount(26);
    ui->tableWidget_cabinetMap->setRowCount(20);

    QStringList hor_header;
    for(int i=0; i<26; ++i) hor_header << QString('A'+i);
    ui->tableWidget_cabinetMap->setHorizontalHeaderLabels(hor_header);

    //初始化机柜table行列间距相等
    ui->tableWidget_cabinetMap->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_cabinetMap->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_cabinetMap->setMouseTracking(true);
    ui->tableWidget_cabinetMap->horizontalHeader()->setVisible(true); //坐标码显示
    ui->tableWidget_cabinetMap->verticalHeader()->setVisible(true);

    ui->label->setFont(QFont("微软雅黑",12));
    ui->label_2->setFont(QFont("微软雅黑",12));
    ui->label_4->setFont(QFont("微软雅黑",12));
    ui->splitter->setStyleSheet("QSplitter::handle { background-color: rgb(12,62,180);}");
    ui->listWidget_roomList->setStyleSheet("QListWidget{border:0px solid gray; color:black; }"     //设置边框
                                           "QListWidget::item:selected{background:rgb(12,62,180); color:white; }"  //选中后的背景色
                                           "QListWidget{font:12pt;}" );
}


/**
 * @brief 菜单初始化
 */
void Tp_TopologyWid::initMenu()
{
    //初始化机柜、机房菜单列表。
    m_menuCabinetModify = new QMenu(this); //右键菜单
    m_menuCabinetModify->addAction(ui->action_moveCabinet);
    m_menuCabinetModify->addAction(ui->action_lockCabinet);
    m_menuCabinetModify->addAction(ui->action_modifyCabinet);
    m_menuCabinetModify->addAction(ui->action_deleteCabinet);

    m_menuCabinetAdd = new QMenu(this);  //添加机柜
    m_menuCabinetAdd->addAction(ui->action_addCabinet);
    ui->toolButton_addCabinet->setDefaultAction(ui->action_addCabinet);

    m_menuRoom = new QMenu(this); //机房删除-重命名
    m_menuRoom->addAction(ui->action_deleteRoom);
    m_menuRoom->addAction(ui->action_renameRoom);
}


/**
 * @brief 创建机房对象
 * @param rItem
 * @return
 */
QListWidgetItem *Tp_TopologyWid::createRoomItem(const RoomItemExt &rItem)
{
    QIcon icon;
    icon.addFile(QStringLiteral(":/topology/topology/1.png"), QSize(), QIcon::Normal, QIcon::Off);
    icon.addFile(QStringLiteral(":/topology/topology/2.png"), QSize(), QIcon::Selected, QIcon::Off);
    icon.addFile(QStringLiteral(":/topology/topology/2.png"), QSize(), QIcon::Selected, QIcon::On);
    QListWidgetItem* item = new QListWidgetItem(icon,rItem.room,nullptr,rItem.id);
    item->setData(Qt::UserRole,QVariant::fromValue(rItem));
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled);
    item->setSizeHint(QSize(150,35));
    ui->listWidget_roomList->addItem(item);
    return item;
}


QColor Tp_TopologyWid::getColor(sCabPacket *packet)
{
    QColor color(cm::gray);
    if(packet) {
        color = packet->color;
        if(ui->pushButton_state->isChecked()){ //若是为温度云分布图，则提取温度信息.
            color = packet->tmpColor;
        }
    }

    return color;
}

void Tp_TopologyWid::createPduPacket(QTableWidgetItem* item)
{
    if(item) {
        CabinetItem cItem = item->data(Qt::UserRole).value<CabinetItem>();
        sCabPacket *cab = mCabs->getByCab(cItem.id);
        if(cab) {
            item->setBackgroundColor(getColor(cab));
        }
    }
}


void Tp_TopologyWid::updateBackgroundColor()
{
    //更新机房内机柜信息
    QList<QTableWidgetItem*> items = ui->tableWidget_cabinetMap->findItems("",Qt::MatchContains);
    foreach(QTableWidgetItem* item,items){
        if(item == nullptr)continue;
        createPduPacket(item);
    }
    updateWarningRoom();
}


/**
 * @brief 加载数据库中的机房列表、及指定机柜地图的数据信息。
 */
void Tp_TopologyWid::initData()
{
    //1.加载数据
    QVector<RoomItem> items = DbRoomList::get()->allItems(); // 获取所有机房列表
    foreach(const RoomItem& item,items) {
        createRoomItem(RoomItemExt(item));
    }

    //2.若无机房，则机柜列表置为无效。
    if(!ui->listWidget_roomList->currentIndex().isValid())
        ui->stackedWidget_cabinet->setCurrentIndex(1);

    if(items.size()) ui->listWidget_roomList->setCurrentRow(0);
}




/**
 * @brief 增加机房
 */
void Tp_TopologyWid::on_toolButton_addRoom_clicked()
{
    Tp_AddRoomDlg dlg(ui->listWidget_roomList);
    if(QDialog::Accepted == dlg.exec())
    {
        RoomItemExt item;
        item.id = dlg.room(item.room);
        createRoomItem(item); //创建机房对象
        ui->listWidget_roomList->setCurrentRow(ui->listWidget_roomList->count()-1); //设置当前为最后一项
    }
}

/**
 * @brief 创建机柜Item
 * @param item
 * @return
 */
QTableWidgetItem *Tp_TopologyWid::createCabinetItem(const CabinetItem &item)
{
    QTableWidgetItem* wItem = new QTableWidgetItem(item.cab,item.id);
    wItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    wItem->setData(Qt::UserRole,QVariant::fromValue(item));
    ui->tableWidget_cabinetMap->setItem(item.row,item.column,wItem);
    return wItem;
}




void Tp_TopologyWid:: updateRoomShow(const RoomItemExt &rItem)
{
    bool b = usr_land_jur();
    ui->pushButton_editRoomMap->setVisible(b&&rItem.tempState==false);
    ui->label_editRoomMap->setVisible(b&&rItem.tempState==false);
    ui->toolButton_addRoom->setVisible(b);
    ui->stackedWidget_state->setCurrentIndex(rItem.tempState?0:1);
    ui->pushButton_editRoomMap->setChecked(rItem.editable);
    ui->toolButton_addCabinet->setVisible(ui->pushButton_editRoomMap->isChecked() && ui->pushButton_editRoomMap->isVisible());
    ui->pushButton_state->setChecked(rItem.tempState);
    if(ui->toolButton_addCabinet->isVisible())
        ui->pushButton_editRoomMap->setText(tr("关闭"));
    else
        ui->pushButton_editRoomMap->setText(tr("打开"));
}



/**
 * @brief 更新机房Item
 * @param wItem
 * @param item
 * @return
 */
bool Tp_TopologyWid::updateRoomItem(QListWidgetItem* wItem,const RoomItemExt &item)
{
    if(wItem == nullptr) {
        return false;
    }

    RoomItemExt oldItem = wItem->data(Qt::UserRole).value<RoomItemExt>();
    if(oldItem.id != item.id) {
        return false;
    }

    if(oldItem.unequal(item)){
        DbRoomList::get()->updateItem(item);
        wItem->setData(Qt::UserRole,QVariant::fromValue(item));
    } else if(oldItem.editable!=item.editable || oldItem.tempState!=item.tempState){
        wItem->setData(Qt::UserRole,QVariant::fromValue(item));
    } else {
        return false;
    }
    return true;
}


QColor Tp_TopologyWid::getColor(const CabinetItem &item)
{
    sCabPacket *cab = mCabs->getByCab(item.id);
    return getColor(cab);
}


void Tp_TopologyWid::updateAllBackgroundColor()
{
    QListWidgetItem* item = ui->listWidget_roomList->currentItem();
    if(item){
        RoomItemExt rItem =  item->data(Qt::UserRole).value<RoomItemExt>();
        rItem.tempState = ui->pushButton_state->isChecked();
        if(updateRoomItem(item,rItem))
            updateRoomShow(rItem);
    }

    QList<QTableWidgetItem*> items = ui->tableWidget_cabinetMap->findItems("",Qt::MatchContains);
    foreach(QTableWidgetItem* item,items){
        if(item == nullptr)continue;
        CabinetItem cItem = item->data(Qt::UserRole).value<CabinetItem>();
        item->setBackgroundColor(getColor(cItem));
    }
}



//更新机房背景色，分为告警、被选中、其他三种状态
void Tp_TopologyWid::updateWarningRoom()
{
    for(int row =0;row < ui->listWidget_roomList->count();++row){
        QListWidgetItem * item = ui->listWidget_roomList->item(row);
        RoomItemExt rItem = item->data(Qt::UserRole).value<RoomItemExt>();
        sRoomPacket *p = Dp_RoomPackets::bulid(this)->getByRoom(rItem.id);
        if(p) {
            if(p->alarm){
                item->setBackgroundColor(cm::orange);  //红色
                item->setTextColor(Qt::white);
            } else if(ui->listWidget_roomList->currentRow() == row){
                item->setBackgroundColor(QColor(102,198,249));
                item->setTextColor(Qt::white);
            } else {
                item->setBackgroundColor(Qt::transparent);
                item->setTextColor(Qt::black);
            }
        }
    }
}


/**
 * @brief 机房改变
 * @param current
 * @param previous
 */
void Tp_TopologyWid::on_listWidget_roomList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *)
{
    if(current){
        ui->stackedWidget_cabinet->setCurrentIndex(0);
        m_preCabinetItem = nullptr;
        RoomItemExt rItem = current->data(Qt::UserRole).value<RoomItemExt>();
        updateRoomShow(rItem);

        //从数据库更新机柜信息
        ui->tableWidget_cabinetMap->clearContents();
        QVector<CabinetItem> cItems = DbCabinetList::get()->selItemsByRoomId(rItem.id);
        foreach(const CabinetItem& cItem,cItems)
            createCabinetItem(cItem);
        updateAllBackgroundColor();
    } else {
        ui->stackedWidget_cabinet->setCurrentIndex(1);
    }
    updateWarningRoom();
}

/**
 * @brief 机房列表右键菜单
 * @param pos
 */
void Tp_TopologyWid::on_listWidget_roomList_customContextMenuRequested(const QPoint &pos)
{
    if(usr_land_jur()==false)return;
    QModelIndex index = ui->listWidget_roomList->indexAt(pos);
    if(index.isValid()){
        m_menuRoom->exec(ui->listWidget_roomList->mapToGlobal(pos));
    }
}

/**
 * @brief 重命名机房
 */
void Tp_TopologyWid::on_action_renameRoom_triggered()
{
    Tp_ModifyRoomDlg dlg(ui->listWidget_roomList);
    int currentIndex = ui->listWidget_roomList->currentRow();
    RoomItemExt item = ui->listWidget_roomList->item(currentIndex)->data(Qt::UserRole).value<RoomItemExt>();

    dlg.init(item);
    if(QDialog::Accepted == dlg.exec())
    {
        item.id = dlg.room(item.room);
        updateRoomItem(ui->listWidget_roomList->item(currentIndex),item);
        ui->listWidget_roomList->item(currentIndex)->setData(Qt::UserRole,QVariant::fromValue(item));
        ui->listWidget_roomList->item(currentIndex)->setText(item.room);
    }
}



/**
 * @brief 删除机房
 */
void Tp_TopologyWid::on_action_deleteRoom_triggered()
{
    RoomItemExt rItem;
    Tp_RequestDlg dlg(this,tr("删除机房"),tr("确定删除选中的机房吗？"));
    if(QDialog::Accepted == dlg.exec())
    {
        QListWidgetItem* item = ui->listWidget_roomList->currentItem();
        rItem = item->data(Qt::UserRole).value<RoomItemExt>();
        dlg.delRoom(rItem.id, rItem.room);
        delete item;
    }
}



/**
 * @brief 当鼠标进入不同的机柜槽中
 * @param index
 */
void Tp_TopologyWid::on_tableWidget_cabinetMap_entered(const QModelIndex &index)
{
    if(m_preCabinetItem!=nullptr && index.isValid()){
        QTableWidgetItem* item = ui->tableWidget_cabinetMap->item(index.row(),index.column());
        if(item == nullptr){
            if(m_preCabinetItem->tableWidget() == ui->tableWidget_cabinetMap)
                ui->tableWidget_cabinetMap->takeItem(m_preCabinetItem->row(),m_preCabinetItem->column());
            ui->tableWidget_cabinetMap->setItem(index.row(),index.column(),m_preCabinetItem);
        }
    }
}

/**
 * @brief 机柜地图右键菜单
 * @param pos
 */
void Tp_TopologyWid::on_tableWidget_cabinetMap_customContextMenuRequested(const QPoint &pos)
{
    if(ui->pushButton_editRoomMap->isChecked()&&ui->pushButton_editRoomMap->isVisible())
    {
        QModelIndex index = ui->tableWidget_cabinetMap->indexAt(pos);
        if(index.isValid()){
            QTableWidgetItem* wItem = ui->tableWidget_cabinetMap->item(index.row(),index.column());
            if(wItem){
                m_menuCabinetModify->exec(ui->tableWidget_cabinetMap->mapToGlobal(pos));
            } else {
                m_menuCabinetAdd->exec(ui->tableWidget_cabinetMap->mapToGlobal(pos));
            }
        }
    }
}

/**
 * @brief 添加机柜
 */
void Tp_TopologyWid::on_action_addCabinet_triggered()
{
    CabinetItem *item = mCabAdd->getItem();
    RoomItemExt rItem = currentRoomItem();
    item->room_id = rItem.id;
    item->room = rItem.room;
    if(QDialog::Accepted == mCabAdd->exec())
    {
        //找到空位置。
        bool pass = false;
        for(item->row=0 ; item->row<ui->tableWidget_cabinetMap->rowCount();++item->row){
            for(item->column=0 ; item->column<ui->tableWidget_cabinetMap->columnCount();++item->column) {
                if(ui->tableWidget_cabinetMap->item(item->row,item->column)==nullptr) {
                    pass = true;
                    break;
                }
            }
            if(pass)break;
        }

        if(pass){
            mCabAdd->save();
            m_preCabinetItem = ui->tableWidget_cabinetMap->currentItem();
            m_preCabinetItem = createCabinetItem(*item);
            m_preCabinetItem->setBackgroundColor(getColor(*item));

        } else {
            QMessageBox::warning(this,tr("警告"),tr("当前机房已满！"));
        }
    }
}

/**
 * @brief 移动机柜
 */
void Tp_TopologyWid::on_action_moveCabinet_triggered()
{
    m_preCabinetItem = ui->tableWidget_cabinetMap->currentItem();
}




bool Tp_TopologyWid::updateCabinetItem(QTableWidgetItem* wItem,const CabinetItem &cItem)
{
    if(wItem == nullptr)return false;
    CabinetItem oldItem = wItem->data(Qt::UserRole).value<CabinetItem>();
    if(oldItem.id != cItem.id)return false;

    createPduPacket(wItem);
    if(oldItem.cab != cItem.cab)
        wItem->setText(cItem.cab);
    if(oldItem.unequal(cItem)){
        DbCabinetList::get()->updateItem(cItem);
        wItem->setData(Qt::UserRole,QVariant::fromValue(cItem));
        return true;
    }
    return false;
}


/**
 * @brief 锁定机柜
 */
void Tp_TopologyWid::on_action_lockCabinet_triggered()
{
    if(m_preCabinetItem)
    {
        CabinetItem item = m_preCabinetItem->data(Qt::UserRole).value<CabinetItem>();
        item.row = m_preCabinetItem->row();
        item.column = m_preCabinetItem->column();
        updateCabinetItem(m_preCabinetItem,item);
        m_preCabinetItem = nullptr;
    }
}



/**
 * @brief 修改机柜
 */
void Tp_TopologyWid::on_action_modifyCabinet_triggered()
{
    QTableWidgetItem* item = ui->tableWidget_cabinetMap->currentItem();
    if(item){
        CabinetItem cItem = item->data(Qt::UserRole).value<CabinetItem>();
        Cab_ModifyDlg dlg(ui->tableWidget_cabinetMap, cItem);
        if(QDialog::Accepted == dlg.exec()){
            cItem = *dlg.getItem();
            updateCabinetItem(item,cItem);
        }
    }
}

/**
 * @brief 删除机柜
 */
void Tp_TopologyWid::on_action_deleteCabinet_triggered()
{
    QTableWidgetItem* item = ui->tableWidget_cabinetMap->currentItem();
    if(item) {
        CabinetItem cItem = item->data(Qt::UserRole).value<CabinetItem>();
        mCabAdd->del(cItem);
        delete item;
    }
}

/**
 * @brief 确定是否编辑机房地图
 */
void Tp_TopologyWid::on_pushButton_editRoomMap_clicked(bool checked)
{
    if(checked){
        Tp_RequestDlg dlg(this,tr("编辑机房地图"),tr("是否启用机房地图编辑？"));
        if(QDialog::Accepted != dlg.exec()){
            ui->pushButton_editRoomMap->setChecked(false);
            return;
        }
    }

    QListWidgetItem* item = ui->listWidget_roomList->currentItem();
    if(item){
        RoomItemExt rItem =  item->data(Qt::UserRole).value<RoomItemExt>();
        rItem.editable = checked;
        if(updateRoomItem(item,rItem))
            updateRoomShow(rItem);
    }

    if(ui->toolButton_addCabinet->isVisible())
        ui->pushButton_editRoomMap->setText(tr("关闭"));
    else
        ui->pushButton_editRoomMap->setText(tr("打开"));
}

/**
 * @brief 双击进入机柜位置对话框
 * @param index
 */
void Tp_TopologyWid::on_tableWidget_cabinetMap_doubleClicked(const QModelIndex &index)
{
    if(index.isValid()){
        QTableWidgetItem* item = ui->tableWidget_cabinetMap->item(index.row(),index.column());
        if(item){
            CabinetItem cItem  = item->data(Qt::UserRole).value<CabinetItem>();
            sCabPacket *cab = mCabs->getByCab(cItem.id);
            if(cab) {
                emit selectSig(cab->cab_id);
            }
        }
    }
}


void Tp_TopologyWid::loginStateChange()
{
    QListWidgetItem* item = ui->listWidget_roomList->currentItem();
    if(item){
        RoomItemExt rItem =  item->data(Qt::UserRole).value<RoomItemExt>();
        updateRoomShow(rItem);
    }
}


RoomItemExt Tp_TopologyWid::currentRoomItem()
{
    RoomItemExt rItem;
    QListWidgetItem* item = ui->listWidget_roomList->currentItem();
    if(item) rItem = item->data(Qt::UserRole).value<RoomItemExt>();
    return rItem;
}


