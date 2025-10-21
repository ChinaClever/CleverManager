#ifndef LS_MGROUPLIST_H
#define LS_MGROUPLIST_H

#include <QtCore>
#include <QGridLayout>
#include <QTableWidget>
#include <QGroupBox>
#include <QHeaderView>
#include "pdu/dev/devGroup/pdudglist.h"
#include "common.h"

class LS_MgroupList : public QWidget
{
    Q_OBJECT
public:
    explicit LS_MgroupList(QWidget *parent = 0);
    ~LS_MgroupList();

    void updateWidget(void);
    void updateWidget(int type);
    int selectGroup(QString &group);

protected:
    void initTableWidget(void);

private:
    void initWidget(void);
    void addItemContent(int row, int column, QString &content);
    void addRowContent(QStringList &list);

signals:
    void selectSig(QString &);

public slots:
    void selectSlot(QTableWidgetItem *);

private:
    QTableWidget *mTableWidget;
    QGridLayout *layout;

    int mDevType;
    QString mStr;

public:
    PduDGList *mGroupList;
};

#endif // LS_MGROUPLIST_H
