#ifndef DT_DETAILTREE_H
#define DT_DETAILTREE_H

#include <QTreeWidget>
#include <QGridLayout>
#include "pdu/dev/devGroup/pdudglist.h"
#include "dt_detailtable.h"
#include "topologywidget.h"

class DT_DetailTree : public QWidget
{
    Q_OBJECT
public:
    explicit DT_DetailTree(QWidget *parent = 0);
    ~DT_DetailTree();

    void initData(int id, QString &name);
    void setHeaderLabels(QStringList &);
    bool updateWidget(bool expand);

    void seekDev(QString &);

protected:
    void initLayout(void);
     void initWidget(void);
     void createTable(QTreeWidgetItem *, QString &);

     void getCabList(int id);
     int getCabDevList(const CabinetItem &cItem, QTreeWidgetItem *item);
    void createCabTable(QTreeWidgetItem *item, QStringList &ipList, QList<int> &numList);

private:
     void devTypeWidget(void);
     int devGroupWidget(int type, QTreeWidgetItem *item);
     int devListWidget(QString &group, QTreeWidgetItem *item);
     int getDevList(int type, QTreeWidgetItem *item);
     bool getCabinetById(int id);

signals:
     void selectSig(QString,short);
     void clearSig();

public slots:
    void clickedSLot(QModelIndex);
    void itemChangedSlot(int id,int);

private:
    QTreeWidget *mTreeWidget;
    int mRoomID;
    PduDGList *mPduDg; // 设备分组链表类
};

#endif // DT_DETAILTREE_H
