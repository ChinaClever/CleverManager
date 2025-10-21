#ifndef DBPDUTYPELIST_H
#define DBPDUTYPELIST_H

#include "basicsql.h"

struct PduTypeItem{
    PduTypeItem():id(-1){}
    int id;
    QString des,oid;
};
Q_DECLARE_METATYPE(PduTypeItem)

class DbPduTypeList : public BasicSql1<PduTypeItem>
{
    Q_OBJECT
    explicit DbPduTypeList(QObject *parent = 0);
public:
    static DbPduTypeList *get();
    QString tableName(){return "pdu_types";}
    void insertItem(const PduTypeItem& item);
    void updateItem(const PduTypeItem& item);
    void removeItem(const PduTypeItem &item);
    QVector<PduTypeItem> selectItemsByOid(const QString &oid);
protected:
    void modifyItem(const PduTypeItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,PduTypeItem &item);
};

#endif // DBPDUTYPELIST_H
