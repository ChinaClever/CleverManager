#ifndef LOG_TABLEWIDGET_H
#define LOG_TABLEWIDGET_H

#include <QWidget>
#include <QSplitter>
#include "log_datewidget.h"
#include "log_tableview.h"

class LOG_TableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LOG_TableWidget(QWidget *parent = 0);

signals:

protected:
    void initLayout(void);
    void initWidet(void);

public slots:
    void initFunSLot();

public:
    LOG_TableView *mTableView;
    LOG_DateWidget *mDateWidget;
private:
    QSplitter *mSplitter;
};

#endif // LOG_TABLEWIDGET_H
