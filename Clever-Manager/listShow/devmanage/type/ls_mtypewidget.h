#ifndef LS_MTYPEWIDGET_H
#define LS_MTYPEWIDGET_H

#include <QWidget>
#include <QSplitter>
#include "ls_mtypetable.h"

class LS_MTypeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LS_MTypeWidget(QWidget *parent = 0);

protected:
    void initWidget(void);

    void initTable(void);
    void initBtn(void);

signals:
    void selectSig(int);

protected slots:
    void delSlot();

private:
    QSplitter *splitter;
    QGridLayout *layout;

    LS_MTypeTable *mTable;
    LS_MTypeBtn *mBtn;

};

#endif // LS_MTYPEWIDGET_H
