#ifndef DIALOGMIBREADER_H
#define DIALOGMIBREADER_H

#include <QDialog>

namespace Ui {
class DialogMibReader;
}

class DialogMibReader : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMibReader(QWidget *parent = 0);
    ~DialogMibReader();
    void parseFile(const QString& path);
    QString errMsg();
protected slots:
    void finshed(int type);
private:
    Ui::DialogMibReader *ui;
    class MibReader *reader;
    QString msg;
};

#endif // DIALOGMIBREADER_H
