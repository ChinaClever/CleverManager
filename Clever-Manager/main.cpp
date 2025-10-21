#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include "dialogmibreader.h"
#include "common.h"
#include "register/activeprocess.h"
#include "usrlanddlg.h"
#include "versionselect.h"
#include "common/qtsingleapplication/QtSingleApplication.h"

extern bool usr_landFlag;

int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);
    QtSingleApplication a(QLatin1String("clevermanager_id"),argc, argv);
    if(a.isRunning())  {
        QMessageBox::information(NULL, QObject::tr("提示"), QObject::tr("程序已运行！"),QMessageBox::Ok);
        //QMessageBox::information(NULL, QObject::tr("TIPS"), QObject::tr("The program has been running！"),QMessageBox::Ok);
        a.sendMessage("raise_window_noop", 1000); //4s后激活前个实例
        return EXIT_SUCCESS;
    }

    //注冊异常捕获函数
    //SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);


//    QTranslator *transtror = new QTranslator;
//    //transtror->load("F:/GitHub/CleverManager/Clever-Manager/English.qm");     //读取*.qm文件，路径为
//    transtror->load("English.qm");
//    a.installTranslator(transtror);

#if defined(SNMP_MODULE)
    QDir dir(a.applicationDirPath());
    if(dir.cd("mibs"))
    {
        QScopedPointer<DialogMibReader> dlg(new DialogMibReader);
        QFileInfoList infos = dir.entryInfoList(QStringList()<<"*.txt");
        foreach(const QFileInfo& info,infos)
        {
            dlg->parseFile(info.absoluteFilePath());
            dlg->exec();
        }
        dlg->close();
    }
#endif

    MainWindow w;
    a.setActivationWindow(&w,true); //如果是第一个实例,则绑定,方便下次调用
    if(usr_landFlag) /*用户登录才显示*/
        w.showMaximized();
    else
        return 0;
    ActiveProcess *act=new ActiveProcess;

    act->is_active();     //检测激活码是否正确
    if(act->is_act==false) //如果注册页面强行关闭，软件也将关闭 act->is_close&&
        w.close();

    return a.exec();
}
