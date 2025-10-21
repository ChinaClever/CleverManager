#include <atomic>
#include "topologywidget.h"
#include "ui_topologywidget.h"
#include "dialogaddroom.h"
#include "dialoginforequest.h"
#include "dialogaddcabinet.h"
#include "dialogcabinetpostion.h"
#include "common.h"
#include "data/hash/read/pduhashread.h"
#include "signaldispatch.h"
#include "dialogtempmanager.h"
#include "configtool.h"
#include "common_snmp.h"
#include "db_sqlquery.h"
#include "sql/dc/storage/cloudcabinet.h"
#include "zebra/zebra_client.h"
#include "autoaddfriend/autoaddfriend.h"

extern std::atomic_bool zebra_acquire_flag;
extern bool usr_land_jur(void);

TopologyWidget::TopologyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TopologyWidget)
{
    ui->setupUi(this);
    //    ui->pushButton_editRoomMap->setText(tr("是"));

    ui->listWidget_roomList->setStyleSheet("QListWidget{border:0px solid gray; color:black; }"     //设置边框
                                           "QListWidget::item:selected{background:rgb(12,62,180); color:white; }"  //选中后的背景色
                                           "QListWidget{font:12pt;}"
                                           );

    ui->tableWidget_cabinetMap->setColumnCount(26);
    ui->tableWidget_cabinetMap->setRowCount(20);

//        ui->tableWidget_cabinetMap->setColumnCount(11);
//        ui->tableWidget_cabinetMap->setRowCount(8);//台州地铁

    QStringList hor_header,ver_header;
    hor_header<<tr("A")<<"B"<<"C"<<"D"<<"E"<<"F"<<"G"<<"H"<<"I"<<"J"<<"K"<<"L"<<"M"<<"N"<<"O"<<"P"<<"Q"<<"R"<<"S"<<"T"
             << "U" << "V" << "W" << "X" << "Y" << "Z";
//        hor_header<<tr("A")<<"B"<<"C"<<"D"<<"E"<<"F"<<"G"<<"H"<<"I"<<"J"<<"K";//台州地铁
    ui->tableWidget_cabinetMap->setHorizontalHeaderLabels(hor_header);
    ver_header<<""<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"10"<<"11"<<"12"<<"13"<<"14"<<"15"<<"16"<<"17"<<"18"<<"19"<<"20";
//    ver_header<<""<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8";//台州地铁
    ui->tableWidget_cabinetMap->setVerticalHeaderLabels(ver_header);
    ui->tableWidget_cabinetMap->horizontalHeader()->setVisible(true); //坐标码显示
    ui->tableWidget_cabinetMap->verticalHeader()->setVisible(true);

    ui->label->setFont(QFont("微软雅黑",12));
    //    ui->label->setFixedWidth(90);
    ui->label_2->setFont(QFont("微软雅黑",12));
    //     ui->label_2->setFixedWidth(90);
    //    ui->label_3->setFont(QFont("微软雅黑",12));
    //     ui->label_3->setFixedWidth(90);
    ui->label_4->setFont(QFont("微软雅黑",12));
    //     ui->label_4->setFixedWidth(90);

#if !defined(ZEBRA_MODULE) && !defined(SNMP_MODULE)
    ui->pushButton->setVisible(false);
#endif

#ifdef SNMP_MODULE
    m_snmpTrap = new SnmpTrap();
#endif

    initMenu();
    initWidget();
    initData();
}

TopologyWidget::~TopologyWidget()
{
#ifdef SNMP_MODULE
    delete m_snmpTrap;
    Snmp::socket_cleanup();
#endif
    delete ui;
}


/**
 * @brief 菜单初始化
 */
void TopologyWidget::initMenu(void)
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
 * @brief 窗口初始化
 */
