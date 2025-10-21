#include "mibreader.h"
#include <QFile>
#include <QFileInfo>
#include <QStringList>
#include <QRegExp>
#include <QPair>
#include "dboidtable.h"
#include <QDebug>

MibReader::MibReader(QObject* parent):
    QThread(parent),m_lineIndex(-1),m_progress(0)
{

}

void MibReader::setPath(const QString &path)
{
    m_path = path;
}

int MibReader::reading(const QString &path)
{
    QFileInfo info(path);
    QString masterNumber;//主oid编码
    QList<QStringList> subNumbers;
    //qDebug() << "[MY DEBUG : INFO] - come into MibReader::reading() ";
    //这里仅仅对自定义的.txt的mib文件做处理
    QFile file(path);
    if(info.suffix() == "txt")
    {

        file.open(QFile::ReadOnly);
        m_lineIndex = 0;
        qint64 maxSize = file.size();
        while(!file.atEnd())
        {
            ++m_lineIndex;
            QString str = file.readLine();//按行读取文件
            //qDebug() << "[MY DEBUG : INFO] - line = " << m_lineIndex << " is : " << str;
            m_currentSize = file.pos();
            //若文件是由//注释或者是一个空的行则跳过该行
            if(str.left(2) == "//")
            {
                continue;
            }
            //获取有效字符串列表
            QStringList list = str.split(QRegExp("\\s+"),QString::SkipEmptyParts);
            if(list.isEmpty())
            {
                continue;//为空，跳过
            }
            //通常头先说明主oid编码
            if(masterNumber.isEmpty())
            {
                //主oid就一项，若没有，则说明读取错误。
                if(list.size()!=1)return -1;
                if(isNotOid(list.first()))return -6;
                masterNumber = list.first();
                //若是数据库表中有该主oid编号，则返回失败。
                if(!DbPduTypeList::get()->selectItemsByOid(masterNumber).isEmpty())
                {
                    //qDebug() << "[MY DEBUG : ERROR] - RETURN = -5";
                    file.close();
                    return -5;
                }
            }
            else
            {
                //子oid同名称对应为两项，若没有，则说明读取错误。
                if(list.size() != 2)
                {
                    //qDebug() << "[MY DEBUG : ERROR] - RETURN = -2 list size = " << list.size();
                    file.close();
                    return -2;
                }
                //若第一项不是子oid(由"."加数字构成)，则报错
                if(isNotOid(list.first()))
                {
                    //qDebug() << "[MY DEBUG : ERROR] - RETURN = -3";
                    file.close();
                    return -3;
                }
                subNumbers.append(list);
            }
            int progress = 100*m_currentSize/maxSize;
            if(m_progress != progress)
            {
                m_progress = progress;
                emit progressChange(m_progress);
            }
        }
    }
    //说明文件为空，不是有效的mib文件
    if(masterNumber.isEmpty() || subNumbers.isEmpty())
    {
        //qDebug() << "[MY DEBUG : ERROR] - RETURN = -4";
        file.close();
        return -4;
    }
    //放到数据库中
    PduTypeItem item;
    item.oid = masterNumber;
    item.des = info.baseName();
    item.id = DbPduTypeList::get()->maxId()+1;
    DbPduTypeList::get()->insertItem(item);
    int id = DbOidTable::get()->maxId();
    int index = 0;
    int maxSize = subNumbers.size();
    foreach(const QStringList& list,subNumbers)
    {
        OidItem oItem;
        oItem.id = ++id;
        oItem.name = list.last();
        oItem.oid = list.first();
        oItem.type_id = item.id;//是主表的id
        DbOidTable::get()->insertItem(oItem);
        int progress = 100*++index/maxSize;
        if(m_progress != progress)
        {
            m_progress = progress;
            emit progressDbChange(m_progress);
        }
    }

    file.close();
    return 0;
}

QString MibReader::errMsg(int errType)
{
    switch(errType){
    case 0:return "success!";
    case -1:return "read master oid error at line "+QString::number(m_lineIndex);
    case -2:return "read sub oid error at line "+QString::number(m_lineIndex);
    case -3:return "the sub oid format error at line "+QString::number(m_lineIndex);
    case -4:return "the file is empty";
    case -5:return "the database already has the master oid at line "+QString::number(m_lineIndex);
    case -6:return "the master oid format error at line "+QString::number(m_lineIndex);
    }
    return "unknow errors type.";
}

void MibReader::run()
{
    emit result(reading(m_path));
}

bool MibReader::isNotOid(const QString &oid)
{
    return oid.contains(QRegExp("[^\\d.]"));
}
