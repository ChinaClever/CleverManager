#include "dialogtempmanager.h"
#include "ui_dialogtempmanager.h"
#include "dbpdutypelist.h"
#include <QMessageBox>
#include "common.h"
#include <QDebug>
#include "configtool.h"
#include "dbzebrachannels.h"
#include "zebra/zebra_client.h"
#include "zebra/zebra_delegates.h"
#include "dialogaddfriend.h"
#include "traversalsnmp.h"
#include <atomic>
#include <QVBoxLayout>
#include <QProgressBar>
#include <QLabel>
#include <QFileDialog>
#include "dialogmibreader.h"
#include "dialogsearchchannel.h"
#include "common/signaldispatch.h"

extern std::atomic_bool zebra_acquire_flag;
extern std::atomic_int zebra_network_state;
extern zebra_delegates* im_dele;

DialogTempManager::DialogTempManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTempManager)
{
    ui->setupUi(this);

    this->resize(500, 400);

    QStringList get = ConfigTool::get()->communicationGet();
    setWindowTitle(tr("通信管理"));
    setWindowIcon(QIcon(":/images/logo.jpg"));

#ifdef ZEBRA_MODULE
    m_progressDlg = new QProgressDialog(this, Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    m_progressDlg->setWindowTitle(tr("添加通道"));
    m_progressDlg->setBaseSize(500, 300);
    m_progressDlg->setLabelText(tr("正在请求添加通道.."));
    m_progressDlg->setMinimum(0);
    m_progressDlg->setMaximum(0);
    m_progressDlg->setCancelButton(nullptr);
    m_progressDlg->close();
    connect(im_dele, SIGNAL(add_friend_replied(bool)), this, SLOT(add_friend_replied_slot(bool)));
    add_friend_expired_timer = new QTimer(this);
    add_friend_expired_timer->setSingleShot(true);
    add_friend_expired_timer->setInterval(ADD_FRIEND_EXPIRED);
    connect(add_friend_expired_timer, SIGNAL(timeout()), this, SLOT(add_friend_expired()));
    updateZebraFriendList();
    if(get.contains("zebra"))
    {
        ui->checkBox_zebraGet->setChecked(true);
    }
    connect(ui->checkBox_zebraGet,SIGNAL(clicked()),this,SLOT(updateDataGetMode()));
    zebra_network_timer = new QTimer(this);
    connect(zebra_network_timer, SIGNAL(timeout()), this, SLOT(display_zebra_network_state()));
    zebra_network_timer->setSingleShot(false);
    zebra_network_timer->setInterval(2000);
    zebra_network_timer->start();
    display_zebra_network_state();
    ui->lineEdit_zebraServerIP->setText(ConfigTool::get()->getZebraServerIP());

    ui->tableWidget_zebra->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_zebra->setStyleSheet("QTableWidget::item:selected { background-color: #C6E2FF }");

    connect(SignalDispatch::get(),
            SIGNAL(get_search_channel_key_sig(QString)),
            this,
            SLOT(on_search_zerba_channel(QString)));

    connect(SignalDispatch::get(),
            SIGNAL(update_zebra_security_list_sig(std::map<QString, zebra_security_data>)),
            this,
            SLOT(on_updata_zebra_security_list(std::map<QString, zebra_security_data>)));

    ui->tabWidget_addChannel->setCurrentIndex(0);
    ui->tabWidget_addChannel->setTabText(1, QStringLiteral("Zebra Security"));

    ui->checkBox_tcpGet->setGeometry(20, 30, 100, 25);
    ui->checkBox_zebraGet->setGeometry(20, 60, 100, 25);
    ui->checkBox_snmpGet->setGeometry(20, 90, 100, 25);
    ui->label->setGeometry(20, 150, 120, 25);
    ui->label_zebra_netstate->setGeometry(150, 150, 100, 25);
    ui->label_serverIP->setGeometry(20, 180, 100, 25);
    ui->lineEdit_zebraServerIP->setGeometry(150, 182, 120, 20);
    ui->pushButton_apply->setGeometry(280, 180, 50, 25);
    ui->checkbox_manual_set_ip->setGeometry(20, 210, 100, 25);

    ui->boxip_lineEdit->setGeometry(132, 20, 150, 25);
    ui->boxsearch_pushButton->setGeometry(292, 20, 50, 25);
    ui->del_all_offline_pushButton->setGeometry(350, 20, 80, 25);

    ui->online_tableWidget->setGeometry(32, 60, 200, 280);
    ui->online_tableWidget->setColumnCount(1);
    QStringList online_headers;
    QString on_str= QStringLiteral("在线") + QString::number(0);
    //QString on_str= QStringLiteral("Online") + QString::number(0);
    online_headers << on_str;
    ui->online_tableWidget->setHorizontalHeaderLabels(online_headers);
    ui->online_tableWidget->setColumnWidth(0, 200);
    ui->online_tableWidget->setRowCount(0);
    QHeaderView* online_headerView = ui->online_tableWidget->verticalHeader();
    online_headerView->setHidden(true);
    ui->online_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->online_tableWidget->setStyleSheet("QTableWidget::item:selected { background-color: #C6E2FF}");

    ui->offline_tableWidget->setGeometry(242, 60, 200, 280);
    ui->offline_tableWidget->setColumnCount(1);
    QStringList offline_headers;
    QString off_str = QStringLiteral("离线")+ QString::number(0);
    //QString off_str = QStringLiteral("Offline")+ QString::number(0);
    offline_headers << off_str;
    ui->offline_tableWidget->setHorizontalHeaderLabels(offline_headers);
    ui->offline_tableWidget->setColumnWidth(0, 200);
    ui->offline_tableWidget->setRowCount(0);
    QHeaderView* offline_headerView = ui->offline_tableWidget->verticalHeader();
    offline_headerView->setHidden(true);
    ui->offline_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->offline_tableWidget->setStyleSheet("QTableWidget::item:selected { background-color: #C6E2FF}");

    zebraSecurityTable::get_instance()->get_table_now();

#else
    ui->tabWidget_addChannel->removeTab(ui->tabWidget_addChannel->indexOf(ui->tab_zebraManager));
    ui->checkBox_zebraGet->hide();
    ui->label->hide();
    ui->label_zebra_netstate->hide();
    ui->label_serverIP->hide();
    ui->pushButton_apply->hide();
    ui->lineEdit_zebraServerIP->hide();
#endif

    updateMibList();

#ifdef SNMP_MODULE
    if(get.contains("snmp"))
    {
        ui->checkBox_snmpGet->setChecked(true);
    }
    connect(ui->checkBox_snmpGet,SIGNAL(clicked()),this,SLOT(updateDataGetMode()));
#else
    ui->checkBox_snmpGet->setVisible(false);
#endif

    /* default check TCP channel to get device info */
    if(get.isEmpty() || get.contains("tcp"))
    {
        ui->checkBox_tcpGet->setChecked(true);
        connect(ui->checkBox_tcpGet,SIGNAL(clicked()),this,SLOT(updateDataGetMode()));
    }

//    int status = ConfigTool::get()->getManaulStatus();
//    if (1 == status)
//    {
//        ui->checkbox_manual_set_ip->setChecked(true);
//    }
//    else
//    {
//        ui->checkbox_manual_set_ip->setChecked(false);
//    }
    ui->checkbox_manual_set_ip->hide();

    ui->checkBox_zebraGet->hide();///////////////////2018-11-20 pmd隐藏
    ui->checkBox_snmpGet->hide();
    ui->tabWidget_addChannel->removeTab(3); ///2019-03-11 lzy隐藏Mib管理 zebra snmp节点
}

DialogTempManager::~DialogTempManager()
{
    delete ui;
}

void DialogTempManager::on_pushButton_addMib_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,tr("打开"));
    DialogMibReader dlg(this);
    dlg.parseFile(path);
    if(QDialog::Accepted == dlg.exec()){
        updateMibList();
    } else {
        QMessageBox::warning(this,tr("提醒"),dlg.errMsg());
    }
}

