#ifndef BASICSQL_H
#define BASICSQL_H

#include <QtSql>
#include <QException>
#include <QObject>

#define LOG_MAX_COUNT (10*10000)

class SqlExecption:public QException
{
    QString m_msg;
    qint32  m_type;
public:
    SqlExecption(const QString& msg,qint32 type) :m_msg(msg),m_type(type){}
    QString msg()const{return m_msg;}
    qint32  type()const{return m_type;}
};

class BasicSql:public QObject
{
    Q_OBJECT
public:
    enum{Remove,Insert,Update};
    BasicSql();
    virtual QString tableName() = 0;
    int  maxId(const QString &idName = "id");
    bool remove(const QString &condition);

    bool remove(int id);
    bool removeMinIds(int id);
    int minId();
    int minId(const QString &condition);
    int minId(const QString &idName, const QString &condition);
    bool minIdRemove();
    bool minIdsRemove(int id);

    int counts();
    int count(const QString &column_name, const QString &condition);
    bool countsRemove(int count);
    bool clear();

signals:
    void itemChanged(int id,int type);
protected:
    void throwError(const QSqlError& err); /// inline static
    QString tableMarking();
    void setTableMarking(const QString& marking);
private:
    static void initDb();
};

//该类为sql通用类型。
template <typename T>  //模板
class BasicSql1:public BasicSql
{
public:
    void removeItem(const T& item)
    {
        remove(QString("id = %1").arg(item.id));
        emit itemChanged(item.id,Remove);
    }
    QVector<T> allItems()
    {
        return selectItems("");
    }
    QVector<T> findItemById(int id){
        return selectItems(QString("where id = %1").arg(id));
    }

protected:
    virtual void selectItem(QSqlQuery &query,T &item)=0;
    QVector<T> selectItems(const QString &condition)
    {
        QSqlQuery query;
        QString sql = QString("SELECT * from %1 %2").arg(tableName()).arg(condition);
        query.prepare(sql);
        QVector<T> items;
        if(query.exec()){
            T item;
            while(query.next()){
                selectItem(query,item);
                items.append(item);
            }
        } else {
            qDebug()<<" "<<query.lastError().databaseText();
            qDebug()<<" "<<query.lastError().driverText();
            qDebug()<<sql;
            throwError(query.lastError());
        }
        return items;
    }
};

#endif // BASICSQL_H
