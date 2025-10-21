#ifndef LOG_ELETABVIEW_H
#define LOG_ELETABVIEW_H

#include <QWidget>
#include "db_sqltablemodel.h"
#include "log_eletemptable.h"

namespace Ui {
class Log_EleTabView;
}

class Log_EleTabView : public QWidget
{
    Q_OBJECT    
public:
    explicit Log_EleTabView(QWidget *parent = 0);
    ~Log_EleTabView();

    bool refreshTable(const QString &table = LOG_TEMP_ELE);

protected:
    void initWid();
    void initTable();

public slots:
    void doubleSlot(QModelIndex);
    void clearTableSlot();

private:
    Ui::Log_EleTabView *ui;
    DB_SqlTableModel *model;
};

#endif // LOG_ELETABVIEW_H
