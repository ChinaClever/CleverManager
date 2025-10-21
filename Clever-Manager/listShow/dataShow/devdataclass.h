#ifndef DEVDATACLASS_H
#define DEVDATACLASS_H
#include "dataread.h"

class DevDataClass: public DataRead
{
public:
    DevDataClass();
    ~DevDataClass();

    int readMasterNum(QString &);

    int readVoltage(QString &,short ,short );
    int readVoltage(QString &,short );
    int readVoltage(short);
    int readVoltage(void);

    int readCurrent(QString &,short ,short );
    int readCurrent(short);
    int readAllCurrent(QString &,short );
    int readAllCurrent(void);

    int readWatt(QString &,short ,short );
    int readWatt(short);
    int readAllWatt(QString &,short );
    int readAllWatt(void);

    int readEnergy(QString &,short ,short );
    int readEnergy(short);
    int readAllEnergy(QString &,short );
    int readAllEnergy(void);

    float readTemp(QString &,short ,short );
    float readTemp(short);

    float readHum(QString &,short ,short );
    float readHum(short);

    int limitVoltageMin(QString &,short,short);
    int limitVoltageMin(short id);

    int limitVoltageMax(QString &,short,short);
    int limitVoltageMax(short);

    int limitCurrentMin(QString &,short,short);

    int limitCurrentMax(QString &,short,short);
    int limitCurrentMax(short);

    int limitTempMin(QString &,short,short);
    int limitTempMin(short);

    int limitTempMax(QString &,short,short);
    int limitTempMax(short);

    int limitHumMin(QString &,short,short);
    int limitHumMin(short);

    int limitHumMax(QString &,short,short);
    int limitHumMax(short);

    int isAlarm(QString &,short);
    bool isAlarm(QString &);

    int alarmDevNum(QString &);
    int alarmLine(short &);
    int alarmLine(void);

private:
    Data_devDataType devdata;
};

#endif // DEVDATACLASS_H
