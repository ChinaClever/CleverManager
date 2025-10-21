#ifndef LS_LISTTABLEHEAD_H
#define LS_LISTTABLEHEAD_H

#include <QWidget>

namespace Ui {
class LS_listTableHead;
}

class LS_listTableHead : public QWidget
{
    Q_OBJECT

public:
    explicit LS_listTableHead(QWidget *parent = 0);
    ~LS_listTableHead();

signals:
    int selectSig(int);

private:
    Ui::LS_listTableHead *ui;
};

#endif // LS_LISTTABLEHEAD_H
