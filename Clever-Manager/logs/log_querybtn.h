#ifndef LOG_QUERYBTN_H
#define LOG_QUERYBTN_H

#include "log_datetimebtn.h"

class LOG_QueryBtn : public LOG_DateTimeBtn
{
    Q_OBJECT
public:
    explicit LOG_QueryBtn(QWidget *parent = 0);
    ~LOG_QueryBtn();

    void initRoom();
    void initCab();
    void initWidget(void);

protected slots:
     void btnSlot(void);

private:
    QLabel *roomLab, *cabLab;
    QLineEdit *roomEdit, *cabEdit;
    QPushButton *btn;
};

#endif // LOG_QUERYBTN_H
