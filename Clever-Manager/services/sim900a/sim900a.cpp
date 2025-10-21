/*
 * sim900a.cpp
 * 短信模块基本操作
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "sim900a.h"

QMutex g_sim_mutex; // 锁防止 同时操作SIM

SIM900A::SIM900A(QObject *parent) : QObject(parent)
{
    mSerial = sim_serial;
}

SIM900A::~SIM900A()
{

}


/**
 * @brief 发送字符串
 * @param str
 */
void SIM900A::serialSent(QString &str)
{
    QByteArray array;
    mSerial->write(array.append(str));
}

/**
 * @brief 发送数据
 * @param data
 */
void SIM900A::serialSent(uchar data)
{
    QByteArray array;
    mSerial->write(array.append(data));
}

/**
 * @brief 接收字符串
 * @param str
 * @return
 */
bool SIM900A::serialRecv(QString &str)
{
    QByteArray ackByte;
    int len = mSerial->read(ackByte);
    if(len > 0)
        str = ackByte;
    else
        return false;

    return true;
}


//sim900a发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:false,没有得到期待的应答结果
bool SIM900A::sim900a_check_cmd(QString str)
{
    bool ret = serialRecv(mAckStr);
    if(ret)
    {
        if(mAckStr.contains(str))
            return true;
    }

    return false;
}


//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:true,发送成功(得到了期待的应答结果)
//       false,发送失败
bool SIM900A::sim900a_check_ack(QString ack, short waittime)
{
    waittime /= 20;
    if(!ack.isEmpty() && (waittime>0))		//需要等待应答
    {
        while(--waittime)	//等待倒计时
        {
            if(sim900a_check_cmd(ack))
                break;//得到有效数据
        }

        if(waittime==0)
            return false;
    }

    return true;
}

//向sim900a发送命令
//cmd:发送的命令字符串 需要添加回车
bool SIM900A::sim900a_send_cmd(QString cmd,QString ack, short waittime)
{
    serialSent(cmd.append("\r\n")); //发送命令

    return sim900a_check_ack(ack,waittime);
}

//向sim900a发送命令
//cmd:发送数字(比如发送0X1A) 不需要添加回车了
bool SIM900A::sim900a_send_cmd(uchar cmd,QString ack, short waittime)
{
    serialSent(cmd); //发送命令

    return sim900a_check_ack(ack,waittime);
}

/**
  * @brief 检测是否应答AT指令
  * @return false 未检测到模块
  */
bool SIM900A::sim900a_AT_cmd(void)
{
    bool ret = sim900a_send_cmd("AT","OK",100);
    if(ret)
        sim900a_send_cmd("ATE0","OK",200);//不回显

    return ret;
}

//模块信息
bool SIM900A::sim900a_modu_info(SIM_ModuInfo &info)
{
    QString *str = &mAckStr;
    bool ret = sim900a_send_cmd("AT+CGMI","OK",200); //查询制造商名称
    if(ret)
    {
        str->remove(0,2);
        int index = str->indexOf("\r\n");
        info.manufacturer = str->left(index); //制造商
    }

    ret = sim900a_send_cmd("AT+CGMM","OK",200); //查询模块名字
    if(ret)
    {
        str->remove(0,2);
        int index = str->indexOf("\r\n");
        info.modular = str->left(index); //模块型号
    }

    ret = sim900a_send_cmd("AT+CGSN","OK",200); //查询产品序列号
    if(ret)
    {
        str->remove(0,2);
        int index = str->indexOf("\r\n");
        info.serialNumber = str->left(index); //序列号
    }

    ret = sim900a_send_cmd("AT+CNUM","+CNUM",200); //查询本机号码
    if(ret)
    {
        int index = str->indexOf(",");
        str->remove(0,index+2);
        index = str->indexOf("\"");
        info.localNumbers = str->left(index); //本机号码
    }

    return true;
}

/**
  * @brief 查询SIM卡是否在位
  * @return
  */
bool SIM900A::simp00a_cpin(void)
{
    return sim900a_send_cmd("AT+CPIN?","OK",200);
}

