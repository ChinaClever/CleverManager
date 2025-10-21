#ifndef DIALPANDLG_H
#define DIALPANDLG_H

#include <QWidget>
#include "dialpan.h"

class DialpanDlg : public QWidget
{
    Q_OBJECT
public:
    explicit DialpanDlg(QWidget *parent = 0);
    ~DialpanDlg();

    void setValue(double value);

signals:

public slots:

private:
    dialpan *m_dialpan;
    QLabel *lab;
};

#endif // DIALPANDLG_H
