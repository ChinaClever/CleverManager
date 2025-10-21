#ifndef DC_PDUTHRESHOLDS_H
#define DC_PDUTHRESHOLDS_H


#include "basicmysql.h"
struct DC_PduThresholdItem{
    DC_PduThresholdItem():id(-1),pdu_devices_id(-1),type_index(-1),bit_index(-1),
        min(-1),max(-1),alarm(-1),cr_min(-1),cr_max(-1),cr_alarm(-1){}

    int id, pdu_devices_id, type_index, bit_index,alarm,cr_alarm;
    float min,max ,cr_min ,cr_max ;
    QDateTime create_time, update_time;
};
Q_DECLARE_METATYPE(DC_PduThresholdItem)

class DC_PduThresholds : public BasicMySql<DC_PduThresholdItem>
{
    Q_OBJECT
    explicit DC_PduThresholds();
public:
    static DC_PduThresholds* bulid();

    QString tableName(){return QString("%1_%2").arg(TB_PREFIX).arg("pdu_thresholds") ;}
    bool insertItem(const DC_PduThresholdItem& item); // 插入
    bool updateItem(const DC_PduThresholdItem& item); // 修改

    int getId(DC_PduThresholdItem& item);
    bool containRid(int rid, int type, int bit);
    int getMaxId(int rid, int type, int bit);

protected:
   bool modifyItem(const DC_PduThresholdItem& item,const QString& cmd);
   void selectItem(QSqlQuery &query,DC_PduThresholdItem &item);
   QString ridName(){return QString("pdu_devices_id") ;}
};

#endif // DC_PDUTHRESHOLDS_H
