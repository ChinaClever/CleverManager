#ifndef CAB_CABINETWID_H
#define CAB_CABINETWID_H

#include <QMenu>
#include "dbconsumerdevices.h"
#include "dbcabinetlist.h"
#include "dp_cabpackets.h"

namespace Ui {
class Cab_CabinetWid;
}

class Cab_CabinetWid : public QWidget
{
    Q_OBJECT

public:
    explicit Cab_CabinetWid(QWidget *parent = 0);
    ~Cab_CabinetWid();

public slots:
    void selectSlot(int id);

private slots:
    void on_tableWidget_cabinet_customContextMenuRequested(const QPoint &pos);
    void on_action_add_triggered();
    void on_action_delete_triggered();
    void on_action_modify_triggered();
    void tableAddressChange(int address);
    void buttonClicked(int index,bool opened);

protected:
    bool setPortSwitch(const QString& ip, const QString &num, int index, bool opened);
    void init(const CabinetItem& item);
    void paintEvent(QPaintEvent *e);
    void setEditable(bool e);

private:
    Ui::Cab_CabinetWid *ui;
    CabinetItem m_cabinetitem;
    QMenu m_menuAdd;
    QMenu m_menuModify;
    ConsumerDeviceItem m_deviceItem;
    enum { DirectSpanRow = Qt::UserRole,};
};

#endif // CAB_CABINETWID_H
