#include "mainwindow.h"
#include <QApplication>
#include "QtSingleApplication.h"

int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);
    QtSingleApplication a(QLatin1String("clevermanager_id"),argc, argv);
    if(a.isRunning())  {
        QMessageBox::information(NULL, QObject::tr("提示"), QObject::tr("程序已运行！"),QMessageBox::Ok);
        a.sendMessage("raise_window_noop", 1000); //4s后激活前个实例
        return EXIT_SUCCESS;
    }

    MainWindow w;
    a.setActivationWindow(&w,true); //如果是第一个实例,则绑定,方便下次调用
    w.showMaximized();

    return a.exec();
}
