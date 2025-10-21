#ifndef TOPOLOGYWIDGET_H
#define TOPOLOGYWIDGET_H

#include <QWidget>
#include <QMenu>
#include <QModelIndex>
#include "dbroomlist.h"
#include "dbcabinetlist.h"
#include "traversalsnmp.h"
#include "snmptrap.h"
#include "db_sqlquery.h"
#include"register/activeprocess.h"
#include "pdu\dev\devType\pdudtname.h"

class QListWidgetItem;
class QTableWidgetItem;

namespace Ui {
class TopologyWidget;
}

struct RoomItemExt:public RoomItem{
    RoomItemExt():editable(false),tempState(false){}
    RoomItemExt(const RoomItem &item):RoomItem(item),editable(false),tempState(false){}
    bool editable;
    bool tempState;
};
Q_DECLARE_METATYPE(RoomItemExt)

class TopologyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TopologyWidget(QWidget *parent = 0);
    ~TopologyWidget();

protected:
    void initMenu(void);
    void initWidget(void);
    void initData(void);

protected:
    //房间操作
    QListWidgetItem* createRoomItem(const RoomItemExt &item);//创建root项
    bool updateRoomItem(QListWidgetItem* wItem,const RoomItemExt &item);
    //操作机柜
    QTableWidgetItem* createCabinetItem(const CabinetItem &item);
    bool updateCabinetItem(QTableWidgetItem* wItem,const CabinetItem &cItem);
    void updateRoomShow(const RoomItemExt &rItem);

private slots:
    void on_toolButton_addRoom_clicked();
    void on_pushButton_editRoomMap_clicked(bool checked);
    void on_listWidget_roomList_customContextMenuRequested(const QPoint &pos);
    void on_listWidget_roomList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    //void on_listWidget_roomList_itemChanged(QListWidgetItem *item);

    void on_tableWidget_cabinetMap_doubleClicked(const QModelIndex &index);
    void on_tableWidget_cabinetMap_customContextMenuRequested(const QPoint &pos);
    void on_tableWidget_cabinetMap_entered(const QModelIndex &index);
private slots:
    void on_action_deleteRoom_triggered();
    void on_action_renameRoom_triggered();

    void on_action_addCabinet_triggered();
    void on_action_moveCabinet_triggered();
    void on_action_lockCabinet_triggered();
    void on_action_modifyCabinet_triggered();
    void on_action_deleteCabinet_triggered();
    void on_pushButton_clicked();

    void trapInfo(int errType,const QString& ipAddr,const QString& oid,const QString& info);
    void updateBackgroundColor(PduDataPacket* packet);
    void updateAllBackgroundColor();
    void loginStateChange();
//    void on_listWidget_roomList_itemSelectionChanged();

//    void on_pushButton_state_clicked();

//    void on_pushButton_editRoomMap_clicked();

//    void on_pushButton_state_clicked();

protected:
    QColor getColor(PduDataPacket* packet,PduDataPacket* packet1 = nullptr);
    QColor getColor(const CabinetItem &item);
    void updateWarningList(PduDataPacket* packet);
    void updateWarningRoom();
    void createPduPacket(const CabinetItem& item);
    inline RoomItemExt currentRoomItem();
private:
    Ui::TopologyWidget *ui;

    QMenu* m_menuCabinetModify;
    QMenu* m_menuCabinetAdd;
    QMenu* m_menuRoom;
    QTableWidgetItem* m_preCabinetItem;
     s_RecordLog log;
     ActiveProcess *mActive;

    //告警处理
    QHash<int,QStringList> m_warningList; //key为room的id,value为ip-num字符串。
#ifdef SNMP_MODULE
    SnmpTrap *m_snmpTrap;
#endif
};

#endif // TOPOLOGYWIDGET_H
