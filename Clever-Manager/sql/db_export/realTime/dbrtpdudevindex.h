#ifndef DBRTPDUDEVINDEX_H
#define DBRTPDUDEVINDEX_H
#include "db_export/records/dbpdudevindex.h"

class DbRtPduDevIndex: public DbPduDevIndex
{
    Q_OBJECT
    DbRtPduDevIndex();
public:
    static DbRtPduDevIndex* get();
    virtual QString tableName(){return "rtPduDevIndex";}
    void initTime(PduDevIndexItem &item);
    bool updateItem(PduDevIndexItem &item);
    int getId(QString &ip, int devNum, int devType);
};

#endif // DBRTPDUDEVINDEX_H
