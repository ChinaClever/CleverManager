#ifndef SIM900A_SERIAL_H
#define SIM900A_SERIAL_H

#include "services/config/sysconfigfile.h"
#include "services/serial/serialport.h"

class SIM900A_Serial
{
public:
    SIM900A_Serial();
    ~SIM900A_Serial();

    bool sim_serial_open(QString &);
    bool sim_serial_open(void);
    void sim_serial_close(void);

    bool sim_serial_port(QString &);

private:
    SerialDev *mSerial;
};

extern bool g_sim_isOpen;
extern SerialDev *sim_serial;

#endif // SIM900A_SERIAL_H
