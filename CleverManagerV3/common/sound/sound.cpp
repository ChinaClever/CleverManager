/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "sound.h"
#include <QSound>
#include <QTimer>

Sound::Sound(QObject *parent) : QObject(parent)
{
    isAllow = isRun = false;
}

Sound *Sound::bulid(QObject *parent)
{
    static Sound* sington = nullptr;
    if(sington == nullptr)
        sington = new Sound(parent);
    return sington;
}


bool Sound::check()
{
    bool ret = false;
    if(!isRun && isAllow) {
        ret = isRun = true;
        QTimer::singleShot(10*1000,this,SLOT(workSLot()));
    }

    return ret;
}

void Sound::play()
{
    if(check()) {
        QSound::play(":/alarm");
    }
}

void Sound::workSLot()
{
    isRun = false;
}