//GSM信息显示(信号质量,电池电量,日期时间)
//返回值:0,正常
//    其他,错误代码
bool SIM900A::sim900a_gsm_info(SIM_gsmInfo &info)
{
    info.flag = simp00a_cpin(); //查询SIM卡是否在位
    if(!info.flag)
        return false;

    QString *str = &mAckStr;
    bool ret = sim900a_send_cmd("AT+COPS?","OK",200); //查询运营商名字
    if(ret)
    {
        int index = str->indexOf("\"");
        if(index > 0)
        {
            str->remove(0,index+1);
            index = str->indexOf("\"");
            info.operators = str->left(index); //运营商
        }
    }

    ret = sim900a_send_cmd("AT+CSQ","+CSQ:",200); //查询信号质量
    if(ret)
    {
        int index = str->indexOf(":");
        if(index > 0)
        {
            str->remove(0,index);
            index = str->indexOf(",");
            info.signalQuality = str->left(index); //信号质量
        }
    }

    ret = sim900a_send_cmd("AT+CBC","+CBC:",200); //查询电池电量
    if(ret)
    {
        int index = str->indexOf(",");
        if(index > 0)
        {
            str->remove(0,index+1);
            index = str->indexOf(",");
            QString battery = str->left(index);

            str->remove(0,index+1);
            info.batteryPower = QObject::tr("电池电量:") + battery +"% "+ str->left(2) +"mV";
        }
    }

    ret = sim900a_send_cmd("AT+CCLK?","+CCLK:",200); // 日期
    if(ret)
    {
        int index = str->indexOf("\"");
        if(index > 0)
        {
            str->remove(0,index+1);
            index = str->indexOf(":");
            info.dateTime = QObject::tr("日期时间:") + str->left(index+3);
        }
    }

    return true;
}

/**
  * @brief 查询运营商名字
  * @return
  */
bool SIM900A::simp900a_cops(void)
{
    return sim900a_send_cmd("AT+COPS?","OK",200);//查询运营商名字
}

bool SIM900A::sim900a_call_init(void)
{
    bool ret = simp900a_cops(); //查询运营商名字
    if(ret)
    {
        ret = sim900a_send_cmd("AT+CLIP=1","OK",200); //设置来电显示
        if(ret)
            sim900a_send_cmd("AT+COLP=1","OK",200);	//设置被叫号码显示
    }
    else
        qDebug() << " COPS err";

    return ret;
}


/**
 * @brief 接听电话
 * @return
 */
bool SIM900A::sim900a_call_Connect(void)
{
    return sim900a_send_cmd("ATH","OK",200);//不管有没有在通话,都结束通话
}

/**
 * @brief 挂断电话
 * @return
 */
bool SIM900A::sim900a_call_hangUp(void)
{
    return sim900a_send_cmd("ATH","OK",0);//不管有没有在通话,都结束通话
}

/**
 * @brief 接电话
 * @param pohneNum
 * @return
 */
bool SIM900A::sim900a_answer_Phone(QString &pohneNum)
{
    bool ret = sim900a_check_cmd("+CLIP:"); //接收到来电
    if(ret)
    {
        QString *str = &mAckStr;
        str->remove(0,8);
        int index = str->indexOf("\"");
        pohneNum = str->left(index);
    }

    return ret;
}

/**
 * @brief 打电话
 * @param pohneNum
 * @return
 */
bool SIM900A::sim900a_call_Phone(QString &pohneNum)
{
    bool ret = sim900a_call_init();
    if(ret)
    {
        QString pohneNumCmd = "ATD" + pohneNum +";";
        sim900a_send_cmd(pohneNumCmd,"",0);

        do
        {
            QString ackStr;
            ret = serialRecv(ackStr);
            if(ret)
            {
                if(ackStr.contains("+COLP:")) //拨号成功
                    return true;

                if(ackStr.contains("NO CARRIER")) //对方挂断拨号失败
                    return false;
                if(ackStr.contains("NO ANSWER")) //拨号失败
                    return false;
                if(ackStr.contains("ERROR")) //拨号失败
                    return false;
            }
        } while(ret==false);
    }

    return ret;
}

/**
 * @brief 短信初始化
 * @return
 */
bool SIM900A::sim900a_sms_init(void)
{
    bool ret = simp900a_cops(); //查询运营商名字
    if(ret)
    {
        ret = sim900a_send_cmd("AT+CMGF=1","OK",200);//设置文本模式
        if(ret)
        {
            ret = sim900a_send_cmd("AT+CSCS=\"UCS2\"","OK",200); 	//设置TE字符集为UCS2
            if(ret)
                sim900a_send_cmd("AT+CSMP=17,0,2,25","OK",200);	//设置短消息文本模式参数

            //sim900a_send_cmd("AT+CNMI=2,1","OK",200); //新信息提示
        }
    }

    return ret;
}

/**
 * @brief 消息存储状态
 * @param msgNum 消息条数
 * @param msgMaxNum 消息最大条数
 * @return
 */
