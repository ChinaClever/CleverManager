#ifndef LS_REFRESHBTN_H
#define LS_REFRESHBTN_H

#include <QPushButton>
#include <QHBoxLayout>

class LS_RefreshBtn : public QWidget
{
    Q_OBJECT
public:
    explicit LS_RefreshBtn(QWidget *parent = 0);
    ~LS_RefreshBtn();

protected:
    void initWidget(void);
    void initBtn(void);

signals:
    void refreshSig();

public slots:
    void manageBtnSlot(void);

public:
    QPushButton *refreshBtn;
//    QPushButton *manageBtn;
};

#endif // LS_REFRESHBTN_H
