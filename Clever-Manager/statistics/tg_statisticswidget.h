#ifndef TG_STATISTICSWIDGET_H
#define TG_STATISTICSWIDGET_H

#include <QWidget>
#include "tg_datashow.h"

typedef struct
{
    int id;
    TG_DataShow *widget;
}sStackWidget;

class TG_StatisticsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TG_StatisticsWidget(QWidget *parent = 0);

protected:
    void initWidget(void);
    void createWidget(int id);
    int findByID(int id);
    void delRoom(int id);
signals:

public slots:
    void roomSelectSlot(int id);
    void itemChangedSlot(int id,int type);

private:
    QStackedWidget *mStack;
    QList<sStackWidget*> mList;
    enum{Remove,Insert,Update};
    TG_DataShow *mTGShow;
};

#endif // TG_STATISTICSWIDGET_H
