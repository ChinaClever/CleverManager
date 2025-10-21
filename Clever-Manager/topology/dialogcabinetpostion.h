#ifndef DIALOGCABINETPOSTION_H
#define DIALOGCABINETPOSTION_H

#include <QDialog>
#include <QMenu>
#include "pdudatapacket.h"
#include "dbconsumerdevices.h"
#include "dbcabinetlist.h"


namespace Ui {
class DialogCabinetPostion;
}

class DialogCabinetPostion : public QDialog
{
    Q_OBJECT
    enum
    {
        DirectSpanRow = Qt::UserRole,
    };
public:
    explicit DialogCabinetPostion(const CabinetItem& item,QWidget *parent = 0);
    void setEditable(bool e);
    ~DialogCabinetPostion();

private slots:
    void on_tableWidget_cabinet_customContextMenuRequested(const QPoint &pos);
    void on_action_add_triggered();
    void on_action_delete_triggered();
    void on_action_modify_triggered();
    void tableAddressChange(int address);
    void updatePacket(PduDataPacket* packet);
    void buttonClicked(int index,bool opened);
    void buttonChannelManager_clicke();
protected:
    bool setPortSwitch(const QString& ip,int num,int index,bool opened);
    void paintEvent(QPaintEvent *e);
private:
    Ui::DialogCabinetPostion *ui;
    const CabinetItem m_cabinetitem;
    QMenu m_menuAdd;
    QMenu m_menuModify;
    ConsumerDeviceItem m_deviceItem;
};

#endif // DIALOGCABINETPOSTION_H
