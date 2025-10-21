#ifndef DT_MAINWIDGET_H
#define DT_MAINWIDGET_H

#include <QWidget>
#include <QSplitter>
#include "dt_stackedwidget.h"
#include "dt_rommlistwidget.h"

class DT_MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DT_MainWidget(QWidget *parent = 0);

protected:
    void initLayout(void);

signals:
    void selectSig(QString,short);

public slots:

private:
    QSplitter *mSplitter;

    DT_RommListWidget *mRommList;
    DT_StackedWidget *mStackedWidget;
};

#endif // DT_MAINWIDGET_H
