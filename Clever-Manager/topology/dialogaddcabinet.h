#ifndef DIALOGADDCABINET_H
#define DIALOGADDCABINET_H

#include <QDialog>
#include "dbcabinetlist.h"
#include <QTableWidget>
#include "data/hash/read/pduhashread.h"
extern void dl_save_add(QString &ip);

namespace Ui {
class DialogAddCabinet;
}

class DialogAddCabinet : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddCabinet(CabinetItem &item,QTableWidget* parent);
    ~DialogAddCabinet();
public slots:
    void accept();
protected:
    Ui::DialogAddCabinet *ui;
    CabinetItem &m_item;
    bool isAdd;
    QTableWidget* m_tableWidget;
//    PduDataPacket* mainType;  //主机设备类型
//    PduDataPacket* spareType;  //
    int mainType;
    int spareType;
    QString mainStype;
    QString spareStype;
    QString mainSip;
    QString spareSip;
    int mainNum;
    int spareNum;
    bool isModfy;//是否是修改

protected:
    int typeToInt(QString type);//将设备类型转化为整型
    int intToIndex(int type); //将设备类型转化成相应的index

};

class DialogModifyCabinet : public DialogAddCabinet
{
    Q_OBJECT

public:
    explicit DialogModifyCabinet(CabinetItem &item,QTableWidget* parent);
};

#endif // DIALOGADDCABINET_H
