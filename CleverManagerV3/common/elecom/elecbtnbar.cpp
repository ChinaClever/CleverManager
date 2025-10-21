/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "elecbtnbar.h"

ElecBtnBar::ElecBtnBar(QWidget *parent) : LogBtnBar(parent)
{

}

QString ElecBtnBar::queryBtn()
{
    QString str;
    if(mQueryDlg) {
        int ret = mQueryDlg->Exec();
        if(ret == QDialog::Accepted) {
            emit clearSig(); // 清除现成数据
            QCoreApplication::processEvents(QEventLoop::AllEvents,1250);
            str = mQueryDlg->getCmd();
            emit refreshSig();
        }
    }

    return str;
}
