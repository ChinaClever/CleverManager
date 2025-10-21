#ifndef TOPOLOGYWIDGET_H
#define TOPOLOGYWIDGET_H

#include <QWidget>
#include <QMenu>
#include <QModelIndex>
#include "dbroomlist.h"
#include "dbcabinetlist.h"
#include "dp_roompackets.h"
#include "tp_addroomdlg.h"
#include "cab_adddlg.h"

class QListWidgetItem;
class QTableWidgetItem;

namespace Ui {
class Tp_TopologyWid;
}

struct RoomItemExt:public RoomItem{
    RoomItemExt():editable(false),tempState(false){}
    RoomItemExt(const RoomItem &item):RoomItem(item),editable(false),tempState(false){}
    bool editable;
    bool tempState;
};
Q_DECLARE_METATYPE(RoomItemExt)

class Tp_TopologyWid : public QWidget
{
    Q_OBJECT

public:
    explicit Tp_TopologyWid(QWidget *parent = 0);
    ~Tp_TopologyWid();

protected:
    void initMenu();
    void initWidget();
    void initData();

signals:
    void selectSig(int id);

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

    void updateBackgroundColor();
    void updateAllBackgroundColor();
    void loginStateChange();
protected:
    QColor getColor(sCabPacket *packet);
    QColor getColor(const CabinetItem &item);
    void updateWarningRoom();
    void createPduPacket(QTableWidgetItem* item);
    inline RoomItemExt currentRoomItem();
private:
    Ui::Tp_TopologyWid *ui;

    QTimer *timer;
    Cab_AddDlg *mCabAdd;
    Dp_PduPackets *mPdus;
    Dp_CabPackets *mCabs;    

    QMenu* m_menuCabinetModify;
    QMenu* m_menuCabinetAdd;
    QMenu* m_menuRoom;
    QTableWidgetItem* m_preCabinetItem;
};

#endif // TOPOLOGYWIDGET_H
