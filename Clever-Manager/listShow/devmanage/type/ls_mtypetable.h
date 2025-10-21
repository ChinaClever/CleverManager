#ifndef LS_MTYPETABLE_H
#define LS_MTYPETABLE_H

#include <QWidget>
#include <QTableWidget>
#include <QHeaderView>

#include "ls_mtypebtn.h"
#include "pdu/dev/devType/pdudtname.h"


class LS_MTypeTable : public QWidget
{
    Q_OBJECT
public:
    explicit LS_MTypeTable(QWidget *parent = 0);
    ~LS_MTypeTable();

    void updateWidget(void);

    int getNum(void);
    int getDevType(QString &str);

protected:
    void initTableWidget(void);

private:
    void initWidget(void);
    void addItemContent(int row, int column, QString &content);
    void addRowContent(QStringList &list);

signals:
    void selectSig(int);

public slots:
    void initFunSLot();
    void selectSlot(QTableWidgetItem *);

private:
    QTableWidget *mTableWidget;
    QGridLayout *layout;

    int mTypeNum;
    QString mTypeName;
    PduDTName *mType;
};

#endif // LS_MTYPETABLE_H
