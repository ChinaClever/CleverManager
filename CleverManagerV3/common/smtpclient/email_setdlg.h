#ifndef EMAIL_SETDLG_H
#define EMAIL_SETDLG_H

#include <QDialog>
#include "sysconfigfile.h"
#include "common.h"

namespace Ui {
class Email_SetDlg;
}

#define  Email_Server 	"mail.china-clever.com"     //"smtp.qq.com" /*发件邮箱服务器：*/
#define  Email_UsrName   "test@china-clever.com"       //"pdu_email@foxmail.com"	/* 发送邮箱账号 */
#define  Email_Password   "clever123"	//"luozhiyong131" /*发送邮箱密码：*/
#define  Email_Port 	25 	/*发件邮箱端口号：*/
#define  Email_ToUsrName 	"pdu_email@qq.com" /*接收邮箱：*/

struct email_setInfo
{
    email_setInfo() {
        server = Email_Server;
        usr = Email_UsrName;
        pwd = Email_Password;
        port = Email_Port;
        to = Email_ToUsrName;
        ssl = 0;
    }

    QString usr; //用户名
    QString pwd;  //密码
    QString server; //发件邮箱服务器
    QString to;
    int  port; // 发件邮箱端口号
    int ssl; // 是否启用ssl
};

class Email_SetDlg : public QDialog
{
    Q_OBJECT

public:
    explicit Email_SetDlg(QWidget *parent = 0);
    ~Email_SetDlg();

    email_setInfo *getData(){return mSet;}
protected:
    void initData(void);
    void initWidget(void);
    bool dataCheck(void);
    void saveData(void);

private slots:
    void on_saveBtn_clicked();
    void on_testBtn_clicked();

private:
    Ui::Email_SetDlg *ui;
    email_setInfo *mSet;
};
extern email_setInfo *email_get_setInfo();

#endif // EMAIL_SETDLG_H
