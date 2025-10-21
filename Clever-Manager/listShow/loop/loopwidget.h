#ifndef UI_LOOPWIDGET_H
#define UI_LOOPWIDGET_H

#include <QWidget>
#include<QTableWidget>
#include<QVBoxLayout>
#include<common.h>
#include<QHeaderView>
#include<QListWidgetItem>
#include<QHBoxLayout>
#include<QMultiMap>
#include"pdu/data/packet/pdudatapacket.h"
#include "pdu/data/hash/read/pduhashread.h"
#include"listShow/devSetting/setoutputdlg.h"
#include<QTimer>
#include<pdu/dev/devAlarm/pdu_alarmrecord.h>
#include<QLabel>
#include<QCheckBox>
#include"pdu/dev/devType/pdudtname.h"

class UI_LoopWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UI_LoopWidget(QWidget *parent = 0);

    void getDataPacket(QString &ip, int num);//根据ip获取相应的回路数据包,即切换IP时

signals:

public slots:
    void button_clicked();//按钮点击
    void item_doubleclicked(QTableWidgetItem *item);
    void timeoutDone();
    //回路	空开状态	回路电流	回路功率	最小值(A) 下限界(A) 上限界(A)  最大值(A)  开关设置
protected:
    void initWidget();
    void initLayout();
    void initTalbeWidget();//初始化表格窗口，确定有几个回路
    void additem(int row,int column,QString content); //针对某一个单元格显示数据
    void addRowInformation(QStringList list);//如果已知某一回路信息，那么将该回路信息显示出来

    void setLoopName(int row,int column);
    void setSwitchStatus(int row,int column);
    void setLoopVol(int row,int column);
    void setLoopCurrent(int row,int column);
    void setLoopPower(int row,int column);
    void setLoopMinimum(int row,int column);
    void setLoopDownValue(int row,int column);
    void setLoopMax(int row,int column);
    void setLoopVolMin(int row,int column);
    void setLoopVolMax(int row,int column);
    void setLoopUpValue(int row,int column);

    void updateData(); //更新数据
    bool isCheckOnline(); //检查是否在线
    //    void getDataPacket(QString &ip, int num);//根据ip获取相应的回路数据包,即切换IP时
    void test();//填充数据包，把数据显示到界面
    void initTableWidget();  //确定表格的总行数、总列数
    void clearTable();//当设备离线时，清理表格内容
    bool sentSwitchData(int loopNum,int switchData);
    uchar *getSwitchData(uchar *buf, int switchData);



protected:
    QTableWidget *mLoopWidget;
    QVBoxLayout *mLayOut;
    QStringList horhead,verhead;
    bool is_on;//按钮标志位打开关闭
    QPushButton *button_on;
    QPushButton *button_off;
    long int mAddress[12];
    QMultiMap<int,QPushButton *> mAddressMap;
    int mCurrentButtonRow;//当前点击按钮所在行号
    PduDataPacket *mDataPacket;
    SetOutputDlg *mDlg;
    QTimer *mTimer;
    PDU_AlarmRecord *record;
    QCheckBox *box;
    int mLoopNum;
};

#endif // UI_LOOPWIDGET_H
