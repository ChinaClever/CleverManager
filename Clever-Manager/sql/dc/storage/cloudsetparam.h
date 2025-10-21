#ifndef CLOUDSETPARAM_H
#define CLOUDSETPARAM_H
#include "../mysql/dc_modules.h"

class CloudSetParam
{
    CloudSetParam();
public:
    static CloudSetParam *bulid();

    DC_CompanyItem  company;
    DC_IdcItem  idc;
    DC_ModuleItem module;

    void saveModule();
    void saveCompany();
    void saveIdc();
    void saveAll();

protected:
    void readFile();
};

#endif // CLOUDSETPARAM_H
