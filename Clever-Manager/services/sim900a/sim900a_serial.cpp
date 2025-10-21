/*
 * sim900a_err.cpp
 * SIM900串口管理类
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "sim900a_serial.h"

bool g_sim_isOpen=false; // 全局变量串口是否打开
SerialDev *sim_serial=NULL; /*唯一SIM一个串口对像*/

SIM900A_Serial::SIM900A_Serial()
{
    if(sim_serial == NULL)
        sim_serial = new SerialDev();
     mSerial = sim_serial;

   // if(!g_sim_isOpen)
   //     sim_serial_open();
}

SIM900A_Serial::~SIM900A_Serial()
{

}

bool SIM900A_Serial::sim_serial_port(QString &com)
{
    bool ret = false;

    Sys_ConfigFile_Open();
    com = Sys_ConfigFile_ReadStr("SIM900_COM");
    if(!com.isEmpty())
    {
        SerialPort port;
        ret = port.isContains(com);
    }
    Sys_ConfigFile_Close();

    return ret;
}

/**
 * @brief 打开串口号
 * @param com
 * @return
 */
bool SIM900A_Serial::sim_serial_open(QString &com)
{
    static QString sim_com;

    if(sim_com != com)
    {
        sim_com = com;
        Sys_ConfigFile_WriteParam("SIM900_COM", sim_com);
    }

    bool ret = false;
    if(sim_serial)
    {
        sim_serial_close();
        ret = mSerial->open(com);
    }
    g_sim_isOpen =  ret;

    return ret;
}

bool SIM900A_Serial::sim_serial_open(void)
{
    QString com;
    bool ret = sim_serial_port(com);
    if(ret)
        ret = sim_serial_open(com);

    return ret;
}


void SIM900A_Serial::sim_serial_close(void)
{
    if(mSerial)
    {
        mSerial->close();
        g_sim_isOpen = false;
    }
}