void TopologyWidget::initWidget(void)
{
    ui->splitter->setStyleSheet("QSplitter::handle { background-color: rgb(12,62,180);}");
    //初始化机柜table行列间距相等
    ui->tableWidget_cabinetMap->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_cabinetMap->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_cabinetMap->setMouseTracking(true);
    connect(SignalDispatch::get(),SIGNAL(dispatchChange(PduDataPacket*)),SLOT(updateBackgroundColor(PduDataPacket*)));
#ifdef SNMP_MODULE
    connect(m_snmpTrap,SIGNAL(trapInfo(int,QString,QString,QString)),SLOT(trapInfo(int,QString,QString,QString)));
#endif
    connect(ui->pushButton_state,SIGNAL(clicked()),SLOT(updateAllBackgroundColor()));
    connect(SignalDispatch::get(),SIGNAL(loginStateChange()),SLOT(loginStateChange()));
}

/**
 * @brief 加载数据库中的机房列表、及指定机柜地图的数据信息。
 */
void TopologyWidget::initData(void)
{
    //1.加载数据
    QVector<RoomItem> items = DbRoomList::get()->allItems(); // 获取所有机房列表
    foreach(const RoomItem& item,items)
        createRoomItem(RoomItemExt(item));

    //2.若无机房，则机柜列表置为无效。
    if(!ui->listWidget_roomList->currentIndex().isValid())
        ui->stackedWidget_cabinet->setCurrentIndex(1);

    if(items.size())
        ui->listWidget_roomList->setCurrentRow(0);

    //初始化托盘图中所有的pdu设备到结构体列表。
    QVector<CabinetItem>  cItems = DbCabinetList::get()->allItems();
    foreach(const CabinetItem& item,cItems){
        createPduPacket(item);
    }

    QStringList get_communication = ConfigTool::get()->communicationGet();
#ifdef SNMP_MODULE
    Snmp::socket_startup();
    if(get_communication.contains("snmp"))
    {
        TraversalSnmpManager::get()->startTravel();
    }
    else
    {
        TraversalSnmpManager::get()->stopTravel();
    }

    m_snmpTrap->start();

#endif

#ifdef ZEBRA_MODULE
    if(get_communication.contains("zebra")){
        zebra_acquire_flag = true;
    } else {
        zebra_acquire_flag = false;
    }
#endif
}

/**
 * @brief 创建机房对象
 * @param rItem
 * @return
 */
QListWidgetItem *TopologyWidget::createRoomItem(const RoomItemExt &rItem)
{
    QIcon icon;
    icon.addFile(QStringLiteral(":/topology/topology/1.png"), QSize(), QIcon::Normal, QIcon::Off);
    icon.addFile(QStringLiteral(":/topology/topology/2.png"), QSize(), QIcon::Selected, QIcon::Off);
    icon.addFile(QStringLiteral(":/topology/topology/2.png"), QSize(), QIcon::Selected, QIcon::On);
    QListWidgetItem* item = new QListWidgetItem(icon,rItem.name,nullptr,rItem.id);
    item->setData(Qt::UserRole,QVariant::fromValue(rItem));
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled);
    item->setSizeHint(QSize(150,35));
    ui->listWidget_roomList->addItem(item);
    return item;
}

/**
 * @brief 增加机房
 */
void TopologyWidget::on_toolButton_addRoom_clicked()
{
    DialogAddRoom dlg(ui->listWidget_roomList);
    if(QDialog::Accepted == dlg.exec())
    {
        RoomItemExt item;
        item.name = dlg.roomName();
        try {
            item.id = DbRoomList::get()->maxId() + 1; //最大Id+1
            DbRoomList::get()->insertItem(item); //插入项目
            createRoomItem(item); //创建机房对象
            ui->listWidget_roomList->setCurrentRow(ui->listWidget_roomList->count()-1); //设置当前为最后一项
        }
        catch(const SqlExecption& exception){
            qDebug() << exception.msg() << exception.type();
        }

        /*增加日志信息*/

        log.title = tr("机房管理");
        log.operation = tr("添加机房");
        log.msg = tr("添加机房:%1").arg(dlg.roomName());


        sql_insert_record(log);
    }
}

