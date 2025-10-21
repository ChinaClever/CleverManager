#ifndef USRMANAGEDLG_H
#define USRMANAGEDLG_H

#include <QGroupBox>
#include <QPushButton>
#include "usrlisttab.h"

class UsrManageDlg : public QWidget
{
    Q_OBJECT
public:
    explicit UsrManageDlg(QWidget *parent = 0);
    ~UsrManageDlg();

protected:
    void initWidget(void);
    void initLayout(void);
    void initFun(void);
    bool delCheck(QString &);

signals:

public slots:
    void addBtnSlot(void);
    void editBtnSlot(void);
    void delBtnSlot(void);

private:
    UsrListTab *m_usrList;
    QGridLayout *m_layout;
    QWidget *groupBox;

    QPushButton *m_newBtn; //新增按键
    QPushButton *m_editBtn; //编辑按键
    QPushButton *m_delBtn; //删除
    QHBoxLayout *hLayout;
};

#endif // USRMANAGEDLG_H
