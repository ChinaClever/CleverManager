#ifndef DT_STACKEDWIDGET_H
#define DT_STACKEDWIDGET_H

#include <QStackedWidget>
#include "dt_detaillist.h"

typedef struct
{
    int id; // 机房键值
    QString roomName; // 机房名
    DT_DetailList *widget; //窗口指针
}sStackedWidget;


class DT_StackedWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DT_StackedWidget(QWidget *parent = 0);

protected:
    void initLayout(void);
    void createWidget(int id, const QString &name);
    void initWidget(void);

    int findByID(int id);
    void delRoom(int id);
    bool getRoomById(int id, RoomItem &it);
    void addRoom(int id);

signals:
    void selectSig(QString,short);

public slots:
    void roomSelectSlot(int id, QString);
    void itemChangedSlot(int id,int type);

private:
     QStackedWidget *mStack;
    enum{Remove,Insert,Update};
    DT_DetailList *mDetailList;
    QList<sStackedWidget*> mList;
};

#endif // DT_STACKEDWIDGET_H
