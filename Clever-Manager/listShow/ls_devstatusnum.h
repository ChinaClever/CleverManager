#ifndef LS_DEVSTATUSNUM_H
#define LS_DEVSTATUSNUM_H

#include "ls_listtabledlg.h"

class LS_DevStatusNum : public QWidget
{
    Q_OBJECT
public:
    explicit LS_DevStatusNum(QWidget *parent = 0);
    ~LS_DevStatusNum();

protected:
    void lineDevNum(void);
    void alarmDevNum(void);
    void offLineDevNum(void);

private:
    void initWidget(void);
    void initLayout(void);

signals:

public slots:
    void timeoutDone(void);

private:
    QLabel *lineIconLab;
    QLabel *lineDevLab;

    QLabel *alarmIconLab;
    QLabel *alarmDevLab;

    QLabel *offLineIconLab;
    QLabel *offLineDevLab;

    QTimer *timer;
};

#endif // LS_DEVSTATUSNUM_H
