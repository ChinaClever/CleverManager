#ifndef CAB_TREEWID_H
#define CAB_TREEWID_H

#include "dp_roompackets.h"
#include <QTreeWidgetItem>

namespace Ui {
class Cab_TreeWid;
}

class Cab_TreeWid : public QWidget
{
    Q_OBJECT

public:
    explicit Cab_TreeWid(QWidget *parent = 0);
    ~Cab_TreeWid();

signals:
    void selectedSig(int);

protected:
    void setHeader();
    void initWidget();
    void updateWidget();

    int getCabList(QTreeWidgetItem *item, const QString &room);
    void getRoomList();

    int getId(const QString &room, const QString &cab);

public slots:
    void clickedSlot(QModelIndex index);
    bool selectedItemSlot(QTreeWidgetItem*,int);


private slots:
    void on_btn_clicked();

private:
    Ui::Cab_TreeWid *ui;
};

#endif // CAB_TREEWID_H
