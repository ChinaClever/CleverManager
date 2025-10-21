#ifndef DIALOGADDCABINETDEVICE_H
#define DIALOGADDCABINETDEVICE_H

#include <QDialog>
#include "dbconsumerdevices.h"

namespace Ui {
class Cab_AddDeviceDlg;
}

class Cab_AddDeviceDlg : public QDialog
{
    Q_OBJECT

public:
    explicit Cab_AddDeviceDlg(QWidget *parent = 0);
    ~Cab_AddDeviceDlg();

    void init(ConsumerDeviceItem& item,int left,int right,int height);
    ConsumerDeviceItem getItem() {return m_item;}

protected:
    bool inputCheck();
    virtual void save();

private slots:
    void on_spinBox_pos_valueChanged(int arg1);
    void on_okBtn_clicked();

protected:
    Ui::Cab_AddDeviceDlg *ui;
    ConsumerDeviceItem m_item;
};

class Cab_ModifyDeviceDlg : public Cab_AddDeviceDlg
{
    Q_OBJECT
public:
    explicit Cab_ModifyDeviceDlg(QWidget *parent = 0);

protected:
    void save();
};

#endif // DIALOGADDCABINETDEVICE_H
