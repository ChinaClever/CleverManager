#ifndef SERIALDEV_H
#define SERIALDEV_H

#include <QtCore>
#include <QSerialPort>

class SerialDev : public QThread
{
    Q_OBJECT
public:
    explicit SerialDev(QObject *parent = 0);
    ~SerialDev();

    QString portName(void);
    bool open(QString &name,qint32 baudRate = QSerialPort::Baud115200);
    int read(QByteArray &);
    int write(QByteArray &);
    bool close(void);
    void test(void);

signals:
    void readyReadSig();

public slots:
    void serialReadSlot();

private:
    bool isOpen;
    QSerialPort  *mSerial;

    QReadWriteLock  mRwLock;
    QByteArray mSerialData;
};

#endif // SERIALDEV_H
