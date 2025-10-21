#ifndef LS_MTYPEBTN_H
#define LS_MTYPEBTN_H

#include <QWidget>
#include <QPushButton>
#include <QGroupBox>
#include <QHBoxLayout>
#include "common.h"

class LS_MTypeBtn : public QWidget
{
    Q_OBJECT
public:
    explicit LS_MTypeBtn(QWidget *parent = 0);

protected:
    void initWidget(void);
    void initBtn(void);

signals:
    void refreshSIg();
    void delSig();
//    void addSig();
//    void reloadSig();

public slots:

private:
    QPushButton *refreshBtn;
    QPushButton *delBtn;

//    QPushButton *addBTn;
//    QPushButton *reloadBtn;
};

#endif // LS_MTYPEBTN_H
