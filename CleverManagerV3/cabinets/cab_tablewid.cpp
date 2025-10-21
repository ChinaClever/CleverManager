/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "cab_tablewid.h"
#include <QPainter>
#include <QPaintEvent>
#include <QHeaderView>
#include "common.h"

Cab_TableWid::Cab_TableWid(QWidget *parent) : QTableWidget(parent),m_currentAddress(-1)
{
    this->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->setMouseTracking(true);
}

void Cab_TableWid::setItems(const QVector<ConsumerDeviceItem> &items)
{
    m_items = items;
    QWidget::update();
}

void Cab_TableWid::removeItem(const ConsumerDeviceItem &item)
{
    for(int i=0 ; i < m_items.count() ; ++i){
        if(m_items[i].id == item.id){
            m_items.removeAt(i);
            break;
        }
    }
    QWidget::update();
}

bool Cab_TableWid::updateItem(const ConsumerDeviceItem &item)
{
    for(int i=0 ; i < m_items.count() ; ++i){
        if(m_items[i].id == item.id){
            ConsumerDeviceItem ori_item = m_items.takeAt(i);
            if(isEmptyPostion(item)){
                m_items.append(item);
                QWidget::update();
                return true;
            }
            m_items.append(ori_item);
            break;
        }
    }
    return false;
}

bool Cab_TableWid::addItem(const ConsumerDeviceItem &item)
{
    if(isEmptyPostion(item)){
        m_items.append(item);
        QWidget::update();
        return true;
    }return false;
}

ConsumerDeviceItem Cab_TableWid::deviceItem(int address,bool onlyAddress)
{
    foreach(const ConsumerDeviceItem& item,m_items){
        if(onlyAddress){
            if(address == item.address)
                return item;
        } else if(address >= item.address && address<=item.address+item.occupy_height-1)
            return item;
    }
    return ConsumerDeviceItem();
}

int Cab_TableWid::currentAddress()
{
    QPoint pos = this->viewport()->mapFromGlobal(QCursor::pos());
    return updateCurrentAddress(this->rowCount() - pos.y()*this->rowCount()/this->viewport()->height());
}

const QVector<ConsumerDeviceItem> &Cab_TableWid::items() const
{
    return m_items;
}

QRect Cab_TableWid::itemRect(const ConsumerDeviceItem& item)
{
    int row = this->rowCount() - item.address;
    QRect rect = this->visualItemRect(this->item(row,0));
    if(row%2==0)rect.setBottom((rect.top()+rect.bottom())/2);
    row = row-item.occupy_height+1;
    QRect rect1 = this->visualItemRect(this->item(row,0));
    if(row%2)rect.setTop((rect1.top()+rect1.bottom())/2);
    else rect.setTop(rect1.top());
    return rect;
}

void Cab_TableWid::paintEvent(QPaintEvent *e)
{
    QTableWidget::paintEvent(e);
    QPainter painter(this->viewport());
    painter.setPen(Qt::white);
    foreach(const ConsumerDeviceItem& item,m_items){
        QRect rect = itemRect(item);
        QColor color = cm_color(item.address);
        painter.fillRect(rect,color);
        painter.drawText(rect,Qt::AlignVCenter,QString(" %1-%2: %3").arg(item.address)
                         .arg(item.occupy_height).arg(item.name));
    }
}

void Cab_TableWid::mouseReleaseEvent(QMouseEvent *e)
{
    m_clickedPos = e->pos();
    QTableWidget::mouseReleaseEvent(e);
}

void Cab_TableWid::mouseMoveEvent(QMouseEvent *e)
{
    int address = this->rowCount() - e->pos().y()*this->rowCount()/this->viewport()->height();
    updateCurrentAddress(address);
    QTableWidget::mouseMoveEvent(e);
}

bool Cab_TableWid::isEmptyPostion(const ConsumerDeviceItem &item)
{
    int lower = item.address;
    int upper = lower + item.occupy_height - 1;
    foreach(const ConsumerDeviceItem& item,m_items){
        int lower1 = item.address;
        int upper1 = item.address + item.occupy_height - 1;
        if(cm::bInBound(lower,lower1,upper) || cm::bInBound(lower,upper1,upper))
            return false;
    }
    return true;
}

int Cab_TableWid::updateCurrentAddress(int address)
{
    if(address != m_currentAddress){
        m_currentAddress = address;
        emit currentAddressChange(address);
    }
    return m_currentAddress;
}
