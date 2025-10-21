#include "loopwidget.h"

#include "zebra/zebra_client.h"

UI_LoopWidget::UI_LoopWidget(QWidget *parent) : QWidget(parent)
{
    //    this->setStyleSheet("background-color:rgb(204,232,207)");
    record=new PDU_AlarmRecord(this);
    initWidget();
    initLayout();
}

void UI_LoopWidget::initWidget()
{
    mDataPacket = NULL;
    mLoopWidget=new QTableWidget(6,10,this);  //行、列
    //    horhead<<tr("回路")<<tr("空开状态")<<tr("回路电压")<<tr("回路电流")<<tr("回路功率")<<tr("最小值(A)")<<tr("下限界(A)")<<tr("上限界(A)")<<tr("最大值(A)")<<tr("最小值(V)")<<tr("最大值(V)")<<tr("开关设置");
    //    horhead<<tr("回路")<<tr("空开状态")<<tr("回路电流")<<tr("回路功率")<<tr("最小值(A)")<<tr("下限界(A)")<<tr("上限界(A)")<<tr("最大值(A)")<<tr("开关设置");
    horhead<<tr("回路")<<tr("空开状态")<<tr("回路电压")<<tr("回路电流")<<tr("回路功率")<<tr("最小值(A)")<<tr("最大值(A)")<<tr("最小值(V)")<<tr("最大值(V)")<<tr("开关设置");
    mLoopWidget->setHorizontalHeaderLabels(horhead);
    mLoopWidget->setVerticalHeaderLabels(verhead);
    
    mLoopWidget->setColumnCount(horhead.size());
    mLoopWidget->setRowCount(0);
    
    mLoopWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mLoopWidget->horizontalHeader()->setStretchLastSection(true);
    mLoopWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  //设置不可编辑
    connect(mLoopWidget,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this,SLOT(item_doubleclicked(QTableWidgetItem*)));
    
    mTimer = new QTimer(this);
    connect( mTimer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    mTimer->start(2*1000);
    
    
#if 0
    initTableWidget();
#endif
    
}

void UI_LoopWidget::initLayout()
{
    box=new QCheckBox;
    QLabel *check=new QLabel(tr("开关统一设置:"),this);
    check->setFont(QFont("微软雅黑",12));
    QWidget *littlewidget=new QWidget(this);
    QHBoxLayout *littlelayout=new QHBoxLayout(littlewidget);
    littlelayout->addStretch();
    littlelayout->addWidget(check);
    littlelayout->addWidget(box);
    littlewidget->setFixedHeight(35);
    box->setCheckable(false);
    
    check->setStyleSheet("color:white");
    mLayOut=new QVBoxLayout(this);
    
    mLayOut->addWidget(mLoopWidget);
    mLayOut->addWidget(littlewidget);
    mLayOut->setContentsMargins(18,19,18,14);
    mLayOut->setSpacing(0);
}

void UI_LoopWidget::initTalbeWidget()
{
    
    
    
}

void UI_LoopWidget::additem(int row, int column, QString content)
{
    //    mAddressMap.clear();
    if(column != 9)
    {
        QTableWidgetItem *aitem=new QTableWidgetItem(content);
        aitem->setTextAlignment(Qt::AlignCenter ); //设置文本居中
        mLoopWidget->setItem(row,column,aitem);
    }
    else  //如果是第八列，那么就在单元格中加入按钮
    {
        QWidget *buttonWidget=new QWidget(this);
        QHBoxLayout *buttonLayout=new QHBoxLayout(buttonWidget);
        
        button_on=new QPushButton(this); //在最后一栏添加按钮
        //button_on->setFixedHeight(30);
        button_on->setText(tr("on"));
        btnBlue_style_sheet(button_on);
        
        is_on=false;
        button_off=new QPushButton(this);
        button_off->setText(tr("off"));
        btnBlue_style_sheet(button_off);
        
        buttonLayout->addWidget(button_on);
        buttonLayout->addWidget(button_off);
        buttonLayout->setContentsMargins(10, 2, 10, 3);
        
        mAddressMap.insert(row,button_on);
        mAddressMap.insert(row,button_off);
        
        connect(button_on,SIGNAL(clicked()),this,SLOT(button_clicked()));
        connect(button_off,SIGNAL(clicked()),this,SLOT(button_clicked()));
        
        mLoopWidget->setCellWidget(row,column,buttonWidget);
    }
}

void UI_LoopWidget::addRowInformation(QStringList list)
{
    int count=mLoopWidget->rowCount();//获取当前表格的行数
    mLoopWidget->insertRow(count); //显示数据时先要插入一行
    for(int i=0;i<horhead.size();i++)
        additem(count,i,list[i]);  //确定行列插入内容
}


//
void UI_LoopWidget::button_clicked()
{
    QPushButton *button = dynamic_cast<QPushButton *>(QObject::sender()); //找到信号发送者
    
    //遍历map，通过比较地址，获取当前按钮所在回路
    
    int count = mLoopNum;
    int i,switchData;
    QList<QPushButton *> addresslist ;
    for(i=0;i<count;i++)
    {
        addresslist.clear();
        addresslist=mAddressMap.values(i);
        
        for(int j=0;j<addresslist.size();j++)
        {
            if(addresslist[j] == button)
            {
                mCurrentButtonRow=i;
                switchData = j;
                break;
            }
        }
        
    }
    
    if(mDataPacket !=NULL)//当不为空时，才能发送，否则段错误
    {
        sentSwitchData(mCurrentButtonRow,switchData);//i表示回路数，j为1表示打开，为0表示断开
    }
}

void UI_LoopWidget::getDataPacket(QString &ip,int num)
{
    
    mDataPacket =NULL;
    mDataPacket=pdu_hd_getPacket(ip,num);
    mLoopNum = mDataPacket->data->loop->cur->value->size();
    qDebug() << "回路数："<< mLoopNum;
    box->setCheckable(true);
    box->setChecked(false);
    
    initTableWidget();
    
    if(mDataPacket->devType != PDU_TYPE_M_PDU)
    {
        clearTable();
        
    }else
    {
        mLoopWidget->setVisible(true);
    }
    
    
    //根据ip和设备号获取回路数据包
    
    if(mDataPacket !=NULL && mDataPacket->devType == PDU_TYPE_M_PDU) //如果不为空才能执行下面动作，否则段错误
    {
        updateData();
        mLoopWidget->setRowCount(mDataPacket->data->loop->cur->value->size()); //将表格设置为固定行，多余的不显示，如6回路变成3回路时，将不显示多余的回路
    }
    
}

void UI_LoopWidget::updateData()
{
    qDebug() <<"updateData --- start";
    
    if(mDataPacket !=NULL && isCheckOnline())//检查当前ip是否在线，否则不显示数据,如果数据包在点击ip时未获取到，那么在1s刷新时必须检测数据包，否则段错误
    {
        
        for(int i=0;i<mLoopNum;i++)
        {
            
            int j=0;
            setLoopName(i,j++);  //i为行号，j为列号
            setSwitchStatus(i,j++);
            setLoopVol(i,j++);//设置电压
            setLoopCurrent(i,j++);
            setLoopPower(i,j++);
            setLoopMinimum(i,j++);
            //            setLoopDownValue(i,j++);
            //            setLoopUpValue(i,j++);
            setLoopMax(i,j++);
            setLoopVolMin(i,j++);//设置电压最小值
            setLoopVolMax(i,j++);//设置电压最大值
        }
    }
    else  //设备离线，清空表格
    {
        clearTable();
    }
}

void UI_LoopWidget::setLoopName(int row,int column)
{
    //    qDebug() <<"setLoopName --- start";
    //确定回路数目
    QString loopName=QString("C%1").arg(row+1);
    QTableWidgetItem *newItem=mLoopWidget->item(row,column);
    newItem->setText(loopName);
}

void UI_LoopWidget::setSwitchStatus(int row,int column)
{
    QString status;
    int sw = mDataPacket->data->loop->sw->get(row);
    
    QTableWidgetItem *newItem=mLoopWidget->item(row,column);
    if(sw == 1)
    {
        status = tr("断开");
        newItem->setIcon(QIcon(":/output/output/opalarm.jpg"));
        newItem->setTextColor(Qt::red);
    }
    else if(sw == 2)
    {
        newItem->setIcon(QIcon(":/output/output/opnormal.jpg"));
        status = tr("正常");
        newItem->setTextColor(Qt::black);
    }
    newItem->setText(status);
}

/**
 * @brief 设置回路电压
 * @param row
 * @param column
 */
void UI_LoopWidget::setLoopVol(int row,int column)
{
    qDebug() << "----------设置电压----------";
    int data = mDataPacket->data->loop->vol->value->get(row)/COM_RATE_VOL;
    
    //    if(data != 0x72) //0x72为标志，表示data未再次赋值
    QString vol = QString::number(data,10);
    
    QTableWidgetItem *newItem=mLoopWidget->item(row,column);
    newItem->setText(vol);
    int max=mDataPacket->data->loop->cur->max->get(row);
    int min=mDataPacket->data->loop->cur->min->get(row);
    
    if(data < min ||data >max)
    {
        newItem->setTextColor(Qt::red);
        QString ip = mDataPacket->ip->get();
        record->setDev(ip,mDataPacket->devNum);  //报警日志
    } else
        newItem->setTextColor(Qt::black);
    
    qDebug() << "----------设置电压----------end";
}

void UI_LoopWidget::setLoopCurrent(int row,int column)
{
    int data = mDataPacket->data->loop->cur->value->get(row)/COM_RATE_CUR;
    
    //    if(data != 0x72) //0x72为标志，表示data未再次赋值
    QString cur = QString::number(data,10);
    
    QTableWidgetItem *newItem=mLoopWidget->item(row,column);
    newItem->setText(cur);
    int max=mDataPacket->data->loop->cur->max->get(row);
    int min=mDataPacket->data->loop->cur->min->get(row);
    int crmax=mDataPacket->data->loop->cur->crMax->get(row);
    int crmin=mDataPacket->data->loop->cur->crMin->get(row);
    
    //如果超过临界值字体为黄色，如果超过最值那么就为红色
    if(data <=crmin && data >min || data >=crmax && data < max)
        newItem->setTextColor(Qt::yellow);
    else if(data < min ||data >max)
    {
        newItem->setTextColor(Qt::red);
        QString ip = mDataPacket->ip->get();
        record->setDev(ip,mDataPacket->devNum);  //报警日志
    } else
        newItem->setTextColor(Qt::black);
}

void UI_LoopWidget::setLoopPower(int row,int column)
{
    int data = mDataPacket->data->loop->pow->get(row)/COM_RATE_POW;
    
    //    if(data != 0x72)
    QString pow = QString::number(data,10,3);
    
    QTableWidgetItem *newItem=mLoopWidget->item(row,column);
    newItem->setText(pow);
}

void UI_LoopWidget::setLoopMinimum(int row,int column)
{
    
    int data = mDataPacket->data->loop->cur->min->get(row)/COM_RATE_CUR;
    QString min = QString::number(data,10);
    QTableWidgetItem *newItem=mLoopWidget->item(row,column);
    newItem->setText(min);
}

void UI_LoopWidget::setLoopDownValue(int row,int column)
{
    
    int data = mDataPacket->data->loop->cur->crMin->get(row)/COM_RATE_CUR;
    QString dowvalue = QString::number(data,10);
    QTableWidgetItem *newItem=mLoopWidget->item(row,column);
    newItem->setText(dowvalue);
    
}

void UI_LoopWidget::setLoopMax(int row,int column)
{    
    
    int data = mDataPacket->data->loop->cur->max->get(row)/COM_RATE_CUR;
    QString max= QString::number(data,10);
    QTableWidgetItem *newItem=mLoopWidget->item(row,column);
    newItem->setText(max);
}

/**
 * @brief 设置回路电压最小值
 * @param row
 * @param column
 */
void UI_LoopWidget::setLoopVolMin(int row,int column)
{
    
    int data = mDataPacket->data->loop->vol->min->get(row)/COM_RATE_VOL;
    QString max= QString::number(data,10);
    QTableWidgetItem *newItem=mLoopWidget->item(row,column);
    newItem->setText(max);
}

/**
 * @brief 设置回路电压最大值
 * @param row
 * @param column
 */
void UI_LoopWidget::setLoopVolMax(int row,int column)
{
    
    int data = mDataPacket->data->loop->vol->max->get(row)/COM_RATE_VOL;
    QString max= QString::number(data,10);
    QTableWidgetItem *newItem=mLoopWidget->item(row,column);
    newItem->setText(max);
}

void UI_LoopWidget::setLoopUpValue(int row,int column)
{
    int data = mDataPacket->data->loop->cur->crMax->get(row)/COM_RATE_CUR;
    QString upvalue = QString::number(data,10);
    QTableWidgetItem *newItem=mLoopWidget->item(row,column);
    newItem->setText(upvalue);
}

bool UI_LoopWidget::isCheckOnline()
{
    if(mDataPacket->offLine > 0)//大于0表示在线，否则离线
        return true;
    else
        return false;
    
    
}

void UI_LoopWidget::test()
{
    mDataPacket=new PduDataPacket;
    mDataPacket->ip->set(QString("192.168.112.112"));
    mDataPacket->offLine = 1;//在线
    for(int i=0;i<6;i++)
    {
        mDataPacket->data->loop->sw->set(i,1);
        mDataPacket->data->loop->cur->value->set(i,10);
        mDataPacket->data->loop->pow->set(i,100);
        mDataPacket->data->loop->cur->min->set(i,1000);
        mDataPacket->data->loop->cur->max->set(i,100);
        mDataPacket->data->loop->cur->crMin->set(i,10);
        mDataPacket->data->loop->cur->crMax->set(i,1);
        
    }
}

void UI_LoopWidget::initTableWidget() //初始化列表
{
    qDebug() << "----------initTableWidget----------";
    mAddressMap.clear(); //如果ip改变，那么重新跟新按钮地址
    int num = mLoopWidget->rowCount();
    
    for(int i=0;i<num;i++)
    {
        mLoopWidget->removeCellWidget(0,9);
        mLoopWidget->removeRow(0);
    }
    
    int rowcount = mLoopNum;
    QStringList list;
    list<<"---"<<"---"<<"---"<<"---"<<"---"<<"---"<<"---"<<"---"<<"---"<<"---";
    for(int i=0;i<rowcount;i++)
        addRowInformation(list);
}

void UI_LoopWidget::item_doubleclicked(QTableWidgetItem *item)
{
    int column=item->column();
    if((column ==8 || column ==5 || column ==6 || column ==7) && (mDataPacket != NULL)) //如果不是阈值设置框，则点击无效
    {
        mDlg=new SetOutputDlg(this);
        int num = item->row();  //回路数不同，发送阈值所到的位数也不同
        QString str = tr("回路名称：");
        
        if(column ==5 || column ==6 )
            mDlg->setName(str,true);
        else
            mDlg->setName(str,false);
        
        str = mLoopWidget->item(item->row(),0)->text();
        mDlg->setMode(num,str,mDataPacket);
        mDlg->exec();
    }
}

void UI_LoopWidget::timeoutDone()
{
    if(mDataPacket && (mDataPacket->offLine >0) && mLoopWidget->rowCount())
        updateData();
    else
        clearTable();
}

void UI_LoopWidget::clearTable()
{
    int num = mLoopWidget->rowCount();
    for(int i=0; i<num; i++)
    {
        mLoopWidget->removeCellWidget(0,8);
        mLoopWidget->removeRow(0);
    }
}

bool UI_LoopWidget::sentSwitchData(int loopNum,int switchData)
{
#if defined(ZEBRA_MODULE) || defined(SNMP_MODULE)
    net_dev_data pkt;

    pkt.addr = mDataPacket->devNum;
    pkt.fn[0]=0x72;  //回路

    bool on = box->isChecked();
    if(on) // 统一设置
        pkt.fn[1] = 0;
    else
        pkt.fn[1] = loopNum + 1;  //确定回路号
    pkt.len = 1;

    uchar data[2] = {0};
    pkt.data = getSwitchData(data,switchData);

    uchar buf[64] = {0};
    int len = net_data_packets(mDataPacket->devType, TRA_TYPR_TCP, &pkt, buf);
    int channel = zebra_client::get_instance()->get_send_cmd_channel(mDataPacket->ip->get());
    if(1 == channel)
    {

        return true;
    }
    else if(2 == channel)
    {
        tcp_queue_append(buf, len);

        zebra_client::get_instance()->send_new_protocol_data(mDataPacket->ip->get() ,buf, len);

        return true;
    }
    else if(3 == channel)
    {
        zebra_client::get_instance()->send_new_protocol_data(mDataPacket->ip->get() ,buf, len);

        return true;
    }
    else if(4 == channel)
    {

        return true;
    }
    else if(5 == channel)
    {
        tcp_queue_append(buf, len);

        on = box->isChecked();
        if(on) // 广播数据包
        {
            len = net_data_packets(mDataPacket->devType, TRA_TYPR_UDP, &pkt, buf);
            udp_queue_append(buf, len);
        }

        QString str;
        bool ret = get_tcp_connect(mDataPacket->ip->get());
        if(ret)
            str = tr("开关操作成功!");
        else
            str = tr("开关操作失败!");
        InfoMsgBox pbox(NULL,str + "\n");

        return ret;
    }
    else
    {
        qDebug() << "UI_LoopWidget::sentSwitchData() channel error = " << channel;

        return false;
    }

#else
    net_dev_data pkt;
    
    pkt.addr = mDataPacket->devNum;
    pkt.fn[0]=0x72;  //回路
    
    bool on = box->isChecked();
    if(on) // 统一设置
        pkt.fn[1] = 0;
    else
        pkt.fn[1] = loopNum + 1;  //确定回路号
    pkt.len = 1;
    
    uchar data[2] = {0};
    pkt.data = getSwitchData(data,switchData);
    
    uchar buf[64] = {0};
    int len = net_data_packets(mDataPacket->devType, TRA_TYPR_TCP, &pkt, buf);
    tcp_queue_append(buf, len);
    
    on = box->isChecked();
    if(on) // 广播数据包
    {
        len = net_data_packets(mDataPacket->devType, TRA_TYPR_UDP, &pkt, buf);
        udp_queue_append(buf, len);
    }
    
    QString str;
    bool ret = get_tcp_connect();
    if(ret)
        str = tr("开关操作成功!");
    else
        str = tr("开关操作失败!");
    InfoMsgBox pbox(NULL,str + "\n");
    
    return ret;
#endif
}

/**
 * @brief 获取开关值
 * @return
 */
uchar *UI_LoopWidget::getSwitchData(uchar *buf,int switchData)
{
    int offset=0;
    buf[offset] = switchData & 0xFF;
    return buf;
}