/**
 * @brief 创建机柜Item
 * @param item
 * @return
 */
QTableWidgetItem *TopologyWidget::createCabinetItem(const CabinetItem &item)
{
    QTableWidgetItem* wItem = new QTableWidgetItem(item.name,item.id);
    wItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    wItem->setData(Qt::UserRole,QVariant::fromValue(item));
    ui->tableWidget_cabinetMap->setItem(item.row,item.column,wItem);
    return wItem;
}

/**
 * @brief 机房改变
 * @param current
 * @param previous
 */
void TopologyWidget::on_listWidget_roomList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *)
{
    if(current){
        ui->stackedWidget_cabinet->setCurrentIndex(0);
        m_preCabinetItem = nullptr;
        RoomItemExt rItem = current->data(Qt::UserRole).value<RoomItemExt>();
        updateRoomShow(rItem);
        //从数据库更新机柜信息
        ui->tableWidget_cabinetMap->clearContents();
        try{
            QVector<CabinetItem> cItems = DbCabinetList::get()->selItemsByRoomId(rItem.id);
            foreach(const CabinetItem& cItem,cItems)
                createCabinetItem(cItem);
            updateAllBackgroundColor();
        }catch(const SqlExecption& exception){
            qDebug() << exception.msg() << exception.type();
        }
    } else {
        ui->stackedWidget_cabinet->setCurrentIndex(1);
    }
    updateWarningRoom();
}

/**
 * @brief 机房列表右键菜单
 * @param pos
 */
void TopologyWidget::on_listWidget_roomList_customContextMenuRequested(const QPoint &pos)
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
void TopologyWidget::on_action_renameRoom_triggered()
{

    int currentIndex = ui->listWidget_roomList->currentRow();
    QString previousName = ui->listWidget_roomList->item(currentIndex)->text();


    DialogAddRoom dlg(ui->listWidget_roomList);
    if(QDialog::Accepted == dlg.exec())
    {
        RoomItemExt item = ui->listWidget_roomList->item(currentIndex)->data(Qt::UserRole).value<RoomItemExt>();
        item.name = dlg.roomName();

        updateRoomItem(ui->listWidget_roomList->item(currentIndex),item);
        ui->listWidget_roomList->item(currentIndex)->setData(Qt::UserRole,QVariant::fromValue(item));
        ui->listWidget_roomList->item(currentIndex)->setText(item.name);

    }
    //    if(QDialog::Accepted == dlg.exec())
    //        ui->listWidget_roomList->edit(ui->listWidget_roomList->currentIndex());
    //mnumber=0;
    //    log.title = tr("机房管理");
    //    log.operation = tr("机房重命名");
    //    log.msg = tr("机房%1重命名").arg(previousName);

    //    sql_insert_record(log);
}

/**
 * @brief 更新机房Item
 * @param wItem
 * @param item
 * @return
 */
bool TopologyWidget::updateRoomItem(QListWidgetItem* wItem,const RoomItemExt &item)
{
    if(wItem == nullptr)
    {
        return false;
    }
    RoomItemExt oldItem = wItem->data(Qt::UserRole).value<RoomItemExt>();
    if(oldItem.id != item.id)
    {
        return false;
    }
    if(oldItem.unequal(item)){
        DbRoomList::get()->updateItem(item);
        wItem->setData(Qt::UserRole,QVariant::fromValue(item));
        //qDebug()<<"updateRoomItem()"<<oldItem.name<<item.name<<wItem->data(Qt::UserRole).value<RoomItemExt>().name;
    } else if(oldItem.editable!=item.editable || oldItem.tempState!=item.tempState){
        wItem->setData(Qt::UserRole,QVariant::fromValue(item));
    } else {
        return false;
    }
    return true;
}



/**
 * @brief 删除机房
 */
