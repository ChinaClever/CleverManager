#ifndef DS_LINEBTN_H
#define DS_LINEBTN_H

#include <QComboBox>
#include <QPushButton>
#include <QSignalMapper>
#include <QLabel>
#include <QHBoxLayout>
#include <QGroupBox>
#include "common.h"

class DS_LineBtn : public QWidget
{
    Q_OBJECT
public:
    explicit DS_LineBtn(QWidget *parent = 0);
    ~DS_LineBtn();

    void setLine(int line);

protected:
    void initWidget(void);
    void initLayout(void);

signals:
    void btnClickedSig(int);

public slots:

private:
    QPushButton *mBtn;
    QPushButton *lineBtn_1;
    QPushButton *lineBtn_2;
    QPushButton *lineBtn_3;

    QSignalMapper *m;
    QLabel *lab;
};

#endif // DS_LINEBTN_H
