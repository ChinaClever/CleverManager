#ifndef ACTIVEPROCESS_H
#define ACTIVEPROCESS_H


#include <QDialog>
#include"machinecode.h"
#include<QDate>
#include"registrykey.h"


namespace Ui {
class ActiveProcess;
}

class ActiveProcess : public QDialog
{
    Q_OBJECT

public:
    explicit ActiveProcess(QWidget *parent = 0);
    //explicit ActiveProcess(QWidget *parent);
    ~ActiveProcess();

    QString currentstr;
    void is_active();

    bool is_no;
    bool is_act;
    bool is_close;


    void write_file(QString &strr);
    void printf_info();
    void clearinfo();
    void free_register();
    bool is_number(QString src);
    void sleep(unsigned int msec);


private slots:
    void on_pushButton_clicked();

    // void on_pushButton_2_clicked();

    //void on_pushButton_3_clicked();

private:
    Ui::ActiveProcess *ui;

    MachineCode code;
    bool is_late;
    bool is_num;
    QDate date; //日期
    QString codestr; //机器码
    int num;
    RegistryKey key;


    void getdate();

    void getenum();

    void registe_info(QString str);
    void get_code();//获取激活码
};

#endif // ACTIVEPROCESS_H