bool SIM900A::sim900a_sms_count(short &msgNum, short &msgMaxNum)
{
    QString *str = &mAckStr;

    bool ret = sim900a_send_cmd("AT+CPMS?","+CPMS:",200); //查询优选消息存储器
    if(ret)
    {
        int index = str->indexOf(",");
        str->remove(0,index+1);

        index = str->indexOf(",");
        msgNum = str->left(index).toInt();  //获取储短信条数

        str->remove(0,index+1);
        index = str->indexOf(",");
        msgMaxNum = str->left(index).toInt(); //获取最大存储短信条数
    }

    return ret;
}

/**
 * @brief 编码转化
 * @param str
 */
void SIM900A::sim900a_unigbk_GBK(QString &str)
{
    QString t = str;
    QStringList list;

    for(int i = 0;i < t.length();i += 4) {
        list.append(t.mid(i,4));
    }

    QString t1;
    foreach (const QString &t, list) {
        t1.append(t.toUShort(0,16));
    }

    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    str = codec->fromUnicode(t1);
}

/**
 * @brief 读取短信
 * @param num
 * @param msg
 * @return
 */
bool SIM900A::sim900a_sms_read(short num,SIM_smsInfo *msg)
{
    bool ret = sim900a_sms_init();
    if(!ret)
        return ret;

    QString msgNumCmd = "AT+CMGR=" + QString::number(num);
    ret = sim900a_send_cmd(msgNumCmd,"+CMGR:",600);
    if(ret)
    {
        QString *str = &mAckStr;
        msg->flag = str->contains("UNREAD"); //是否已读

        int index = str->indexOf(",");
        str->remove(0,index+2);

        index = str->indexOf("\"");
        msg->pohneNum = str->left(index); //发送人
        sim900a_unigbk_GBK(msg->pohneNum);

        index = str->indexOf("/");
        str->remove(0,index-2);

        index = str->indexOf("+");
        msg->dateTime = str->left(index); //时间

        index = str->indexOf("\r");
        str->remove(0,index+2);
        index = str->indexOf("\r");
        msg->msg = str->left(index); //信息内容
        sim900a_unigbk_GBK(msg->msg);
    }

    return ret;
}

// 转换为unicode字符串
void SIM900A::sim900a_unigbk_exchange(QString str, QString &unicode_str)
{
    unicode_str.clear();

    for(int i = 0;i <str.length();++i) {
        unicode_str += QString("%1").arg(str[i].unicode(), 4, 16, QChar('0'));
    }
}

/**
 * @brief sim900a_sms_send
 * @return
 */
bool SIM900A::sim900a_sms_send(SIM_smsInfo *msg)
{
    bool ret = sim900a_sms_init();
    if(!ret)
        return ret;

    QString unicode_str;
    sim900a_unigbk_exchange(msg->pohneNum,unicode_str); //将电话号码转换为unicode字符串

    QString cmd = "AT+CMGS=\"" + unicode_str + "\"";
    ret = sim900a_send_cmd(cmd,">",200); //发送短信命令+电话号码
    if(ret)
    {
        sim900a_unigbk_exchange(msg->msg,unicode_str); //将短信内容转换为unicode字符串.
        serialSent(unicode_str); //发送短信内容到GSM模块

        ret = sim900a_send_cmd((uchar)0X1A,"+CMGS:",8000);//发送结束符,等待发送完成(最长等待10秒钟,因为短信长了的话,等待时间会长一些)
    }
    msg->flag = ret;

    QDateTime time = QDateTime::currentDateTime();
    msg->dateTime = time.toString("yyyy/MM/dd,hh:mm:ss");

    return ret;
}

/**
  * @brief 命令 AT+CMGD=? -
        参数的详细说明
        <index> 1～255 整数型；关联存储器支持的地址编号范围内的取值
        <delflag>
        - 删除<index>指定的短信
        0 删除<index>指定的短信
        1 全部删除存储器中的已读短信
        2 全部删除存储器中的已读和已发送短信
        3 全部删除存储器中的已读、已发送和未发送短信
        4 全部删除存储器中的已读、未读、已发送和未发送短信

  * @return
  */
bool SIM900A::sim900a_sms_del(short index,short delflag)
{
    QString cmd = "AT+CMGD=" + QString::number(index)+"," + QString::number(delflag);
    return sim900a_send_cmd(cmd,"OK",200);
}


void SIM900A::test(void)
{
    bool ret = sim900a_AT_cmd();
    if(ret)
    {
        SIM_smsInfo msg;

        /*msg.pohneNum = "18576689472";
        msg.msg = "这是测试短信 123456789";
       bool  ret = sim900a_sms_send(msg);
        qDebug() << ret;


         short msgNum,msgMaxNum;
         sim900a_sms_count(msgNum,msgMaxNum);

         qDebug() << ret << msgNum << msgMaxNum;

         sim900a_sms_read(msgNum,msg);
         qDebug() << msg.pohneNum << msg.msg;
         */

    }
}