void DialogTempManager::on_pushButton_deleteMib_clicked()
{
    int row = ui->tableWidget_snmp->currentRow();
    if(row >= 0){
        QTableWidgetItem* item = ui->tableWidget_snmp->item(row,1);
        QString oid = item->text();
        QVector<PduTypeItem> items = DbPduTypeList::get()->selectItemsByOid(oid);
        foreach(const PduTypeItem& item,items)
            DbPduTypeList::get()->removeItem(item);
        ui->tableWidget_snmp->removeRow(row);
    } else {
        QMessageBox::warning(this,tr("提醒"),tr("请选择要删除的mib文件。"));
    }
}

void DialogTempManager::on_pushButton_addChannel_clicked()
{
    DialogAddFriend dlgAddFriend;
    if(QDialog::Accepted == dlgAddFriend.exec())
    {
        AddFriendThr *add_friend_thr = new AddFriendThr(dlgAddFriend.get_invite_code(), dlgAddFriend.get_ipaddr());
        connect(add_friend_thr, SIGNAL(finished()), add_friend_thr, SLOT(deleteLater()));
        connect(add_friend_thr, SIGNAL(add_friend_result(bool)), this, SLOT(add_friend_replied_slot(bool)));
        add_friend_thr->start();
        add_friend_expired_timer->start();
        m_progressDlg->exec();
    }

}

