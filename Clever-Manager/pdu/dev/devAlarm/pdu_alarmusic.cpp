#include "pdu_alarmusic.h"

Pdu_Alarmusic::Pdu_Alarmusic()
{
    mPlayer=new QMediaPlayer;
    mPlayer->setVolume(80);
    musiclist=new QMediaPlaylist;
}


void Pdu_Alarmusic::playMusic(QList<QString> &list)
{
    QListIterator<QString> i(list);
    for(;i.hasNext();)
    {
        musiclist->addMedia(QUrl::fromLocalFile(i.next()));
    }

    mPlayer->setPlaylist(musiclist);
    mPlayer->play();
}

void Pdu_Alarmusic::stopMusic()
{
    mPlayer->pause();
}

void Pdu_Alarmusic::playMusic(QString &path)
{
    mPlayer->setMedia(QUrl::fromLocalFile(path));
    mPlayer->play();
}


