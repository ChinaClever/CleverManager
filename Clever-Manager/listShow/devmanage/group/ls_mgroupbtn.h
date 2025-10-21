#ifndef LS_MGROUPBTN_H
#define LS_MGROUPBTN_H

#include <QWidget>
#include <QPushButton>
#include <QGroupBox>
#include <QHBoxLayout>
#include "common.h"

class LS_MGroupBtn : public QWidget
{
    Q_OBJECT
public:
    explicit LS_MGroupBtn(QWidget *parent = 0);

protected:
    void initWidget(void);
    void initBtn(void);

signals:
    void addSig();
    void delSig();
    void editSig();
    void moveSig();

public slots:

private:
    QPushButton *addBtn;
    QPushButton *delBtn;

    QPushButton *editBtn;
    QPushButton *moveBtn;
};

#endif // LS_MGROUPBTN_H
