#ifndef VERSIONSELECT_H
#define VERSIONSELECT_H

#include <QDialog>
#include<QDebug>

namespace Ui {
class VersionSelect;
}

class VersionSelect : public QDialog
{
    Q_OBJECT

public:
    explicit VersionSelect(QWidget *parent = 0);
    ~VersionSelect();

    int number;


private slots:
void on_pushButton_clicked();

private:
    Ui::VersionSelect *ui;
//    QString str;
};

#endif // VERSIONSELECT_H
