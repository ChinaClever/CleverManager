#ifndef USRCLASS_H
#define USRCLASS_H
#include "usrjson.h"



typedef struct
{
    bool jurisdiction; /*权限 0 管理员，1 访客*/
    QString name;   /*用户名*/
    QString pwd;    /*密码*/
    QString telephone; /*电话*/
    QString email; /*邮箱*/
    QString remarks; /*备注*/
}UsrStruct;

typedef  QMap<QString, UsrStruct*> UsrMaptype;

class UsrClass
{
//    Q_OBJECT
public:
    UsrClass();
    ~UsrClass();

    bool addUsr(UsrStruct *);
    UsrStruct * getUsr(QString &);

    bool editUsr(UsrStruct *);
    bool delUsr(QString &);
    int getCount(void);
    UsrMaptype * getUsrMap(void);

protected:
    void initUsr(void);

private:
    UsrJson m_usrJson;
    UsrMaptype *m_usrMap;
};

extern UsrMaptype g_usrMap;
extern int usr_telephone_list(QStringList &phone);
int usr_email_list(QStringList &email);

#endif // USRCLASS_H
