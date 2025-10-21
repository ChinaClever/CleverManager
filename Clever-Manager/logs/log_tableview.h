#ifndef LOG_TABLEVIEW_H
#define LOG_TABLEVIEW_H

#include "common/msgBox/msgbox.h"
#include "db_sqltablemodel.h"
#include <QTableView>
#include <QHeaderView>
#include <QGridLayout>
#include "database/basicsql.h"


class LOG_TableView : public QWidget
{
    Q_OBJECT
public:
    explicit LOG_TableView(QWidget *parent = 0);
    ~LOG_TableView();

    void initAlarmTable(void);
    void addAlarmQueryBtn(void);

    void initOffLineTable(void);
    void addOffLineQueryBtn(void);

    void initRecordTable(const QString &table=LOG_ReCORD_TABLE);
    void addRecordQueryBtn(void);

    void initLandTable(void);
    void addLandQueryBtn(void);

    void initRoomTable(void);
    void addRoomQueryBtn(void);

    void initCabTable(void);
    void addCabQueryBtn(void);

    QString getTableName();
    void getHeadList(QStringList &);
protected:
    bool refreshTable(QString &);

signals:
    void querySig(QString);

public slots:
    void doubleSlot(QModelIndex);
    void refreshSlot(void);
    void delBtnSlot(void);
    void selectedSlot(QString&);
    void clearTableSlot();
    void autoDelSlot();

private:    
    QTableView *view;
    QGridLayout *mLayout;
    DB_SqlTableModel *model;

    QString m_table;
    QStringList mHeadList;
    QTimer *timer;
     BasicSql *mDb;
};

#endif // LOG_TABLEVIEW_H
