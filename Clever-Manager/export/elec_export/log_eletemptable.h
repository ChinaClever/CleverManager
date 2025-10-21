#ifndef LOG_ELETEMPTABLE_H
#define LOG_ELETEMPTABLE_H

#include "sql/database/basicsql.h"
struct EleTempItem{
    EleTempItem():id(-1),startEle(0), endEle(0), ele(0){}

    int id;
    QString name;
    QString startTime;
    double startEle;

    QString endTime;
    double endEle;
    double ele;
};
Q_DECLARE_METATYPE(EleTempItem)

#define LOG_TEMP_ELE    "log_temp_ele"

class Log_EleTempTable : public BasicSql1<EleTempItem>
{
    Q_OBJECT
public:
    explicit Log_EleTempTable(QObject *parent = 0);

    static Log_EleTempTable* get();
    virtual QString tableName(){return LOG_TEMP_ELE;}
    void insertItem(EleTempItem &item);

    void clear();

protected:
    void createTable();
    bool modifyItem(const EleTempItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,EleTempItem &item);

};

#endif // LOG_ELETEMPTABLE_H
