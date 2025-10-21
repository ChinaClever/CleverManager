#ifndef IPSWID_H
#define IPSWID_H

#include <QWidget>

namespace Ui {
class Up_IpsWid;
}

class Up_IpsWid : public QWidget
{
    Q_OBJECT

public:
    explicit Up_IpsWid(QWidget *parent = 0);
    ~Up_IpsWid();

protected:
    void disEnable();
    bool readFile(QString &fn, QStringList &list);
    int bulidIps(QString startIp, QString &endIp);
    int getIpInt(QString &ip);

private slots:
    void on_exportBtn_clicked();
    void on_ipBtn_clicked();
    void on_ipsBtn_clicked();
    void on_ipRadio_clicked(bool checked);
    void on_ipsRadio_clicked(bool checked);
    void on_exportRadio_clicked(bool checked);
    void on_getRadio_clicked(bool checked);
    void on_getBtn_clicked();

private:
    Ui::Up_IpsWid *ui;
    QStringList mList;
};

#endif // IPSWID_H
