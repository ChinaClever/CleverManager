#ifndef DT_DETAILTABLE_H
#define DT_DETAILTABLE_H

#include <QWidget>
#include <QTableWidget>
#include <QTreeWidget>
#include <QGridLayout>
#include "dt_detaildev.h"

class DT_DetailTable : public QWidget
{
    Q_OBJECT
public:
    explicit DT_DetailTable(QWidget *parent = 0);
    ~DT_DetailTable();

    void updateWidget(QString &);
    void updateWidget(int roomID, QStringList &ipList, QList<int> &numList);

protected:
    void updateDev(QString &ip);
    void devTotalVC(QString &ip,short num);
    bool selectDev(short &num);

private:
    void initWidget(void);
    void setHeight(void);

    void clearDev(void);

signals:
    void selectSig(QString ip, short num);

public slots:
    void itemDoubleSlot(QTableWidgetItem *);
    void closeSlot(void);
    void remove_detailDev();

private:
    QTableWidget *mTableWidget;
    QList<DT_DetailDev *> mDevList;

    QStringList list;
    QString m_ip;

    int mRoomID;
    QStringList mIpList;
    QList<int> mNumList;
};

#endif // DT_DETAILTABLE_H
