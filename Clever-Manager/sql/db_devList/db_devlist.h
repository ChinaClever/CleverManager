#ifndef DB_DEVLIST_H
#define DB_DEVLIST_H
#include <QSqlQuery>

#define LOG_DEVLIST_TABLE    "devList" // 设备链表数据库表

typedef struct
{
    int type; // 设备种类

    QString IP; // 设备IP
    QString name; // 设备名
    int num; // 副机数量

    QString area; // 区
    QString group; // 设备所属组
    QString cab;  // 机柜
}db_devStruct;


bool db_createDevListTable(void);

bool db_insert_devList(db_devStruct *dev);
bool db_del_devList(QString &ip);
bool db_select_devList(QList<db_devStruct*> &list);


#endif // DB_DEVLIST_H
