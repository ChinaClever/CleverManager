#ifndef DB_SQLTABLEMODEL_H
#define DB_SQLTABLEMODEL_H

#include "db_sqlquery.h"
#include <QWidget>
//#include"sql/records/mqsqltablemodel.h"


class DB_SqlTableModel : public QWidget
{
    Q_OBJECT
public:
    explicit DB_SqlTableModel(QWidget *parent = 0);
    ~DB_SqlTableModel();

    void setHeaders(QStringList &);

    bool removeRow(int);
    bool refreshTable(const QString &);

    void ascendingOrder(int);
    void descendingOrder(int);

protected:

signals:

public slots:
    bool submitChanges(void);
    void revertChanges(void);
    void queryFilter(QString);

public:
    QSqlTableModel *model;

private:
    bool isRun;
};

#endif // DB_SQLTABLEMODEL_H
