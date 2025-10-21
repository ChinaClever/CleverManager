#ifndef DIALOGTEMPMANAGER_H
#define DIALOGTEMPMANAGER_H

#include <QDialog>
#include <QThread>
#include <QProgressDialog>
#include "addfriendthr.h"
#include "autoaddfriend/zebrasecuritymanager.h"

#define ADD_FRIEND_EXPIRED 700 * 1000

class QTableWidgetItem;
namespace Ui {
class DialogTempManager;
}

class DialogTempManager : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTempManager(QWidget *parent = 0);
    ~DialogTempManager();

public slots:
    void add_friend_replied_slot(bool accepted);

private slots:
    void on_pushButton_addMib_clicked();
    void on_pushButton_deleteMib_clicked();

    void on_pushButton_addChannel_clicked();
    void on_pushButton_deleteChannel_clicked();

    void updateDataGetMode();
    void display_zebra_network_state();

    void add_friend_expired();

    void on_pushButton_apply_clicked();

    void on_channel_search_pushButton_clicked();

    void on_search_zerba_channel(QString channel_name);

    void on_boxsearch_pushButton_clicked();

    void on_updata_zebra_security_list(std::map<QString, zebra_security_data> map_data);

    void on_del_all_offline_pushButton_clicked();

protected:
    void updateMibList();
    void updateZebraFriendList();

private:
    Ui::DialogTempManager *ui;
    QTimer *zebra_network_timer;
    QProgressDialog *m_progressDlg;
    QTimer *add_friend_expired_timer;
};

#endif // DIALOGTEMPMANAGER_H
