#ifndef TG_DATADSPLAY_H
#define TG_DATADSPLAY_H

#include "tg_curvolgraph.h"
#include "barsliderdlg.h"
#include "dialpandlg.h"
#include "pdudatatgthread.h"
#include "tg_lcdnum.h"

typedef sPduTGDataStr TG_DataType; // 统计数据结构体重新定义


class TG_DataDsplay : public QWidget
{
    Q_OBJECT
public:
    explicit TG_DataDsplay(QWidget *parent = 0);
    ~TG_DataDsplay();

    void updateData(TG_DataType *);

protected:
    void createControl(void);
    void layoutControl(void);
    void initView();

    void changeVolMode(long double);
    void changeCurMode(long double);
    void changePowMode(long double);
    void changeEleMode(long double);

signals:

public slots:

private:
    QLabel *devLab; // 设备名
    QHBoxLayout *devHlayout;

    TG_LcdNum *mCurLcd;
    TG_LcdNum *mVolLcd;
    TG_LcdNum *mPowLcd;
    TG_LcdNum *mEleLcd;

    QLabel *eleLab;
    TG_LcdNum *mTemLcd;
     QVBoxLayout *out_layout;
    // QLabel *tempLab;
    // DialpanDlg *m_pDialPan; // 温度表盘

    TG_LcdNum *mHumLcd;
    // QLabel *humLab;
    // BarSliderDlg *m_pBarSliderDlg;

    QVBoxLayout *dspVLayout;
    QWidget *groupBox;
    QGridLayout *m_pLayout;
};

#endif // TG_DATADSPLAY_H
