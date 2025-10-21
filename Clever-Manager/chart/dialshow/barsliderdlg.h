#ifndef BARSLIDERDLG_H
#define BARSLIDERDLG_H

#include "barslider.h"
#include <QGridLayout>
#include <QLabel>

class BarSliderDlg : public QWidget
{
    Q_OBJECT
public:
    explicit BarSliderDlg(QWidget *parent = 0);
    ~BarSliderDlg();

    void setValue(double value);

signals:

public slots:

private:
    BarSlider *m_barSlider;
    QLabel *lab;
};

#endif // BARSLIDERDLG_H
