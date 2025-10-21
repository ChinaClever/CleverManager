#ifndef PDU_ALARMMUSICRUN_H
#define PDU_ALARMMUSICRUN_H
#include<QThread>
#include<QObject>
#include<QtCore>
#include"pdu_alarmdev.h"
#include"pdu_alarmusic.h"
#include<QMediaPlaylist>
#include<listShow/ls_rightheadwid.h>



class PDU_AlarmMusicRun : public QThread
{
      Q_OBJECT

public:
    PDU_AlarmMusicRun(QObject *parent = 0);
    ~PDU_AlarmMusicRun();


private slots:
    void timeoutDone();
    void alarmPlaySlot();

protected:
    void checkAlarm();
    void initData();
    void run(void);
    void getAlarmInformation();
    void compoundMusic();
    void getMusicPath(QString str);//将字符串形式的ip转化成一个一个的


private:
    bool is_run;
    alarmDevHash *mAlarmDevHash;//保存报警hash表
    Pdu_Alarmusic *mMusic;
//    QMediaPlaylist *mPlayList;
    QList<QString> list;



};

#endif // PDU_ALARMMUSICRUN_H
