#ifndef UI_ENVTABLE_H
#define UI_ENVTABLE_H

#include <QWidget>
#include <QTableWidget>
#include <QGridLayout>
#include <QHeaderView>
#include <QtCore>
#include <QGroupBox>
#include "pdu/data/hash/read/pduhashread.h"

class UI_EnvTable : public QWidget
{
    Q_OBJECT
public:
    explicit UI_EnvTable(QWidget *parent = 0);

    void updateWidget(QString &ip, int num);
protected:
    void initIcon(void);
    bool checkDev(void);
    bool checkTable(void);

    void setDataBase(int id, PduDataBase *data, int row, int column, double rate, const QString &str);
    void setAlarmColor(int id, PduDataBase *data, int row);
    void setOneRow(int id, PduDataUnit *unit, int row, double rate, const QString &str);
    void setTemp(void);
    void setHum(void);
    void setDoor(void);

    void setOtherColor(int value, QTableWidgetItem *);
    void setDataOther(PduDataBase *data, int row);
    void setWater(void);
    void setSmoke(void);

    void updateData(void);

private:
    void initWidget(void);
    void addItemContent(int, int, const QString &);
    void addRowContent(QStringList &);
    void clearTable(void);
    void initTableWidget(void);
    void initTable(const QString &str);

    void initLayout(void);

signals:

public slots:
    void initFunSlot();
    void timeoutDone();
    void updateWidget(QTableWidgetItem *);

private:
    QGroupBox *mGroupBox;
    QTableWidget *mTableWidget;
    QTimer *timer;
    PduDataPacket *mDataPacket;
    QList<QTableWidgetItem *> mItemList;
    QWidget *little_widget;
    QVBoxLayout *little_layout;

    QIcon mIcon, mAlarmIcon;
    QIcon mOpenIcon, mCloseIcon;
};

#endif // UI_ENVTABLE_H