void TopologyWidget::on_action_deleteRoom_triggered()
{
    RoomItemExt rItem;
    DialogInfoRequest dlg(this,tr("删除机房"),tr("确定删除选中的机房吗？"));
    if(QDialog::Accepted == dlg.exec())
    {
        QListWidgetItem* item = ui->listWidget_roomList->currentItem();
        rItem = item->data(Qt::UserRole).value<RoomItemExt>();
#if defined(CLOUD_MODULE)
        CloudRoom::bulid()->del(rItem.id); ///=== 只用来做试验 验证、测试用
#endif
        DbRoomList::get()->removeItem(rItem);
        delete item;

        /*增加日志信息*/
        log.title = tr("机房管理");
        log.operation = tr("删除机房");
        log.msg = tr("删除机房:%1").arg(rItem.name);

        sql_insert_record(log);
    }
}

/**
 * @brief 当鼠标进入不同的机柜槽中
 * @param index
 */
void TopologyWidget::on_tableWidget_cabinetMap_entered(const QModelIndex &index)
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
void TopologyWidget::on_tableWidget_cabinetMap_customContextMenuRequested(const QPoint &pos)
{
    if(ui->pushButton_editRoomMap->isChecked()&&ui->pushButton_editRoomMap->isVisible())
    {
        //        ui->pushButton_editRoomMap->setText(tr("否"));
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
void TopologyWidget::on_action_addCabinet_triggered()
{
    CabinetItem item;
    static int s_height = 42;
    item.height = s_height;
    DialogAddCabinet dlg(item,ui->tableWidget_cabinetMap);
    if(QDialog::Accepted == dlg.exec())
    {
        //找到空位置。
        bool pass = false;
        for(item.row=0 ; item.row<ui->tableWidget_cabinetMap->rowCount();++item.row){
            for(item.column=0 ; item.column<ui->tableWidget_cabinetMap->columnCount();++item.column) {
                if(ui->tableWidget_cabinetMap->item(item.row,item.column)==nullptr) {
                    pass = true;
                    break;
                }
            }
            if(pass)break;
        }
        if(pass){
            s_height = item.height;
            item.id = DbCabinetList::get()->maxId()+1;
            item.room_id = currentRoomItem().id;

            DbCabinetList::get()->insertItem(item);
            m_preCabinetItem = createCabinetItem(item);
            createPduPacket(item);
            m_preCabinetItem->setBackgroundColor(getColor(item));

            //#ifdef ZEBRA_MODULE
            //            //bool ret = false;
            //            if((!item.main_ip.isEmpty()) && (PDU_TYPE_ZPDU == item.main_type))
            //            {
            //                //ret = AutoAddFriend::get_instance()->check_exist(item.main_ip); //2019-1-10 避免重复加好友 pmd
            //                //if(ret)
            //                AutoAddFriend::get_instance()->addFriend(item.main_ip);
            //            }


            //            if((!item.spare_ip.isEmpty()) && (PDU_TYPE_ZPDU == item.spare_type))
            //            {
            //                //ret = AutoAddFriend::get_instance()->check_exist(item.spare_ip);
            //                //if(ret)
            //                AutoAddFriend::get_instance()->addFriend(item.spare_ip);
            //            }
            //#endif

            /*增加日志信息*/
            log.title = tr("机柜管理");
            log.operation = tr("添加机柜");
            log.msg = tr("添加机柜:%1").arg(item.name);

            sql_insert_record(log);
        } else {
            QMessageBox::warning(this,tr("警告"),tr("当前机房已满！"));
        }
    }


}

/**
 * @brief 移动机柜
 */
void TopologyWidget::on_action_moveCabinet_triggered()
{
    m_preCabinetItem = ui->tableWidget_cabinetMap->currentItem();
}

/**
 * @brief 锁定机柜
 */
void TopologyWidget::on_action_lockCabinet_triggered()
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


bool TopologyWidget::updateCabinetItem(QTableWidgetItem* wItem,const CabinetItem &cItem)
{
    if(wItem == nullptr)return false;
    CabinetItem oldItem = wItem->data(Qt::UserRole).value<CabinetItem>();
    if(oldItem.id != cItem.id)return false;
    createPduPacket(cItem);
    if(oldItem.name != cItem.name)
        wItem->setText(cItem.name);
    if(oldItem.unequal(cItem)){
        DbCabinetList::get()->updateItem(cItem);
        wItem->setData(Qt::UserRole,QVariant::fromValue(cItem));
        return true;
    }
    return false;
}

void TopologyWidget:: updateRoomShow(const RoomItemExt &rItem)
{
    bool b = usr_land_jur();
    ui->pushButton_editRoomMap->setVisible(b&&rItem.tempState==false);
    ui->label_editRoomMap->setVisible(b&&rItem.tempState==false);
    ui->toolButton_addRoom->setVisible(b);
#if defined(ZEBRA_MODULE) || defined(SNMP_MODULE)
    ui->pushButton->setVisible(b);
#else
    ui->pushButton->setVisible(false);
#endif
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
 * @brief 修改机柜
 */
void TopologyWidget::on_action_modifyCabinet_triggered()
{
    QTableWidgetItem* item = ui->tableWidget_cabinetMap->currentItem();
    if(item){
        CabinetItem cItem = item->data(Qt::UserRole).value<CabinetItem>();
        DialogModifyCabinet dlg(cItem,ui->tableWidget_cabinetMap);
        if(QDialog::Accepted == dlg.exec()){
            updateCabinetItem(item,cItem);
        }
    }
}

/**
 * @brief 删除机柜
 */
void TopologyWidget::on_action_deleteCabinet_triggered()
{
    CabinetItem cItem ;
    QTableWidgetItem* item = ui->tableWidget_cabinetMap->currentItem();
    if(item)
    {
        cItem = item->data(Qt::UserRole).value<CabinetItem>();

#if defined(ZEBRA_MODULE)
        if(!cItem.main_ip.isEmpty())
        {
            if(cItem.main_num == 0)//2019-1-10 只有主机才删除好友  pmd
            {
                int ret = zebra_client::get_instance()->delete_friend(cItem.main_ip);
                if(ret == -1)
                {
                    qDebug() << "删除机柜时未找到要删除的 " << cItem.main_ip<< " 好友通道";
                }
            }
        }
        if(!cItem.spare_ip.isEmpty())
        {
            if(cItem.spare_num == 0)//2019-1-10 只有主机才删除好友  pmd
            {
                int ret = zebra_client::get_instance()->delete_friend(cItem.spare_ip);
                if(ret == -1)
                {
                    qDebug() << "删除机柜时未找到要删除的 " << cItem.spare_ip<< " 好友通道";
                }
            }
        }
#endif

#if defined(CLOUD_MODULE)
        CloudCabinet::bulid()->del(cItem.id); ///=== 只用来做试验 验证、测试用
#endif
        DbCabinetList::get()->removeItem(cItem);
        delete item;

        /*增加日志信息*/
        log.title = tr("机柜管理");
        log.operation = tr("删除机柜");
        log.msg = tr("删除机柜:%1").arg(cItem.name);


        sql_insert_record(log);
    }


}

/**
 * @brief 确定是否编辑机房地图
 */
void TopologyWidget::on_pushButton_editRoomMap_clicked(bool checked)
{
    if(checked){
        //        ui->pushButton_editRoomMap->setText(tr("否"));
        DialogInfoRequest dlg(this,tr("编辑机房地图"),tr("是否启用机房地图编辑？"));
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
void TopologyWidget::on_tableWidget_cabinetMap_doubleClicked(const QModelIndex &index)
{
    if(index.isValid()){
        QTableWidgetItem* item = ui->tableWidget_cabinetMap->item(index.row(),index.column());
        if(item){
            DialogCabinetPostion dlg(item->data(Qt::UserRole).value<CabinetItem>(),this);
            dlg.setEditable(ui->pushButton_editRoomMap->isChecked() && ui->pushButton_editRoomMap->isVisible());
            dlg.exec();
        }
    }
}

/**
 * @brief TopologyWidget::on_pushButton_clicked
 * 添加Mib文件的临时处理
 */
void TopologyWidget::on_pushButton_clicked()
{
    DialogTempManager dlg;
    dlg.exec();
}

void TopologyWidget::trapInfo(int errType, const QString &ipAddr, const QString &oid, const QString &info)
{
    if(errType)
    {
        QMessageBox::warning(this,tr("Response contains error"),Snmp::error_msg(errType));
    }
    else
    {
        QString ip = ipAddr.split('/').first();
        OidItem item = MibConvert::oid2Item(oid);
        QString mibName = item.name;
        int dev_num = MibConvert::takeoffMSNum(mibName);

        PduDataPacket *packet = pdu_hd_getPacket(ip,dev_num);
        packet->state = 1;//设置为告警状态
        updateWarningList(packet);
        ////===
        //QMessageBox::warning(this,tr("Trap Warning"),QString("设备(%1:%2)的%3告警：%4").arg(ip).arg(dev_num).arg(mibName).arg(info));
    }
}

void TopologyWidget::updateBackgroundColor(PduDataPacket *packet)
{
    //更新机房内机柜信息
    QList<QTableWidgetItem*> items = ui->tableWidget_cabinetMap->findItems("",Qt::MatchContains);
    foreach(QTableWidgetItem* item,items){
        if(item == nullptr)continue;
        CabinetItem cItem = item->data(Qt::UserRole).value<CabinetItem>();
        PduDataPacket * packet1 = nullptr;
        PduDataPacket * packet2= nullptr;
        if(cItem.isValideMain()) {
            QString ip = cItem.main_ip;
            packet1 = ::pdu_hd_getPacket(ip,cItem.main_num);
        }
        if(cItem.isValideSpare()) {
            QString ip = cItem.spare_ip;
            packet2= ::pdu_hd_getPacket(ip,cItem.spare_num);
        }
        if(packet==packet1 || packet==packet2)
            item->setBackgroundColor(getColor(packet1,packet2));
    }
}

QColor TopologyWidget::getColor(PduDataPacket *packet,PduDataPacket* packet1)
{
    //若是为温度云分布图，则提取温度信息.
    if(ui->pushButton_state->isChecked()){
        int value = 0;
        int index1 = 0;
        int index2 = 0;
        if(packet){
            PduDataBase *temp = packet->data->env->tem->value;
            int size = temp->size();

            if(size){
                int value1 = 0;
                for(int i = 0;i < size;++i)
                {
                    if(temp->get(i) > 0)
                    {
                        value1 += temp->get(i);
                        index1++;
                    }
                }
                if(index1 > 0)
                    value = value1/index1;
            }
        }

        if(packet1){
            PduDataBase *temp = packet1->data->env->tem->value;
            int size = temp->size();
            if(size){
                int value1 = 0;
                for(int i = 0;i < size;++i)
                    if(temp->get(i) > 0)
                    {
                        value1 += temp->get(i);
                        index2++;
                    }
                if(index2 > 0)
                {
                    if(packet)
                        value = (value + value1 / index2) / 2;
                    else
                        value = value1/index2;
                }
            }
        }

        return cm::temp2Color(value/COM_RATE_TEM);
    } else {
        //为在线状态时
        int state = -1, state1 = -1;//-1表示没有Pdu设备,0表示正常, 1表示离线，2表示警告
        if(packet)
        {
            if(packet->offLine > 0)
                state = packet->state==0?0:2;
            else
                state = 1;
        }

        if(packet1)
        {
            if(packet1->offLine > 0)
                state1 = packet1->state==0?0:2;
            else
                state1 = 1;
        }

        if(state1 > state) state = state1;
        switch (state) {
        case -1:return cm::gray;
        case 0:return cm::green;
        case 1:return cm::blue;
        case 2:return cm::orange;
        case 3:return cm::yellow;
        }
    }
}




QColor TopologyWidget::getColor(const CabinetItem &item)
{
    PduDataPacket * packet = nullptr;
    PduDataPacket * packet1= nullptr;
    if(item.isValideMain()) {
        QString ip = item.main_ip;
        packet = ::pdu_hd_getPacket(ip,item.main_num);
    }
    if(item.isValideSpare()) {
        QString ip = item.spare_ip;
        packet1= ::pdu_hd_getPacket(ip,item.spare_num);
    }
    return getColor(packet,packet1);
}

void TopologyWidget::updateAllBackgroundColor()
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

void TopologyWidget::loginStateChange()
{
    QListWidgetItem* item = ui->listWidget_roomList->currentItem();
    if(item){
        RoomItemExt rItem =  item->data(Qt::UserRole).value<RoomItemExt>();
        updateRoomShow(rItem);
    }
}

void TopologyWidget::updateWarningList(PduDataPacket *packet)
{
    //更新机房列表告警
    bool bWarning = packet->offLine>0 && packet->state!=0;//当前告警状态
    QString dev = QString("%1:%2").arg(packet->ip->get()).arg(packet->devNum);
    QVector<int> room_ids = DbCabinetList::get()->selRoomIdsByAddress(packet->ip->get(),packet->devNum);
    foreach(int room_id,room_ids){
        QList<QString>& list = m_warningList[room_id];
        if(bWarning){
            //若为警告，则加入警告列表。
            if(!list.contains(dev))list.append(dev);
        } else {
            //若非警告，列表中有该数据，则移除。
            if(list.contains(dev))list.removeOne(dev);
        }
    }
    updateWarningRoom();
}

//更新机房背景色，分为告警、被选中、其他三种状态
void TopologyWidget::updateWarningRoom()
{
    for(int row =0;row < ui->listWidget_roomList->count();++row){
        QListWidgetItem * item = ui->listWidget_roomList->item(row);
        RoomItemExt rItem = item->data(Qt::UserRole).value<RoomItemExt>();
        if(!m_warningList[rItem.id].isEmpty()){
            item->setBackgroundColor(cm::orange);  //红色
            //             item->setBackgroundColor(Qt::red);
            item->setTextColor(Qt::white);
            //            InfoMsgBox(this,tr("报警信息"),tr("%1").arg(m_warningList.value()));
        } else if(ui->listWidget_roomList->currentRow() == row){
            item->setBackgroundColor(QColor(102,198,249));
            item->setTextColor(Qt::white);
        } else {
            item->setBackgroundColor(Qt::transparent);
            item->setTextColor(Qt::black);
        }
    }
}

void TopologyWidget::createPduPacket(const CabinetItem &item)
{
    if(item.isValideMain()){
        //        PduDataPacket *packet = ::pdu_hd_get(0x01010101,(QString&)item.main_ip,(int)item.main_num);
        PduDataPacket *packet = ::pdu_hd_getPacket((QString&)item.main_ip,(int)item.main_num);
        if(packet) {
            packet->ip->set(item.main_ip);
            packet->devNum = item.main_num;
            updateBackgroundColor(packet);
        }
    }

    if(item.isValideSpare()){
        //        PduDataPacket *packet = ::pdu_hd_get(0x01010101,(QString&)item.spare_ip,(int)item.spare_num);
        PduDataPacket *packet = ::pdu_hd_getPacket((QString&)item.spare_ip,(int)item.spare_num);
        if(packet) {
            packet->ip->set(item.spare_ip);
            packet->devNum = item.spare_num;
            updateBackgroundColor(packet);
        }
    }
}

RoomItemExt TopologyWidget::currentRoomItem()
{
    RoomItemExt rItem;
    QListWidgetItem* item = ui->listWidget_roomList->currentItem();
    if(item) rItem = item->data(Qt::UserRole).value<RoomItemExt>();
    return rItem;
}


