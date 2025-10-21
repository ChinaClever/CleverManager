#ifndef LS_MGROUPWIDGET_H
#define LS_MGROUPWIDGET_H

#include <QWidget>
#include <QSplitter>
#include "ls_mgrouplist.h"
#include "ls_mgroupbtn.h"

class ls_MGroupWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ls_MGroupWidget(QWidget *parent = 0);

protected:
    void initWidget(void);

    void initTable(void);
    void initBtn(void);

    bool isExist(QString &);

    void renameGroup(QString &oldGroup, QString &);
    void moveGroup(QString &oldGroup, QString &);

signals:
    void selectSig(QString &);

public slots:
    void updateSlot(int);

protected slots:
    void addSlot();
    void delSlot();
    void editSlot();
    void moveSlot();

public:
    QSplitter *splitter;
    QGridLayout *layout;

    LS_MgroupList *mTable;
    LS_MGroupBtn *mBtn;
};

#endif // LS_MGROUPWIDGET_H
