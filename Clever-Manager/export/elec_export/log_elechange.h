#ifndef LOG_ELECHANGE_H
#define LOG_ELECHANGE_H

#include "log_tgroomdb.h"
#include "log_eletemptable.h"
#include "log_pdudevdb.h"

class Log_Elechange : public QObject
{
    Q_OBJECT
public:
    explicit Log_Elechange(QObject *parent = 0);

    bool roomEleChange(QVector<TgRoomItem> &items, EleTempItem &eleItem);

    bool roomDevChange(QVector<PduDevIndexItem> indexItem, QVector<PduElecParamsItem> &startItem,
                       QVector<PduElecParamsItem> &endItem,
                       QVector<EleTempItem> &eleItems);
    bool cabEleChange(QVector<TgCabItem> &items, EleTempItem &eleItemA, EleTempItem &eleItemB);
protected:
    QString getTypeStr(int type, int index);
signals:

public slots:
};

#endif // LOG_ELECHANGE_H
