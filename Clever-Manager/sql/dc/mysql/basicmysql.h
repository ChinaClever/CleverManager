#ifndef BASICMYSQL_H
#define BASICMYSQL_H

#include <QtSql>
#include <QException>
#include <QObject>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#define TB_PREFIX  "dc"

class MysqlExecption:public QException
{
    QString m_msg;
    qint32  m_type;
public:
    MysqlExecption(const QString& msg,qint32 type) :m_msg(msg),m_type(type){}
    QString msg()const{return m_msg;}
    qint32  type()const{return m_type;}
};

class MySqlBasic : public QObject
{
    Q_OBJECT
public:
    enum{Remove,Insert,Update};
    explicit MySqlBasic(QObject *parent = 0);

    virtual QString tableName() = 0;
    void remove(const QString &condition);
    int  maxId(const QString &idName="id", const QString &condition="");
    int count(const QString &column_name, const QString &condition);

    QStringList listColumn(const QString &column_name, const QString &condition);
    QVector<int> listColumnToInt(const QString &column_name, const QString &condition);
    QVector<int> selectIds(const QString &condition);

    bool updateColumn(const QString& column_name, double value, const QString &condition);
    bool updateColumn(const QString& column_name, const QString& value, const QString &condition);

signals:
    void itemChanged(int id,int type);
protected:
    bool updateTime(const QString &condition);
    void throwError(const QSqlError& err);

protected:
    QSqlDatabase *mDb;
private:
    static void initDb();
};

//该类为sql通用类型。
template <typename T>
class BasicMySql:public MySqlBasic
{
public:
    void removeById(int id) {
        remove(QString("id = %1").arg(id));
        emit itemChanged(id,Remove);
    }

    void removeItem(const T& item) {
        removeById(item.id);
    }
    QVector<T> allItems() {
        return selectItems("");
    }
    QVector<T> findItemById(int id){
        return selectItems(QString("where id = %1").arg(id));
    }

    T findById(int id){
        return findItemById(id).first();
    }

protected:
    virtual void selectItem(QSqlQuery &query,T &item)=0;
    QVector<T> selectItems(const QString &condition)
    {
        QVector<T> items;
        if(mDb) {
            QSqlQuery query(*mDb);
            QString sql = QString("SELECT * from %1 %2").arg(tableName()).arg(condition);
            query.prepare(sql);            
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
        }
        return items;
    }
};
#endif // BASICMYSQL_H
