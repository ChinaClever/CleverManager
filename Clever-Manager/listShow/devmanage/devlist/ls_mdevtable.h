#ifndef LS_MDEVTABLE_H
#define LS_MDEVTABLE_H

#include <QWidget>
#include <QGroupBox>
#include <QSplitter>
#include <QTableWidget>
#include <QGridLayout>
#include <QHeaderView>
#include "pdu/dev/devGroup/pdudglist.h"
#include "common.h"

class LS_MdevTable: public QWidget
{
      Q_OBJECT
public:
    LS_MdevTable(QWidget *parent = 0);
    ~LS_MdevTable();

    void updateWidget(void);
    void updateWidget(QString &);

    bool getGroup(QString &);
    bool getSelectDev(QString &,QString &);
    bool getIP(QString &);

protected:
    void initTableWidget(void);
    void showDev(QString &);
    bool selectIP(QString &,QString & );

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

    QString mIP, mName;
    QString mGroup;

public:
    PduDGList *mGroupList;
};

#endif // LS_MDEVTABLE_H
