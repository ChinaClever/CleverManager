#ifndef SETOUTPUTDLG_H
#define SETOUTPUTDLG_H

#include <QDialog>
#include "setthresholddlg.h"

namespace Ui {
class SetOutputDlg;
}

class SetOutputDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SetOutputDlg(QWidget *parent = 0);
    ~SetOutputDlg();

    void setMode(int line,QString &name,PduDataPacket *packet);
    void setLoopMode();
    void setName(QString &name, bool is_cur);//设置名称，如回路还是输出位

protected:
    void initWidget(void);
    void initData(int line);

    bool checkData(void);

    void saveLog(void);
    uchar *getData(uchar *buf);
    bool sentData(void);
    void saveData(void);

    void saveNameLog(void);
    bool sentNameData(void);
    void saveName(void);

private slots:
    void on_saveBtn_clicked();

private:
    Ui::SetOutputDlg *ui;

    int mLine;
    QString mName;
    PduDataUnit *mUnit;
    PduDataPacket *mPacket;
    bool is_loop;
    bool is_loopcur;
};

#endif // SETOUTPUTDLG_H