void DialogTempManager::on_pushButton_deleteChannel_clicked()
{
    int row = ui->tableWidget_zebra->currentRow();
    if(row < 0)
    {
        QMessageBox::warning(this,tr("提醒"),tr("请选中要删除的通道"));
        return;
    }

    if(QMessageBox::Yes == QMessageBox::question(this, tr("提醒"), tr("是否要删除所选通道")))
    {
        int ret = zebra_client::get_instance()->delete_friend(ui->tableWidget_zebra->item(row, 0)->text());
        if(ret == -1)
        {
            QMessageBox::warning(this,tr("提醒"),tr("未找到要删除的通道"));
        }
        else
        {
            InfoMsgBox(this,tr("删除通道成功"));
        }
        updateZebraFriendList();
    }


}

void DialogTempManager::updateDataGetMode()
{
    QStringList strs;
    if(ui->checkBox_snmpGet->isChecked())
    {
        strs << "snmp";
        TraversalSnmpManager::get()->startTravel();
    }
    else
    {
        TraversalSnmpManager::get()->stopTravel();
    }

    if(ui->checkBox_zebraGet->isChecked())
    {
        strs << "zebra";
        zebra_acquire_flag = true;
    }
    else
    {
        zebra_acquire_flag = false;
    }

    if(ui->checkBox_tcpGet->isChecked())
    {
        strs << "tcp";
    }
    else
    {

    }

    ConfigTool::get()->setCommunicationGet(strs);
}

void DialogTempManager::display_zebra_network_state()
{
    if(zebra_network_state <= 0)
    {
        ui->label_zebra_netstate->setText(tr("离线"));
        //ui->label_zebra_netstate->setText(tr("Offline"));
        ui->label_zebra_netstate->setStyleSheet("QLabel{color:red;font:12pt 微软雅黑;}");
    }
    else if(zebra_network_state <= 3)
    {
        ui->label_zebra_netstate->setText(tr("正在加入网络..."));
       // ui->label_zebra_netstate->setText(tr("Adding network..."));
        zebraSecurityTable::get_instance()->get_table_now();
        ui->label_zebra_netstate->setStyleSheet("QLabel{color:black;font:12pt 微软雅黑;}");
    }
    else
    {
        ui->label_zebra_netstate->setText(tr("在线"));
        //ui->label_zebra_netstate->setText(tr("Online"));
        ui->label_zebra_netstate->setStyleSheet("QLabel{color:green;font:12pt 微软雅黑;}");
    }
}

void DialogTempManager::add_friend_replied_slot(bool accepted)
{
    if(m_progressDlg->isVisible())
    {
        add_friend_expired_timer->stop();
        if(accepted)
        {
            m_progressDlg->close();
            InfoMsgBox(this, tr("增加zebra通道成功!"));
        }
        else
        {
            m_progressDlg->close();
            InfoMsgBox(this,  tr("增加zebra通道失败!"));
        }
        updateZebraFriendList();
    }
    else
    {
        qDebug()<<"m_progressDlg has already closed";
    }

}

