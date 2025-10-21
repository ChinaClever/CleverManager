#ifndef DC_OUTPUTNAME_H
#define DC_OUTPUTNAME_H

#include "basicmysql.h"

struct DC_OutputNameItem
{
    DC_OutputNameItem():id(-1),pdu_devices_id(-1),output_bit(-1),name(" "){}
    int id, pdu_devices_id, output_bit ;
    QString name;
    QDateTime create_time, update_time;
};

class DC_OutputName : public BasicMySql<DC_OutputNameItem>
{
    Q_OBJECT
    explicit DC_OutputName();
public:
    static DC_OutputName* bulid();

    QString tableName(){return QString("%1_%2").arg(TB_PREFIX).arg("outputs_name") ;}
    bool insertItem(const DC_OutputNameItem& item); // 插入
    bool updateItem(const DC_OutputNameItem& item); // 修改

    int countOutput(int dev_id); // 输出位个数
    bool containOutput(int dev_id,int bit); // 输出位是否存在
    QVector<int> listOutput(int dev_id); // 列出所有输出位

    QString getOutputName(int dev_id,int bit); // 获取输出位的名称
    bool setOutputName(int dev_id,int bit,const QString& name); // 设置输出位的名称

protected:
    bool modifyItem(const DC_OutputNameItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,DC_OutputNameItem &item);
};

#endif // DC_OUTPUTNAME_H
