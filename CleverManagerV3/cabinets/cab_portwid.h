#ifndef FORMPOWERPORT_H
#define FORMPOWERPORT_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <atomic>

class Cab_PortWid : public QWidget
{
    Q_OBJECT

public:
    explicit Cab_PortWid(bool alignLeft,QWidget *parent = 0);
    ~Cab_PortWid();
    void setIndex(int id);
    int index();
    void setCurrentValue(double cur);
    void setSwitchState(bool b);
    void setSwitchDisabled();
    void setWarningState(bool b);
    QPushButton* button();
protected slots:
    void buttonClicked(bool b);
    void timeOut();
signals:
    void clicked(bool);
private:
    QPushButton*    m_pushbutton;
    QTimer          m_timer;
    QLabel*         m_label_index;
    QLabel*         m_label_currentValue;
    QLabel*         m_label_state;
    std::atomic_bool set_switch_flag;
};

#endif // FORMPOWERPORT_H