void DialogTempManager::add_friend_expired()
{
    qDebug()<<"add_friend_expired";
    if(m_progressDlg->isVisible())
    {
        m_progressDlg->close();
        InfoMsgBox(this, tr("增加zebra通道超时!"));
    }
}

void DialogTempManager::updateMibList()
{
    QVector<PduTypeItem> items = DbPduTypeList::get()->allItems();
    ui->tableWidget_snmp->setRowCount(items.count());
    int index = -1;
    foreach(const PduTypeItem& item,items){
        ui->tableWidget_snmp->setItem(++index,0,new QTableWidgetItem(item.des));
        ui->tableWidget_snmp->setItem(index,1,new QTableWidgetItem(item.oid));
    }
}

void DialogTempManager::updateZebraFriendList()
{
    ui->tableWidget_zebra->clearContents();//  更新时先清除之前的列表，再添加 避免出现空白 pmd 2018-12-12
    QVector<DbZebraChannelItem> items = DbZebraChannels::get()->allItems();
    ui->tableWidget_zebra->setRowCount(items.count());
    int index = -1;
    foreach(const DbZebraChannelItem& item,items)
        ui->tableWidget_zebra->setItem(++index,0,new QTableWidgetItem(item.channel_name));
}

void DialogTempManager::on_pushButton_apply_clicked()
{
    QString ipAddr = ui->lineEdit_zebraServerIP->text();
    if(ConfigTool::get()->getZebraServerIP() == ipAddr){
        QMessageBox::warning(this,tr("提醒"),tr("当前IP地址没有改变！"));
    } else if(cm::isValidIpAddress(ipAddr)){
        ConfigTool::get()->setZebraServerIP(ipAddr);
        InfoMsgBox(this,tr("Server IP已经更改，重启应用程序后有效！"));
    } else {
        QMessageBox::warning(this,tr("提醒"),tr("当前Server IP地址格式无效！"));
    }

    bool status = ui->checkbox_manual_set_ip->isChecked();
    if (true == status)
    {
        ConfigTool::get()->setManaulStatus(1);
    }
    else
    {
        ConfigTool::get()->setManaulStatus(0);
    }
}

void DialogTempManager::on_channel_search_pushButton_clicked()
{
    int32_t col_count = ui->tableWidget_zebra->columnCount();
    int32_t row_conut = ui->tableWidget_zebra->rowCount();

    if((0 != col_count) && (0 != row_conut))
    {
        dialogsearchchannel search_tmp;
        search_tmp.exec();
    }
    else
    {
        QMessageBox::warning(this,tr("提醒"),tr("zebra通道列表为空！"));
    }
}

void DialogTempManager::on_search_zerba_channel(QString channel_name)
{
    if(channel_name.isEmpty())
    {
        ui->tableWidget_zebra->clearSelection();
        return ;
    }

    int32_t col_count = ui->tableWidget_zebra->columnCount();
    int32_t row_conut = ui->tableWidget_zebra->rowCount();
    int32_t i = 0;
    int32_t j = 0;
    bool get_flag = false;

    for(i = 0; i < row_conut; i++)
    {
        for(j = 0; j < col_count; j++)
        {
            QTableWidgetItem* item_tmp = ui->tableWidget_zebra->item(i, j);
            QVariant value = item_tmp->data(Qt::DisplayRole);

            if(0 == value.compare(channel_name))
            {
                ui->tableWidget_zebra->clearSelection();
                ui->tableWidget_zebra->setCurrentCell(i, j, QItemSelectionModel::Select);
                get_flag = true;
                break;
            }
        }

        if(get_flag)
        {
            break;
        }
    }

    if(!get_flag)
    {
        ui->tableWidget_zebra->clearSelection();
        QString message = tr("不存在") + channel_name + tr("的zebra通道！");
        QMessageBox::warning(this, tr("提醒"), message);
    }

    return ;
}

