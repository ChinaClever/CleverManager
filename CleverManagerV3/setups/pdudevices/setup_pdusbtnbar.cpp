/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "setup_pdusbtnbar.h"

Setup_PdusBtnBar::Setup_PdusBtnBar(QWidget *parent) : SqlBtnBar(parent)
{
    setNoEdit();
    clearHidden();
    mQueryDlg = nullptr;
}


QString Setup_PdusBtnBar::queryBtn()
{
    QString str;
    if(mQueryDlg) {
        int ret = mQueryDlg->Exec();
        if(ret == QDialog::Accepted) {
            str = mQueryDlg->getCmd();
        }
    }

    return str;
}

