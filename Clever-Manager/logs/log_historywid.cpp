#include "log_historywid.h"
#include "ui_log_historywid.h"

LOG_HistoryWid::LOG_HistoryWid(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LOG_HistoryWid)
{
    ui->setupUi(this);
    mData = new chartData;
    initDateTimeEdit();
}

LOG_HistoryWid::~LOG_HistoryWid()
{
    delete ui;
}

void LOG_HistoryWid::on_pushButton_search_clicked()
{
    if(checkEnter())
    {
#if 0
        qDebug() << "==============true==============" << ui->dateTimeEdit_start->dateTime() << ui->dateTimeEdit_end->dateTime();
        diagram * mDiagram = new diagram;

        mDiagram->setDateTime(ui->dateTimeEdit_start->dateTime(),ui->dateTimeEdit_end->dateTime());
        mDiagram->setIp(ui->lineEdit_ip->text());

        if(!mDiagram->setType(ui->comboBox->currentIndex()))
            return;
        mDiagram->dataShow();
        mDiagram->exec();
        //                mDiagram->dataShow();
        this->close();
#endif       
        writeChartData();
        emit(signalChart(mData));
    }
}

void LOG_HistoryWid::initDateTimeEdit()
{
    mDatetime = QDate::currentDate();
    ui->dateTimeEdit_start->setDate(mDatetime);
    ui->dateTimeEdit_end->setDate(mDatetime);

    ui->dateTimeEdit_start->setTime(QTime(0,0,0));
    ui->dateTimeEdit_end->setTime(QTime(23,59,59));
}

bool LOG_HistoryWid::checkEnter()
{
    qDebug() << "-------startTime------" << ui->dateTimeEdit_start->dateTime()<< "-------endTime------" << ui->dateTimeEdit_end->dateTime() ;
    if(ui->dateTimeEdit_end->dateTime().operator <=(ui->dateTimeEdit_start->dateTime()))
    {
        QMessageBox::warning(this,tr("waring"),tr("起始时间不能等于或晚于结束时间，请重新选择时间！"),tr("确定"));
        return false;
    }

    if(!isIPaddress(ui->lineEdit_ip->text()))
    {
        QMessageBox::warning(this,tr("waring"),tr("请输入正确的IP地址！"),tr("确定"));
        return false;
    }

    return true;
}

void LOG_HistoryWid::on_lastDayBtn_clicked()
{
    QDate date = QDate::currentDate();
    ui->dateTimeEdit_end->setDate(date);
    ui->dateTimeEdit_start->setDate(mDatetime.addDays(-1));
    setTime();
}

void LOG_HistoryWid::on_lastWeekBtn_clicked()
{
    QDate date = QDate::currentDate();
    ui->dateTimeEdit_end->setDate(date);
    ui->dateTimeEdit_start->setDate(date.addDays(-7));
    setTime();
}

void LOG_HistoryWid::on_lastMonthBtn_clicked()
{
    QDate date = QDate::currentDate();
    ui->dateTimeEdit_end->setDate(date);
    ui->dateTimeEdit_start->setDate(date.addMonths(-1));
    setTime();
}

void LOG_HistoryWid::setTime()
{
    ui->dateTimeEdit_start->setTime(QTime(0,0,0));
    ui->dateTimeEdit_end->setTime(QTime(23,59,59));
}


void LOG_HistoryWid::writeChartData()
{
    mData->startDateTime = ui->dateTimeEdit_start->dateTime();
    mData->endDateTime = ui->dateTimeEdit_end->dateTime();
    mData->title = QString(tr("时间 - %1 曲线")).arg(ui->comboBox->currentText());
    mData->type = 1;
    mData->strIp = ui->lineEdit_ip->text();

    int index = ui->comboBox->currentIndex();

    if(index <6)
    {
        mData->index = index/2 + 1;
        if(index%2)
            mData->dataType = 1;
        else
            mData->dataType = 2;
    }
}