void DialogTempManager::on_boxsearch_pushButton_clicked()
{
    QString device_ip = ui->boxip_lineEdit->text();
    ui->boxip_lineEdit->clear();
    if(device_ip.isEmpty())
    {
        ui->online_tableWidget->clearSelection();
        ui->offline_tableWidget->clearSelection();

        return ;
    }

    int32_t online_col_count = ui->online_tableWidget->columnCount();
    int32_t online_row_conut = ui->online_tableWidget->rowCount();
    int32_t i = 0;
    int32_t j = 0;
    bool get_flag = false;

    for(i = 0; i < online_row_conut; i++)
    {
        for(j = 0; j < online_col_count; j++)
        {
            QTableWidgetItem* item_tmp = ui->online_tableWidget->item(i, j);
            if(nullptr != item_tmp)
            {
                QVariant value = item_tmp->data(Qt::DisplayRole);

                if(0 == value.compare(device_ip))
                {
                    ui->online_tableWidget->clearSelection();
                    ui->online_tableWidget->setCurrentCell(i, j, QItemSelectionModel::Select);
                    get_flag = true;
                    break;
                }
            }
        }

        if(get_flag)
        {
            break;
        }
    }

    int32_t offline_col_count = ui->offline_tableWidget->columnCount();
    int32_t offline_row_conut = ui->offline_tableWidget->rowCount();
    if(!get_flag)
    {
        for(i = 0; i < offline_row_conut; i++)
        {
            for(j = 0; j < offline_col_count; j++)
            {
                QTableWidgetItem* item_tmp = ui->offline_tableWidget->item(i, j);
                if(nullptr != item_tmp)
                {
                    QVariant value = item_tmp->data(Qt::DisplayRole);

                    if(0 == value.compare(device_ip))
                    {
                        ui->offline_tableWidget->clearSelection();
                        ui->offline_tableWidget->setCurrentCell(i, j, QItemSelectionModel::Select);
                        get_flag = true;
                        break;
                    }
                }
            }

            if(get_flag)
            {
                break;
            }
        }
    }

    if(!get_flag)
    {
        ui->online_tableWidget->clearSelection();
        ui->offline_tableWidget->clearSelection();
        QString message = tr("不存在") + device_ip + tr("的zebra security！");
        QMessageBox::warning(this, tr("提醒"), message);
    }

    return ;
}

void DialogTempManager::on_updata_zebra_security_list(std::map<QString, zebra_security_data> map_data)
{
    int online_num = 0;
    int offline_num = 0;

    ui->online_tableWidget->clear();
    ui->offline_tableWidget->clear();
    ui->online_tableWidget->setRowCount(map_data.size());//设置行数
    ui->offline_tableWidget->setRowCount(map_data.size());//设置行数

    std::map<QString, zebra_security_data>::iterator it;
    for(it = map_data.begin(); it != map_data.end(); it++)
    {
        if(it->second.beat_beriod > 0)
        {
            ui->online_tableWidget->setItem(online_num, 0, new QTableWidgetItem(it->second.dev_ip));
            ui->online_tableWidget->item(online_num, 0)->setForeground(QBrush(QColor(0x76, 0xee, 0x00)));
            online_num++;
        }
        else
        {
            ui->offline_tableWidget->setItem(offline_num, 0, new QTableWidgetItem(it->second.dev_ip));
            ui->offline_tableWidget->item(offline_num, 0)->setForeground(QBrush(QColor(255, 0, 0)));
            offline_num++;
        }
    }

    QStringList online_headers;
    QString on_str= tr("在线") + QString::number(online_num);
    //QString on_str= tr("Online") + QString::number(online_num);
    online_headers << on_str;
    ui->online_tableWidget->setHorizontalHeaderLabels(online_headers);

    QStringList offline_headers;
    QString off_str = tr("离线")+ QString::number(offline_num);
    //QString off_str = tr("Offline")+ QString::number(offline_num);
    offline_headers << off_str;
    ui->offline_tableWidget->setHorizontalHeaderLabels(offline_headers);

    return ;
}

void DialogTempManager::on_del_all_offline_pushButton_clicked()
{
    zebraSecurityTable::get_instance()->delete_all_offline_zebra_security();
}
