/*
 * editusrdlg.cpp
 *  用户信息修改界面
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "editusrdlg.h"
#include "sql/records/db_sqlquery.h"

EditUsrDlg::EditUsrDlg(QWidget *parent) :
    NewUsrDlg(parent)
{
    QString str = tr("编辑用户");
    editTitle(str);

    mUsr = new UsrClass;
}

EditUsrDlg::~EditUsrDlg()
{
    delete mUsr;
}


 bool EditUsrDlg::setUsrName(QString &name)
 {
     bool ret = true;

    UsrStruct *usrInfo = mUsr->getUsr(name);
    if(usrInfo)
        loadUsrInfo(usrInfo);
    else
        ret = false;

    return ret;
 }


bool EditUsrDlg::saveUsrInfo(UsrStruct *s_temp)
{
    bool ret = mUsr->editUsr(s_temp);
    if(ret) {
        /*增加日志信息*/
        sLandLog log;
        log.title = tr("修改");
        log.operation = tr("用户修改");
        log.msg = tr("修改用户:%1信息").arg(s_temp->name);
        sql_insert_land(log);
    }

    return ret;
}
