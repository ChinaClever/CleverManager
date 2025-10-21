/*
 * common.cpp
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "common.h"
#include <QWidget>
#include <QHeaderView>
#include <QDir>
#include <windows.h>
//#include <dbghelp.h>

//long ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
//{
//        {
//        // 在程序exe的上级目录中创建dmp文件夹
//        QDir *dmp = new QDir;
//        bool exist = dmp->exists("../dmp/");
//        if(exist == false)
//            dmp->mkdir("../dmp/");
//        }

//        QDateTime current_date_time = QDateTime::currentDateTime();
//        QString current_date = current_date_time.toString("yyyy_MM_dd_hh_mm_ss");
//        QString time =  current_date + ".dmp";
//          EXCEPTION_RECORD *record = pException->ExceptionRecord;
//          QString errCode(QString::number(record->ExceptionCode, 16));
//          QString errAddr(QString::number((uint)record->ExceptionAddress, 16));
//          QString errFlag(QString::number(record->ExceptionFlags, 16));
//          QString errPara(QString::number(record->NumberParameters, 16));
//          qDebug()<<"errCode: "<<errCode;
//          qDebug()<<"errAddr: "<<errAddr;
//          qDebug()<<"errFlag: "<<errFlag;
//          qDebug()<<"errPara: "<<errPara;
//          HANDLE hDumpFile = CreateFile((LPCWSTR)QString("../dmp/" + time).utf16(),
//                   GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//          if(hDumpFile != INVALID_HANDLE_VALUE) {
//              MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
//              dumpInfo.ExceptionPointers = pException;
//              dumpInfo.ThreadId = GetCurrentThreadId();
//              dumpInfo.ClientPointers = TRUE;
//              MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
//              CloseHandle(hDumpFile);
//          }
//          else{
//              qDebug()<<"hDumpFile == null";
//          }
//          //FatalAppExitA(-1,"*****Unhandled Exceptions!*****");
//          return EXCEPTION_CONTINUE_SEARCH ;
//}
void set_background_icon(QWidget *widget, QString icon)
{
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/images/"+icon)));
    widget->setPalette(palette);
    widget->setAutoFillBackground(true);
}


void set_background_color(QWidget *widget, QColor color)
{
    QPalette palette;
    palette.setColor(QPalette::Background, color);
    widget->setPalette(palette);
    widget->setAutoFillBackground(true);
}


void window_background_color(QWidget *widget, QColor color)
{
    QPalette palette;
    palette.setColor(QPalette::Background, color);
    widget->setPalette(palette);
    widget->setAutoFillBackground(true);
}


/**
 * @brief Box 背景图片
 */
void groupBox_background_icon(QWidget *target)
{
    set_background_icon(target,"box_back.jpg");
}



void set_mainTitle_back(QWidget *widget)
{
//    set_background_color(widget, QColor(0,71,171));
     set_background_color(widget, QColor(12,62,180));
}

/**
 * @brief QSplitter 样式定义
 */
void splitter_style_sheet(QWidget *target)
{

     target->setStyleSheet("QSplitter::handle { background-color: rgb(203, 201, 173); }");
}

/**
 * @brief button 样式
 */
void button_style_sheet(QWidget *target)
{
    target->setStyleSheet("QPushButton{background-color:rgb(102, 199, 249);}"
                          "QPushButton:hover{background-color:rgb(91, 237, 238);}"
                          "QPushButton:pressed{background-color:rgb(143, 185, 181);}"
                          "QComboBox{background-color:rgb(255,255,255);}" );
}

/**
 * @brief button 样式 背景色为黄色，字体为白色
 */
void btnyellow_style_sheet(QWidget *target)
{
    target->setStyleSheet("QPushButton{background-color:rgb(12,62,180); color:white}"
                          "QPushButton:hover{background-color:rgb(91, 237, 238);}"
                          "QPushButton:pressed{background-color:rgb(143, 185, 181);}"
                          );
    target->setFont(QFont("微软雅黑",12));
}

/**
 * @brief button 样式 背景色为蓝色，字体为白色
 */
void btnBlue_style_sheet(QWidget *target)
{
    target->setStyleSheet("QPushButton{background-color:rgb(12,62,180); color:rgb(255, 255, 255);}"
                          "QPushButton:hover{background-color:rgb(91, 237, 238);color:rgb(0, 0, 0);}"
                          "QPushButton:pressed{background-color:rgb(143, 185, 181);color:rgb(0, 0, 0);}");
    target->setFont(QFont("微软雅黑",12));
}

void splitter_style_color(QWidget *target)
{
    target->setStyleSheet("QSplitter::handle { background-color: rgb(22, 117, 185); }");
}


void tableview_style_png(QTableView *view)
{
//    view->setStyleSheet("background-image: url(:/images/other/back.png);" "color:white");
//    view->horizontalHeader()->setStyleSheet("QHeaderView::section {background-color:rgb(239,239,239);};" "color:rgb(255,168,3)");
//    view->verticalHeader()->setStyleSheet( "color:black");
////    view->verticalHeader()->setVisible(false);
    view->horizontalHeader()->setFixedHeight(34);
//    view->verticalHeader()->setDefaultSectionSize(34);
//    view->verticalHeader()->setFixedWidth(40);
}

/**
 * @brief 是否为IP地址
 * @param ip
 * @return
 */
bool isIPaddress(QString &ip)
{
    ip = ip.trimmed();
    QRegExp regExp("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    if(regExp.exactMatch(ip))
        return true ;

    return false;
}


/***
  *判断一个字符串是否为纯数字
  */
bool isDigitStr(QString src)
{
    QByteArray ba = src.toLatin1();//QString 转换为 char*
    const char *s = ba.data();

    while(*s && *s>='0' && *s<='9') s++;

    if (*s) //不是纯数字
        return false;

    return true;
}
