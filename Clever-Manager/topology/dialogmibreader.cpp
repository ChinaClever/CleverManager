#include "dialogmibreader.h"
#include "ui_dialogmibreader.h"
#include "mibreader.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include "common.h"

DialogMibReader::DialogMibReader(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMibReader)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint);
    reader = new MibReader(this);

    connect(reader,SIGNAL(progressChange(int)),ui->progressBar_readFile,SLOT(setValue(int)));
    connect(reader,SIGNAL(progressDbChange(int)),ui->progressBar_saveDb,SLOT(setValue(int)));
    connect(reader,SIGNAL(result(int)),SLOT(finshed(int)));
}

DialogMibReader::~DialogMibReader()
{
    delete ui;
}

void DialogMibReader::parseFile(const QString &path)
{
    ui->label_mibPath->setText(path);
    reader->setPath(path);
    reader->start();
}

QString DialogMibReader::errMsg()
{
    return msg;
}

void DialogMibReader::finshed(int type)
{
    if(type < 0){
        msg = reader->errMsg(type);
        reject();
    } else {
        accept();
    }
}
