#ifndef DT_DETAILDEV_H
#define DT_DETAILDEV_H

#include <QTableWidget>
#include "pdupackettg.h"
#include "common/icon/iconpath.h"

class DT_DetailDev: public QThread
{
    Q_OBJECT
public:
    explicit DT_DetailDev(QTableWidget *parent = 0);
    ~DT_DetailDev();

    void setDev(PduDataPacket *data);

protected:
    void run(void);

protected:
    void addRowContent(QStringList &list);
    void addItemContent(int row, int column, QString &content);

    void updateDev(void);
    void devTotalVC(QStringList &list, PduDataPacket *data);
    void refreshData(QStringList &list);

    void initIcon(void);
    void setIcon(int id);
signals:

public slots:
    void timeoutDone();
    void resetTableWidget();
private:
    QTableWidget *mTableWidget;
    PduPacketTG *mPcaketTg;

    QIcon mIcon, mAlarmIcon, mOffLineIcon;

    int mid;
    QTimer *timer;
    bool isFrist;
    bool isRun;

    sPduTGDataStr *mTg;
    PduDataPacket *mPacketData;
    QString mIP;
    QList<QTableWidgetItem *> mItemList;
};

#endif // DT_DETAILDEV_H
