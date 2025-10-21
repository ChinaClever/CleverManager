#ifndef WIDGETCABINETPOSTION_H
#define WIDGETCABINETPOSTION_H

#include <QTableWidget>
#include "dbconsumerdevices.h"

class Cab_TableWid : public QTableWidget
{
    Q_OBJECT
public:
    explicit Cab_TableWid(QWidget *parent = 0);
    void setItems(const QVector<ConsumerDeviceItem>& items);
    void removeItem(const ConsumerDeviceItem& item);
    bool updateItem(const ConsumerDeviceItem& item);
    bool addItem(const ConsumerDeviceItem& item);
    ConsumerDeviceItem  deviceItem(int address,bool onlyAddress = false);
    int  currentAddress();
    const QVector<ConsumerDeviceItem>& items()const;
    QRect itemRect(const ConsumerDeviceItem& item);
signals:
    void currentAddressChange(int row);
protected:
    void paintEvent(QPaintEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent* e);
    bool isEmptyPostion(const ConsumerDeviceItem& item);
    int updateCurrentAddress(int address);
private:
    QVector<ConsumerDeviceItem> m_items;
    QPoint m_clickedPos;
    int m_currentAddress;
};

#endif // WIDGETCABINETPOSTION_H
