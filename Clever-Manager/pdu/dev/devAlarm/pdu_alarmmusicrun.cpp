#include "pdu_alarmmusicrun.h"
#include<QHashIterator>


/** 设置定时器，每隔一段时间启动一次线程
 * @brief PDU_AlarmMusicRun::PDU_AlarmMusicRun
 */
PDU_AlarmMusicRun::PDU_AlarmMusicRun(QObject *parent) : QThread(parent)
{
    initData();
    QTimer *mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(timeoutDone()));
    //mTimer->start(5*1000);
}

PDU_AlarmMusicRun::~PDU_AlarmMusicRun()
{
    wait();
}


/**初始化一些数据
 * @brief PDU_AlarmMusicRun::initData
 */
void PDU_AlarmMusicRun::initData()
{
    is_run=false;
}

void PDU_AlarmMusicRun::run(void)
{
    checkAlarm();
    is_run=false;
}

void PDU_AlarmMusicRun::alarmPlaySlot()
{
    checkAlarm();
    is_run=false;
}

/**检查是否有报警信息，如果有，就报警
 * @brief PDU_AlarmMusicRun::checkAlarm
 */
void PDU_AlarmMusicRun::checkAlarm()
{
    getAlarmInformation();

}

void PDU_AlarmMusicRun::timeoutDone()
{
    if(!is_run)
    {
        is_run=true;
        start();
    }
}


/**获取报警信息
 * @brief PDU_AlarmMusicRun::getAlarmInformation
 */
void PDU_AlarmMusicRun::getAlarmInformation()
{
    QString path="music/ALARM1.wav";
    mAlarmDevHash=getAlarmHash();
    if(mAlarmDevHash->size() > 0 && !getAlarmSignal())  //如果报警列表不为0，则语音报警
    {
        mMusic=new Pdu_Alarmusic;
        mMusic->playMusic(path); //单曲播放

    }else if(mAlarmDevHash->size() > 0 && getAlarmSignal()) //如果有报警设备，且在扫描之前即铃声响起之前关闭报警，可能导致崩溃
    {
        mMusic->stopMusic();
    }
}

/**根据mAlarmDevHash合成播放列表
 * @brief PDU_AlarmMusicRun::compoundMusic
 */
void PDU_AlarmMusicRun::compoundMusic()
{
    QString ipstr="292.168.1.112";
    getMusicPath(ipstr);
}

/**如”192.168.1.112“转化成”1“等再分别调用文件,将歌曲加入歌单
 * @brief PDU_AlarmMusicRun::ipToPath
 * @return
 */
void PDU_AlarmMusicRun::getMusicPath(QString str)
{
    QString numstr,pathstr;
    for(int i=0;i<str.length();i++)
    {
        numstr[0]=str.at(i);
        pathstr="F:/KuGou/"+numstr+".wav";
       list<<pathstr;
    }
}
