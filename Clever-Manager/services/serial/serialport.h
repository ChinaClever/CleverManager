#ifndef SERIALPORT_H
#define SERIALPORT_H

#include "serialdev.h"
#include <QSerialPortInfo>

class SerialPort : public QObject
{
    Q_OBJECT
public:
    explicit SerialPort(QObject *parent = 0);
    ~SerialPort();

    QStringList &portsList(void);
    bool isContains(QString &);
protected:
    void initSeialPort();

signals:

public slots:

private:
    QStringList mPortNameList;
};

#endif // SERIALPORT_H
