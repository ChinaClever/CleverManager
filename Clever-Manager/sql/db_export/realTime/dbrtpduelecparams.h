#ifndef DBRTPDUELECPARAMS_H
#define DBRTPDUELECPARAMS_H
#include "db_export/records/dbpduelecparams.h"

class DbRtPduElecParams: public DbPduElecParams
{
     Q_OBJECT
    DbRtPduElecParams();
public:
    static DbRtPduElecParams* get();
    virtual QString tableName(){return "rtPduElecParams";}
    bool updateItem(PduElecParamsItem &item);
    int getId(int rid, int type, int index);

};

#endif // DBRTPDUELECPARAMS_H
