#ifndef SOUND_H
#define SOUND_H

#include <QObject>

class Sound : public QObject
{
    Q_OBJECT
    explicit Sound(QObject *parent = nullptr);
public:
    static Sound* bulid(QObject *parent = nullptr);

    void play();
    void setAllow(bool en) {isAllow=en;}

protected:
    bool check();

protected slots:
    void workSLot();

private:
    bool isRun;
    bool isAllow;
};

#endif // SOUND_H
