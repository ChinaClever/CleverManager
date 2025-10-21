#ifndef DC_LOGINDEX_H
#define DC_LOGINDEX_H
#include "basicmysql.h"
struct DC_LogIndexItem
{
    DC_LogIndexItem():id(-1),item_type (0),name(" "){}
    int id, item_type;
    QString name ;
    QDateTime create_time, update_time;
};
Q_DECLARE_METATYPE(DC_LogIndexItem)

class DC_LogIndex : public BasicMySql<DC_LogIndexItem>
{
    Q_OBJECT
    explicit DC_LogIndex();
public:
    static DC_LogIndex* bulid();

    QString tableName(){return QString("%1_%2").arg(TB_PREFIX).arg("log_index") ;}
    bool insertItem(const DC_LogIndexItem& item); // 插入
    bool updateItem(const DC_LogIndexItem& item); // 修改

    bool containType(int type); // 类型是否存在
    bool setTypeName(int type, const QString& name); // 修改状态名称
    QString getTypeName(int type); // 获取类型名称
    QVector<int> listTypes();

protected:
   bool modifyItem(const DC_LogIndexItem& item,const QString& cmd);
   void selectItem(QSqlQuery &query,DC_LogIndexItem &item);
};

#endif // DC_LOGINDEX_H
