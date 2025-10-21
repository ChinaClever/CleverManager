#ifndef DS_DEVDATA_H
#define DS_DEVDATA_H

#include "ds_threshold.h"

class DS_DevData : public QWidget
{
    Q_OBJECT
public:
    explicit DS_DevData(QWidget *parent = 0);
    ~DS_DevData();

    void updateWidget(QString &ip,int num);

protected:
    bool checkDev(void);
    void updateData(void);

    void setDataBase(int id, PduDataBase *base, double rate, const QString &symbol, bool alarm=false);
    bool checkAlarm(int id, PduDataUnit *unit);
    void setDataUnit(PduDataUnit *unit, double rate, const QString &symbol);
    void setBase(PduDataBase *base, double rate, const QString &symbol);

private:
    void initWidget(void);
    void addItemContent(int, int, QString &);
    void addRowContent(QStringList &);
    void clearTable(void);
    void initTableWidget(void);
    void initTable(const QString &);

signals:

public slots:
     void initFunSlot();
    void timeoutDone(void);

private:
    QTableWidget *mTableWidget;
    QTimer *timer;
    QGridLayout *layout;

    int mOffset;
    QList<QTableWidgetItem *> mItemList;
    PduDataPacket *mDataPacket;
};

#endif // DS_DEVDATA_H
