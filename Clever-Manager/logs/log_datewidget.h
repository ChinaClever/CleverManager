#ifndef LOG_DATEWIDGET_H
#define LOG_DATEWIDGET_H

#include <QWidget>
#include "log_datehash.h"
#include <QTreeWidgetItem>

namespace Ui {
class LOG_DateWidget;
}

class LOG_DateWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LOG_DateWidget(QWidget *parent = 0);
    ~LOG_DateWidget();

    void setTable(const QString &name);
    void updateWidget();

    void setHeadList(QStringList &list){mHeadList = list;}
    void setTitle(QString &str){mTableTile = str;}

    void seekDate(const QString &date);
signals:
    void selectedSig(QString&);

protected:
    void initWidget(void);
    void setHeaderLabels(QStringList &headers);

    void yearShow(void);
    int monthShow(QString &year, QTreeWidgetItem *item);
    int dayShow(QString &month, QTreeWidgetItem *item);
    bool isDate(const QString &date);

public slots:
    void querySlot(QString str);

protected slots:
    bool selectedItemSlot(QTreeWidgetItem *item,int column);
    void clickedSLot(QModelIndex index);

private slots:
    void on_updateBtn_clicked();

    void on_exportBtn_clicked();

private:
    Ui::LOG_DateWidget *ui;
    QString mTableName, mDate, mQueryCmd;
    LOG_DateHash *mDateHash;
    QStringList mHeadList;
    QString mTableTile;
};

#endif // LOG_DATEWIDGET_H
