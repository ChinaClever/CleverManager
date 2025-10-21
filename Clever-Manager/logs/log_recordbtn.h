#ifndef LOG_RECORDBTN_H
#define LOG_RECORDBTN_H

#include "log_alarmbtn.h"

class LOG_RecordBtn : public LOG_DateTimeBtn
{
    Q_OBJECT
public:
    explicit LOG_RecordBtn(QWidget *parent = 0);
    ~LOG_RecordBtn();

    void initRecord(void);

protected:
    void initUsr(void);
    void initContent(void);
    void initInfo(void);

    void initRecordLayout(void);

signals:

public slots:
    void usrBtnSLot(void);
    void contentBtnSlot(void);
    void infoBtnSlot(void);
    void button_clicked(void);

private:
    QLabel *usrLab;
    QLineEdit *usrEdit;
    QPushButton *usrBtn;

    QLabel *contentLab;
    QLineEdit *contentEdit;
    QPushButton *contentBtn;

    QLabel *infoLab;
    QLineEdit *infoEdit;
    QPushButton *infoBtn;

    QComboBox *box;
};

#endif // LOG_RECORDBTN_H
