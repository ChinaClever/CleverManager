#ifndef DS_DEVSTATE_H
#define DS_DEVSTATE_H

#include "ds_devdata.h"
#include "pdupackettg.h"

class DS_DevState : public QWidget
{
    Q_OBJECT
public:
    explicit DS_DevState(QWidget *parent = 0);
    ~DS_DevState();

    void updateWidget(QString &ip, int num);

protected:
    bool checkDev(void);
    void readTgData(void);
    void setData(double data, const QString &symbol);
    void updateData(void);

private:
    void initWidget(void);
    void addItemContent(int, int, QString &);
    void addRowContent(QStringList &);
    void initTable(const QString &str);
    void initTableWidget(void);
    void clearTable(void);

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
    PduPacketTG *mPduTG;
    sPduTGDataStr *mPduData;
};

#endif // DS_DEVSTATE_H
