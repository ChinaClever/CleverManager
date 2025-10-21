#ifndef DBCABINETLIST_H
#define DBCABINETLIST_H

#include "basicsql.h"
#include "common.h"

struct CabinetItem{
    int id,room_id,height,
        main_num,main_type,spare_num,spare_type,
        row,column;
    QString name,main_ip,spare_ip;

    CabinetItem():id(-1),room_id(-1),main_num(0),spare_num(0),row(0),column(0),height(42),main_type(0),spare_type(0){}
    bool unequal(const CabinetItem& item)const{
        return id!=item.id || room_id!=item.room_id || height!=item.height || main_num!=item.main_num ||
                main_type!=item.main_type || spare_type!=item.spare_type || spare_num!=item.spare_num ||
                row!=item.row || column!=item.column || name!=item.name || main_ip!=item.main_ip || spare_ip!=item.spare_ip;
    }
    bool isValideMain()const{return main_num>=0&&main_num<=9&&cm::isValidIpAddress(main_ip);}
    bool isValideSpare()const{return spare_num>=0&&spare_num<=9&&cm::isValidIpAddress(spare_ip);}
};
Q_DECLARE_METATYPE(CabinetItem)

class DbCabinetList : public BasicSql1<CabinetItem>
{
    Q_OBJECT
    DbCabinetList();
public:
    static DbCabinetList* get();
    QString tableName(){return "cabinets";}
    void insertItem(const CabinetItem& item);
    void updateItem(const CabinetItem& item);
    void removeItemsByRoomId(int roomId);
    QVector<CabinetItem> selItemsByRoomId(int room_id);
    QVector<int> selRoomIdsByAddress(const QString& addr,int num);

signals:
    void updateSig(int ,QString);

protected slots:
    void roomItemChange(int roomId,int type);
protected:
    void modifyItem(const CabinetItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,CabinetItem &item);
};

#endif // DBCABINETLIST_H
