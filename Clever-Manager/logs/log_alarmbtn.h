#ifndef LOG_ALARMBTN_H
#define LOG_ALARMBTN_H

#include<QComboBox>
#include "log_datetimebtn.h"

class LOG_AlarmBtn : public LOG_DateTimeBtn
{
    Q_OBJECT
public:
    explicit LOG_AlarmBtn(QWidget *parent = 0);
    ~LOG_AlarmBtn();

    void initAlarmBtn(void);
    void initOffLineBtn(void);

protected:
    void initDevIP(void);
    void initDevName(void);
    void initAlarmType(void);
    void initAlarmLayout(void);

signals:

public slots:
    void devIPBtnSlot(void);
    void devNameBtnSlot(void);
    void alarmTypeBtnSlot(void);
    void comboxIndexChanged();

private:
    QLabel *devIPLab;
    QLineEdit *devIPEdit;
    QPushButton *devIPBtn;

    QLabel *devNameLab;
    QLineEdit *devNameEdit;
    QPushButton *devNameBtn;

    QLabel *alarmTypeLab;
    QLineEdit *alarmTypeEdit;
    QPushButton *alarmTypeBtn;

    QLineEdit *Edit;
    QComboBox *box;
};

#endif // LOG_ALARMBTN_H
