#ifndef DC_BASICMYSQL_H
#define DC_BASICMYSQL_H
#include "basicmysql.h"

template <typename T>
class DC_BasicMySql:public BasicMySql<T>
{
public:
    int countName(int rid) {return count("name", QString("where %1 = %2 ").arg(ridName()).arg(rid));}

    bool containRid(int rid) {
        int ret = count(ridName(), QString("where %1 = %2").arg(ridName()).arg(rid));
        return (ret > 0) ? true:false;
    }

    bool containName(int rid, const QString& name) {
        int ret = count("id", QString("where %1 = %2 and name = \"%3\"").arg(ridName()).arg(rid).arg(name));
        return (ret > 0) ? true:false;
    }

    QStringList listName(int rid) {return listColumn("name", QString("where %1 = %2").arg(ridName()).arg(rid));}

    T findByName(int rid, const QString& name) { // 根据IDC索引ID查找
        QVector<T> items = selItemsByName(rid, name);
        return items.first();
    }

    QVector<T> selItemsByName(int rid, const QString& name) {
        return selectItems(QString("where %1 = %2 and name = \"%3\"").arg(ridName()).arg(rid).arg(name));
    }

    int getId(int rid, const QString& name) {
        int id = -1;
        QString cmd = QString("where %1 = %2 and name = \"%3\"").arg(ridName()).arg(rid).arg(name);
        QVector<int> items = selectIds(cmd);
        if(items.size() > 0) {
            id = items.first();
        }
        return id;
    }

    QVector<T> selItemsByRid(int rid) {
        QString cmd = QString("where %1 = %2 ").arg(ridName()).arg(rid);
        return selectItems(cmd);
    }

    T findByRid(int rid) {QVector<DC_SerDevItem> items = selItemsByRid(rid); return items.first();}

    int getId(int rid) {
        int id = -1;
        QString cmd = QString("where %1 = %2").arg(ridName()).arg(rid);
        QVector<int> items = selectIds(cmd);
        if(items.size() > 0) {
            id = items.first();
        }
        return id;
    }

    bool setName(int id, const QString& name)
    {
        return updateColumn("name", name, QString("where id=%1").arg(id));
    }


protected:
    virtual QString ridName()=0;
};

#endif // DC_BASICMYSQL_H
