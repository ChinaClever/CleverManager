#ifndef DS_THRESHOLD_H
#define DS_THRESHOLD_H

#include <QTableWidget>
#include <QGridLayout>
#include <QHeaderView>
#include <QtCore>
#include "common.h"
#include "pdu/data/hash/read/pduhashread.h"

class DS_Threshold : public QWidget
{
    Q_OBJECT
public:
    explicit DS_Threshold(QWidget *parent = 0);
    ~DS_Threshold();

protected:
    void setDataBase(int id, PduDataBase *base, double rate, const QString &symbol);
    void setDataUnit(int id, PduDataUnit *unit, double rate, const QString &symbol);
    void checkAlarm(int id, PduDataUnit *unit);

    bool checkDev(void);
    void updateData(void);

private:
    void initWidget(void);
    void addItemContent(int row, int column, const QString &content);
    void addRowContent(QStringList &);
    void initTable(const QString &str);
    void initTableWidget(void);
    void initSpanTable(void);
    void clearTable(void);

signals:

public slots:
    void initFunSlot();
    void updateWidget(QString &, int);
    void updateWidget(QTableWidgetItem *);
    void timeoutDone();

private:
    QTableWidget *mTableWidget;
    QList<QTableWidgetItem *> mItemList;
    QTimer *timer;
    QGridLayout *layout;

    int mOffset, mAlarmOffset;
    PduDataPacket *mDataPacket;
};

#endif // DS_THRESHOLD_H
