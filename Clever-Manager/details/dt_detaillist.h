#ifndef DT_DETAILLIST_H
#define DT_DETAILLIST_H

#include <QtCore>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include "dt_detailtree.h"
#include<QComboBox>

class DT_DetailList : public QWidget
{
    Q_OBJECT
public:
    explicit DT_DetailList(QWidget *parent = 0);
    ~DT_DetailList();

    void initData(int id, QString &name);

protected:
    void initWidget(void);
    void initLayout(void);

signals:
    void selectSig(QString,short);

public slots:
    void timeoutDone(void);
    void refreshBtnSlot(void);
    void seekBtnSlot(void);
    void manageBtnSlot(void);

private:
    QTimer *timer;
    DT_DetailTree *mDetailTree;

    QWidget *widget;
    QGroupBox *groupBox;
    QPushButton *refreshBtn;
    QLabel *lab;
    QLineEdit *edit;
    QPushButton *seekBtn;
    QComboBox *box;

    QPushButton *manageBtn;    
};

#endif // DT_DETAILLIST_H
