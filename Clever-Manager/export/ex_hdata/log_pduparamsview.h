#ifndef LOG_PDUPARAMSVIEW_H
#define LOG_PDUPARAMSVIEW_H

#include <QWidget>
#include "log_pdudevtabview.h"
#include "database/basicsql.h"

namespace Ui {
class Log_PduParamsVIew;
}

class Log_PduParamsVIew : public QWidget
{
    Q_OBJECT

public:
    explicit Log_PduParamsVIew(QWidget *parent = 0);
    ~Log_PduParamsVIew();

    void getList(QList<QStringList> &list);
    bool refreshTable(const QString &table = "pduElecParams");
    bool refreshInitTable(const QString &table="pduElecParams");

protected:
    void initWid();
    void initTable();

signals:


public slots:
   void selectedSlot(int);
   void clearTableSlot();
   void autoDelSlot();

private:
    Ui::Log_PduParamsVIew *ui;
    DB_SqlTableModel *model;
    QTimer *timer;
    BasicSql *mDb;
};

#endif // LOG_PDUPARAMSVIEW_H
