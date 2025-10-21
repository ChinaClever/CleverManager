#include "ls_listtablehead.h"
#include "ui_ls_listtablehead.h"

LS_listTableHead::LS_listTableHead(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LS_listTableHead)
{
    ui->setupUi(this);

    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(selectSig(int)));
}

LS_listTableHead::~LS_listTableHead()
{
    delete ui;
}
