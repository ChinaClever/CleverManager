#include "dialogremoverack.h"
#include "ui_dialogremoverack.h"
#include <QMessageBox>

DialogRemoveRack::DialogRemoveRack(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRemoveRack)
{
    ui->setupUi(this);
}

DialogRemoveRack::~DialogRemoveRack()
{
    delete ui;
}

void DialogRemoveRack::setAddress(int i)
{
    ui->spinBox->setValue(i);
}

int DialogRemoveRack::address()
{
    return ui->spinBox->value();
}
