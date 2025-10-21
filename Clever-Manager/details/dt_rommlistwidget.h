#ifndef DT_ROMMLISTWIDGET_H
#define DT_ROMMLISTWIDGET_H

#include <QWidget>
#include <QListWidgetItem>
#include "topologywidget.h"
#include "common.h"

namespace Ui {
class DT_RommListWidget;
}

class DT_RommListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DT_RommListWidget(QWidget *parent = 0);
    ~DT_RommListWidget();

protected:
    void initData(void);
    void insertItem(void);
    QListWidgetItem *createRoomItem(const RoomItemExt &rItem);

private:
    QListWidgetItem *findById(int id);
    void delRoom(int id);
    bool getRoomById(int id, RoomItem &it);
    void addRoom(int id);
    void upRoom(int id);

signals:
    void roomSelectSig(int id, QString);

protected slots:
    void currentItemChangedSlot(QListWidgetItem*,QListWidgetItem*);
    void itemChangedSlot(int id,int type);

private:
    Ui::DT_RommListWidget *ui;
    enum{Remove,Insert,Update};
};

#endif // DT_ROMMLISTWIDGET_H
