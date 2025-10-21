#ifndef DBPDUELECPARAMS_H
#define DBPDUELECPARAMS_H
#include "database/basicsql.h"

// 电气参数表
struct PduElecParamsItem{
    PduElecParamsItem():id(-1),rid(-1),type(-1),index(-1),
        vol(-1),cur(-1),pow(-1),ele(-1),pf(-1),sw(-1){}

    int id, rid, type, index; // (类型 1:输入相  2：输出位)   
    double vol, cur, pow, ele,pf;
    int sw;
    QString typeStr, swStr;
};
Q_DECLARE_METATYPE(PduElecParamsItem)

class DbPduElecParams: public BasicSql1<PduElecParamsItem>
{
    Q_OBJECT
public:
    DbPduElecParams();
     static DbPduElecParams* get();
    virtual QString tableName(){return "pduElecParams";}
    bool insertItem(PduElecParamsItem &item);


    QVector<PduElecParamsItem> selItemsByRid(int rid);
    QVector<PduElecParamsItem> query(const QString& condition);
    void removeItemsByRid(int rid);

protected:
    void createTable();
    bool modifyItem(PduElecParamsItem &item, const QString& cmd);
    void selectItem(QSqlQuery &query,PduElecParamsItem &item);

private:
    QString getTypeStr(int type);
    QString getSwStr(int sw);

};

#endif // DBPDUELECPARAMS_H
