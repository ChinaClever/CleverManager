#ifndef UI_OUTPUTTABLE_H
#define UI_OUTPUTTABLE_H

#include <QWidget>
#include <QTableWidget>
#include <QGridLayout>
#include <QHeaderView>
#include <QtCore>
#include "pdu/data/hash/read/pduhashread.h"

class UI_OutputTable : public QWidget
{
    Q_OBJECT
public:
    explicit UI_OutputTable(QWidget *parent = 0);

    void updateWidget(QString &ip, int num);

protected:
    void initIcon(void);
    bool checkDev(void);
    bool checkTable(void);

    void setIcon(int, int);
    void setOutputName(int, int);
    void setSwState(int, int);
    void setCur(int, int);
    void setPow(int, int);
    void setPf(int, int);

    void setCurMin(int, int);
    void setCurMax(int, int);

    void setCrMin(int, int);
    void setCrMax(int, int);
    void setCritical(int, int);
    void updateData(void);

    void setOutput(void);
    void switchControl(void);

private:
    void initWidget(void);
    void addItemContent(int, int, const QString &);
    void addRowContent(QStringList &);
    void clearTable(void);
    void initTableWidget(void);
    void initTable(void);
    void reconizeSerial(int);
    void cSerialSetting();
    void aSerialSetting();
    void bSerialSetting();
signals:

public slots:
    void initFunSlot();
    void timeoutDone(void);
    void updateWidget(QTableWidgetItem *);

private:
    QTableWidget *mTableWidget;
    QTimer *timer;
    QGridLayout *layout;
    PduDataPacket *mDataPacket;

    QIcon mIcon, mAlarmIcon;
    QIcon mOpenIcon, mCloseIcon;
};

#endif // UI_OUTPUTTABLE_H
