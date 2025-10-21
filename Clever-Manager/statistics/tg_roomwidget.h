#ifndef TG_ROOMWIDGET_H
#define TG_ROOMWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QGridLayout>
#include <QGroupBox>
#include "topologywidget.h"
#include "common.h"

using namespace std;

namespace Ui {
class TG_RoomWidget;
}

class TG_RoomWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TG_RoomWidget(QWidget *parent = 0);
    ~TG_RoomWidget();


protected:
    void initData(void);
    void insertRoomItem(void);
    void createRoomItem(const RoomItemExt &rItem);

    int findById(int id);
    void delRoom(int id);
    bool getRoomById(int id, RoomItem &it);
    void addRoom(int id);
    void upRoom(int id);

signals:
    void roomSelectSIg(int id);

protected slots:
    void currentItemChangedSlot(int);
    void itemChangedSlot(int,int);
    void initFunSLot();

private slots:
    void on_upBtn_clicked();

    void on_downBtn_clicked();

//    void on_comboBox_2_currentIndexChanged(const QString &arg1);

private:
    Ui::TG_RoomWidget *ui;
    enum{Remove,Insert,Update};
};

//extern QString comtext;

#endif // TG_ROOMWIDGET_H
