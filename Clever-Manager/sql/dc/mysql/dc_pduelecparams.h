#ifndef DC_PDUELECPARAMS_H
#define DC_PDUELECPARAMS_H

#include "basicmysql.h"
struct DC_PduElecParamsItem{
    DC_PduElecParamsItem():id(-1),pdu_devices_id(-1),type_index(-1),bit_index(-1),
        vol(-1),cur(-1),pow(-1),ele(-1),pf(-1),sw(-1),carbon(0), rate(0){}

    int id, pdu_devices_id, type_index, bit_index; // (类型 1:输入相  2：输出位)
    double vol, cur, pow, ele,pf, carbon;
    int sw,rate ;
    QDateTime create_time, update_time;
};
Q_DECLARE_METATYPE(DC_PduElecParamsItem)

class DC_PduElecParams : public BasicMySql<DC_PduElecParamsItem>
{
    Q_OBJECT
    explicit DC_PduElecParams();
public:
    static DC_PduElecParams* bulid();

    QString tableName(){return QString("%1_%2").arg(TB_PREFIX).arg("pdu_elec_params") ;}
    bool insertItem(const DC_PduElecParamsItem& item); // 插入
    bool updateItem(const DC_PduElecParamsItem& item); // 修改

    bool containRid(int rid, int type, int bit);
    int getMaxId(int rid, int type, int bit);

protected:
   bool modifyItem(const DC_PduElecParamsItem& item,const QString& cmd);
   void selectItem(QSqlQuery &query,DC_PduElecParamsItem &item);
   QString ridName(){return QString("pdu_devices_id") ;}

};

#endif // DC_PDUELECPARAMS_H
