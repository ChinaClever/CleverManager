#include "ls_mdevwidget.h"
#include <QInputDialog>
#include "common.h"
#include "common/msgBox/msgbox.h"
#include "pdu/dev/devmanage/pdudmmanage.h"

ls_MDevWidget::ls_MDevWidget(QWidget *parent) : QWidget(parent)
{
    splitter=new QSplitter(Qt::Vertical,this);
    initWidget();

    layout = new QGridLayout(this);
    layout->addWidget(splitter);
    groupBox_background_icon(this);
}

/**
 * @brief 初始化按键
 */
void ls_MDevWidget::initBtn(void)
{
    mBtn = new LS_MDevBtn(splitter);

    connect(mBtn,SIGNAL(addSig()),this,SLOT(addSlot()));
    connect(mBtn,SIGNAL(delSig()),this,SLOT(delSlot()));
    connect(mBtn,SIGNAL(moveSig()),this,SLOT(moveSlot()));
    connect(mBtn,SIGNAL(reloadSig()),this,SLOT(reloadSlot()));
}

/**
 * @brief 初始化窗口
 */
void ls_MDevWidget::initWidget(void)
{
    mTable = new LS_MdevTable(splitter);
    initBtn();

    splitter->setStretchFactor(0,7);
    splitter->setStretchFactor(1,3);
}

/**
 * @brief 更新设备列表
 * @param group
 */
void ls_MDevWidget::updateSlot(QString &group)
{
    mTable->updateWidget(group);
}




/**
 * @brief 新增设备
 *      如果设备不存在就增加新设备
 *      如果设备存在，就把设备转移至此组中来
 */
void ls_MDevWidget::addSlot()
{
    bool isOK;
    QString ip = QInputDialog::getText(this,tr("增加设备"),tr("新增IP："),
                                       QLineEdit::Normal,"192.168.1.89",&isOK);
    if(isOK && !ip.isEmpty())
    {
        isOK = isIPaddress(ip); // 地址是否合法
        if(isOK)
        {
            /**
             * 第一步：判断设备是否存在，
             * 第二步：如果存在，是否已存在当前列表中
             * 第三步：存在，但不处于当前列表中，把设备移到当前列表中
             * 第四步：不存在，则创立此设备
             */
            QString group;
            isOK = mTable->getGroup(group);
            if(isOK)
            {
                PduDGList *groupDG = mTable->mGroupList;
                isOK = groupDG->containsDev(ip); // 设备存在
                if(isOK)
                {
                    isOK = groupDG->containsDev(group, ip); // 设备已存在当前列表中
                    if(isOK)
                    {
                        InfoMsgBox box(this,tr("设备 %1 已存在当前分组中!!! ").arg(ip));
                        return ;
                    }
                    else // 设备存在其它组中
                    {
                        pdu_dm_renameGroup(ip, group); // 设备移至当前组中
                        groupDG->moveDev(ip, group);
                        mTable->updateWidget();

                        ////======= 日志记录
                        ///
                        ///
                        ///
                    }
                }
                else // 设备不存在
                {
                    QuMsgBox box(this,tr("设备 %1 不存在!\n 是否继续增加").arg(ip));
                    isOK = box.Exec();
                    if(isOK)
                    {
                        ///=========
                        ///
                        ///
                        ///
                    }
                }
            }
        }
        else
            CriticalMsgBox box(this,ip + " 不是合法的IP地址!!! ");
    }
}


/**
 * @brief 删除设备
 */
void ls_MDevWidget::delSlot()
{
    QString ip, name;
    bool ret = mTable->getSelectDev(ip, name);
    if(ret)
    {
        QString str =  tr("确定删除此设备？\n设备IP：%1 \n设备名: %2").arg(ip).arg(name);
        QuMsgBox box(this,str);
        ret = box.Exec();
        if(ret)
        {
            pdu_dm_delDev(ip); // 删除设备数据

            PduDGList *groupDG = mTable->mGroupList;
            groupDG->delDev(ip);
            mTable->updateWidget();


            //            m_mdevTable->delDev();

            //            /*增加日志信息*/
            //            s_RecordLog log;
            //            log.title = tr("设备管理");
            //            log.operation = tr("设备删除");
            //            log.msg = tr("设备名：%1 设备IP：%2").arg(dev.Name).arg(dev.IP);
            //           sql_insert_record(log);
        }
    }
}


/**
 * @brief 移动设备
 */
void ls_MDevWidget::moveSlot()
{
    QString group;
    bool isOK = mTable->getGroup(group);
    if(isOK)
    {
        PduDGList *groupDG = mTable->mGroupList;
        QStringList groupList;
        groupDG->listGroup(groupList); //获得所有组
        groupList.removeOne(group); //过滤自己

        if(groupList.size() > 0)
        {
            QString ip;
            mTable->getIP(ip);

            QString str = ip +tr("移动到：");
            QString newGroup = QInputDialog::getItem(this,tr("移动设备"),str,groupList,0,false,&isOK);
            if(isOK && !newGroup.isEmpty())
            {
                /**
                 * @brief 这组中只有一个设备时，设备被移出该组，就增加一个空设备,保留组名
                 */
                QStringList devList;
                groupDG->listDev(group, devList);
                if(devList.size() == 1)
                {
                    QString str;
                    groupDG->add(group, str);
                }

                pdu_dm_renameGroup(ip, newGroup);
                groupDG->moveDev(ip, newGroup);
                mTable->updateWidget();


                //                /*增加日志信息*/
                //                s_RecordLog log;
                //                log.title = tr("设备管理");
                //                log.operation = tr("设备移动");
                //                log.msg = tr("设备IP：%1 %2").arg(str).arg(group);
                //                sql_insert_record(log);

            }
        }
        else
            InfoMsgBox box(this,tr("仅有一个组不能移动！！"));
    }
}

/**
 * @brief 重新加载设备
 */
void ls_MDevWidget::reloadSlot()
{
    mTable->updateWidget();
}
