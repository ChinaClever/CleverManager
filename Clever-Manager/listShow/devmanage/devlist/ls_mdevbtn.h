#ifndef LS_MDEVBTN_H
#define LS_MDEVBTN_H

#include <QWidget>
#include <QPushButton>
#include <QGroupBox>
#include <QHBoxLayout>
#include "common.h"

class LS_MDevBtn : public QWidget
{
    Q_OBJECT
public:
    explicit LS_MDevBtn(QWidget *parent = 0);

protected:
    void initWidget(void);
    void initBtn(void);

signals:
    void addSig();
    void delSig();
    void moveSig();
    void reloadSig();

public slots:

private:
    QPushButton *addBtn;
    QPushButton *delBtn;

    QPushButton *moveBtn;
    QPushButton *reloadBtn;
};

#endif // LS_MDEVBTN_H
