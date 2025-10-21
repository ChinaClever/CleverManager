#ifndef DC_IDCSREMARKS_H
#define DC_IDCSREMARKS_H
#include "dc_idcs.h"

struct DC_IdcRemarkItem{
    DC_IdcRemarkItem():id(-1),idcs_id(-1){}
    bool unequal(const DC_IdcRemarkItem& item) const
    {
        return id!=item.id || idcs_id!=item.idcs_id;
    }

    int id, idcs_id;
    QString country, area;
    QDateTime create_time, update_time;
};
Q_DECLARE_METATYPE(DC_IdcRemarkItem)

class DC_IdcsRemarks : public DC_BasicMySql<DC_IdcRemarkItem>
{    
    Q_OBJECT
    DC_IdcsRemarks();
public:
    static DC_IdcsRemarks* bulid();

    QString tableName(){return QString("%1_%2").arg(TB_PREFIX).arg("idcs_remarks") ;}
    bool insertItem(const DC_IdcRemarkItem& item); // 插入
    bool updateItem(const DC_IdcRemarkItem& item); // 修改

protected:
    bool modifyItem(const DC_IdcRemarkItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query, DC_IdcRemarkItem &item);
    QString ridName(){ return "idcs_id";}
};

#endif // DC_IDCSREMARKS_H
