#ifndef PDU_LISTWID_H
#define PDU_LISTWID_H

#include "dp_roompackets.h"
#include <QTreeWidgetItem>

namespace Ui {
class Pdu_ListWid;
}

class Pdu_ListWid : public QWidget
{
    Q_OBJECT

public:
    explicit Pdu_ListWid(QWidget *parent = 0);
    ~Pdu_ListWid();

signals:
    void selectedSig(sDataPacket *);

protected:
    void setHeader();
    void initWidget();
    void updateWidget();

    int getDevList(QTreeWidgetItem *item, sCabPacket *cap);
    int getCabList(QTreeWidgetItem *item, const QString &room);
    void getRoomList();

    sDataPacket *getPacket(const QString &str);

public slots:
    void clickedSlot(QModelIndex index);
    bool selectedItemSlot(QTreeWidgetItem*,int);


private slots:
    void on_btn_clicked();

private:
    Ui::Pdu_ListWid *ui;
};

#endif // PDU_LISTWID_H
