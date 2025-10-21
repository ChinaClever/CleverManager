#ifndef PDU_ALARMUSIC_H
#define PDU_ALARMUSIC_H
#include<qmediaplayer.h>
#include<qmediaplaylist.h>


class Pdu_Alarmusic
{
public:
    Pdu_Alarmusic();

    void playMusic(QList<QString> &list);//播放音乐
    void playMusic(QString &path);
    void stopMusic();


private:
    QMediaPlayer *mPlayer;
    QMediaPlaylist *musiclist;

};

#endif // PDU_ALARMUSIC_H
