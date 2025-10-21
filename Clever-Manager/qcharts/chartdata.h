#ifndef CHARTDATA_H
#define CHARTDATA_H
#include <QVariant>

struct chartData
{
    QDateTime startDateTime; //起始时间
    QDateTime endDateTime; //结束时间
    QString title; //曲线图名称
    int type; //(类型 1:输入相  2：输出位 3：机房)
    QString strIp; //选择IP
    int index;//输出位某一相，输出位某一位
    int dataType; //（1：电流 2：电压 3：功率 4：电能 5：功率因数）
    QString cabin; //机房名
};

Q_DECLARE_METATYPE(chartData)

#endif // CHARTDATA_H
