#ifndef LS_TREEWIDGET_H
#define LS_TREEWIDGET_H

#include <QObject>
#include <QtGui>
#include <QTreeWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include "pdu/dev/devGroup/pdudglist.h"
#include "pdudevlist.h"


class LS_treeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LS_treeWidget(QWidget *parent = 0);
    ~LS_treeWidget();

    void setHeaderLabels(QStringList &);

   bool updateListDev(bool);
   bool updateline(bool);
   bool updateAlarm(bool);
   bool updateOffLine(bool);

   bool isContain(QString &ip);

protected:
    void initWidget(void);
    bool updateWidget(bool);
    void initLayout(void);
    QString getDevStateIcon(PduDataPacket *data);

protected:
    void devTypeWidget(void);
    void devGroup(int type);
    int devGroupWidget(int type, QTreeWidgetItem *item);
    int devListWidget(QString &group, QTreeWidgetItem *item);
    int createDevTable(QTreeWidgetItem *item, QString &ip);

    void getGroup(int);
    short getDevNum(QString &str);

    void getDevListMode(int type, PduDevList &pduDevList, QStringList &devList);
    int getDevList(int type, QTreeWidgetItem *item);

signals:
    void selectedSig(QString &, int);

public slots:
    void clickedSLot(QModelIndex index);
    bool selectedItemSlot(QTreeWidgetItem*,int);
    void seekDev(void);

public:
    QTreeWidget *mTreeWidget;

private:
    PduDGList *mDevList;
    QString mSelStr;
    int mMode;

    QLabel *lab;
    QPushButton *btn;
    QLineEdit *edit;
};

#endif // LS_TREEWIDGET_H
