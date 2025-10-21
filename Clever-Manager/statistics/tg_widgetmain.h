#ifndef TG_WIDGETMAIN_H
#define TG_WIDGETMAIN_H

#include <QWidget>
#include "tg_roomwidget.h"
#include "tg_statisticswidget.h"

namespace Ui {
class TG_WidgetMain;
}

class TG_WidgetMain : public QWidget
{
    Q_OBJECT

public:
    explicit TG_WidgetMain(QWidget *parent = 0);
    ~TG_WidgetMain();

protected:
    void initLayout(void);

private:
    Ui::TG_WidgetMain *ui;

    TG_RoomWidget *mRoomWidget;
    TG_StatisticsWidget *mStatistics;
};

#endif // TG_WIDGETMAIN_H
