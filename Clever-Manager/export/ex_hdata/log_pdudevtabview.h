#ifndef LOG_PDUDEVTABVIEW_H
#define LOG_PDUDEVTABVIEW_H

#include <QWidget>
#include "db_sqltablemodel.h"
#include "ex_common/log_common.h"
#include "database/basicsql.h"

namespace Ui {
class Log_PduDevTabView;
}

class Log_PduDevTabView : public QWidget
{
    Q_OBJECT

public:
    explicit Log_PduDevTabView(QWidget *parent = 0);
    ~Log_PduDevTabView();

    bool refreshInitTable(const QString &table="pduDevIndex");
    bool refreshTable(const QString &table = "pduDevIndex");
    void selected(const QString &cmd);
    void setSelectRow(int row);
    void getList(QList<QStringList> &list);

protected:
    void initWid();
    void initTable();

signals:
    void selectedSig(int);

public slots:
    void doubleSlot(QModelIndex);
    void clearTableSlot();
     void autoDelSlot();

private:
    Ui::Log_PduDevTabView *ui;
    DB_SqlTableModel *model;
    QTimer *timer;
    BasicSql *mDb;
};

#endif // LOG_PDUDEVTABVIEW_H
