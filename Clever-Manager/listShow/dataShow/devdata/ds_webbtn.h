#ifndef DS_WEBBTN_H
#define DS_WEBBTN_H

#include <QUrl>
#include <QPushButton>
#include <QVBoxLayout>

class DS_WebBtn : public QWidget
{
    Q_OBJECT
public:
    explicit DS_WebBtn(QWidget *parent = 0);
    ~DS_WebBtn();

    void updateIPAddr(QString &);

signals:

public slots:
    void manageBtnSlot();
    void logBtnSlot();

protected:


private:
    QPushButton *manageBtn;
    QPushButton *logBtn;
    QVBoxLayout *vLayout;

    QString m_ip;    
};

#endif // DS_WEBBTN_H
